/**************************************************************************
 *                                                                        *
 * Copyright (C) 2021 by Tidop Research Group                             *
 * Copyright (C) 2021 by Esteban Ruiz de Oña Crespo                       *
 *                                                                        *
 * This file is part of TidopLib                                          *
 *                                                                        *
 * TidopLib is free software: you can redistribute it and/or modify       *
 * it under the terms of the GNU Lesser General Public License as         *
 * published by the Free Software Foundation, either version 3 of the     *
 * License, or (at your option) any later version.                        *
 *                                                                        *
 * TidopLib is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 * GNU Lesser General Public License for more details.                    *
 *                                                                        *
 * You should have received a copy of the GNU Lesser General Public       *
 * License along with TidopLib. If not, see <http://www.gnu.org/licenses>.*
 *                                                                        *
 * @license LGPL-3.0 <https://www.gnu.org/licenses/lgpl-3.0.html>         *
 *                                                                        *
 **************************************************************************/

#pragma once

#include "tidop/config.h"
#include "tidop/vectortools/io/vectreader.h"

class OGRLayer;
class OGRGeometry;
class OGRPoint;
class OGRLineString;
class OGRPolygon;
class OGRMultiPoint;
class OGRMultiLineString;
class OGRMultiPolygon;
class OGRStyleMgr;
class OGRStylePen;
class OGRStyleBrush;
class OGRStyleSymbol;
class OGRStyleLabel;
class OGRFeature;
class OGRFeatureDefn;
class GDALDataset;

namespace tl
{

class GraphicEntity;
class GPoint;
class GPoint3D;
class GLineString;
class GLineString3D;
class GPolygon;
class GPolygon3D;
class GMultiPoint;
class GMultiPoint3D;
class GMultiLineString;
class GMultiLineString3D;
class GMultiPolygon;
class GMultiPolygon3D;
class Pen;
class Brush;
class Symbol;
class Label;
class TableRegister;

#ifdef TL_HAVE_GDAL

/// \cond

class VectorReaderGdal
  : public VectorReader
{

    GENERATE_UNIQUE_PTR(VectorReaderGdal)

public:

    VectorReaderGdal(Path file);
    
    ~VectorReaderGdal() override;

    void open() override;

    auto isOpen() const -> bool override;
    
    void close() override;
    
    auto layersCount() const -> int override;

    auto read(int layerId) -> std::shared_ptr<GLayer> override;

    auto read(const std::string& layerName) -> std::shared_ptr<GLayer> override;

    auto crsWkt() const -> std::string override;

private:

    auto read(OGRLayer *ogrLayer) const -> std::shared_ptr<GLayer>;
    static auto readEntity(OGRGeometry *ogrGeometry) -> std::shared_ptr<GraphicEntity>;
    static auto readPoint(const OGRPoint *ogrPoint) -> std::shared_ptr<GPoint>;
    static auto readPoint3D(const OGRPoint *ogrPoint) -> std::shared_ptr<GPoint3D>;
    static auto readLineString(const OGRLineString *ogrLineString) -> std::shared_ptr<GLineString>;
    static auto readLineString3D(const OGRLineString *ogrLineString) -> std::shared_ptr<GLineString3D>;
    static auto readPolygon(OGRPolygon *ogrPolygon) -> std::shared_ptr<GPolygon>;
    static auto readPolygon3D(OGRPolygon *ogrPolygon) -> std::shared_ptr<GPolygon3D>;
    static auto readMultiPoint(OGRMultiPoint *ogrMultiPoint) -> std::shared_ptr<GMultiPoint>;
    static auto readMultiPoint3D(OGRMultiPoint *ogrMultiPoint) -> std::shared_ptr<GMultiPoint3D>;
    static auto readMultiLineString(OGRMultiLineString *ogrMultiLineString) -> std::shared_ptr<GMultiLineString>;
    static auto readMultiLineString3D(OGRMultiLineString *ogrMultiLineString) -> std::shared_ptr<GMultiLineString3D>;
    static auto readMultiPolygon(OGRMultiPolygon *ogrMultiPolygon) -> std::shared_ptr<GMultiPolygon>;
    static auto readMultiPolygon3D(OGRMultiPolygon *ogrMultiPolygon) -> std::shared_ptr<GMultiPolygon3D>;
    static void readStyles(OGRStyleMgr *ogrStyle, GraphicEntity *gStyle);
    static auto readPen(OGRStylePen *ogrStylePen) -> std::shared_ptr<Pen>;
    static void readPenColor(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenCap(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenPattern(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenJoin(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenName(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenWidth(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenPerpendicularOffset(OGRStylePen *ogrStylePen, Pen *pen);
    static void readPenPriorityLevel(OGRStylePen *ogrStylePen, Pen *pen);
    static auto readBrush(OGRStyleBrush *ogrStyleBrush) -> std::shared_ptr<Brush>;
    static void readBrushAngle(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushBackColor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushForeColor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushName(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushPriorityLevel(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushScalingFactor(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static void readBrushSpacing(OGRStyleBrush *ogrStyleBrush, Brush *brush);
    static auto readSymbol(OGRStyleSymbol *ogrStyleSymbol) -> std::shared_ptr<Symbol>;
    static void readSymbolAngle(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolName(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolOffset(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolOutlineColor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolPriorityLevel(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static void readSymbolScalingFactor(OGRStyleSymbol *ogrStyleSymbol, Symbol *symbol);
    static auto readLabel(OGRStyleLabel *ogrStyleLabel) -> std::shared_ptr<Label>;
    static void readLabelAnchorPosition(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelAngle(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelBackgroundColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelForegroundColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelOutlineColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelShadowColor(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelLabelPlacement(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelOffset(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelStretch(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readLabelFont(OGRStyleLabel *ogrStyleLabel, Label *label);
    static void readData(const OGRFeature *ogrFeature,
                         OGRFeatureDefn *ogrFeatureDefinition,
                         TableRegister *data);

private:

    GDALDataset *mDataset;

};


inline auto VectorReaderGdal::isOpen() const -> bool
{
    return mDataset != nullptr;
}



/// \endcond

#endif // TL_HAVE_GDAL

} // End namespace tl
