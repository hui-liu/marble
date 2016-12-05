//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011      Konstantin Oblaukhov <oblaukhov.konstantin@gmail.com>
//

#include "AbstractGeoPolygonGraphicsItem.h"

#include "GeoDataLinearRing.h"
#include "GeoDataPolygon.h"
#include "GeoPainter.h"
#include "GeoDataLatLonAltBox.h"
#include "GeoDataStyle.h"
#include "GeoDataIconStyle.h"
#include "GeoDataLineStyle.h"
#include "GeoDataPlacemark.h"
#include "GeoDataPolyStyle.h"
#include "GeoDataTypes.h"
#include "OsmPlacemarkData.h"
#include "MarbleDebug.h"
#include "ViewportParams.h"

#include <QtMath>
#include <QImageReader>

namespace Marble
{

QPixmapCache AbstractGeoPolygonGraphicsItem::m_textureCache = QPixmapCache();

AbstractGeoPolygonGraphicsItem::AbstractGeoPolygonGraphicsItem(const GeoDataPlacemark *placemark, const GeoDataPolygon *polygon) :
    GeoGraphicsItem(placemark),
    m_polygon(polygon),
    m_ring(0)
{
}

AbstractGeoPolygonGraphicsItem::AbstractGeoPolygonGraphicsItem(const GeoDataPlacemark *placemark, const GeoDataLinearRing *ring) :
    GeoGraphicsItem(placemark),
    m_polygon(0),
    m_ring(ring)
{
}

AbstractGeoPolygonGraphicsItem::~AbstractGeoPolygonGraphicsItem()
{
}

const GeoDataLatLonAltBox& AbstractGeoPolygonGraphicsItem::latLonAltBox() const
{
    if( m_polygon ) {
        return m_polygon->latLonAltBox();
    }

    return m_ring->latLonAltBox();
}

void AbstractGeoPolygonGraphicsItem::paint( GeoPainter* painter, const ViewportParams* viewport, const QString &layer, int tileZoomLevel)
{
    Q_UNUSED(layer);
    Q_UNUSED(tileZoomLevel);

    painter->save();
    configurePainter(painter, viewport);
    if ( m_polygon ) {
        bool innerResolved = false;

        for(auto const & ring : m_polygon->innerBoundaries()) {
            if (viewport->resolves(ring.latLonAltBox(), 4)) {
               innerResolved = true;
               break;
            }
        }

        if (innerResolved) {
            painter->drawPolygon(*m_polygon);
        }
        else {
            painter->drawPolygon(m_polygon->outerBoundary());
        }
    } else if ( m_ring ) {
        painter->drawPolygon( *m_ring );
    }

    painter->restore();
}

QPen AbstractGeoPolygonGraphicsItem::configurePainter(GeoPainter *painter, const ViewportParams *viewport)
{
    QPen currentPen = painter->pen();

    GeoDataStyle::ConstPtr style = this->style();
    if (!style) {
        painter->setPen( QPen() ); // "style-less" polygons: a 1px black solid line
    }
    else {
        const GeoDataPolyStyle& polyStyle = style->polyStyle();

        if (polyStyle.outline()) { // polygons without outline: Qt::NoPen (not drawn)
            const GeoDataLineStyle& lineStyle = style->lineStyle();

            currentPen.setColor(lineStyle.paintedColor());
            currentPen.setWidthF(lineStyle.width());
            currentPen.setCapStyle(lineStyle.capStyle());
            currentPen.setStyle(lineStyle.penStyle());

            if (painter->pen().color() != currentPen.color()) {
                painter->setPen(currentPen);
            }
        }

        if (!polyStyle.fill()) {
            painter->setBrush(QColor(Qt::transparent));
        }
        else {
            const QColor paintedColor = polyStyle.paintedColor();
            if (painter->brush().color() != paintedColor ||
                painter->brush().style() != polyStyle.brushStyle()) {
                if (!polyStyle.texturePath().isEmpty() || !polyStyle.textureImage().isNull()) {
                    GeoDataCoordinates coords = latLonAltBox().center();
                    qreal x, y;
                    viewport->screenCoordinates(coords, x, y);
                    QBrush brush(texture(polyStyle.texturePath(), paintedColor));
                    painter->setBrush(brush);
                    painter->setBrushOrigin(QPoint(x,y));
                }
                else {
                    painter->setBrush(QBrush(paintedColor, polyStyle.brushStyle()));
                }
            }
        }
    }

    return currentPen;
}

int AbstractGeoPolygonGraphicsItem::extractElevation(const GeoDataPlacemark &placemark)
{
    int elevation = 0;

    const OsmPlacemarkData &osmData = placemark.osmData();

    const auto tagIter = osmData.findTag(QStringLiteral("ele"));
    if (tagIter != osmData.tagsEnd()) {
        elevation = tagIter.value().toInt();
    }

    return elevation;
}

QPixmap AbstractGeoPolygonGraphicsItem::texture(const QString &texturePath, const QColor &color)
{
    QString const key = QString("%1/%2").arg(color.rgba()).arg(texturePath);
    QPixmap texture;
    if (!m_textureCache.find(key, texture)) {
        QImageReader imageReader(style()->polyStyle().resolvePath(texturePath));
        texture = QPixmap::fromImageReader(&imageReader);

        if (texture.hasAlphaChannel()) {
            QPixmap pixmap (texture.size());
            pixmap.fill(color);
            QPainter imagePainter(&pixmap);
            imagePainter.drawPixmap(0, 0, texture);
            imagePainter.end();
            texture = pixmap;
        }
        m_textureCache.insert(key, texture);
    }
    return texture;
}

}
