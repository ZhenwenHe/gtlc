/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits            
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include "assert.h"
#include "buffer.h"  
begin_gtl_namespace
begin_gdb_namespace
//from gdal ogr_srs_api.h
enum AxisOrientation {
	AO_Other = 0,
	AO_North = 1,
	AO_South = 2,
	AO_East = 3,
	AO_West = 4,
	AO_Up = 5,
	AO_Down = 6
};
const char *axisEnumToName(AxisOrientation eOrientation);

enum DatumType {
	DT_HD_Min = 1000,
	DT_HD_Other = 1000,
	DT_HD_Classic = 1001,
	DT_HD_Geocentric = 1002,
	DT_HD_Max = 1999,
	DT_VD_Min = 2000,
	DT_VD_Other = 2000,
	DT_VD_Orthometric = 2001,
	DT_VD_Ellipsoidal = 2002,
	DT_VD_AltitudeBarometric = 2003,
	DT_VD_Normal = 2004,
	DT_VD_GeoidModelDerived = 2005,
	DT_VD_Depth = 2006,
	DT_VD_Max = 2999,
	DT_LD_Min = 10000,
	DT_LD_Max = 32767
};

/* ==================================================================== */
/*      Some standard WKT geographic coordinate systems.                */
/* ==================================================================== */

#define SRS_WKT_WGS84 "GEOGCS[\"WGS 84\",DATUM[\"WGS_1984\",SPHEROID[\"WGS 84\",6378137,298.257223563,AUTHORITY[\"EPSG\",\"7030\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6326\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4326\"]]"

/* ==================================================================== */
/*      Some "standard" strings.                                        */
/* ==================================================================== */

#define SRS_PT_ALBERS_CONIC_EQUAL_AREA                                  \
                                "Albers_Conic_Equal_Area"
#define SRS_PT_AZIMUTHAL_EQUIDISTANT "Azimuthal_Equidistant"
#define SRS_PT_CASSINI_SOLDNER  "Cassini_Soldner"
#define SRS_PT_CYLINDRICAL_EQUAL_AREA "Cylindrical_Equal_Area"
#define SRS_PT_BONNE            "Bonne"
#define SRS_PT_ECKERT_I         "Eckert_I"
#define SRS_PT_ECKERT_II        "Eckert_II"
#define SRS_PT_ECKERT_III       "Eckert_III"
#define SRS_PT_ECKERT_IV        "Eckert_IV"
#define SRS_PT_ECKERT_V         "Eckert_V"
#define SRS_PT_ECKERT_VI        "Eckert_VI"
#define SRS_PT_EQUIDISTANT_CONIC                                        \
                                "Equidistant_Conic"
#define SRS_PT_EQUIRECTANGULAR  "Equirectangular"
#define SRS_PT_GALL_STEREOGRAPHIC                                       \
                                "Gall_Stereographic"
#define SRS_PT_GAUSSSCHREIBERTMERCATOR                                      \
                                "Gauss_Schreiber_Transverse_Mercator"
#define SRS_PT_GEOSTATIONARY_SATELLITE                                  \
                                "Geostationary_Satellite"
#define SRS_PT_GOODE_HOMOLOSINE "Goode_Homolosine"
#define SRS_PT_IGH              "Interrupted_Goode_Homolosine"
#define SRS_PT_GNOMONIC         "Gnomonic"
#define SRS_PT_HOTINE_OBLIQUE_MERCATOR_AZIMUTH_CENTER                   \
                                "Hotine_Oblique_Mercator_Azimuth_Center"
#define SRS_PT_HOTINE_OBLIQUE_MERCATOR                                  \
                                "Hotine_Oblique_Mercator"
#define SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN         \
                            "Hotine_Oblique_Mercator_Two_Point_Natural_Origin"
#define SRS_PT_LABORDE_OBLIQUE_MERCATOR                                 \
                                "Laborde_Oblique_Mercator"
#define SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP                              \
                                "Lambert_Conformal_Conic_1SP"
#define SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP                              \
                                "Lambert_Conformal_Conic_2SP"
#define SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM                      \
                                "Lambert_Conformal_Conic_2SP_Belgium"
#define SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA                     \
                                "Lambert_Azimuthal_Equal_Area"
#define SRS_PT_MERCATOR_1SP     "Mercator_1SP"
#define SRS_PT_MERCATOR_2SP     "Mercator_2SP"
#define SRS_PT_MILLER_CYLINDRICAL "Miller_Cylindrical"
#define SRS_PT_MOLLWEIDE        "Mollweide"
#define SRS_PT_NEW_ZEALAND_MAP_GRID                                     \
                                "New_Zealand_Map_Grid"
#define SRS_PT_OBLIQUE_STEREOGRAPHIC                                    \
                                "Oblique_Stereographic"
#define SRS_PT_ORTHOGRAPHIC     "Orthographic"
#define SRS_PT_POLAR_STEREOGRAPHIC                                      \
                                "Polar_Stereographic"
#define SRS_PT_POLYCONIC        "Polyconic"
#define SRS_PT_ROBINSON         "Robinson"
#define SRS_PT_SINUSOIDAL       "Sinusoidal"
#define SRS_PT_STEREOGRAPHIC    "Stereographic"
#define SRS_PT_SWISS_OBLIQUE_CYLINDRICAL                                \
                                "Swiss_Oblique_Cylindrical"
#define SRS_PT_TRANSVERSE_MERCATOR                                      \
                                "Transverse_Mercator"
#define SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED                       \
                                "Transverse_Mercator_South_Orientated"

/* special mapinfo variants on Transverse Mercator */
#define SRS_PT_TRANSVERSE_MERCATOR_MI_21 \
                                "Transverse_Mercator_MapInfo_21"
#define SRS_PT_TRANSVERSE_MERCATOR_MI_22 \
                                "Transverse_Mercator_MapInfo_22"
#define SRS_PT_TRANSVERSE_MERCATOR_MI_23 \
                                "Transverse_Mercator_MapInfo_23"
#define SRS_PT_TRANSVERSE_MERCATOR_MI_24 \
                                "Transverse_Mercator_MapInfo_24"
#define SRS_PT_TRANSVERSE_MERCATOR_MI_25 \
                                "Transverse_Mercator_MapInfo_25"

#define SRS_PT_TUNISIA_MINING_GRID                                      \
                                "Tunisia_Mining_Grid"
#define SRS_PT_TWO_POINT_EQUIDISTANT                                    \
                                "Two_Point_Equidistant"
#define SRS_PT_VANDERGRINTEN    "VanDerGrinten"
#define SRS_PT_KROVAK           "Krovak"
#define SRS_PT_IMW_POLYCONIC    "International_Map_of_the_World_Polyconic"
#define SRS_PT_WAGNER_I         "Wagner_I"
#define SRS_PT_WAGNER_II        "Wagner_II"
#define SRS_PT_WAGNER_III       "Wagner_III"
#define SRS_PT_WAGNER_IV        "Wagner_IV"
#define SRS_PT_WAGNER_V         "Wagner_V"
#define SRS_PT_WAGNER_VI        "Wagner_VI"
#define SRS_PT_WAGNER_VII       "Wagner_VII"


#define SRS_PP_CENTRAL_MERIDIAN         "central_meridian"
#define SRS_PP_SCALE_FACTOR             "scale_factor"
#define SRS_PP_STANDARD_PARALLEL_1      "standard_parallel_1"
#define SRS_PP_STANDARD_PARALLEL_2      "standard_parallel_2"
#define SRS_PP_PSEUDO_STD_PARALLEL_1    "pseudo_standard_parallel_1"
#define SRS_PP_LONGITUDE_OF_CENTER      "longitude_of_center"
#define SRS_PP_LATITUDE_OF_CENTER       "latitude_of_center"
#define SRS_PP_LONGITUDE_OF_ORIGIN      "longitude_of_origin"
#define SRS_PP_LATITUDE_OF_ORIGIN       "latitude_of_origin"
#define SRS_PP_FALSE_EASTING            "false_easting"
#define SRS_PP_FALSE_NORTHING           "false_northing"
#define SRS_PP_AZIMUTH                  "azimuth"
#define SRS_PP_LONGITUDE_OF_POINT_1     "longitude_of_point_1"
#define SRS_PP_LATITUDE_OF_POINT_1      "latitude_of_point_1"
#define SRS_PP_LONGITUDE_OF_POINT_2     "longitude_of_point_2"
#define SRS_PP_LATITUDE_OF_POINT_2      "latitude_of_point_2"
#define SRS_PP_LONGITUDE_OF_POINT_3     "longitude_of_point_3"
#define SRS_PP_LATITUDE_OF_POINT_3      "latitude_of_point_3"
#define SRS_PP_RECTIFIED_GRID_ANGLE     "rectified_grid_angle"
#define SRS_PP_LANDSAT_NUMBER           "landsat_number"
#define SRS_PP_PATH_NUMBER              "path_number"
#define SRS_PP_PERSPECTIVE_POINT_HEIGHT "perspective_point_height"
#define SRS_PP_SATELLITE_HEIGHT         "satellite_height"
#define SRS_PP_FIPSZONE                 "fipszone"
#define SRS_PP_ZONE                     "zone"
#define SRS_PP_LATITUDE_OF_1ST_POINT    "Latitude_Of_1st_Point"
#define SRS_PP_LONGITUDE_OF_1ST_POINT   "Longitude_Of_1st_Point"
#define SRS_PP_LATITUDE_OF_2ND_POINT    "Latitude_Of_2nd_Point"
#define SRS_PP_LONGITUDE_OF_2ND_POINT   "Longitude_Of_2nd_Point"

#define SRS_UL_METER            "Meter"
#define SRS_UL_FOOT             "Foot (International)" /* or just "FOOT"? */
#define SRS_UL_FOOT_CONV                    "0.3048"
#define SRS_UL_US_FOOT          "Foot_US" /* or "US survey foot" from EPSG */
#define SRS_UL_US_FOOT_CONV                 "0.3048006096012192"
#define SRS_UL_NAUTICAL_MILE    "Nautical Mile"
#define SRS_UL_NAUTICAL_MILE_CONV           "1852.0"
#define SRS_UL_LINK             "Link"          /* Based on US Foot */
#define SRS_UL_LINK_CONV                    "0.20116684023368047"
#define SRS_UL_CHAIN            "Chain"         /* based on US Foot */
#define SRS_UL_CHAIN_CONV                   "20.116684023368047"
#define SRS_UL_ROD              "Rod"           /* based on US Foot */
#define SRS_UL_ROD_CONV                     "5.02921005842012"
#define SRS_UL_LINK_Clarke      "Link_Clarke"          
#define SRS_UL_LINK_Clarke_CONV              "0.2011661949"

#define SRS_UA_DEGREE           "degree"
#define SRS_UA_DEGREE_CONV                  "0.0174532925199433"
#define SRS_UA_RADIAN           "radian"

#define SRS_PM_GREENWICH        "Greenwich"

#define SRS_DN_NAD27            "North_American_Datum_1927"
#define SRS_DN_NAD83            "North_American_Datum_1983"
#define SRS_DN_WGS72            "WGS_1972"
#define SRS_DN_WGS84            "WGS_1984"

#define SRS_WGS84_SEMIMAJOR     6378137.0                                
#define SRS_WGS84_INVFLATTENING 298.257223563


end_gdb_namespace
end_gtl_namespace

