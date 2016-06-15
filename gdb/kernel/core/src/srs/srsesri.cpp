#include "spatialreference.h" 
//derived from gdal
begin_gtl_namespace
begin_gdb_namespace
static const char *apszGcsNameMapping[] = {
	"North_American_Datum_1983", "GCS_North_American_1983",
	"North_American_Datum_1927", "GCS_North_American_1927",
	"NAD27_CONUS", "GCS_North_American_1927",
	"Reseau_Geodesique_de_Nouvelle_Caledonie_1991-93", "GCS_RGNC_1991-93",
	"Reseau_Geodesique_de_la_Polynesie_Francaise", "GCS_RGPF",
	"Rauenberg_1983", "GCS_RD/83",
	"Phillipine_Reference_System_1992", "GCS_PRS_1992",
	"Potsdam_1983", "GCS_PD/83",
	"Datum_Geodesi_Nasional_1995", "GCS_DGN_1995",
	"Islands_Network_1993", "GCS_ISN_1993",
	"Institut_Geographique_du_Congo_Belge_1955", "GCS_IGCB_1955",
	"IGC_1962_Arc_of_the_6th_Parallel_South", "GCS_IGC_1962_6th_Parallel_South",
	"Jamaica_2001", "GCS_JAD_2001",
	"European_Libyan_1979", "GCS_European_Libyan_Datum_1979",
	"Madrid_1870", "GCS_Madrid_1870_Madrid",
	"Azores_Occidental_Islands_1939", "GCS_Azores_Occidental_1939",
	"Azores_Central_Islands_1948", "GCS_Azores_Central_1948",
	"Azores_Oriental_Islands_1940", "GCS_Azores_Oriental_1940",
	"Lithuania_1994", "GCS_LKS_1994",
	"Libyan_Geodetic_Datum_2006", "GCS_LGD2006",
	//"Lisbon", "GCS_Lisbon_Lisbon",
	"Stockholm_1938", "GCS_RT38",
	"Latvia_1992", "GCS_LKS_1992",
	"Azores_Oriental_Islands_1995", "GCS_Azores_Oriental_1995",
	"Azores_Central_Islands_1948", "GCS_Azores_Central_1948",
	"Azores_Central_Islands_1995", "GCS_Azores_Central_1995",
	"ATF", "GCS_ATF_Paris",
	//"ITRF_2000", "GCS_MONREF_1997",
	"Faroe_Datum_1954", "GCS_FD_1954",
	"Vietnam_2000", "GCS_VN_2000",
	//"Belge_1950", "GCS_Belge_1950_Brussels",
	"Qatar_1948", "GCS_Qatar_1948",
	"Qatar", "GCS_Qatar_1974",
	"Kuwait_Utility", "GCS_KUDAMS",
	"ED_1950_16", "GCS_European_1950",
	"SAD_1969_Mean", "GCS_South_American_1969",
	"Sphere_of_Radius_6370997m", "GCS_Sphere_ARC_INFO",
	"Australian_Geodetic_1966", "GCS_Australian_1966",
	"Australian_Geodetic_1984", "GCS_Australian_1984",
	"AGD84", "GCS_Australian_1984",
	"AGD66", "GCS_Australian_1966",
	"Rome_1940", "GCS_Monte_Mario",
	"Tokyo_Japan", "GCS_Tokyo",
	"Graciosa_Base_SW_1948_1", "GCS_Graciosa_Base_SW_1948",
	"Datum_Lisboa_Bessel_1", "GCS_Datum_Lisboa_Bessel",
	"Datum_Lisboa_Hayford_1", "GCS_Datum_Lisboa_Hayford",
	"Observatorio_Metereo_1939_Grupo_Ocidental", "GCS_Observ_Meteorologico_1939",
	"Porto_Santo_1936_1", "GCS_Porto_Santo_1936",
	"Sao_Braz_1", "GCS_Sao_Braz",
	"GDA94", "GCS_GDA_1994",
	"HARN", "GCS_North_American_1983_HARN",
	"NAD83_HARN", "GCS_North_American_1983_HARN",
	"Voirol_1875", "GCS_Voirol_1875",
	"Voirol_1960", "GCS_Voirol_Unifie_1960",
	"Ain_el_Abd_1970_Bahrain", "GCS_Ain_el_Abd_1970",
	"ED_1950_ED77", "GCS_European_1950_ED77",
	"Naparima_1955_2", "GCS_Naparima_1955",
	"Aratu_Brazil_Campos_Espirito_Santo_and_Santos_basins", "GCS_Aratu",
	"Camacupa_Angola_1", "GCS_Camacupa",
	"Cape_1", "GCS_Cape",
	"Carthage_Tunisia", "GCS_Carthage",
	"Deir_ez_Zor_2", "GCS_Deir_ez_Zor",
	"Old_Egyptian_1907", "GCS_Egypt_1907",
	"PSAD56", "GCS_Provisional_S_American_1956",
	"Indian 1975", "GCS_Indian_1975",
	"Indian_1960_1", "GCS_Indian_1960",
	"Kalianpur_1937_1", "GCS_Kalianpur_1937",
	"Kertau_1948", "GCS_Kertau",
	"Kertau_1968", "GCS_Kertau",
	"Luzon", "GCS_Luzon_1911",
	"Malongo_1987_1", "GCS_Malongo_1987",
	"Minna_Cameroon", "GCS_Minna",
	"Mporaloko_1", "GCS_Mporaloko",
	"Nahrwan_Oman", "GCS_Nahrwan_1967",
	"Naparima_BWI", "GCS_Naparima_1972",
	"Geodetic_Datum_1949", "GCS_New_Zealand_1949",
	"Qatar_National", "GCS_Qatar_1974",
	"SAD_1969_Mean", "GCS_South_American_1969",
	"Tananarive_Observatory_1925", "GCS_Tananarive_1925",
	"Tananarive", "GCS_Tananarive_1925",
	"Ireland_1965", "GCS_TM65",
	"DE_DHDN_whole_country_2001_to_ETRS89", "GCS_Deutsches_Hauptdreiecksnetz",
	"Belge_1972_1", "GCS_Belge_1972",
	"WGS_72", "GCS_WGS_1972",
	"JGD2000", "GCS_JGD_2000",
	"NZGD49", "GCS_New_Zealand_1949",
	"CH1903_1", "GCS_CH1903",
	"DE_42/83_to_ETRS89", "GCS_Pulkovo_1942",
	"DE_42_83_to_ETRS89", "GCS_Pulkovo_1942",
	"Amersfoort_1", "GCS_Amersfoort",
	"CH1903+_L+T1997", "GCS_CH1903+",
	"Ord_Survey_G_Britain_1936", "GCS_OSGB_1936",
	"European_Datum_1950", "GCS_European_1950",
	"Geocentric_Datum_of_Australia_1994", "GCS_GDA_1994",
	"NAD83_High_Accuracy_Regional_Network", "GCS_North_American_1983_HARN",
	"Bogota_1975", "GCS_Bogota",
	"North_American_Datum_1927_CGQ77", "GCS_NAD_1927_CGQ77",
	"North_American_Datum_1927_1976", "GCS_NAD_1927_Definition_1976",
	"European_Datum_1950_1977", "GCS_European_1950_ED77",
	"WGS_1972_Transit_Broadcast_Ephemeris", "GCS_WGS_1972_BE",
	"Greek_Geodetic_Reference_System_1987", "GCS_GGRS_1987",
	"Militar_Geographische_Institute", "GCS_MGI",
	"ED50", "GCS_European_1950",
	"ETRS89", "GCS_ETRS_1989",
	NULL, NULL };

static const char *apszGcsNameMappingBasedOnProjCS[] = {
	"EUREF_FIN_TM35FIN", "GCS_ETRS_1989", "GCS_EUREF_FIN",
	"Nord_Maroc_Degree", "GCS_Merchich", "GCS_Merchich_Degree",
	"Sahara_Degree", "GCS_Merchich", "GCS_Merchich_Degree",
	"Sud_Maroc_Degree", "GCS_Merchich", "GCS_Merchich_Degree",
	"Merchich_Degree_UTM_Zone_28N", "GCS_Merchich", "GCS_Merchich_Degree",
	"Lambert_Conformal_Conic", "GCS_Merchich", "GCS_Merchich_Degree",
	"UTM", "GCS_Merchich", "GCS_Merchich_Degree",
	"UTM_Zone_28_Northern_Hemisphere", "GCS_Merchich", "GCS_Merchich_Degree",
	"Portuguese_National_Grid", "GCS_Lisbon", "GCS_Lisbon_Lisbon",
	"Belge_Lambert_1950", "GCS_Belge_1950", "GCS_Belge_1950_Brussels",
	"MONREF_1997_UTM_Zone_46N", "GCS_ITRF_2000", "GCS_MONREF_1997",
	"MONREF_1997_UTM_Zone_47N", "GCS_ITRF_2000", "GCS_MONREF_1997",
	NULL, NULL, NULL };



static const char *apszGcsNameMappingBasedOnUnit[] = {
	"Voirol_Unifie_1960", "Degree", "GCS_Voirol_Unifie_1960_Degree",
	"Voirol_1960", "Degree", "GCS_Voirol_Unifie_1960_Degree",
	"Voirol 1960", "Degree", "GCS_Voirol_Unifie_1960_Degree",
	"Voirol_1875", "Degree", "GCS_Voirol_1875_Degree",
	"Voirol 1875", "Degree", "GCS_Voirol_1875_Degree",
	"NTF", "Grad", "GCS_NTF_Paris",
	NULL, NULL, NULL };

static const char *apszGcsNameMappingBasedPrime[] = {
	"Bern_1898", "Bern", "GCS_Bern_1898_Bern",
	"Madrid_1870", "Madrid", "GCS_Madrid_1870_Madrid",
	"MGI", "Ferro", "GCS_MGI_Ferro",
	"MGI", "Stockholm", "GCS_RT38_Stockholm",
	"Monte_Mario", "Rome", "GCS_Monte_Mario_Rome",
	"NGO_1948", "Oslo", "GCS_NGO_1948_Oslo",
	"S_JTSK", "Ferro", "GCS_S_JTSK_Ferro",
	"Stockholm_1938", "Stockholm", "GCS_RT38_Stockholm",
	NULL, NULL, NULL };

static const char *apszInvFlatteningMapping[] = {
	"293.464999999", "293.465",
	"293.465000003", "293.465",
	"293.465073361", "293.465",
	"293.466020000", "293.46602",
	"293.466021293", "293.46602",
	"293.4663077168286", "293.466307656",
	"293.4664236085404", "293.466307656",
	"294.2606763690", "294.260676369",
	"294.9786981999", "294.9786982",
	"294.978698213", "294.9786982",
	"295.9999999999", "296.0",
	"297.0000000000", "297.0",
	"297.0000000284", "297.0",
	"297.0000535480", "297.0",
	"298.2499972761", "298.25",
	"298.2500000654", "298.25",
	"298.2500112226", "298.25",
	"298.256999999", "298.257",
	"298.2600000000", "298.26",
	"298.2571643544962", "298.257223563",
	"298.25716435449", "298.257222101",
	"298.257222096042", "298.257222101",
	"298.25722210100", "298.257222101",
	"298.25722356299", "298.257223563",
	"298.25722356300", "298.257223563",
	"298.25999858999", "298.26",
	"298.2684109950054", "298.268410995005",
	"298.2999", "298.3",
	"298.3000", "298.3",
	"299.1527033239203", "299.1528128",
	"299.15281280000", "299.1528128",
	"299.15281283", "299.1528128",
	"299.15281310607", "299.1528128",
	"299.15281327254", "299.1528128",
	"299.32496460000", "299.3249646",
	"299.32496405862", "299.3249646",
	"299.32497531503", "299.3249646",
	"300.80158474106", "300.8017",
	"300.80169943849", "300.8017",
	"300.80169999999", "300.8017",
	"300.80170000000", "300.8017",
	"300.80170009712", "300.8017",
	NULL, NULL };

static const char *apszParamValueMapping[] = {
	"Cassini", "false_easting", "283799.9999", "283800.0",
	"Cassini", "false_easting", "132033.9199", "132033.92",
	"Cassini", "false_northing", "214499.9999", "214500.0",
	"Cassini", "false_northing", "62565.9599", "62565.95",
	"Transverse_Mercator", "false_easting", "499999.1331", "500000.0",
	"Transverse_Mercator", "false_easting", "299999.4798609", "300000.0",
	"Transverse_Mercator", "false_northing", "399999.30648", "400000.0",
	"Transverse_Mercator", "false_northing", "499999.1331", "500000.0",
	"Transverse_Mercator", "central_meridian","51.21666666666668", "51.21666666666667",
	"Transverse_Mercator", "Scale_Factor", "0.999601272", "0.9996012717",
	"Lambert_Conformal_Conic", "central_meridian", "-90.33333333333334", "-90.33333333333333",
	"Lambert_Conformal_Conic", "central_meridian", "-76.83333333333334", "-76.83333333333333",
	"Krovak", "longitude_of_center", "24.83333333333334", "24.83333333333333",
	"Hotine_Oblique_Mercator_Azimuth_Center", "longitude_of_center", "7.439583333333334", "7.439583333333333",
	"Hotine_Oblique_Mercator_Azimuth_Center", "latitude_of_center",	"46.95240555555557", "46.95240555555556",
	NULL, NULL, NULL, NULL };

static const char *apszParamNameMapping[] = {
	"Lambert_Azimuthal_Equal_Area", "longitude_of_center", "Central_Meridian",
	"Lambert_Azimuthal_Equal_Area", "Latitude_Of_Center", "Latitude_Of_Origin",
	"Miller_Cylindrical", "longitude_of_center", "Central_Meridian",
	"Gnomonic", "central_meridian", "Longitude_Of_Center",
	"Gnomonic", "latitude_of_origin", "Latitude_Of_Center",
	"Orthographic", "central_meridian", "Longitude_Of_Center",
	"Orthographic", "latitude_of_origin", "Latitude_Of_Center",
	"New_Zealand_Map_Grid", "central_meridian", "Longitude_Of_Origin",
	NULL, NULL, NULL };

static const char *apszDeleteParametersBasedOnProjection[] = {
	"Stereographic_South_Pole", "scale_factor",
	"Stereographic_North_Pole", "scale_factor",
	"Mercator", "scale_factor",
	"Miller_Cylindrical", "latitude_of_center",
	"Equidistant_Cylindrical", "pseudo_standard_parallel_1",
	"Equidistant_Cylindrical", "latitude_of_origin",
	"Plate_Carree", "latitude_of_origin",
	"Plate_Carree", "pseudo_standard_parallel_1",
	"Plate_Carree", "standard_parallel_1",
	"Hotine_Oblique_Mercator_Azimuth_Center", "rectified_grid_angle",
	"Hotine_Oblique_Mercator_Azimuth_Natural_Origin", "rectified_grid_angle",
	NULL, NULL };

static const char *apszAddParametersBasedOnProjection[] = {
	"Cassini", "scale_factor", "1.0",
	"Mercator", "standard_parallel_1", "0.0",
	NULL, NULL, NULL };

static int statePlaneZoneMapping[] = {
	/* old zone code, prj code, new zone code */
	3126, -1,    101,
	3151, -1,    102,
	3176, -1,    202,
	3201, -1,    203,
	3226, -1,    301,
	3251, -1,    302,
	3326, -1,    403,
	3351, -1,    404,
	3376, 26945, 405,
	3426, -1,    407,
	3451, -1,    501,
	3476, -1,    502,
	3526, -1,    600,
	3551, -1,    700,
	3576, -1,    903,
	3626, -1,    902,
	3651, -1,    1001,
	3676, -1,    1002,
	3726, -1,    1102,
	3751, -1,    1103,
	3776, -1,    1201,
	3801, -1,    1202,
	3826, -1,    1301,
	3851, -1,    1302,
	3876, -1,    1401,
	3926, -1,    1501,
	3951, -1,    1502,
	3976, -1,    1601,
	4026, -1,    1701,
	6426, -1,    1703,
	4076, -1,    1801,
	4101, -1,    1802,
	4126, -1,    1900,
	4151, -1,    2001,
	4176, -1,    2002,
	4226, -1,    2102,
	4251, -1,    2103,
	6351, -1,    2111,
	6376, -1,    2112,
	6401, -1,    2113,
	4276, -1,    2201,
	4326, -1,    2203,
	4351, -1,    2301,
	4376, -1,    2302,
	4400, 32045, 3400,
	4401, -1,    2401,
	4426, -1,    2402,
	4451, -1,    2403,
	4476, 32100, 2500,
	4476, -1,    2501,
	4701, 32111,	2900,
	4801, 2260,  3101,
	4801, 32115, 3101,
	4526, -1,    2503,
	4551, -1,    2601,
	4576, -1,    2602,
	4626, -1,    2702,
	4651, -1,    2703,
	4676, -1,    2800,
	4726, -1,    3001,
	4751, -1,    3002,
	4776, -1,    3003,
	4826, -1,    3102,
	4851, -1,    3103,
	4876, -1,    3104,
	4926, -1,    3301,
	4951, -1,    3302,
	4976, -1,    3401,
	5026, -1,    3501,
	5051, -1,    3502,
	5076, -1,    3601,
	5126, -1,    3701,
	5151, -1,    3702,
	5176, -1,    3800,
	5226, -1,    3902,
	5251, -1,    4001,
	5276, -1,    4002,
	5301, -1,    4100,
	5326, -1,    4201,
	5351, -1,    4202,
	5376, -1,    4203,
	5401, -1,    4204,
	5426, -1,    4205,
	5451, -1,    4301,
	5476, -1,    4302,
	5501, -1,    4303,
	5526, -1,    4400,
	5551, -1,    4501,
	5576, -1,    4502,
	5601, -1,    4601,
	5626, -1,    4602,
	5651, -1,    4701,
	5676, -1,    4702,
	5701, -1,    4801,
	5726, -1,    4802,
	5751, -1,    4803,
	5776, -1,    4901,
	5801, -1,    4902,
	5826, -1,    4903,
	5851, -1,    4904,
	6101, -1,    5001,
	6126, -1,    5002,
	6151, -1,    5003,
	6176, -1,    5004,
	6201, -1,    5005,
	6226, -1,    5006,
	6251, -1,    5007,
	6276, -1,    5008,
	6301, -1,    5009,
	6326, -1,    5010,
	5876, -1,    5101,
	5901, -1,    5102,
	5926, -1,    5103,
	5951, -1,    5104,
	5976, -1,    5105,
	6001, -1,    5201,
	6026, -1,    5200,
	6076, -1,    5200,
	6051, -1,    5202,
	0,    0,     0
};

/* This is not a complete mapping. Need to add more. */
static int statePlanePcsCodeToZoneCode[] = {
	/* pcs code, state plane prj str index*/
	2222, 2016,
	2223, 2026,
	2224, 2036,
	2225, 4012,
	2226, 4022,
	2227, 4032,
	2228, 4042,
	2229, 4052,
	2230, 4062,
	2231, 5012,
	2232, 5022,
	2233, 5032,
	2234, 6002,
	2235, 7002,
	2236, 9012,
	2237, 9022,
	2238, 9032,
	2239, 10012,
	2240, 10022,
	2241, 11012,
	2242, 11022,
	2243, 11032,
	2251, 21116,
	2252, 21126,
	2253, 21136,
	2256, 25006,
	2265, 33016,
	2266, 33026,
	2965, 13012,
	2966, 13022,
	2246, 16012,
	2247, 16022,
	2248, 19002,
	2249, 20012,
	2250, 20022,
	2254, 23012,
	2255, 23022,
	2257, 30012,
	2258, 30022,
	2259, 30032,
	2260, 31012,
	2261, 31022,
	2262, 31032,
	2263, 31042,
	2264, 32002,
	2267, 35012,
	2268, 35022,
	2269, 36016,
	2270, 36026,
	2271, 37012,
	2272, 37022,
	2273, 39006,
	2274, 41002,
	2275, 42012,
	2276, 42022,
	2277, 42032,
	2278, 42042,
	2279, 42052,
	2280, 43016,
	2281, 43026,
	2282, 43036,
	2283, 45012,
	2284, 45022,
	2285, 46012,
	2286, 46022,
	2287, 48012,
	2288, 48022,
	2289, 48032,
	2867, 2015,
	2868, 2025,
	2869, 2035,
	2896, 21115,
	2897, 21125,
	2898, 21135,
	2901, 25005,
	2909, 33015,
	2910, 33025,
	2913, 36015,
	2914, 36025,
	2921, 43015,
	2922, 43025,
	2923, 43035,
	2870, 4013,
	2871, 4023,
	2872, 4033,
	2873, 4043,
	2874, 4053,
	2875, 4063,
	2876, 5013,
	2877, 5023,
	2878, 5033,
	2879, 6003,
	2880, 7003,
	2881, 9013,
	2882, 9023,
	2883, 9033,
	2884, 10013,
	2885, 10023,
	2886, 11013,
	2887, 11023,
	2888, 11033,
	2967, 13013,
	2968, 13023,
	2891, 16013,
	2892, 16023,
	2893, 19003,
	2894, 20013,
	2895, 20023,
	2899, 23013,
	2900, 23023,
	2902, 30013,
	2903, 30023,
	2904, 30033,
	2905, 31013,
	2906, 31023,
	2907, 31033,
	2908, 31043,
	2911, 35013,
	2912, 35023,
	2915, 41003,
	2916, 42013,
	2917, 42023,
	2918, 42033,
	2919, 42043,
	2920, 42053,
	2924, 45013,
	2925, 45023,
	2926, 46013,
	2927, 46023,
	2928, 48013,
	2929, 48023,
	2930, 48033,
	// following are state systems (not complete)
	2964, 102965,
	2991, 102991,
	2992, 102992,
	2993, 102993,
	2994, 102994,
	// following are NAD 1983 SPCS Zone  
	26929, 1011,
	26930, 1021,
	26931, 50011,
	26932, 50021,
	26933, 50031,
	26934, 50041,
	26935, 50051,
	26936, 50061,
	26937, 50071,
	26938, 50081,
	26939, 50091,
	26940, 50101,
	26948, 2011,
	26949, 2021,
	26950, 2031,
	26951, 3011,
	26952, 3021,
	26941, 4011,
	26942, 4021,
	26943, 4031,
	26944, 4041,
	26945, 4051,
	26946, 4061,
	26953, 5011,
	26954, 5021,
	26955, 5031,
	26956, 6001,
	26957, 7001,
	26958, 9011,
	26959, 9021,
	26960, 9031,
	26966, 10011,
	26967, 10021,
	26961, 51011,
	26962, 51021,
	26963, 51031,
	26964, 51041,
	26965, 51051,
	26968, 11011,
	26969, 11021,
	26970, 11031,
	26971, 12011,
	26972, 12021,
	26973, 13011,
	26974, 13021,
	26975, 14011,
	26976, 14021,
	26977, 15011,
	26978, 15021,
	26979, 16011,
	26980, 16021,
	26981, 17011,
	26982, 17021,
	26983, 18011,
	26984, 18021,
	26985, 19001,
	26986, 20011,
	26987, 20021,
	26988, 21111,
	26989, 21121,
	26990, 21131,
	26991, 22011,
	26992, 22021,
	26993, 22031,
	26994, 23011,
	26995, 23021,
	26996, 24011,
	26997, 24021,
	26998, 24031,
	32100, 25001,
	32104, 26001,
	32107, 27011,
	32108, 27021,
	32109, 27031,
	32110, 28001,
	32111, 29001,
	32112, 30011,
	32113, 30021,
	32114, 30031,
	32115, 31011,
	32116, 31021,
	32117, 31031,
	32118, 31041,
	32119, 32001,
	32120, 33011,
	32121, 33021,
	32122, 34011,
	32123, 34021,
	32124, 35011,
	32125, 35021,
	32126, 36011,
	32127, 36021,
	32128, 37011,
	32129, 37021,
	32130, 38001,
	32133, 39001,
	32134, 40011,
	32135, 40021,
	32136, 41001,
	32137, 42011,
	32138, 42021,
	32139, 42031,
	32140, 42041,
	32141, 42051,
	32142, 43011,
	32143, 43021,
	32144, 43031,
	32145, 44001,
	32146, 45011,
	32147, 45021,
	32148, 46011,
	32149, 46021,
	32150, 47011,
	32151, 47021,
	32152, 48011,
	32153, 48021,
	32154, 48031,
	32155, 49011,
	32156, 49021,
	32157, 49031,
	32158, 49041,
	32161, 52000,
	65161, 54001,
	0,     0
};

/* ==================================================================== */
/*      WISCRS Table                                                    */
/* ==================================================================== */
static double apszWISCRS_LCC_meter[] = {
	// Central_Meridian, Latitude_Of_Origin, SR code
	-91.1527777777, 46.6696483772, 103303.0,
	-92.4577777777, 45.8987148658, 103306.0,
	-91.2944444444, 44.9778568986, 103308.0,
	-89.3944444444, 43.4625466458, 103310.0,
	-90.9388888888, 43.2000556050, 103311.0,
	-89.4222222222, 43.0695160375, 103312.0,
	-91.2888888888, 45.8722811263, 103317.0,
	-89.8388888888, 42.6375622769, 103322.0,
	-89.2416666666, 43.8070001177, 103323.0,
	-89.8388888888, 42.6375622769, 103332.0,
	-89.0333333333, 45.1542371052, 103333.0,
	-89.7700000000, 44.9009044236, 103336.0,
	-89.2416666666, 43.8070001177, 103338.0,
	-90.6416666666, 44.0000739286, 103341.0,
	-89.5444444444, 45.7042237702, 103343.0,
	-92.2277777777, 44.6361488719, 103346.0,
	-92.2277777777, 44.6361488719, 103347.0,
	-89.5000000000, 44.4168239752, 103349.0,
	-90.4305555555, 43.3223129275, 103352.0,
	-91.1166666666, 45.9000991313, 103356.0,
	-90.4833333333, 45.1778220858, 103360.0,
	-90.7833333333, 43.5750329397, 103362.0,
	-89.4888888888, 46.0778440905, 103363.0,
	-88.5416666667, 42.6694620969, 103364.0,
	-91.7833333333, 45.9612198333, 103365.0,
	-89.2416666666, 44.1139440458, 103369.0,
	-90.0000000000, 44.3625954694, 103371.0,
	0.0, 0,0, 0,0
};

static double apszWISCRS_TM_meter[] = {
	// Central_Meridian, Latitude_Of_Origin, SR code 
	-90.0000000000, 43.3666666666, 103300.0,
	-90.6222222222, 45.7061111111, 103301.0,
	-91.8500000000, 45.1333333333, 103302.0,
	-88.0000000000, 43.0000000000, 103304.0,
	-91.7972222222, 43.4813888888, 103305.0,
	-88.5000000000, 42.7194444444, 103307.0,
	-90.7083333333, 43.6000000000, 103309.0,
	-88.7750000000, 41.4722222222, 103313.0,
	-87.2722222222, 44.4000000000, 103314.0,
	-91.9166666666, 45.8833333333, 103315.0,
	-91.8944444444, 44.4083333333, 103316.0,
	-88.1416666666, 45.4388888888, 103318.0,
	-88.5000000000, 42.7194444444, 103319.0,
	-88.6333333333, 44.0055555556, 103320.0,
	-90.8000000000, 41.4111111111, 103321.0,
	-90.1611111111, 42.5388888888, 103324.0,
	-90.2555555555, 45.4333333333, 103325.0,
	-90.8442965194, 44.2533351277, 103326.0,
	-88.7750000000, 41.4722222222, 103327.0,
	-90.0000000000, 43.3666666666, 103328.0,
	-87.8944444444, 42.2166666666, 103329.0,
	-87.5500000000, 43.2666666666, 103330.0,
	-91.3166666666, 43.4511111111, 103331.0,
	-89.7333333333, 44.8444444444, 103334.0,
	-87.5500000000, 43.2666666666, 103335.0,
	-87.7111111111, 44.6916666666, 103337.0,
	-88.4166666666, 44.7166666666, 103339.0,
	-87.8944444444, 42.2166666666, 103340.0,
	-87.9083333333, 44.3972222222, 103342.0,
	-88.5000000000, 42.7194444444, 103344.0,
	-87.8944444444, 42.2166666666, 103345.0,
	-92.6333333333, 44.6611111111, 103348.0,
	-90.4888888889, 44.5555555556, 103350.0,
	-87.8944444444, 42.2166666666, 103351.0,
	-89.0722222222, 41.9444444444, 103353.0,
	-91.0666666666, 43.9194444444, 103354.0,
	-89.9000000000, 42.8194444444, 103355.0,
	-88.6055555556, 44.0361111111, 103357.0,
	-87.5500000000, 43.2666666666, 103358.0,
	-92.6333333333, 44.0361111111, 103359.0,
	-91.3666666666, 43.1611111111, 103361.0,
	-88.0638888888, 42.9180555555, 103366.0,
	-88.2250000000, 42.5694444444, 103367.0,
	-88.8166666666, 43.4202777777, 103368.0,
	-88.5000000000, 42.7194444444, 103370.0,
	0.0, 0,0, 0,0
};

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////


void  SetNewName(SpatialReference* pOgr, const char* keyName, const char* newName);
int   RemapImgWGSProjcsName(SpatialReference* pOgr, const char* pszProjCSName,
	const char* pszProgCSName);
int   RemapImgUTMNames(SpatialReference* pOgr, const char* pszProjCSName,
	const char* pszProgCSName, char **mappingTable);
int   RemapNameBasedOnKeyName(SpatialReference* pOgr, const char* pszName,
	const char* pszkeyName, char **mappingTable);
int   RemapNamesBasedOnTwo(SpatialReference* pOgr, const char* name1, const char* name2,
	char **mappingTable, long nTableStepSize,
	char** pszkeyNames, long nKeys);
int   RemapPValuesBasedOnProjCSAndPName(SpatialReference* pOgr,
	const char* pszProgCSName, char **mappingTable);
int   RemapPNamesBasedOnProjCSAndPName(SpatialReference* pOgr,
	const char* pszProgCSName, char **mappingTable);
int   DeleteParamBasedOnPrjName(SpatialReference* pOgr,
	const char* pszProjectionName, char **mappingTable);
int   AddParamBasedOnPrjName(SpatialReference* pOgr,
	const char* pszProjectionName, char **mappingTable);
int   RemapGeogCSName(SpatialReference* pOgr, const char *pszGeogCSName);

static bool   findCodeFromDict(const char* pszDictFile, const char* CSName, char* code);

static const char *apszProjMapping[] = {
	"Albers", SRS_PT_ALBERS_CONIC_EQUAL_AREA,
	"Cassini", SRS_PT_CASSINI_SOLDNER,
	"Equidistant_Cylindrical", SRS_PT_EQUIRECTANGULAR,
	"Plate_Carree", SRS_PT_EQUIRECTANGULAR,
	"Hotine_Oblique_Mercator_Azimuth_Natural_Origin",
	SRS_PT_HOTINE_OBLIQUE_MERCATOR,
	"Lambert_Conformal_Conic", SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP,
	"Lambert_Conformal_Conic", SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP,
	"Van_der_Grinten_I", SRS_PT_VANDERGRINTEN,
	SRS_PT_TRANSVERSE_MERCATOR, SRS_PT_TRANSVERSE_MERCATOR,
	"Gauss_Kruger", SRS_PT_TRANSVERSE_MERCATOR,
	"Mercator", SRS_PT_MERCATOR_1SP,
	NULL, NULL };

static const char *apszAlbersMapping[] = {
	SRS_PP_CENTRAL_MERIDIAN, SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_LATITUDE_OF_ORIGIN, SRS_PP_LATITUDE_OF_CENTER,
	"Central_Parallel", SRS_PP_LATITUDE_OF_CENTER,
	NULL, NULL };

static const char *apszECMapping[] = {
	SRS_PP_CENTRAL_MERIDIAN, SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_LATITUDE_OF_ORIGIN, SRS_PP_LATITUDE_OF_CENTER,
	NULL, NULL };

static const char *apszMercatorMapping[] = {
	SRS_PP_STANDARD_PARALLEL_1, SRS_PP_LATITUDE_OF_ORIGIN,
	NULL, NULL };

static const char *apszPolarStereographicMapping[] = {
	SRS_PP_STANDARD_PARALLEL_1, SRS_PP_LATITUDE_OF_ORIGIN,
	NULL, NULL };

static const char *apszOrthographicMapping[] = {
	"Longitude_Of_Center", SRS_PP_CENTRAL_MERIDIAN,
	"Latitude_Of_Center", SRS_PP_LATITUDE_OF_ORIGIN,
	NULL, NULL };

static const char *apszLambertConformalConicMapping[] = {
	"Central_Parallel", SRS_PP_LATITUDE_OF_ORIGIN,
	NULL, NULL };

static char **papszDatumMapping = NULL;
static void* hDatumMappingMutex = NULL;

static const char *apszDefaultDatumMapping[] = {
	"6267", "North_American_1927", SRS_DN_NAD27,
	"6269", "North_American_1983", SRS_DN_NAD83,
	NULL, NULL, NULL };

static const char *apszSpheroidMapping[] = {
	"WGS_84", "WGS_1984",
	"WGS_72", "WGS_1972",
	"GRS_1967_Modified", "GRS_1967_Truncated",
	"Krassowsky_1940", "Krasovsky_1940",
	"Everest_1830_1937_Adjustment", "Everest_Adjustment_1937",
	NULL, NULL };

static const char *apszUnitMapping[] = {
	"Meter", "meter",
	"Meter", "metre",
	"Foot", "foot",
	"Foot", "feet",
	"Foot", "international_feet",
	"Foot_US", SRS_UL_US_FOOT,
	"Foot_Clarke", "clarke_feet",
	"Degree", "degree",
	"Degree", "degrees",
	"Degree", SRS_UA_DEGREE,
	"Radian", SRS_UA_RADIAN,
	NULL, NULL };

/* -------------------------------------------------------------------- */
/*      Table relating USGS and ESRI state plane zones.                 */
/* -------------------------------------------------------------------- */
static const int anUsgsEsriZones[] =
{
	101, 3101,
	102, 3126,
	201, 3151,
	202, 3176,
	203, 3201,
	301, 3226,
	302, 3251,
	401, 3276,
	402, 3301,
	403, 3326,
	404, 3351,
	405, 3376,
	406, 3401,
	407, 3426,
	501, 3451,
	502, 3476,
	503, 3501,
	600, 3526,
	700, 3551,
	901, 3601,
	902, 3626,
	903, 3576,
	1001, 3651,
	1002, 3676,
	1101, 3701,
	1102, 3726,
	1103, 3751,
	1201, 3776,
	1202, 3801,
	1301, 3826,
	1302, 3851,
	1401, 3876,
	1402, 3901,
	1501, 3926,
	1502, 3951,
	1601, 3976,
	1602, 4001,
	1701, 4026,
	1702, 4051,
	1703, 6426,
	1801, 4076,
	1802, 4101,
	1900, 4126,
	2001, 4151,
	2002, 4176,
	2101, 4201,
	2102, 4226,
	2103, 4251,
	2111, 6351,
	2112, 6376,
	2113, 6401,
	2201, 4276,
	2202, 4301,
	2203, 4326,
	2301, 4351,
	2302, 4376,
	2401, 4401,
	2402, 4426,
	2403, 4451,
	2500,    0,
	2501, 4476,
	2502, 4501,
	2503, 4526,
	2600,    0,
	2601, 4551,
	2602, 4576,
	2701, 4601,
	2702, 4626,
	2703, 4651,
	2800, 4676,
	2900, 4701,
	3001, 4726,
	3002, 4751,
	3003, 4776,
	3101, 4801,
	3102, 4826,
	3103, 4851,
	3104, 4876,
	3200, 4901,
	3301, 4926,
	3302, 4951,
	3401, 4976,
	3402, 5001,
	3501, 5026,
	3502, 5051,
	3601, 5076,
	3602, 5101,
	3701, 5126,
	3702, 5151,
	3800, 5176,
	3900,    0,
	3901, 5201,
	3902, 5226,
	4001, 5251,
	4002, 5276,
	4100, 5301,
	4201, 5326,
	4202, 5351,
	4203, 5376,
	4204, 5401,
	4205, 5426,
	4301, 5451,
	4302, 5476,
	4303, 5501,
	4400, 5526,
	4501, 5551,
	4502, 5576,
	4601, 5601,
	4602, 5626,
	4701, 5651,
	4702, 5676,
	4801, 5701,
	4802, 5726,
	4803, 5751,
	4901, 5776,
	4902, 5801,
	4903, 5826,
	4904, 5851,
	5001, 6101,
	5002, 6126,
	5003, 6151,
	5004, 6176,
	5005, 6201,
	5006, 6226,
	5007, 6251,
	5008, 6276,
	5009, 6301,
	5010, 6326,
	5101, 5876,
	5102, 5901,
	5103, 5926,
	5104, 5951,
	5105, 5976,
	5201, 6001,
	5200, 6026,
	5200, 6076,
	5201, 6051,
	5202, 6051,
	5300,    0,
	5400,    0
};

/* -------------------------------------------------------------------- */
/*      Datum Mapping functions and definitions                         */
/* -------------------------------------------------------------------- */
/* TODO adapt existing code and test */
#define DM_IDX_EPSG_CODE            0
#define DM_IDX_ESRI_NAME            1
#define DM_IDX_EPSG_NAME            2
#define DM_ELT_SIZE                 3

#define DM_GET_EPSG_CODE(map, i)          map[(i)*DM_ELT_SIZE + DM_IDX_EPSG_CODE]
#define DM_GET_ESRI_NAME(map, i)          map[(i)*DM_ELT_SIZE + DM_IDX_ESRI_NAME]
#define DM_GET_EPSG_NAME(map, i)          map[(i)*DM_ELT_SIZE + DM_IDX_EPSG_NAME]

char *DMGetEPSGCode(int i) { return DM_GET_EPSG_CODE(papszDatumMapping, i); }
char *DMGetESRIName(int i) { return DM_GET_ESRI_NAME(papszDatumMapping, i); }
char *DMGetEPSGName(int i) { return DM_GET_EPSG_NAME(papszDatumMapping, i); }


void EPSGDatumNameMassage(char ** ppszDatum);

/************************************************************************/
/*                           ESRIToUSGSZone()                           */
/*                                                                      */
/*      Convert ESRI style state plane zones to USGS style state        */
/*      plane zones.                                                    */
/************************************************************************/

static int ESRIToUSGSZone(int nESRIZone)

{
	int         nPairs = sizeof(anUsgsEsriZones) / (2 * sizeof(int));
	int         i;

	for (i = 0; i < nPairs; i++)
	{
		if (anUsgsEsriZones[i * 2 + 1] == nESRIZone)
			return anUsgsEsriZones[i * 2];
	}

	return 0;
}

/************************************************************************/
/*                          MorphNameToESRI()                           */
/*                                                                      */
/*      Make name ESRI compatible. Convert spaces and special           */
/*      characters to underscores and then strip down.                  */
/************************************************************************/

static void MorphNameToESRI(char ** ppszName)

{
	int         i, j;
	char        *pszName = *ppszName;

	if (pszName[0] == '\0')
		return;

	/* -------------------------------------------------------------------- */
	/*      Translate non-alphanumeric values to underscores.               */
	/* -------------------------------------------------------------------- */
	for (i = 0; pszName[i] != '\0'; i++)
	{
		if (pszName[i] != '+'
			&& !(pszName[i] >= 'A' && pszName[i] <= 'Z')
			&& !(pszName[i] >= 'a' && pszName[i] <= 'z')
			&& !(pszName[i] >= '0' && pszName[i] <= '9'))
		{
			pszName[i] = '_';
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Remove repeated and trailing underscores.                       */
	/* -------------------------------------------------------------------- */
	for (i = 1, j = 0; pszName[i] != '\0'; i++)
	{
		if (pszName[j] == '_' && pszName[i] == '_')
			continue;

		pszName[++j] = pszName[i];
	}
	if (pszName[j] == '_')
		pszName[j] = '\0';
	else
		pszName[j + 1] = '\0';
}

/************************************************************************/
/*                     CleanESRIDatumMappingTable()                     */
/************************************************************************/

void CleanupESRIDatumMappingTable()

{
	if (papszDatumMapping == NULL)
		return;

	if (papszDatumMapping != (char **)apszDefaultDatumMapping)
	{
		cslDestroy(papszDatumMapping);
		papszDatumMapping = NULL;
	}

	/*if (hDatumMappingMutex != NULL)
	{
		CPLDestroyMutex(hDatumMappingMutex);
		hDatumMappingMutex = NULL;
	}*/
}


/************************************************************************/
/*                       InitDatumMappingTable()                        */
/************************************************************************/

static void InitDatumMappingTable()

{
	//CPLMutexHolderD(&hDatumMappingMutex);
	if (papszDatumMapping != NULL)
		return;

	/* -------------------------------------------------------------------- */
	/*      Try to open the datum.csv file.                                 */
	/* -------------------------------------------------------------------- */
	//const char  *pszFilename = CSVFilename("gdal_datum.csv");
	//FILE * fp = VSIFOpen(pszFilename, "rb");
	CommaSeparatedValues csv(gtl::getDataHome() + std::string("\\gdal_datum.csv"));

	/* -------------------------------------------------------------------- */
	/*      Use simple default set if we can't find the file.               */
	/* -------------------------------------------------------------------- */
	//if (fp == NULL)
	//{
	//	papszDatumMapping = (char **)apszDefaultDatumMapping;
	//	return;
	//}

	/* -------------------------------------------------------------------- */
	/*      Figure out what fields we are interested in.                    */
	/* -------------------------------------------------------------------- */
	char **papszFieldNames = cslDuplicate((CSTRLIST)(csv.getFieldNames()));//CSVReadParseLine(fp);
	int  nDatumCodeField = csv.indexField("DATUM_CODE");//CSLFindString(papszFieldNames, "DATUM_CODE");
	int  nEPSGNameField = csv.indexField("DATUM_NAME");//CSLFindString(papszFieldNames, "DATUM_NAME");
	int  nESRINameField = csv.indexField("ESRI_DATUM_NAME");//CSLFindString(papszFieldNames, "ESRI_DATUM_NAME");

	cslDestroy(papszFieldNames);

	if (nDatumCodeField == -1 || nEPSGNameField == -1 || nESRINameField == -1)
	{
		gtl::error(gtl::ERRORTYPE::GET_FAILURE,gtl::ERRORCODE::GEC_APPDEFINED,
			"Failed to find required field in gdal_datum.csv in InitDatumMappingTable(), using default table setup.");

		papszDatumMapping = (char **)apszDefaultDatumMapping;
	//	VSIFClose(fp);
		csv.clear();
		return;
	}

	/* -------------------------------------------------------------------- */
	/*      Read each line, adding a detail line for each.                  */
	/* -------------------------------------------------------------------- */
	int nMappingCount = 0;
	const int nMaxDatumMappings = 1000;
	char **papszFields;
	papszDatumMapping = (char **)calloc(sizeof(char*), nMaxDatumMappings * 3);
	int lineCount = csv.getLineCount();
	/*for (papszFields = CSVReadParseLine(fp);
	papszFields != NULL;
		papszFields = CSVReadParseLine(fp))*/
	for (int k = 1; k < lineCount;k++)
	{
		papszFields = CommaSeparatedValues::tokenizeLine(csv.getLine(k),csv.getSeparator());
		int nFieldCount = cslCount(papszFields);

		assert(nMappingCount + 1 < nMaxDatumMappings);

		if (max(nEPSGNameField, max(nDatumCodeField, nESRINameField))
			< nFieldCount
			&& nMaxDatumMappings > nMappingCount + 1)
		{
			papszDatumMapping[nMappingCount * 3 + 0] =
				cslDuplicateString(papszFields[nDatumCodeField]);
			papszDatumMapping[nMappingCount * 3 + 1] =
				cslDuplicateString(papszFields[nESRINameField]);
			papszDatumMapping[nMappingCount * 3 + 2] =
				cslDuplicateString(papszFields[nEPSGNameField]);
			EPSGDatumNameMassage(&(papszDatumMapping[nMappingCount * 3 + 2]));

			nMappingCount++;
		}
		cslDestroy(papszFields);
	}

	//VSIFClose(fp);

	papszDatumMapping[nMappingCount * 3 + 0] = NULL;
	papszDatumMapping[nMappingCount * 3 + 1] = NULL;
	papszDatumMapping[nMappingCount * 3 + 2] = NULL;
}



/************************************************************************/
/*                              OSR_GDV()                               */
/*                                                                      */
/*      Fetch a particular parameter out of the parameter list, or      */
/*      the indicated default if it isn't available.  This is a         */
/*      helper function for importFromESRI().                           */
/************************************************************************/

static double OSR_GDV(char **papszNV, const char * pszField,
	double dfDefaultValue)

{
	int         iLine;

	if (papszNV == NULL || papszNV[0] == NULL)
		return dfDefaultValue;

	if (cslNIEqualString(pszField, "PARAM_", 6))
	{
		int     nOffset;

		for (iLine = 0;
		papszNV[iLine] != NULL && !cslNIEqualString(papszNV[iLine], "Paramet", 7);
			iLine++) {
		}

		for (nOffset = atoi(pszField + 6);
		papszNV[iLine] != NULL && nOffset > 0;
			iLine++)
		{
			if (strlen(papszNV[iLine]) > 0)
				nOffset--;
		}

		while (papszNV[iLine] != NULL && strlen(papszNV[iLine]) == 0)
			iLine++;

		if (papszNV[iLine] != NULL)
		{
			char        **papszTokens, *pszLine = papszNV[iLine];
			double      dfValue;

			int         i;

			// Trim comments.
			for (i = 0; pszLine[i] != '\0'; i++)
			{
				if (pszLine[i] == '/' && pszLine[i + 1] == '*')
					pszLine[i] = '\0';
			}

			papszTokens = cslTokenizeString(papszNV[iLine]);
			if (cslCount(papszTokens) == 3)
			{
				/* http://agdcftp1.wr.usgs.gov/pub/projects/lcc/akcan_lcc/akcan.tar.gz contains */
				/* weird values for the second. Ignore it and the result looks correct */
				double dfSecond = atof(papszTokens[2]);
				if (dfSecond < 0.0 || dfSecond >= 60.0)
					dfSecond = 0.0;

				dfValue = fabs(atof(papszTokens[0]))
					+ atof(papszTokens[1]) / 60.0
					+ dfSecond / 3600.0;

				if (atof(papszTokens[0]) < 0.0)
					dfValue *= -1;
			}
			else if (cslCount(papszTokens) > 0)
				dfValue = atof(papszTokens[0]);
			else
				dfValue = dfDefaultValue;

			cslDestroy(papszTokens);

			return dfValue;
		}
		else
			return dfDefaultValue;
	}
	else
	{
		for (iLine = 0;
		papszNV[iLine] != NULL &&
			!cslNIEqualString(papszNV[iLine], pszField, (int)strlen(pszField));
			iLine++) {
		}

		if (papszNV[iLine] == NULL)
			return dfDefaultValue;
		else
			return atof(papszNV[iLine] + strlen(pszField));
	}
}

/************************************************************************/
/*                              OSR_GDS()                               */
/************************************************************************/

static String OSR_GDS(char **papszNV, const char * pszField,
	const char *pszDefaultValue)

{
	int         iLine;

	if (papszNV == NULL || papszNV[0] == NULL)
		return pszDefaultValue;

	for (iLine = 0;
	papszNV[iLine] != NULL &&
		!cslNIEqualString(papszNV[iLine], pszField, (int)strlen(pszField));
		iLine++) {
	}

	if (papszNV[iLine] == NULL)
		return pszDefaultValue;
	else
	{
		String osResult;
		char    **papszTokens;

		papszTokens = cslTokenizeString(papszNV[iLine]);

		if (cslCount(papszTokens) > 1)
			osResult = papszTokens[1];
		else
			osResult = pszDefaultValue;

		cslDestroy(papszTokens);
		return osResult;
	}
}

/************************************************************************/
/*                          importFromESRI()                            */
/************************************************************************/

/**
* \brief Import coordinate system from ESRI .prj format(s).
*
* This function will read the text loaded from an ESRI .prj file, and
* translate it into an SpatialReference definition.  This should support
* many (but by no means all) old style (Arc/Info 7.x) .prj files, as well
* as the newer pseudo-OGC WKT .prj files.  Note that new style .prj files
* are in OGC WKT format, but require some manipulation to correct datum
* names, and units on some projection parameters.  This is addressed within
* importFromESRI() by an automatical call to morphFromESRI().
*
* Currently only GEOGRAPHIC, UTM, STATEPLANE, GREATBRITIAN_GRID, ALBERS,
* EQUIDISTANT_CONIC, TRANSVERSE (mercator), POLAR, MERCATOR and POLYCONIC
* projections are supported from old style files.
*
* At this time there is no equivelent exportToESRI() method.  Writing old
* style .prj files is not supported by SpatialReference. However the
* morphToESRI() and exportToWkt() methods can be used to generate output
* suitable to write to new style (Arc 8) .prj files.
*
* This function is the equilvelent of the C function OSRImportFromESRI().
*
* @param papszPrj NULL terminated list of strings containing the definition.
*
* @return true on success or an error code in case of failure.
*/

bool SpatialReference::importFromESRI(char **papszPrj)

{
	if (papszPrj == NULL || papszPrj[0] == NULL)
		return false;

	/* -------------------------------------------------------------------- */
	/*      ArcGIS and related products now use a varient of Well Known     */
	/*      Text.  Try to recognise this and ingest it.  WKT is usually     */
	/*      all on one line, but we will accept multi-line formats and      */
	/*      concatenate.                                                    */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(papszPrj[0], "GEOGCS", 6)
		|| cslNIEqualString(papszPrj[0], "PROJCS", 6)
		|| cslNIEqualString(papszPrj[0], "LOCAL_CS", 8))
	{
		char    *pszWKT, *pszWKT2;
		bool  eErr;
		int     i;

		pszWKT = cslDuplicateString(papszPrj[0]);
		for (i = 1; papszPrj[i] != NULL; i++)
		{
			pszWKT = (char *)
				realloc(pszWKT, strlen(pszWKT) + strlen(papszPrj[i]) + 1);
			strcat(pszWKT, papszPrj[i]);
		}
		pszWKT2 = pszWKT;
		eErr = importFromWkt(&pszWKT2);
		free(pszWKT);

		if (eErr == true)
			eErr = morphFromESRI();
		return eErr;
	}

	/* -------------------------------------------------------------------- */
	/*      Operate on the basis of the projection name.                    */
	/* -------------------------------------------------------------------- */
	String osProj = OSR_GDS(papszPrj, "Projection", "");

	if (cslIEqualString(osProj, ""))
	{
		gtl::debug("OGR_ESRI", "Can't find Projection\n");
		return false;
	}

	else if (cslIEqualString(osProj, "GEOGRAPHIC"))
	{
	}

	else if (cslIEqualString(osProj, "utm"))
	{
		if ((int)OSR_GDV(papszPrj, "zone", 0.0) != 0)
		{
			double      dfYShift = OSR_GDV(papszPrj, "Yshift", 0.0);

			setUTM((int)OSR_GDV(papszPrj, "zone", 0.0),
				dfYShift == 0.0);
		}
		else
		{
			double      dfCentralMeridian, dfRefLat;
			int         nZone;

			dfCentralMeridian = OSR_GDV(papszPrj, "PARAM_1", 0.0);
			dfRefLat = OSR_GDV(papszPrj, "PARAM_2", 0.0);

			nZone = (int)((dfCentralMeridian + 183) / 6.0 + 0.0000001);
			setUTM(nZone, dfRefLat >= 0.0);
		}
	}

	else if (cslIEqualString(osProj, "STATEPLANE"))
	{
		int nZone = (int)OSR_GDV(papszPrj, "zone", 0.0);
		if (nZone != 0)
			nZone = ESRIToUSGSZone(nZone);
		else
			nZone = (int)OSR_GDV(papszPrj, "fipszone", 0.0);

		if (nZone != 0)
		{
			if (cslIEqualString(OSR_GDS(papszPrj, "Datum", "NAD83"), "NAD27"))
				setStatePlane(nZone, FALSE);
			else
				setStatePlane(nZone, TRUE);
		}
	}

	else if (cslIEqualString(osProj, "GREATBRITIAN_GRID")
		|| cslIEqualString(osProj, "GREATBRITAIN_GRID"))
	{
		const char *pszWkt =
			"PROJCS[\"OSGB 1936 / British National Grid\",GEOGCS[\"OSGB 1936\",DATUM[\"OSGB_1936\",SPHEROID[\"Airy 1830\",6377563.396,299.3249646]],PRIMEM[\"Greenwich\",0],UNIT[\"degree\",0.0174532925199433]],PROJECTION[\"Transverse_Mercator\"],PARAMETER[\"latitude_of_origin\",49],PARAMETER[\"central_meridian\",-2],PARAMETER[\"scale_factor\",0.999601272],PARAMETER[\"false_easting\",400000],PARAMETER[\"false_northing\",-100000],UNIT[\"metre\",1]]";

		importFromWkt((char **)&pszWkt);
	}

	else if (cslIEqualString(osProj, "ALBERS"))
	{
		setACEA(OSR_GDV(papszPrj, "PARAM_1", 0.0),
			OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_4", 0.0),
			OSR_GDV(papszPrj, "PARAM_3", 0.0),
			OSR_GDV(papszPrj, "PARAM_5", 0.0),
			OSR_GDV(papszPrj, "PARAM_6", 0.0));
	}

	else if (cslIEqualString(osProj, "LAMBERT"))
	{
		setLCC(OSR_GDV(papszPrj, "PARAM_1", 0.0),
			OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_4", 0.0),
			OSR_GDV(papszPrj, "PARAM_3", 0.0),
			OSR_GDV(papszPrj, "PARAM_5", 0.0),
			OSR_GDV(papszPrj, "PARAM_6", 0.0));
	}

	else if (cslIEqualString(osProj, "LAMBERT_AZIMUTHAL"))
	{
		setLAEA(OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_1", 0.0),
			OSR_GDV(papszPrj, "PARAM_3", 0.0),
			OSR_GDV(papszPrj, "PARAM_4", 0.0));
	}

	else if (cslIEqualString(osProj, "EQUIDISTANT_CONIC"))
	{
		int     nStdPCount = (int)OSR_GDV(papszPrj, "PARAM_1", 0.0);

		if (nStdPCount == 1)
		{
			setEC(OSR_GDV(papszPrj, "PARAM_2", 0.0),
				OSR_GDV(papszPrj, "PARAM_2", 0.0),
				OSR_GDV(papszPrj, "PARAM_4", 0.0),
				OSR_GDV(papszPrj, "PARAM_3", 0.0),
				OSR_GDV(papszPrj, "PARAM_5", 0.0),
				OSR_GDV(papszPrj, "PARAM_6", 0.0));
		}
		else
		{
			setEC(OSR_GDV(papszPrj, "PARAM_2", 0.0),
				OSR_GDV(papszPrj, "PARAM_3", 0.0),
				OSR_GDV(papszPrj, "PARAM_5", 0.0),
				OSR_GDV(papszPrj, "PARAM_4", 0.0),
				OSR_GDV(papszPrj, "PARAM_5", 0.0),
				OSR_GDV(papszPrj, "PARAM_7", 0.0));
		}
	}

	else if (cslIEqualString(osProj, "TRANSVERSE"))
	{
		setTM(OSR_GDV(papszPrj, "PARAM_3", 0.0),
			OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_1", 0.0),
			OSR_GDV(papszPrj, "PARAM_4", 0.0),
			OSR_GDV(papszPrj, "PARAM_5", 0.0));
	}

	else if (cslIEqualString(osProj, "POLAR"))
	{
		setPS(OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_1", 0.0),
			1.0,
			OSR_GDV(papszPrj, "PARAM_3", 0.0),
			OSR_GDV(papszPrj, "PARAM_4", 0.0));
	}

	else if (cslIEqualString(osProj, "MERCATOR"))
	{
		setMercator(OSR_GDV(papszPrj, "PARAM_1", 0.0),
			OSR_GDV(papszPrj, "PARAM_0", 0.0),
			1.0,
			OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_3", 0.0));
	}

	else if (cslIEqualString(osProj, "POLYCONIC"))
	{
		setPolyconic(OSR_GDV(papszPrj, "PARAM_2", 0.0),
			OSR_GDV(papszPrj, "PARAM_1", 0.0),
			OSR_GDV(papszPrj, "PARAM_3", 0.0),
			OSR_GDV(papszPrj, "PARAM_4", 0.0));
	}

	else
	{
		gtl::debug("OGR_ESRI", "Unsupported projection: %s", osProj.c_str());
		setLocalCS(osProj);
	}

	/* -------------------------------------------------------------------- */
	/*      Try to translate the datum/spheroid.                            */
	/* -------------------------------------------------------------------- */
	if (!isLocal() && getAttrNode("GEOGCS") == NULL)
	{
		String osDatum;

		osDatum = OSR_GDS(papszPrj, "Datum", "");

		if (cslIEqualString(osDatum, "NAD27") || cslIEqualString(osDatum, "NAD83")
			|| cslIEqualString(osDatum, "WGS84") || cslIEqualString(osDatum, "WGS72"))
		{
			setWellKnownGeogCS(osDatum);
		}
		else if (cslIEqualString(osDatum, "EUR")
			|| cslIEqualString(osDatum, "ED50"))
		{
			setWellKnownGeogCS("EPSG:4230");
		}
		else if (cslIEqualString(osDatum, "GDA94"))
		{
			setWellKnownGeogCS("EPSG:4283");
		}
		else
		{
			String osSpheroid;

			osSpheroid = OSR_GDS(papszPrj, "Spheroid", "");

			if (cslIEqualString(osSpheroid, "INT1909")
				|| cslIEqualString(osSpheroid, "INTERNATIONAL1909"))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(4022);
				copyGeogCSFrom(&oGCS);
			}
			else if (cslIEqualString(osSpheroid, "AIRY"))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(4001);
				copyGeogCSFrom(&oGCS);
			}
			else if (cslIEqualString(osSpheroid, "CLARKE1866"))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(4008);
				copyGeogCSFrom(&oGCS);
			}
			else if (cslIEqualString(osSpheroid, "GRS80"))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(4019);
				copyGeogCSFrom(&oGCS);
			}
			else if (cslIEqualString(osSpheroid, "KRASOVSKY")
				|| cslIEqualString(osSpheroid, "KRASSOVSKY")
				|| cslIEqualString(osSpheroid, "KRASSOWSKY"))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(4024);
				copyGeogCSFrom(&oGCS);
			}
			else if (cslIEqualString(osSpheroid, "Bessel"))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(4004);
				copyGeogCSFrom(&oGCS);
			}
			else
			{
				// If we don't know, default to WGS84 so there is something there.
				setWellKnownGeogCS("WGS84");
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Linear units translation                                        */
	/* -------------------------------------------------------------------- */
	if (isLocal() || isProjected())
	{
		String osValue;
		double dfOldUnits = getLinearUnits();

		osValue = OSR_GDS(papszPrj, "Units", "");
		if (cslIEqualString(osValue, ""))
			setLinearUnitsAndUpdateParameters(SRS_UL_METER, 1.0);
		else if (cslIEqualString(osValue, "FEET"))
			setLinearUnitsAndUpdateParameters(SRS_UL_US_FOOT, atof(SRS_UL_US_FOOT_CONV));
		else if (atof(osValue) != 0.0)
			setLinearUnitsAndUpdateParameters("user-defined",
				1.0 / atof(osValue));
		else
			setLinearUnitsAndUpdateParameters(osValue, 1.0);

		// If we have reset the linear units we should clear any authority
		// nodes on the PROJCS.  This especially applies to state plane
		// per bug 1697
		double dfNewUnits = getLinearUnits();
		if (dfOldUnits != 0.0
			&& (dfNewUnits / dfOldUnits < 0.9999999
				|| dfNewUnits / dfOldUnits > 1.0000001))
		{
			if (getRoot()->findChild("AUTHORITY") != -1)
				getRoot()->destroyChild(getRoot()->findChild("AUTHORITY"));
		}
	}

	return true;
}

/************************************************************************/
/*                            morphToESRI()                             */
/************************************************************************/
/**
* \brief Convert in place to ESRI WKT format.
*
* The value nodes of this coordinate system are modified in various manners
* more closely map onto the ESRI concept of WKT format.  This includes
* renaming a variety of projections and arguments, and stripping out
* nodes note recognised by ESRI (like AUTHORITY and AXIS).
*
* This does the same as the C function OSRMorphToESRI().
*
* @return true unless something goes badly wrong.
*/

bool SpatialReference::morphToESRI()

{
	bool      eErr;

	//CPLLocaleC localeC; 
	String oLocale = setlocale(LC_NUMERIC, NULL);
	/* -------------------------------------------------------------------- */
	/*      fixup ordering, missing linear units, etc.                      */
	/* -------------------------------------------------------------------- */
	eErr = fixup();
	if (eErr != true)
		return eErr;

	/* -------------------------------------------------------------------- */
	/*      Strip all CT parameters (AXIS, AUTHORITY, TOWGS84, etc).        */
	/* -------------------------------------------------------------------- */
	eErr = stripCTParms();
	if (eErr != true)
		return eErr;

	if (getRoot() == NULL)
		return true;

	/* -------------------------------------------------------------------- */
	/*      There is a special case for Hotine Oblique Mercator to split    */
	/*      out the case with an angle to rectified grid.  Bug 423          */
	/* -------------------------------------------------------------------- */
	const char *pszProjection = getAttrValue("PROJECTION");

	if (pszProjection != NULL
		&& cslIEqualString(pszProjection, SRS_PT_HOTINE_OBLIQUE_MERCATOR)
		&& fabs(getProjParm(SRS_PP_AZIMUTH, 0.0) - 90) < 0.0001
		&& fabs(getProjParm(SRS_PP_RECTIFIED_GRID_ANGLE, 0.0) - 90) < 0.0001)
	{
		setNode("PROJCS|PROJECTION",
			"Hotine_Oblique_Mercator_Azimuth_Center");

		/* ideally we should strip out of the rectified_grid_angle */
		// strip off rectified_grid_angle -- I hope it is 90!
		SRSNode *poPROJCS = getAttrNode("PROJCS");
		int iRGAChild = findProjParm("rectified_grid_angle", poPROJCS);
		if (iRGAChild != -1)
			poPROJCS->destroyChild(iRGAChild);

		pszProjection = getAttrValue("PROJECTION");
	}

	/* -------------------------------------------------------------------- */
	/*      Polar_Stereographic maps to ESRI codes                          */
	/*      Stereographic_South_Pole or Stereographic_North_Pole based      */
	/*      on latitude.                                                    */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL
		&& (cslIEqualString(pszProjection, SRS_PT_POLAR_STEREOGRAPHIC)))
	{
		if (getProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0) < 0.0)
		{
			setNode("PROJCS|PROJECTION",
				"Stereographic_South_Pole");
			pszProjection = getAttrValue("PROJECTION");
		}
		else
		{
			setNode("PROJCS|PROJECTION",
				"Stereographic_North_Pole");
			pszProjection = getAttrValue("PROJECTION");
		}
	}

	/* -------------------------------------------------------------------- */
	/*      OBLIQUE_STEREOGRAPHIC maps to ESRI Double_Stereographic         */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL
		&& (cslIEqualString(pszProjection, SRS_PT_OBLIQUE_STEREOGRAPHIC)))
	{
		setNode("PROJCS|PROJECTION", "Double_Stereographic");
	}

	/* -------------------------------------------------------------------- */
	/*      Translate PROJECTION keywords that are misnamed.                */
	/* -------------------------------------------------------------------- */
	getRoot()->applyRemapper("PROJECTION",
		(char **)apszProjMapping + 1,
		(char **)apszProjMapping, 2);
	pszProjection = getAttrValue("PROJECTION");

	/* -------------------------------------------------------------------- */
	/*      Translate DATUM keywords that are misnamed.                     */
	/* -------------------------------------------------------------------- */
	InitDatumMappingTable();

	getRoot()->applyRemapper("DATUM",
		papszDatumMapping + 2, papszDatumMapping + 1, 3);

	const char *pszProjCSName = NULL;
	const char *pszGcsName = NULL;
	SRSNode *poProjCS = NULL;
	SRSNode *poProjCSNodeChild = NULL;

	/* -------------------------------------------------------------------- */
	/*      Very specific handling for some well known geographic           */
	/*      coordinate systems.                                             */
	/* -------------------------------------------------------------------- */
	SRSNode *poGeogCS = getAttrNode("GEOGCS");
	if (poGeogCS != NULL)
	{
		const char *pszGeogCSName = poGeogCS->getChild(0)->getValue();
		const char *pszAuthName = getAuthorityName("GEOGCS");
		const char *pszUTMPrefix = NULL;
		int nGCSCode = -1;

		if (pszAuthName != NULL && cslIEqualString(pszAuthName, "EPSG"))
			nGCSCode = atoi(getAuthorityCode("GEOGCS"));

		if (nGCSCode == 4326
			|| cslIEqualString(pszGeogCSName, "WGS84")
			|| cslIEqualString(pszGeogCSName, "WGS 84"))
		{
			poGeogCS->getChild(0)->setValue("GCS_WGS_1984");
			pszUTMPrefix = "WGS_1984";
		}
		else if (nGCSCode == 4267
			|| cslIEqualString(pszGeogCSName, "NAD27")
			|| cslIEqualString(pszGeogCSName, "NAD 27"))
		{
			poGeogCS->getChild(0)->setValue("GCS_North_American_1927");
			pszUTMPrefix = "NAD_1927";
		}
		else if (nGCSCode == 4269
			|| cslIEqualString(pszGeogCSName, "NAD83")
			|| cslIEqualString(pszGeogCSName, "NAD 83"))
		{
			poGeogCS->getChild(0)->setValue("GCS_North_American_1983");
			pszUTMPrefix = "NAD_1983";
		}
		else if (nGCSCode == 4167
			|| cslIEqualString(pszGeogCSName, "NZGD2000")
			|| cslIEqualString(pszGeogCSName, "NZGD 2000"))
		{
			poGeogCS->getChild(0)->setValue("GCS_NZGD_2000");
			pszUTMPrefix = "NZGD_2000";
		}
		else if (nGCSCode == 4272
			|| cslIEqualString(pszGeogCSName, "NZGD49")
			|| cslIEqualString(pszGeogCSName, "NZGD 49"))
		{
			poGeogCS->getChild(0)->setValue("GCS_New_Zealand_1949");
			pszUTMPrefix = "NZGD_1949";
		}

		/* -------------------------------------------------------------------- */
		/*      Force Unnamed to Unknown for most common locations.             */
		/* -------------------------------------------------------------------- */
		static const char *apszUnknownMapping[] = {
			"Unknown", "Unnamed",
			NULL, NULL
		};

		getRoot()->applyRemapper("PROJCS",
			(char **)apszUnknownMapping + 1,
			(char **)apszUnknownMapping + 0, 2);
		getRoot()->applyRemapper("GEOGCS",
			(char **)apszUnknownMapping + 1,
			(char **)apszUnknownMapping + 0, 2);
		getRoot()->applyRemapper("DATUM",
			(char **)apszUnknownMapping + 1,
			(char **)apszUnknownMapping + 0, 2);
		getRoot()->applyRemapper("SPHEROID",
			(char **)apszUnknownMapping + 1,
			(char **)apszUnknownMapping + 0, 2);
		getRoot()->applyRemapper("PRIMEM",
			(char **)apszUnknownMapping + 1,
			(char **)apszUnknownMapping + 0, 2);

		/* -------------------------------------------------------------------- */
		/*      If the PROJCS name is unset, use the PROJECTION name in         */
		/*      place of unknown, or unnamed.  At the request of Peng Gao.      */
		/* -------------------------------------------------------------------- */
		if ((poProjCS = getAttrNode("PROJCS")) != NULL)
			poProjCSNodeChild = poProjCS->getChild(0);

		if (poProjCSNodeChild)
		{
			pszProjCSName = poProjCSNodeChild->getValue();
			char *pszNewValue = cslDuplicateString(pszProjCSName);
			MorphNameToESRI(&pszNewValue);
			poProjCSNodeChild->setValue(pszNewValue);
			free(pszNewValue);
			pszProjCSName = poProjCSNodeChild->getValue();
		}

		if (pszProjCSName != NULL
			&& (cslIEqualString(pszProjCSName, "unnamed")
				|| cslIEqualString(pszProjCSName, "unknown")
				|| cslIEqualString(pszProjCSName, "")))
		{
			if (getAttrValue("PROJECTION", 0) != NULL)
			{
				pszProjCSName = getAttrValue("PROJECTION", 0);
				poProjCSNodeChild->setValue(pszProjCSName);
			}
		}

		/* -------------------------------------------------------------------- */
		/*      Prepare very specific PROJCS names for UTM coordinate           */
		/*      systems.                                                        */
		/* -------------------------------------------------------------------- */
		int bNorth = 0;
		int nZone = 0;

		/* get zone from name first */
		if (pszProjCSName && cslNIEqualString(pszProjCSName, "UTM Zone ", 9))
		{
			nZone = atoi(pszProjCSName + 9);
			if (strstr(pszProjCSName, "North"))
				bNorth = 1;
		}

		/* if can not get from the name, from the parameters */
		if (nZone <= 0)
			nZone = getUTMZone(&bNorth);

		if (nZone > 0 && pszUTMPrefix)
		{
			char szUTMName[128];
			if (bNorth)
				sprintf(szUTMName, "%s_UTM_Zone_%dN", pszUTMPrefix, nZone);
			else
				sprintf(szUTMName, "%s_UTM_Zone_%dS", pszUTMPrefix, nZone);

			if (poProjCSNodeChild)
				poProjCSNodeChild->setValue(szUTMName);
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Translate UNIT keywords that are misnamed, or even the wrong    */
	/*      case.                                                           */
	/* -------------------------------------------------------------------- */
	getRoot()->applyRemapper("UNIT",
		(char **)apszUnitMapping + 1,
		(char **)apszUnitMapping, 2);

	/* -------------------------------------------------------------------- */
	/*      reset constants for decimal degrees to the exact string ESRI    */
	/*      expects when encountered to ensure a matchup.                   */
	/* -------------------------------------------------------------------- */
	SRSNode *poUnit = getAttrNode("GEOGCS|UNIT");

	if (poUnit != NULL && poUnit->getChildCount() >= 2
		&& fabs(getAngularUnits() - 0.0174532925199433) < 0.00000000001)
	{
		poUnit->getChild(0)->setValue("Degree");
		poUnit->getChild(1)->setValue("0.017453292519943295");
	}

	/* -------------------------------------------------------------------- */
	/*      Make sure we reproduce US Feet exactly too.                     */
	/* -------------------------------------------------------------------- */
	poUnit = getAttrNode("PROJCS|UNIT");

	if (poUnit != NULL && poUnit->getChildCount() >= 2
		&& fabs(getLinearUnits() - 0.30480060960121924) < 0.000000000000001)
	{
		poUnit->getChild(0)->setValue("Foot_US");
		poUnit->getChild(1)->setValue("0.30480060960121924");
	}

	/* -------------------------------------------------------------------- */
	/*      Remap parameters used for Albers and Mercator.                  */
	/* -------------------------------------------------------------------- */
	pszProjection = getAttrValue("PROJECTION");
	poProjCS = getAttrNode("PROJCS");

	if (pszProjection != NULL && cslIEqualString(pszProjection, "Albers"))
		getRoot()->applyRemapper(
			"PARAMETER", (char **)apszAlbersMapping + 1,
			(char **)apszAlbersMapping + 0, 2);

	if (pszProjection != NULL
		&& (cslIEqualString(pszProjection, SRS_PT_EQUIDISTANT_CONIC) ||
			cslIEqualString(pszProjection, SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA) ||
			cslIEqualString(pszProjection, SRS_PT_AZIMUTHAL_EQUIDISTANT) ||
			cslIEqualString(pszProjection, SRS_PT_SINUSOIDAL) ||
			cslIEqualString(pszProjection, SRS_PT_ROBINSON)))
		getRoot()->applyRemapper(
			"PARAMETER", (char **)apszECMapping + 1,
			(char **)apszECMapping + 0, 2);

	if (pszProjection != NULL && cslIEqualString(pszProjection, "Mercator"))
		getRoot()->applyRemapper(
			"PARAMETER",
			(char **)apszMercatorMapping + 1,
			(char **)apszMercatorMapping + 0, 2);

	if (pszProjection != NULL
		&& cslNIEqualString(pszProjection, "Stereographic_", 14)
		&& cslNIEqualString(pszProjection + strlen(pszProjection) - 5, "_Pole", 5))
		getRoot()->applyRemapper(
			"PARAMETER",
			(char **)apszPolarStereographicMapping + 1,
			(char **)apszPolarStereographicMapping + 0, 2);

	if (pszProjection != NULL && cslIEqualString(pszProjection, "Plate_Carree"))
		if (findProjParm(SRS_PP_STANDARD_PARALLEL_1, poProjCS) < 0)
			getRoot()->applyRemapper(
				"PARAMETER",
				(char **)apszPolarStereographicMapping + 1,
				(char **)apszPolarStereographicMapping + 0, 2);

	/* -------------------------------------------------------------------- */
	/*      ESRI's Equidistant_Cylindrical does not support the             */
	/*      latitude_of_origin keyword.                                     */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL
		&& cslIEqualString(pszProjection, "Equidistant_Cylindrical"))
	{
		if (getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0) != 0.0)
		{
			gtl::debug("OGR_ESRI", "Equirectangular with non-zero latitude of origin - not supported.");
		}
		else
		{
			SRSNode *poPROJCS = getAttrNode("PROJCS");
			if (poPROJCS)
				poPROJCS->destroyChild(
					findProjParm(SRS_PP_LATITUDE_OF_ORIGIN));
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Convert SPHEROID name to use underscores instead of spaces.     */
	/* -------------------------------------------------------------------- */
	SRSNode *poSpheroid;
	SRSNode *poSpheroidChild = NULL;
	poSpheroid = getAttrNode("SPHEROID");
	if (poSpheroid != NULL)
		poSpheroidChild = poSpheroid->getChild(0);

	if (poSpheroidChild != NULL)
	{
		//        char *pszNewValue = cslDuplicateString(RemapSpheroidName(poSpheroidChild->getValue()));
		char *pszNewValue = cslDuplicateString(poSpheroidChild->getValue());

		MorphNameToESRI(&pszNewValue);

		poSpheroidChild->setValue(pszNewValue);
		free(pszNewValue);

		getRoot()->applyRemapper("SPHEROID",
			(char **)apszSpheroidMapping + 0,
			(char **)apszSpheroidMapping + 1, 2);
	}

	if (poSpheroid != NULL)
		poSpheroidChild = poSpheroid->getChild(2);

	if (poSpheroidChild != NULL)
	{
		const char * dfValue = poSpheroidChild->getValue();
		for (int i = 0; apszInvFlatteningMapping[i] != NULL; i += 2)
		{
			if (cslNIEqualString(apszInvFlatteningMapping[i], dfValue, (int)strlen(apszInvFlatteningMapping[i])))
			{
				poSpheroidChild->setValue(apszInvFlatteningMapping[i + 1]);
				break;
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Try to insert a D_ in front of the datum name.                  */
	/* -------------------------------------------------------------------- */
	SRSNode *poDatum;

	poDatum = getAttrNode("DATUM");
	if (poDatum != NULL)
		poDatum = poDatum->getChild(0);

	if (poDatum != NULL)
	{
		const char* pszDatumName = poDatum->getValue();
		if (!cslNIEqualString(pszDatumName, "D_", 2))
		{
			char *pszNewValue;

			pszNewValue = (char *)malloc(strlen(poDatum->getValue()) + 3);
			strcpy(pszNewValue, "D_");
			strcat(pszNewValue, poDatum->getValue());
			poDatum->setValue(pszNewValue);
			free(pszNewValue);
		}
	}

	/* -------------------------------------------------------------------- */
	/*                        final check names                             */
	/* -------------------------------------------------------------------- */
	if (poProjCSNodeChild)
		pszProjCSName = poProjCSNodeChild->getValue();

	if (pszProjCSName)
	{
		pszGcsName = getAttrValue("GEOGCS");
		if (pszGcsName && !cslNIEqualString(pszGcsName, "GCS_", 4))
		{
			char* newGcsName = (char *)malloc(strlen(pszGcsName) + 5);
			strcpy(newGcsName, "GCS_");
			strcat(newGcsName, pszGcsName);
			SetNewName(this, "GEOGCS", newGcsName);
			free(newGcsName);
			pszGcsName = getAttrValue("GEOGCS");
		}
		RemapGeogCSName(this, pszGcsName);

		// Specific processing and remapping 
		pszProjection = getAttrValue("PROJECTION");
		if (pszProjection)
		{
			if (cslIEqualString(pszProjection, "Lambert_Conformal_Conic"))
			{
				if (findProjParm(SRS_PP_STANDARD_PARALLEL_2, poProjCS) < 0)
				{
					int iChild = findProjParm(SRS_PP_LATITUDE_OF_ORIGIN, poProjCS);
					int iChild1 = findProjParm(SRS_PP_STANDARD_PARALLEL_1, poProjCS);
					if (iChild >= 0 && iChild1 < 0)
					{
						const SRSNode *poParameter = poProjCS->getChild(iChild);
						if (poParameter)
						{
							SRSNode *poNewParm = new SRSNode("PARAMETER");
							poNewParm->addChild(new SRSNode("standard_parallel_1"));
							poNewParm->addChild(new SRSNode(poParameter->getChild(1)->getValue()));
							poProjCS->addChild(poNewParm);
						}
					}
				}
			}

			if (cslIEqualString(pszProjection, "Plate_Carree"))
			{
				int iChild = findProjParm(SRS_PP_STANDARD_PARALLEL_1, poProjCS);
				if (iChild < 0)
					iChild = findProjParm(SRS_PP_PSEUDO_STD_PARALLEL_1, poProjCS);

				if (iChild >= 0)
				{
					const SRSNode *poParameter = poProjCS->getChild(iChild);
					if (!cslIEqualString(poParameter->getChild(1)->getValue(), "0.0") && !cslIEqualString(poParameter->getChild(1)->getValue(), "0"))
					{
						setNode("PROJCS|PROJECTION", "Equidistant_Cylindrical");
						pszProjection = getAttrValue("PROJECTION");
					}
				}
			}
			DeleteParamBasedOnPrjName(this, pszProjection, (char **)apszDeleteParametersBasedOnProjection);
			AddParamBasedOnPrjName(this, pszProjection, (char **)apszAddParametersBasedOnProjection);
			RemapPValuesBasedOnProjCSAndPName(this, pszProjection, (char **)apszParamValueMapping);
			RemapPNamesBasedOnProjCSAndPName(this, pszProjection, (char **)apszParamNameMapping);
		}
	}

	setlocale(LC_NUMERIC, oLocale.c_str());

	return true;
}
 

/************************************************************************/
/*                           morphFromESRI()                            */
/*                                                                      */
/*      modify this definition from the ESRI definition of WKT to       */
/*      the "Standard" definition.                                      */
/************************************************************************/

/**
* \brief Convert in place from ESRI WKT format.
*
* The value notes of this coordinate system are modified in various manners
* to adhere more closely to the WKT standard.  This mostly involves
* translating a variety of ESRI names for projections, arguments and
* datums to "standard" names, as defined by Adam Gawne-Cain's reference
* translation of EPSG to WKT for the CT specification.
*
* Starting with GDAL 1.9.0, missing parameters in TOWGS84, DATUM or GEOGCS
* nodes can be added to the WKT, comparing existing WKT parameters to GDAL's
* databases.  Note that this optional procedure is very conservative and should
* not introduce false information into the WKT defintion (altough caution
* should be advised when activating it). Needs the Configuration Option
* GDAL_FIX_ESRI_WKT be set to one of the following values (TOWGS84 is
* recommended for proper datum shift calculations):
*
* <b>GDAL_FIX_ESRI_WKT values</b>
* <table border="0">
* <tr><td>&nbsp;&nbsp;</td><td><b>TOWGS84</b></td><td>&nbsp;&nbsp;</td><td>
* Adds missing TOWGS84 parameters (necessary for datum transformations),
* based on named datum and spheroid values.</td></tr>
* <tr><td>&nbsp;&nbsp;</td><td><b>DATUM</b></td><td>&nbsp;&nbsp;</td><td>
* Adds EPSG AUTHORITY nodes and sets SPHEROID name to OGR spec.</td></tr>
* <tr><td>&nbsp;&nbsp;</td><td><b>GEOGCS</b></td><td>&nbsp;&nbsp;</td><td>
* Adds EPSG AUTHORITY nodes and sets GEOGCS, DATUM and SPHEROID
* names to OGR spec. Effectively replaces GEOGCS node with the result of
* importFromEPSG(n), using EPSG code n corresponding to the existing GEOGCS.
* Does not impact PROJCS values.</td></tr>
* </table>
*
* This does the same as the C function OSRMorphFromESRI().
*
* @return true unless something goes badly wrong.
*/

bool SpatialReference::morphFromESRI()

{
	bool      eErr = true;
	SRSNode *poDatum;
	char        *pszDatumOrig = NULL;

	if (getRoot() == NULL)
		return true;

	InitDatumMappingTable();

	/* -------------------------------------------------------------------- */
	/*      Save original datum name for later                              */
	/* -------------------------------------------------------------------- */
	poDatum = getAttrNode("DATUM");
	if (poDatum != NULL)
	{
		poDatum = poDatum->getChild(0);
		pszDatumOrig = cslDuplicateString(poDatum->getValue());
	}

	/* -------------------------------------------------------------------- */
	/*      Translate DATUM keywords that are oddly named.                  */
	/* -------------------------------------------------------------------- */
	getRoot()->applyRemapper("DATUM",
		(char **)papszDatumMapping + 1,
		(char **)papszDatumMapping + 2, 3);

	/* -------------------------------------------------------------------- */
	/*      Try to remove any D_ in front of the datum name.                */
	/* -------------------------------------------------------------------- */
	poDatum = getAttrNode("DATUM");
	if (poDatum != NULL)
		poDatum = poDatum->getChild(0);

	if (poDatum != NULL)
	{
		if (cslNIEqualString(poDatum->getValue(), "D_", 2))
		{
			char *pszNewValue = cslDuplicateString(poDatum->getValue() + 2);
			poDatum->setValue(pszNewValue);
			free(pszNewValue);
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Translate some SPHEROID keywords that are oddly named.          */
	/* -------------------------------------------------------------------- */
	getRoot()->applyRemapper("SPHEROID",
		(char **)apszSpheroidMapping + 1,
		(char **)apszSpheroidMapping + 0, 2);

	/* -------------------------------------------------------------------- */
	/*      Split Lambert_Conformal_Conic into 1SP or 2SP form.             */
	/*                                                                      */
	/*      See bugzilla.remotesensing.org/show_bug.cgi?id=187              */
	/*                                                                      */
	/*      We decide based on whether it has 2SPs.  We used to assume      */
	/*      1SP if it had a scale factor but that turned out to be a        */
	/*      poor test.                                                      */
	/* -------------------------------------------------------------------- */
	const char *pszProjection = getAttrValue("PROJECTION");

	if (pszProjection != NULL
		&& cslIEqualString(pszProjection, "Lambert_Conformal_Conic"))
	{
		if (getProjParm(SRS_PP_STANDARD_PARALLEL_1, 1000.0) != 1000.0
			&& getProjParm(SRS_PP_STANDARD_PARALLEL_2, 1000.0) != 1000.0)
			setNode("PROJCS|PROJECTION",
				SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP);
		else
			setNode("PROJCS|PROJECTION",
				SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP);

		pszProjection = getAttrValue("PROJECTION");
	}

	/* -------------------------------------------------------------------- */
	/*      If we are remapping Hotine_Oblique_Mercator_Azimuth_Center      */
	/*      add a rectified_grid_angle parameter - to match the azimuth     */
	/*      I guess.                                                        */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL
		&& cslIEqualString(pszProjection, "Hotine_Oblique_Mercator_Azimuth_Center"))
	{
		setProjParm(SRS_PP_RECTIFIED_GRID_ANGLE,
			getProjParm(SRS_PP_AZIMUTH, 0.0));
		fixupOrdering();
	}

	/* -------------------------------------------------------------------- */
	/*      Remap Albers, Mercator and Polar Stereographic parameters.      */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL && cslIEqualString(pszProjection, "Albers"))
		getRoot()->applyRemapper(
			"PARAMETER", (char **)apszAlbersMapping + 0,
			(char **)apszAlbersMapping + 1, 2);

	if (pszProjection != NULL
		&& (cslIEqualString(pszProjection, SRS_PT_EQUIDISTANT_CONIC) ||
			cslIEqualString(pszProjection, SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA) ||
			cslIEqualString(pszProjection, SRS_PT_AZIMUTHAL_EQUIDISTANT) ||
			cslIEqualString(pszProjection, SRS_PT_SINUSOIDAL) ||
			cslIEqualString(pszProjection, SRS_PT_ROBINSON)))
		getRoot()->applyRemapper(
			"PARAMETER", (char **)apszECMapping + 0,
			(char **)apszECMapping + 1, 2);

	if (pszProjection != NULL && cslIEqualString(pszProjection, "Mercator"))
		getRoot()->applyRemapper(
			"PARAMETER",
			(char **)apszMercatorMapping + 0,
			(char **)apszMercatorMapping + 1, 2);

	if (pszProjection != NULL && cslIEqualString(pszProjection, "Orthographic"))
		getRoot()->applyRemapper(
			"PARAMETER", (char **)apszOrthographicMapping + 0,
			(char **)apszOrthographicMapping + 1, 2);

	if (pszProjection != NULL
		&& cslNIEqualString(pszProjection, "Stereographic_", 14)
		&& cslNIEqualString(pszProjection + strlen(pszProjection) - 5, "_Pole", 5))
		getRoot()->applyRemapper(
			"PARAMETER",
			(char **)apszPolarStereographicMapping + 0,
			(char **)apszPolarStereographicMapping + 1, 2);

	/* -------------------------------------------------------------------- */
	/*      Remap south and north polar stereographic to one value.         */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL
		&& cslNIEqualString(pszProjection, "Stereographic_", 14)
		&& cslNIEqualString(pszProjection + strlen(pszProjection) - 5, "_Pole", 5))
	{
		setNode("PROJCS|PROJECTION", SRS_PT_POLAR_STEREOGRAPHIC);
		pszProjection = getAttrValue("PROJECTION");
	}

	/* -------------------------------------------------------------------- */
	/*      Remap Double_Stereographic to Oblique_Stereographic.            */
	/* -------------------------------------------------------------------- */
	if (pszProjection != NULL
		&& cslIEqualString(pszProjection, "Double_Stereographic"))
	{
		setNode("PROJCS|PROJECTION", SRS_PT_OBLIQUE_STEREOGRAPHIC);
		pszProjection = getAttrValue("PROJECTION");
	}

	/* -------------------------------------------------------------------- */
	/*      Remap Equidistant_Cylindrical parameter. It is same as          */
	/*      Stereographic                                                   */
	/* -------------------------------------------------------------------- */
#ifdef notdef
	if (pszProjection != NULL && cslIEqualString(pszProjection, "Equidistant_Cylindrical"))
		getRoot()->applyRemapper(
			"PARAMETER",
			(char **)apszPolarStereographicMapping + 0,
			(char **)apszPolarStereographicMapping + 1, 2);
#endif

	/*
	** Handle the value of Central_Parallel -> latitude_of_center.
	** See ticket #3191.  Other mappings probably need to be added.
	*/
	if (pszProjection != NULL &&
		(cslIEqualString(pszProjection, SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP) ||
			cslIEqualString(pszProjection, SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP)))
	{
		getRoot()->applyRemapper(
			"PARAMETER", (char **)apszLambertConformalConicMapping + 0,
			(char **)apszLambertConformalConicMapping + 1, 2);
	}

	/* -------------------------------------------------------------------- */
	/*      Translate PROJECTION keywords that are misnamed.                */
	/* -------------------------------------------------------------------- */
	getRoot()->applyRemapper("PROJECTION",
		(char **)apszProjMapping,
		(char **)apszProjMapping + 1, 2);

	/* -------------------------------------------------------------------- */
	/*      Translate DATUM keywords that are misnamed.                     */
	/* -------------------------------------------------------------------- */
	InitDatumMappingTable();

	getRoot()->applyRemapper("DATUM",
		(char **)papszDatumMapping + 1,
		(char **)papszDatumMapping + 2, 3);

	/* -------------------------------------------------------------------- */
	/*      Fix TOWGS84, DATUM or GEOGCS                                    */
	/* -------------------------------------------------------------------- */
	/* TODO test more ESRI WKT; also add PROJCS */

	/* Check GDAL_FIX_ESRI_WKT config option (default=NO); if YES, set to DATUM */
	//const char *pszFixWktConfig = CPLGetConfigOption("GDAL_FIX_ESRI_WKT", "NO");
	const char *pszFixWktConfig=Config::getSRConfigOptionValue("GDAL_FIX_ESRI_WKT", "NO").c_str();
	if (cslIEqualString(pszFixWktConfig, "YES"))
		pszFixWktConfig = "DATUM";

	if (!cslIEqualString(pszFixWktConfig, "NO") && poDatum != NULL)
	{
		gtl::debug("OGR_ESRI",
			"morphFromESRI() looking for missing TOWGS84, datum=%s, config=%s",
			pszDatumOrig, pszFixWktConfig);

		/* Special case for WGS84 and other common GCS? */

		for (int i = 0; DMGetESRIName(i) != NULL; i++)
		{
			/* we found the ESRI datum name in the map */
			if (cslIEqualString(DMGetESRIName(i), pszDatumOrig))
			{
				const char *pszFilename = NULL;
				char **papszRecord = NULL;

				/* look for GEOGCS corresponding to this datum */
				//pszFilename = CSVFilename("gcs.csv");
				pszFilename = (gtl::getDataHome()+std::string("\\gcs.csv")).c_str();// CSVFilename("gcs.csv");
				CommaSeparatedValues csv(pszFilename);
				//papszRecord = CSVScanFileByName(pszFilename, "DATUM_CODE",
				//	DMGetEPSGCode(i), CC_Integer);
				papszRecord=csv.getLine("DATUM_CODE", DMGetEPSGCode(i), CSVCompareCriteria::CC_Integer);
				if (papszRecord != NULL)
				{
					/* make sure we got a valid EPSG code and it is not DEPRECATED */
					int nGeogCS = atoi(papszRecord[csv.indexField("COORD_REF_SYS_CODE")]);
					// int bDeprecated = atoi( CSLGetField( papszRecord,
					//                                      CSVGetFileFieldId(pszFilename,"DEPRECATED")) );

					gtl::debug("OGR_ESRI", "morphFromESRI() got GEOGCS node #%d", nGeogCS);

					// if ( nGeogCS >= 1 && bDeprecated == 0 )
					if (nGeogCS >= 1)
					{
						SpatialReference oSRSTemp;
						if (oSRSTemp.importFromEPSG(nGeogCS) == true)
						{
							/* make clone of GEOGCS and strip CT parms for testing */
							SpatialReference *poSRSTemp2 = NULL;
							int bIsSame = FALSE;
							char *pszOtherValue = NULL;
							double dfThisValue, dfOtherValue;
							SRSNode *poNode = NULL;

							poSRSTemp2 = oSRSTemp.cloneGeogCS();
							poSRSTemp2->stripCTParms();
							bIsSame = this->isSameGeogCS(poSRSTemp2);
							exportToWkt(&pszOtherValue);
							gtl::debug("OGR_ESRI",
								"morphFromESRI() got SRS %s, matching: %d",
								pszOtherValue, bIsSame);
							free(pszOtherValue);
							delete poSRSTemp2;

							/* clone GEOGCS from original if they match and if allowed */
							if (cslIEqualString(pszFixWktConfig, "GEOGCS")
								&& bIsSame)
							{
								this->copyGeogCSFrom(&oSRSTemp);
								gtl::debug("OGR_ESRI",
									"morphFromESRI() cloned GEOGCS from EPSG:%d",
									nGeogCS);
								/* exit loop */
								break;
							}
							/* else try to copy only DATUM or TOWGS84
							we got here either because of config option or
							GEOGCS are not strictly equal */
							else if (cslIEqualString(pszFixWktConfig, "GEOGCS") ||
								cslIEqualString(pszFixWktConfig, "DATUM") ||
								cslIEqualString(pszFixWktConfig, "TOWGS84"))
							{
								/* test for matching SPHEROID, because there can be 2 datums with same ESRI name
								but different spheroids (e.g. EPSG:4618 and EPSG:4291) - see bug #4345 */
								/* instead of testing for matching SPHEROID name (which can be error-prone), test
								for matching parameters (semi-major and inverse flattening ) - see bug #4673 */
								bIsSame = TRUE;
								dfThisValue = this->getSemiMajor();
								dfOtherValue = oSRSTemp.getSemiMajor();
								if (fabs(dfThisValue - dfOtherValue) > 0.01)
									bIsSame = FALSE;
								gtl::debug("OGR_ESRI",
									"morphFromESRI() SemiMajor: this = %.15g other = %.15g",
									dfThisValue, dfOtherValue);
								dfThisValue = this->getInvFlattening();
								dfOtherValue = oSRSTemp.getInvFlattening();
								if (fabs(dfThisValue - dfOtherValue) > 0.0001)
									bIsSame = FALSE;
								gtl::debug("OGR_ESRI",
									"morphFromESRI() InvFlattening: this = %g other = %g",
									dfThisValue, dfOtherValue);

								if (bIsSame)
								{
									/* test for matching PRIMEM, because there can be 2 datums with same ESRI name
									but different prime meridian (e.g. EPSG:4218 and EPSG:4802)  - see bug #4378 */
									/* instead of testing for matching PRIMEM name (which can be error-prone), test
									for matching value - see bug #4673 */
									dfThisValue = this->getPrimeMeridian();
									dfOtherValue = oSRSTemp.getPrimeMeridian();
									gtl::debug("OGR_ESRI",
										"morphFromESRI() PRIMEM: this = %.15g other = %.15g",
										dfThisValue, dfOtherValue);
									if (fabs(dfThisValue - dfOtherValue) > 0.0001)
										bIsSame = FALSE;
								}

								/* found a matching spheroid */
								if (bIsSame)
								{
									/* clone DATUM */
									if (cslIEqualString(pszFixWktConfig, "GEOGCS") ||
										cslIEqualString(pszFixWktConfig, "DATUM"))
									{
										SRSNode *poGeogCS = this->getAttrNode("GEOGCS");
										const SRSNode *poDatumOther = oSRSTemp.getAttrNode("DATUM");
										if (poGeogCS && poDatumOther)
										{
											/* make sure we preserve the position of the DATUM node */
											int nPos = poGeogCS->findChild("DATUM");
											if (nPos >= 0)
											{
												poGeogCS->destroyChild(nPos);
												poGeogCS->insertChild(poDatumOther->clone(), nPos);
												gtl::debug("OGR_ESRI",
													"morphFromESRI() cloned DATUM from EPSG:%d",
													nGeogCS);
											}
										}
									}
									/* just copy TOWGS84 */
									else if (cslIEqualString(pszFixWktConfig, "TOWGS84"))
									{
										poNode = oSRSTemp.getAttrNode("DATUM|TOWGS84");
										if (poNode)
										{
											poNode = poNode->clone();
											getAttrNode("DATUM")->addChild(poNode);
											gtl::debug("OGR_ESRI",
												"morphFromESRI() found missing TOWGS84 from EPSG:%d",
												nGeogCS);
										}
									}
									/* exit loop */
									break;
								}
							}
						}
					}
				}
			}
		}
	}

	free(pszDatumOrig);

	return eErr;
}
 

/************************************************************************/
/*                           SetNewName()                               */
/*                                                                      */
/*      Set an esri name                                                */
/************************************************************************/
void SetNewName(SpatialReference* pOgr, const char* keyName, const char* newName)
{
	SRSNode *poNode = pOgr->getAttrNode(keyName);
	SRSNode *poNodeChild = NULL;
	if (poNode)
		poNodeChild = poNode->getChild(0);
	if (poNodeChild)
		poNodeChild->setValue(newName);
}

/************************************************************************/
/*                           RemapImgWGSProjcsName()                    */
/*                                                                      */
/*      Convert Img projcs names to ESRI style                          */
/************************************************************************/
int RemapImgWGSProjcsName(SpatialReference* pOgr, const char* pszProjCSName, const char* pszProgCSName)
{
	if (cslIEqualString(pszProgCSName, "WGS_1972") || cslIEqualString(pszProgCSName, "WGS_1984"))
	{
		char* newName = (char *)malloc(strlen(pszProjCSName) + 10);
		sprintf(newName, "%s_", pszProgCSName);
		strcat(newName, pszProjCSName);
		SetNewName(pOgr, "PROJCS", newName);
		free(newName);
		return 1;
	}
	return -1;
}

/************************************************************************/
/*                           RemapImgUTMNames()                         */
/*                                                                      */
/*      Convert Img UTM names to ESRI style                             */
/************************************************************************/

int RemapImgUTMNames(SpatialReference* pOgr, const char* pszProjCSName, const char* pszProgCSName,
	char **mappingTable)
{
	long i;
	long iIndex = -1;
	for (i = 0; mappingTable[i] != NULL; i += 5)
	{
		if (cslIEqualString(pszProjCSName, mappingTable[i]))
		{
			long j = i;
			while (mappingTable[j] != NULL && cslIEqualString(mappingTable[i], mappingTable[j]))
			{
				if (cslIEqualString(pszProgCSName, mappingTable[j + 1]))
				{
					iIndex = j;
					break;
				}
				j += 5;
			}
			if (iIndex >= 0)
				break;
		}
	}
	if (iIndex >= 0)
	{
		SRSNode *poNode = pOgr->getAttrNode("PROJCS");
		SRSNode *poNodeChild = NULL;
		if (poNode)
			poNodeChild = poNode->getChild(0);
		if (poNodeChild && strlen(poNodeChild->getValue()) > 0)
			poNodeChild->setValue(mappingTable[iIndex + 2]);

		poNode = pOgr->getAttrNode("GEOGCS");
		poNodeChild = NULL;
		if (poNode)
			poNodeChild = poNode->getChild(0);
		if (poNodeChild && strlen(poNodeChild->getValue()) > 0)
			poNodeChild->setValue(mappingTable[iIndex + 3]);

		poNode = pOgr->getAttrNode("DATUM");
		poNodeChild = NULL;
		if (poNode)
			poNodeChild = poNode->getChild(0);
		if (poNodeChild && strlen(poNodeChild->getValue()) > 0)
			poNodeChild->setValue(mappingTable[iIndex + 4]);
	}
	return iIndex;
}

/************************************************************************/
/*                           RemapNameBasedOnKeyName()                  */
/*                                                                      */
/*      Convert a name to ESRI style name                               */
/************************************************************************/

int RemapNameBasedOnKeyName(SpatialReference* pOgr, const char* pszName, const char* pszkeyName,
	char **mappingTable)
{
	long i;
	long iIndex = -1;
	for (i = 0; mappingTable[i] != NULL; i += 2)
	{
		if (cslIEqualString(pszName, mappingTable[i]))
		{
			iIndex = i;
			break;
		}
	}
	if (iIndex >= 0)
	{
		SRSNode *poNode = pOgr->getAttrNode(pszkeyName);
		SRSNode *poNodeChild = NULL;
		if (poNode)
			poNodeChild = poNode->getChild(0);
		if (poNodeChild && strlen(poNodeChild->getValue()) > 0)
			poNodeChild->setValue(mappingTable[iIndex + 1]);
	}
	return iIndex;
}

/************************************************************************/
/*                     RemapNamesBasedOnTwo()                           */
/*                                                                      */
/*      Convert a name to ESRI style name                               */
/************************************************************************/

int RemapNamesBasedOnTwo(SpatialReference* pOgr, const char* name1, const char* name2,
	char **mappingTable, long nTableStepSize,
	char** pszkeyNames, long nKeys)
{
	long i, n, n1;
	long iIndex = -1;
	for (i = 0; mappingTable[i] != NULL; i += nTableStepSize)
	{
		n =(long) strlen(name1);
		n1 =(long) strlen(mappingTable[i]);
		if (cslNIEqualString(name1, mappingTable[i], n1 <= n ? n1 : n))
		{
			long j = i;
			while (mappingTable[j] != NULL && cslIEqualString(mappingTable[i], mappingTable[j]))
			{
				if (cslNIEqualString(name2, mappingTable[j + 1], (int)strlen(mappingTable[j + 1])))
				{
					iIndex = j;
					break;
				}
				j += 3;
			}
			if (iIndex >= 0)
				break;
		}
	}
	if (iIndex >= 0)
	{
		for (i = 0; i < nKeys; i++)
		{
			SRSNode *poNode = pOgr->getAttrNode(pszkeyNames[i]);
			SRSNode *poNodeChild = NULL;
			if (poNode)
				poNodeChild = poNode->getChild(0);
			if (poNodeChild && strlen(poNodeChild->getValue()) > 0)
				poNodeChild->setValue(mappingTable[iIndex + i + 2]);
		}

	}
	return iIndex;
}

/************************************************************************/
/*                RemapPValuesBasedOnProjCSAndPName()                   */
/*                                                                      */
/*      Convert a parameters to ESRI style name                         */
/************************************************************************/

int RemapPValuesBasedOnProjCSAndPName(SpatialReference* pOgr, const char* pszProgCSName,
	char **mappingTable)
{
	long ret = 0;
	SRSNode *poPROJCS = pOgr->getAttrNode("PROJCS");
	for (int i = 0; mappingTable[i] != NULL; i += 4)
	{
		while (mappingTable[i] != NULL && cslNIEqualString(pszProgCSName, mappingTable[i], (int)strlen(mappingTable[i])))
		{
			SRSNode *poParm;
			const char* pszParamName = mappingTable[i + 1];
			const char* pszParamValue = mappingTable[i + 2];
			for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
			{
				poParm = poPROJCS->getChild(iChild);

				if (cslIEqualString(poParm->getValue(), "PARAMETER")
					&& poParm->getChildCount() == 2
					&& cslIEqualString(poParm->getChild(0)->getValue(), pszParamName)
					&& cslNIEqualString(poParm->getChild(1)->getValue(), pszParamValue, (int) strlen(pszParamValue)))
				{
					poParm->getChild(1)->setValue(mappingTable[i + 3]);
					break;
				}
			}
			ret++;
			i += 4;
		}
		if (ret > 0)
			break;
	}
	return ret;
}

/************************************************************************/
/*                  RemapPNamesBasedOnProjCSAndPName()                  */
/*                                                                      */
/*      Convert a parameters to ESRI style name                         */
/************************************************************************/

int RemapPNamesBasedOnProjCSAndPName(SpatialReference* pOgr, const char* pszProgCSName,
	char **mappingTable)
{
	long ret = 0;
	SRSNode *poPROJCS = pOgr->getAttrNode("PROJCS");
	for (int i = 0; mappingTable[i] != NULL; i += 3)
	{
		while (mappingTable[i] != NULL && cslNIEqualString(pszProgCSName, mappingTable[i], (int) strlen(mappingTable[i])))
		{
			SRSNode *poParm;
			const char* pszParamName = mappingTable[i + 1];
			for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
			{
				poParm = poPROJCS->getChild(iChild);

				if (cslIEqualString(poParm->getValue(), "PARAMETER")
					&& poParm->getChildCount() == 2
					&& cslIEqualString(poParm->getChild(0)->getValue(), pszParamName))
				{
					poParm->getChild(0)->setValue(mappingTable[i + 2]);
					break;
				}
			}
			ret++;
			i += 3;
		}
		if (ret > 0)
			break;
	}
	return ret;
}

/************************************************************************/
/*                        DeleteParamBasedOnPrjName                     */
/*                                                                      */
/*      Delete non-ESRI parameters                                      */
/************************************************************************/
int DeleteParamBasedOnPrjName(SpatialReference* pOgr, const char* pszProjectionName,
	char **mappingTable)
{
	long iIndex = -1, ret = -1;
	for (int i = 0; mappingTable[i] != NULL; i += 2)
	{
		if (cslNIEqualString(pszProjectionName, mappingTable[i], (int)strlen(mappingTable[i])))
		{
			SRSNode *poPROJCS = pOgr->getAttrNode("PROJCS");
			SRSNode *poParm;
			const char* pszParamName = mappingTable[i + 1];
			iIndex = -1;
			for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
			{
				poParm = poPROJCS->getChild(iChild);

				if (cslIEqualString(poParm->getValue(), "PARAMETER")
					&& poParm->getChildCount() == 2
					&& cslIEqualString(poParm->getChild(0)->getValue(), pszParamName))
				{
					iIndex = iChild;
					break;
				}
			}
			if (iIndex >= 0)
			{
				poPROJCS->destroyChild(iIndex);
				ret++;
			}
		}
	}
	return ret;
}
/************************************************************************/
/*                          AddParamBasedOnPrjName()                    */
/*                                                                      */
/*      Add ESRI style parameters                                       */
/************************************************************************/
int AddParamBasedOnPrjName(SpatialReference* pOgr, const char* pszProjectionName,
	char **mappingTable)
{
	long ret = -1;
	SRSNode *poPROJCS = pOgr->getAttrNode("PROJCS");
	for (int i = 0; mappingTable[i] != NULL; i += 3)
	{
		if (cslNIEqualString(pszProjectionName, mappingTable[i], (int) strlen(mappingTable[i])))
		{
			SRSNode *poParm;
			int exist = 0;
			for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
			{
				poParm = poPROJCS->getChild(iChild);

				if (cslIEqualString(poParm->getValue(), "PARAMETER")
					&& poParm->getChildCount() == 2
					&& cslIEqualString(poParm->getChild(0)->getValue(), mappingTable[i + 1]))
					exist = 1;
			}
			if (!exist)
			{
				poParm = new SRSNode("PARAMETER");
				poParm->addChild(new SRSNode(mappingTable[i + 1]));
				poParm->addChild(new SRSNode(mappingTable[i + 2]));
				poPROJCS->addChild(poParm);
				ret++;
			}
		}
	}
	return ret;
}

/************************************************************************/
/*                                   RemapGeogCSName()                  */
/*                                                                      */
/*      Convert names to ESRI style                                     */
/************************************************************************/
int RemapGeogCSName(SpatialReference* pOgr, const char *pszGeogCSName)
{
	static const char *keyNamesG[] = {
		"GEOGCS" };
	int ret = -1;

	const char* pszUnitName = pOgr->getAttrValue("GEOGCS|UNIT");
	if (pszUnitName)
		ret = RemapNamesBasedOnTwo(pOgr, pszGeogCSName + 4, pszUnitName, (char**)apszGcsNameMappingBasedOnUnit, 3, (char**)keyNamesG, 1);
	if (ret < 0)
	{
		const char* pszPrimeName = pOgr->getAttrValue("PRIMEM");
		if (pszPrimeName)
			ret = RemapNamesBasedOnTwo(pOgr, pszGeogCSName + 4, pszPrimeName, (char**)apszGcsNameMappingBasedPrime, 3, (char**)keyNamesG, 1);
		if (ret < 0)
			ret = RemapNameBasedOnKeyName(pOgr, pszGeogCSName + 4, "GEOGCS", (char**)apszGcsNameMapping);
	}
	if (ret < 0)
	{
		const char* pszProjCS = pOgr->getAttrValue("PROJCS");
		ret = RemapNamesBasedOnTwo(pOgr, pszProjCS, pszGeogCSName, (char**)apszGcsNameMappingBasedOnProjCS, 3, (char**)keyNamesG, 1);
	}
	return ret;
}

/************************************************************************/
/*                    ImportFromESRIStatePlaneWKT()                     */
/*                                                                      */
/*      Search a ESRI State Plane WKT and import it.                    */
/************************************************************************/

bool SpatialReference::importFromESRIStatePlaneWKT(int code, const char* datumName, const char* unitsName, int pcsCode, const char* csName)
{
	int i;
	long searchCode = -1;

	/* if the CS name is known */
	if (code == 0 && !datumName && !unitsName && pcsCode == 32767 && csName)
	{
		char codeS[10];
		if (findCodeFromDict("esri_StatePlane_extra.wkt", csName, codeS) != true)
			return false;
		return importFromDict("esri_StatePlane_extra.wkt", codeS);
	}

	/* Find state plane prj str by pcs code only */
	if (code == 0 && !datumName && pcsCode != 32767)
	{

		int unitCode = 1;
		if (cslIEqualString(unitsName, "international_feet"))
			unitCode = 3;
		else if (strstr(unitsName, "feet") || strstr(unitsName, "foot"))
			unitCode = 2;
		for (i = 0; statePlanePcsCodeToZoneCode[i] != 0; i += 2)
		{
			if (pcsCode == statePlanePcsCodeToZoneCode[i])
			{
				searchCode = statePlanePcsCodeToZoneCode[i + 1];
				int unitIndex = searchCode % 10;
				if ((unitCode == 1 && !(unitIndex == 0 || unitIndex == 1))
					|| (unitCode == 2 && !(unitIndex == 2 || unitIndex == 3 || unitIndex == 4))
					|| (unitCode == 3 && !(unitIndex == 5 || unitIndex == 6)))
				{
					searchCode -= unitIndex;
					switch (unitIndex)
					{
					case 0:
					case 3:
					case 5:
						if (unitCode == 2)
							searchCode += 3;
						else if (unitCode == 3)
							searchCode += 5;
						break;
					case 1:
					case 2:
					case 6:
						if (unitCode == 1)
							searchCode += 1;
						if (unitCode == 2)
							searchCode += 2;
						else if (unitCode == 3)
							searchCode += 6;
						break;
					case 4:
						if (unitCode == 2)
							searchCode += 4;
						break;
					}
				}
				break;
			}
		}
	}
	else /* Find state plane prj str by all inputs. */
	{
		/* Need to have a specail EPSG-ESRI zone code mapping first. */
		for (i = 0; statePlaneZoneMapping[i] != 0; i += 3)
		{
			if (code == statePlaneZoneMapping[i]
				&& (statePlaneZoneMapping[i + 1] == -1 || pcsCode == statePlaneZoneMapping[i + 1]))
			{
				code = statePlaneZoneMapping[i + 2];
				break;
			}
		}
		searchCode = (long)code * 10;
		if (cslIEqualString(datumName, "HARN"))
		{
			if (cslIEqualString(unitsName, "international_feet"))
				searchCode += 5;
			else if (strstr(unitsName, "feet") || strstr(unitsName, "foot"))
				searchCode += 3;
		}
		else if (strstr(datumName, "NAD") && strstr(datumName, "83"))
		{
			if (cslIEqualString(unitsName, "meters"))
				searchCode += 1;
			else if (cslIEqualString(unitsName, "international_feet"))
				searchCode += 6;
			else if (strstr(unitsName, "feet") || strstr(unitsName, "foot"))
				searchCode += 2;
		}
		else if (strstr(datumName, "NAD") && strstr(datumName, "27") && !cslIEqualString(unitsName, "meters"))
		{
			searchCode += 4;
		}
		else
			searchCode = -1;
	}
	if (searchCode > 0)
	{
		char codeS[10];
		sprintf(codeS, "%d", (int)searchCode);
		return importFromDict("esri_StatePlane_extra.wkt", codeS);
	}
	return false;
}

/************************************************************************/
/*                     ImportFromESRIWisconsinWKT()                     */
/*                                                                      */
/*      Search a ESRI State Plane WKT and import it.                    */
/************************************************************************/

bool SpatialReference::importFromESRIWisconsinWKT(const char* prjName, double centralMeridian, double latOfOrigin, const char* unitsName, const char* csName)
{
	/* if the CS name is known */
	if (!prjName && !unitsName && csName)
	{
		char codeS[10];
		if (findCodeFromDict("esri_Wisconsin_extra.wkt", csName, codeS) != true)
			return false;
		return importFromDict("esri_Wisconsin_extra.wkt", codeS);
	}
	double* tableWISCRS;
	if (cslNIEqualString(prjName, "Lambert_Conformal_Conic", 22))
		tableWISCRS = apszWISCRS_LCC_meter;
	else if (cslIEqualString(prjName, SRS_PT_TRANSVERSE_MERCATOR))
		tableWISCRS = apszWISCRS_TM_meter;
	else
		return false;
	int k = -1;
	for (int i = 0; tableWISCRS[i] != 0; i += 3)
	{
		if (fabs(centralMeridian - tableWISCRS[i]) <= 0.0000000001 && fabs(latOfOrigin - tableWISCRS[i + 1]) <= 0.0000000001)
		{
			k = (long)tableWISCRS[i + 2];
			break;
		}
	}
	if (k > 0)
	{
		if (!cslIEqualString(unitsName, "meters"))
			k += 100;
		char codeS[10];
		sprintf(codeS, "%d", k);
		return importFromDict("esri_Wisconsin_extra.wkt", codeS);
	}
	return false;
}

/************************************************************************/
/*                       findCodeFromDict()                             */
/*                                                                      */
/*      Find the code from a dict file.                                 */
/************************************************************************/
static bool findCodeFromDict(const char* pszDictFile, const char* CSName, char* code)
{
	const char *pszFilename;
	FILE *fp;
	bool eErr = false;

	/* -------------------------------------------------------------------- */
	/*      Find and open file.                                             */
	/* -------------------------------------------------------------------- */
	//pszFilename = CPLFindFile("gdal", pszDictFile);
	pszFilename =(gtl::getDataHome()+std::string("\\")+std::string(pszDictFile)).c_str();
	if (pszFilename == NULL)
		return false;

	fp = fopen(pszFilename, "rb");
	if (fp == NULL)
		return false;

	/* -------------------------------------------------------------------- */
	/*      Process lines.                                                  */
	/* -------------------------------------------------------------------- */
	const char *pszLine;

	while ((pszLine = readLine(fp)) != NULL)

	{
		if (pszLine[0] == '#')
			/* do nothing */;

		else if (strstr(pszLine, CSName))
		{
			const char* pComma = strchr(pszLine, ',');
			if (pComma)
			{
				strncpy(code, pszLine, pComma - pszLine);
				code[pComma - pszLine] = '\0';
				eErr = true;
			}
			break;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Cleanup                                                         */
	/* -------------------------------------------------------------------- */
	fclose(fp);

	return eErr;
}

end_gdb_namespace
end_gtl_namespace