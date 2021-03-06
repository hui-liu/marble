//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Sanjiban Bairagya  <sanjiban22393@gmail.com>
//

#ifndef MARBLE_KML_GXALTITUDEMODETAGHANDLER_H
#define MARBLE_KML_GXALTITUDEMODETAGHANDLER_H

#include "GeoTagHandler.h"

namespace Marble
{
namespace kml
{
namespace gx
{
class KmlaltitudeModeTagHandler : public GeoTagHandler
{
public:
    GeoNode* parse(GeoParser&) const override;
};

}
}
}
#endif
