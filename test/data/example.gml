<?xml version="1.0" encoding="UTF-8"?>
<gml:FeatureCollection 
    xmlns:gml="http://www.opengis.net/gml"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.opengis.net/gml http://schemas.opengis.net/gml/3.2.1/feature.xsd">
    
    <!-- Punto -->
    <gml:featureMember>
    <gml:point gml:id="point.0">
      <gml:boundedBy><gml:Envelope srsName="urn:ogc:def:crs:EPSG::4326"><gml:lowerCorner>39.57422 -105.01621</gml:lowerCorner><gml:upperCorner>39.57422 -105.01621</gml:upperCorner></gml:Envelope></gml:boundedBy>
      <gml:geometryProperty><gml:Point srsName="urn:ogc:def:crs:EPSG::4326" gml:id="point.geom.0"><gml:pos>39.57422 -105.01621</gml:pos></gml:Point></gml:geometryProperty>
      <gml:name>Ubicación de ejemplo</gml:name>
    </gml:point>
  </gml:featureMember>

    <!-- Línea -->
    <gml:featureMember>
    <gml:linestring gml:id="linestring.0">
      <gml:boundedBy><gml:Envelope srsName="urn:ogc:def:crs:EPSG::4326"><gml:lowerCorner>39.57422 -105.01621</gml:lowerCorner><gml:upperCorner>39.59931 -105.00376</gml:upperCorner></gml:Envelope></gml:boundedBy>
      <gml:geometryProperty><gml:LineString srsName="urn:ogc:def:crs:EPSG::4326" gml:id="linestring.geom.0"><gml:posList>39.57422 -105.01621 39.59931 -105.00376</gml:posList></gml:LineString></gml:geometryProperty>
      <gml:name>Línea de ejemplo</gml:name>
    </gml:linestring>
  </gml:featureMember>

    <!-- Polígono -->
    <gml:featureMember>
    <gml:polygon gml:id="polygon.0">
      <gml:boundedBy><gml:Envelope srsName="urn:ogc:def:crs:EPSG::4326"><gml:lowerCorner>39.59781 -105.02311</gml:lowerCorner><gml:upperCorner>39.60235 -105.01114</gml:upperCorner></gml:Envelope></gml:boundedBy>
      <gml:geometryProperty><gml:Polygon srsName="urn:ogc:def:crs:EPSG::4326" gml:id="polygon.geom.0"><gml:exterior><gml:LinearRing><gml:posList>39.60058 -105.02311 39.59781 -105.01431 39.60235 -105.01114 39.60058 -105.02311</gml:posList></gml:LinearRing></gml:exterior></gml:Polygon></gml:geometryProperty>
      <gml:name>Polígono de ejemplo</gml:name>
    </gml:polygon>
  </gml:featureMember>
	
</gml:FeatureCollection>