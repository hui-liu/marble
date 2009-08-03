//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2007      Andrew Manson  <g.real.ate@gmail.com>
// Copyright 2008      Torsten Rahn   <rahn@kde.org>
//


#ifndef GEODATALATLONALTBOX_H
#define GEODATALATLONALTBOX_H


#include "global.h"

#include "GeoDataObject.h"
#include "GeoDataPoint.h"

#include "geodata_export.h"

#include "GeoDataLatLonBox.h"

namespace Marble
{

class GeoDataLatLonAltBoxPrivate;

class GeoDataLineString;


/**
 * @short A class that defines a 3D bounding box for geographic data.
 *
 * GeoDataLatLonAltBox is a 3D bounding box that describes a geographic area
 * in terms of latitude, longitude and altitude.
 *
 * The bounding box gets described by assigning the northern, southern, 
 * eastern and western boundary.
 * So usually the value of the eastern boundary is bigger than the
 * value of the western boundary. Only if the bounding box crosses the
 * date line then the eastern boundary has got a smaller value than 
 * the western one.
 */

class GEODATA_EXPORT GeoDataLatLonAltBox : public GeoDataLatLonBox
{
    friend bool GEODATA_EXPORT operator==( GeoDataLatLonAltBox const& lhs, GeoDataLatLonAltBox const& rhs );

 public:
    GeoDataLatLonAltBox();
    GeoDataLatLonAltBox( const GeoDataLatLonAltBox & );
    GeoDataLatLonAltBox( const GeoDataLatLonBox & );

    virtual ~GeoDataLatLonAltBox();

    GeoDataLatLonAltBox& operator=( const GeoDataLatLonAltBox& other );

    /// Provides type information for downcasting a GeoNode
    virtual QString nodeType() { return geonode::GeoDataLatLonAltBoxType; }

    /**
     * @brief Get the lower altitude boundary of the bounding box.
     * @return the height of the lower altitude boundary.
     */
    qreal minAltitude() const;
    void setMinAltitude( const qreal minAltitude );

    /**
     * @brief Get the upper altitude boundary of the bounding box.
     * @return the height of the upper altitude boundary.
     */
    qreal maxAltitude() const;
    void setMaxAltitude( const qreal maxAltitude );

    /**
     * @brief Get the reference system for the altitude.
     * @return the point of reference which marks the origin 
     * for measuring the altitude.
     */
    AltitudeMode altitudeMode() const;
    void setAltitudeMode( const AltitudeMode altitudeMode );

    virtual bool contains( const GeoDataPoint & ) const; // NOTE: Why do we need this one?
    virtual bool contains( const GeoDataCoordinates & ) const;
    bool     contains( const GeoDataLatLonAltBox & ) const;

    virtual bool intersects( const GeoDataLatLonAltBox & ) const;

    using GeoDataLatLonBox::intersects;

    /**
     * @brief Create the smallest bounding box from a line string.
     * @return the smallest bounding box that contains the linestring.
     */
    static GeoDataLatLonAltBox fromLineString( const GeoDataLineString& lineString );

    /**
     * @brief Creates a text string of the bounding box
     */
    virtual QString toString( GeoDataCoordinates::Unit unit = GeoDataCoordinates::Radian ) const;

    /**
     * @brief Indicates whether the bounding box only contains a single 2D point ("singularity").
     * @return Return value is true if the height and the width of the bounding box equal zero.
     */
    bool isNull() const;

    /**
     * @brief returns the center of this box
     * @return a coordinate, body-center of the box
     */
    virtual GeoDataCoordinates center() const;

    /// Serialize the contents of the feature to @p stream.
    virtual void pack( QDataStream& stream ) const;
    /// Unserialize the contents of the feature from @p stream.
    virtual void unpack( QDataStream& stream );

 private:
    GeoDataLatLonAltBoxPrivate  * const d;
};

bool GEODATA_EXPORT operator==( GeoDataLatLonAltBox const& lhs, GeoDataLatLonAltBox const& rhs );

}

#endif // GEODATALATLONALTBOX
