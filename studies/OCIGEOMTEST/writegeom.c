/* Copyright (c) 1997, 2005, Oracle. All rights reserved.  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci1.h>
#include <oci.h>
#include <math.h>

#include "writegeom.h"

/*******************************************************************************
**                        PUBLIC GLOBALS
*******************************************************************************/
OCIEnv 		*envhp;                       /* OCI general handles */
OCIError 	*errhp;

static double test_ordinates[] = {
	0, 0
	,1, 1
	,2, 2
	,3, 3
	,4, 4
	,5, 5
	,6, 6
	,7, 7
	,8, 8
	,9, 9
	,10, 10
	,11, 11
	,12, 12
	,13, 13
	,14, 14
	,15, 15
	,16, 16
	,17, 17
	,18, 18
	,19, 19
	,20, 20
	,21, 21
	,22, 22
	,23, 23
	,24, 24
	,25, 25
	,26, 26
	,27, 27
	,28, 28
	,29, 29
	,30, 30
	,31, 31
	,32, 32
	,33, 33
	,34, 34
	,35, 35
	,36, 36
	,37, 37
	,38, 38
	,39, 39
	,40, 40
	,41, 41
	,42, 42
	,43, 43
	,44, 44
	,45, 45
	,46, 46
	,47, 47
	,48, 48
	,49, 49
	,50, 50
	,51, 51
	,52, 52
	,53, 53
	,54, 54
	,55, 55
	,56, 56
	,57, 57
	,58, 58
	,59, 59
	,60, 60
	,61, 61
	,62, 62
	,63, 63
	,64, 64
	,65, 65
	,66, 66
	,67, 67
	,68, 68
	,69, 69
	,70, 70
	,71, 71
	,72, 72
	,73, 73
	,74, 74
	,75, 75
	,76, 76
	,77, 77
	,78, 78
	,79, 79
	,80, 80
	,81, 81
	,82, 82
	,83, 83
	,84, 84
	,85, 85
	,86, 86
	,87, 87
	,88, 88
	,89, 89
	,90, 90
	,91, 91
	,92, 92
	,93, 93
	,94, 94
	,95, 95
	,96, 96
	,97, 97
	,98, 98
	,99, 99
	,100, 100
	,101, 101
	,102, 102
	,103, 103
	,104, 104
	,105, 105
	,106, 106
	,107, 107
	,108, 108
	,109, 109
	,110, 110
	,111, 111
	,112, 112
	,113, 113
	,114, 114
	,115, 115
	,116, 116
	,117, 117
	,118, 118
	,119, 119
	,120, 120
	,121, 121
	,122, 122
	,123, 123
	,124, 124
	,125, 125
	,126, 126
	,127, 127
	,128, 128
	,129, 129
	,130, 130
	,131, 131
	,132, 132
	,133, 133
	,134, 134
	,135, 135
	,136, 136
	,137, 137
	,138, 138
	,139, 139
	,140, 140
	,141, 141
	,142, 142
	,143, 143
	,144, 144
	,145, 145
	,146, 146
	,147, 147
	,148, 148
	,149, 149
	,150, 150
	,151, 151
	,152, 152
	,153, 153
	,154, 154
	,155, 155
	,156, 156
	,157, 157
	,158, 158
	,159, 159
	,160, 160
	,161, 161
	,162, 162
	,163, 163
	,164, 164
	,165, 165
	,166, 166
	,167, 167
	,168, 168
	,169, 169
	,170, 170
	,171, 171
	,172, 172
	,173, 173
	,174, 174
	,175, 175
	,176, 176
	,177, 177
	,178, 178
	,179, 179
	,180, 180
	,181, 181
	,182, 182
	,183, 183
	,184, 184
	,185, 185
	,186, 186
	,187, 187
	,188, 188
	,189, 189
	,190, 190
	,191, 191
	,192, 192
	,193, 193
	,194, 194
	,195, 195
	,196, 196
	,197, 197
	,198, 198
	,199, 199
	,200, 200
	,201, 201
	,202, 202
	,203, 203
	,204, 204
	,205, 205
	,206, 206
	,207, 207
	,208, 208
	,209, 209
	,210, 210
	,211, 211
	,212, 212
	,213, 213
	,214, 214
	,215, 215
	,216, 216
	,217, 217
	,218, 218
	,219, 219
	,220, 220
	,221, 221
	,222, 222
	,223, 223
	,224, 224
	,225, 225
	,226, 226
	,227, 227
	,228, 228
	,229, 229
	,230, 230
	,231, 231
	,232, 232
	,233, 233
	,234, 234
	,235, 235
	,236, 236
	,237, 237
	,238, 238
	,239, 239
	,240, 240
	,241, 241
	,242, 242
	,243, 243
	,244, 244
	,245, 245
	,246, 246
	,247, 247
	,248, 248
	,249, 249
	,250, 250
	,251, 251
	,252, 252
	,253, 253
	,254, 254
	,255, 255
	,256, 256
	,257, 257
	,258, 258
	,259, 259
	,260, 260
	,261, 261
	,262, 262
	,263, 263
	,264, 264
	,265, 265
	,266, 266
	,267, 267
	,268, 268
	,269, 269
	,270, 270
	,271, 271
	,272, 272
	,273, 273
	,274, 274
	,275, 275
	,276, 276
	,277, 277
	,278, 278
	,279, 279
	,280, 280
	,281, 281
	,282, 282
	,283, 283
	,284, 284
	,285, 285
	,286, 286
	,287, 287
	,288, 288
	,289, 289
	,290, 290
	,291, 291
	,292, 292
	,293, 293
	,294, 294
	,295, 295
	,296, 296
	,297, 297
	,298, 298
	,299, 299
	,300, 300
	,301, 301
	,302, 302
	,303, 303
	,304, 304
	,305, 305
	,306, 306
	,307, 307
	,308, 308
	,309, 309
	,310, 310
	,311, 311
	,312, 312
	,313, 313
	,314, 314
	,315, 315
	,316, 316
	,317, 317
	,318, 318
	,319, 319
	,320, 320
	,321, 321
	,322, 322
	,323, 323
	,324, 324
	,325, 325
	,326, 326
	,327, 327
	,328, 328
	,329, 329
	,330, 330
	,331, 331
	,332, 332
	,333, 333
	,334, 334
	,335, 335
	,336, 336
	,337, 337
	,338, 338
	,339, 339
	,340, 340
	,341, 341
	,342, 342
	,343, 343
	,344, 344
	,345, 345
	,346, 346
	,347, 347
	,348, 348
	,349, 349
	,350, 350
	,351, 351
	,352, 352
	,353, 353
	,354, 354
	,355, 355
	,356, 356
	,357, 357
	,358, 358
	,359, 359
	,360, 360
	,361, 361
	,362, 362
	,363, 363
	,364, 364
	,365, 365
	,366, 366
	,367, 367
	,368, 368
	,369, 369
	,370, 370
	,371, 371
	,372, 372
	,373, 373
	,374, 374
	,375, 375
	,376, 376
	,377, 377
	,378, 378
	,379, 379
	,380, 380
	,381, 381
	,382, 382
	,383, 383
	,384, 384
	,385, 385
	,386, 386
	,387, 387
	,388, 388
	,389, 389
	,390, 390
	,391, 391
	,392, 392
	,393, 393
	,394, 394
	,395, 395
	,396, 396
	,397, 397
	,398, 398
	,399, 399
	,400, 400
	,401, 401
	,402, 402
	,403, 403
	,404, 404
	,405, 405
	,406, 406
	,407, 407
	,408, 408
	,409, 409
	,410, 410
	,411, 411
	,412, 412
	,413, 413
	,414, 414
	,415, 415
	,416, 416
	,417, 417
	,418, 418
	,419, 419
	,420, 420
	,421, 421
	,422, 422
	,423, 423
	,424, 424
	,425, 425
	,426, 426
	,427, 427
	,428, 428
	,429, 429
	,430, 430
	,431, 431
	,432, 432
	,433, 433
	,434, 434
	,435, 435
	,436, 436
	,437, 437
	,438, 438
	,439, 439
	,440, 440
	,441, 441
	,442, 442
	,443, 443
	,444, 444
	,445, 445
	,446, 446
	,447, 447
	,448, 448
	,449, 449
	,450, 450
	,451, 451
	,452, 452
	,453, 453
	,454, 454
	,455, 455
	,456, 456
	,457, 457
	,458, 458
	,459, 459
	,460, 460
	,461, 461
	,462, 462
	,463, 463
	,464, 464
	,465, 465
	,466, 466
	,467, 467
	,468, 468
	,469, 469
	,470, 470
	,471, 471
	,472, 472
	,473, 473
	,474, 474
	,475, 475
	,476, 476
	,477, 477
	,478, 478
	,479, 479
	,480, 480
	,481, 481
	,482, 482
	,483, 483
	,484, 484
	,485, 485
	,486, 486
	,487, 487
	,488, 488
	,489, 489
	,490, 490
	,491, 491
	,492, 492
	,493, 493
	,494, 494
	,495, 495
	,496, 496
	,497, 497
	,498, 498
	,499, 499
	,500, 500};

/*******************************************************************************
**                        PRIVATE GLOBALS
*******************************************************************************/
static OCIServer 	*srvhp;
static OCISvcCtx 	*svchp;
static OCISession 	*usrhp;
static OCIStmt 		*stmthp;
static OCIDescribe	*dschp = NULL;

static OCIType  *elem_info_tdo = NULL;     /* Varrays and the type descriptors */
static OCIArray *elem_info;
static OCIType  *ordinates_tdo = NULL;
static OCIArray *ordinates;

/*******************************************************************************
**                        PRIVATE FUNCTIONS
*******************************************************************************/
void mddisconnect(void);


/*******************************************************************************
**
** Routine:     checkerr
**
** Description: Error message routine
**
*******************************************************************************/
void checkerr(OCIError *errhp, sword status)
{
	text errbuf[512];
	sb4 errcode = 0;

	switch (status)
	{
	case OCI_SUCCESS:
		break;
	case OCI_SUCCESS_WITH_INFO:
		fprintf(stderr, "OCI_SUCCESS_WITH_INFO\n");
		break;
	case OCI_ERROR:
		OCIErrorGet((dvoid *)errhp, (ub4)1, (text *)NULL, &errcode, 
		    errbuf, (ub4)sizeof(errbuf), OCI_HTYPE_ERROR);
		fprintf(stderr, "%.*s\n", 512, errbuf);
		break;
	case OCI_NEED_DATA:
		fprintf(stderr, "OCI_NEED_DATA\n");
		break;
	case OCI_NO_DATA:
		fprintf(stderr, "OCI_NO_DATA\n");
		break;
	case OCI_INVALID_HANDLE:
		fprintf(stderr, "OCI_INVALID_HANDLE\n");
		break;
	case OCI_STILL_EXECUTING:
		fprintf(stderr, "OCI_STILL_EXECUTING\n");
		break;
	case OCI_CONTINUE:
		fprintf(stderr, "OCI_CONTINUE\n");
		break;
	default:
		break;
	}

	if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO){
		mddisconnect();
		exit(1);
	}
}

/*******************************************************************************
**
** Routine:     disconnect
**
** Description: Disconnect from Oracle
**
*******************************************************************************/
void mddisconnect(void)
{
	/* free the OCIArray objects */
	checkerr(errhp, OCIObjectFree(envhp, errhp, (dvoid *)ordinates, 
	    (ub2)OCI_OBJECTFREE_FORCE));
	checkerr(errhp, OCIObjectFree(envhp, errhp, (dvoid *)elem_info, 
	    (ub2)OCI_OBJECTFREE_FORCE));

	/* finalize type descriptor */
	checkerr(errhp, OCIHandleFree((dvoid *)dschp, (ub4)OCI_HTYPE_DESCRIBE));

	/* finalize stmthp */
	OCIHandleFree((dvoid *)stmthp, (ub4)OCI_HTYPE_STMT);

	/* session ends */
	OCISessionEnd(svchp, errhp, usrhp, (ub4)OCI_DEFAULT);
	OCIServerDetach(srvhp, errhp, (ub4)OCI_DEFAULT);

	/* finalize svchp, srvhp, and errhp */
	OCIHandleFree((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX);
	OCIHandleFree((dvoid *)srvhp, (ub4)OCI_HTYPE_SERVER);
	OCIHandleFree((dvoid *)errhp, (ub4)OCI_HTYPE_ERROR);

	printf ("\nDisconnected from Oracle.\n");
	exit(0);
}


/******************************************************************************
**
** Routine:     get_tdo
**
** Description: Returns the 
**
******************************************************************************/
OCIType *get_tdo(char *typename)
{
	OCIParam *paramp = NULL;
	OCIRef *type_ref = NULL;
	OCIType *tdo = NULL;

	checkerr(errhp, OCIDescribeAny(svchp, errhp, (text *)typename, 
	    (ub4)strlen((char *)typename), 
	    OCI_OTYPE_NAME, (ub1)1, 
	    (ub1)OCI_PTYPE_TYPE, dschp));
	checkerr(errhp, OCIAttrGet((dvoid *)dschp, (ub4)OCI_HTYPE_DESCRIBE,
	    (dvoid *)&paramp, (ub4 *)0, 
	    (ub4)OCI_ATTR_PARAM, errhp));
	checkerr(errhp, OCIAttrGet((dvoid *)paramp, (ub4)OCI_DTYPE_PARAM,
	    (dvoid *)&type_ref, (ub4 *)0, 
	    (ub4)OCI_ATTR_REF_TDO, errhp));
	checkerr(errhp, OCIObjectPin(envhp, errhp, type_ref, (OCIComplexObject *)0, 
	    OCI_PIN_ANY, OCI_DURATION_SESSION, 
	    OCI_LOCK_NONE, (dvoid **)&tdo));
	if (!tdo)
	{
		fprintf(stderr, "Null tdo returned for type %s.\n", typename);
		mddisconnect();
	}

	return tdo;
}


/*******************************************************************************
**
** Routine:     connect
**
** Description: Connects to oracle, and set error routine sqlerror.
**
*******************************************************************************/
void mdwconnect(char *username, char *password)
{
	/* Allocate and initialize OCI environment handle, envhp */
	OCIInitialize((ub4)(OCI_OBJECT), (dvoid *)0, 
	    (dvoid *(*)())0, (dvoid *(*)())0, (void (*)())0);
	OCIEnvInit(&envhp, (ub4)OCI_DEFAULT, (size_t)0, (dvoid **)0);

	/* 
	  **  Initialize error report handle, errhp
	  **  Initialize sever context handle, srvhp 
	  */
	OCIHandleAlloc((dvoid *)envhp, (dvoid **)&errhp, (ub4)OCI_HTYPE_ERROR,
	    (size_t)0, (dvoid **)0);
	OCIHandleAlloc((dvoid *)envhp, (dvoid **)&srvhp, (ub4)OCI_HTYPE_SERVER,
	    (size_t)0, (dvoid **)0);
	OCIServerAttach(srvhp, errhp, (text *)0, (sb4)0, (ub4)OCI_DEFAULT);

	/* initialize svchp */
	OCIHandleAlloc((dvoid *)envhp, (dvoid **)&svchp, (ub4)OCI_HTYPE_SVCCTX, 
	    (size_t)0, (dvoid **)0);
	OCIAttrSet((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX, (dvoid *)srvhp, (ub4)0, 
	    (ub4)OCI_ATTR_SERVER, errhp);

	/* initialize usrhp */
	OCIHandleAlloc((dvoid *)envhp, (dvoid **)&usrhp, (ub4)OCI_HTYPE_SESSION, 
	    (size_t)0, (dvoid **)0);
	OCIAttrSet((dvoid *)usrhp, (ub4)OCI_HTYPE_SESSION, 
	    (dvoid *)username, (ub4)strlen(username), 
	    (ub4)OCI_ATTR_USERNAME, errhp);
	OCIAttrSet((dvoid *)usrhp, (ub4)OCI_HTYPE_SESSION, 
	    (dvoid *)password, (ub4)strlen(password), 
	    (ub4)OCI_ATTR_PASSWORD, errhp);

	/* session begins */
	checkerr(errhp, OCISessionBegin(svchp, errhp, usrhp, OCI_CRED_RDBMS,
	    OCI_DEFAULT));
	OCIAttrSet((dvoid *)svchp, (ub4)OCI_HTYPE_SVCCTX, (dvoid *)usrhp, (ub4)0, 
	    (ub4)OCI_ATTR_SESSION, errhp);


	/* initialize stmthp */
	checkerr(errhp, OCIHandleAlloc((dvoid *)envhp, (dvoid **)&stmthp, 
	    (ub4)OCI_HTYPE_STMT, (size_t)0, (dvoid **)0));

	/* describe spatial object types */
	checkerr(errhp, OCIHandleAlloc(envhp, (dvoid **)&dschp, 
	    (ub4)OCI_HTYPE_DESCRIBE, (size_t)0,
	    (dvoid **)0));

	elem_info_tdo = get_tdo(SDO_ELEM_INFO_ARRAY);
	ordinates_tdo = get_tdo(SDO_ORDINATE_ARRAY);

	/* instantiate OCIArray objects */
	checkerr(errhp, OCIObjectNew(envhp, errhp, svchp, OCI_TYPECODE_VARRAY,
	    elem_info_tdo, (dvoid *)NULL, 
	    OCI_DURATION_SESSION,
	    FALSE, (dvoid **)&elem_info));
	checkerr(errhp, OCIObjectNew(envhp, errhp, svchp, OCI_TYPECODE_VARRAY,
	    ordinates_tdo, (dvoid *)NULL, 
	    OCI_DURATION_SESSION,
	    FALSE, (dvoid **)&ordinates));

	printf ("\nConnected to Oracle.\n");
}


/*******************************************************************************
**
** Routine:     write_geometries
**
** Description: 
**
*******************************************************************************/
void write_geometries(char *table,
	char *geom_column,
	char *id_column,
	int   num_dimensions)
{
	int i;
	int starting_offset = 1;
	int element_type = 2;
	int interpretation = 1;
	OCINumber oci_number;
	OCIBind *bnd1p = NULL, *bnd2p = NULL;
	char query[500];

	for (i = 0; i < 1002; i++)
	{
		checkerr(errhp, OCINumberFromReal(errhp, (dvoid *)&(test_ordinates[i]), 
		    (uword)sizeof(double),(dvoid *)&oci_number));

		checkerr(errhp, OCICollAppend(envhp, errhp, 
		    (dvoid *) &oci_number,
		    (dvoid *)0, (OCIColl *)ordinates));
	}

	checkerr(errhp, OCINumberFromInt(errhp, (dvoid *)&starting_offset, 
	    (uword)sizeof(ub4), OCI_NUMBER_UNSIGNED, 
	    (dvoid *)&oci_number));
	checkerr(errhp, OCICollAppend(envhp, errhp, (dvoid *)&oci_number,
	    (dvoid *)0, (OCIColl *)elem_info));

	checkerr(errhp, OCINumberFromInt(errhp, (dvoid *)&element_type, 
	    (uword)sizeof(ub4), OCI_NUMBER_UNSIGNED, 
	    (dvoid *)&oci_number));
	checkerr(errhp, OCICollAppend(envhp, errhp, (dvoid *)&oci_number,
	    (dvoid *)0, (OCIColl *)elem_info));

	checkerr(errhp, OCINumberFromInt(errhp, (dvoid *)&interpretation, 
	    (uword)sizeof(ub4), OCI_NUMBER_UNSIGNED, 
	    (dvoid *)&oci_number));
	checkerr(errhp, OCICollAppend(envhp, errhp, (dvoid *)&oci_number,
	    (dvoid *)0, (OCIColl *)elem_info));

	sprintf(query, "INSERT INTO %s (%s, %s) "
	    "VALUES (1, %s(4, NULL, NULL, :elem_info, :ordinates))", 
		table, id_column,geom_column, SDO_GEOMETRY);

	checkerr(errhp, OCIStmtPrepare(stmthp, errhp, 
	    (text *)query, (ub4)strlen(query), 
	    (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT));


	/* bind info_obj varray object */
	checkerr(errhp, OCIBindByName(stmthp, &bnd1p, errhp, 
	    (text *)":elem_info", (sb4)-1, (dvoid *)0, 
	    (sb4)0, SQLT_NTY, (dvoid *)0, (ub2 *)0, 
	    (ub2 *)0, (ub4)0, (ub4 *)0, 
	    (ub4)OCI_DEFAULT));
	checkerr(errhp, OCIBindObject(bnd1p, errhp, elem_info_tdo, 
	    (dvoid **)&elem_info, (ub4 *)0, 
	    (dvoid **)0, (ub4 *)0));
	/* bind coordinate varray object */
	checkerr(errhp, OCIBindByName(stmthp, &bnd2p, errhp, 
	    (text *)":ordinates", (sb4)-1, (dvoid *)0, 
	    (sb4)0, SQLT_NTY, (dvoid *)0, (ub2 *)0, 
	    (ub2 *)0, (ub4)0, (ub4 *)0, 
	    (ub4)OCI_DEFAULT));
	checkerr(errhp, OCIBindObject(bnd2p, errhp, ordinates_tdo, 
	    (dvoid **)&ordinates, (ub4 *)0, 
	    (dvoid **)0, (ub4 *)0));

	checkerr(errhp, OCIStmtExecute(svchp, stmthp, errhp, (ub4)1, (ub4)0, 
	    (OCISnapshot *)NULL, (OCISnapshot *)NULL, 
	    (ub4)OCI_DEFAULT));
}


/*******************************************************************************
**
** Routine:     Main
**
** Description: Program main
**
*******************************************************************************/
void main(int argc, char **argv)
{
	int  num_dimensions;
	char *user,
	*passwd,
	*table,
	*geom_column,
	*id_column;

//CREATE TABLE TEST (GID NUMBER(10) , GEOM SDO_GEOMETRY);
	user = "C##MVDEMO";
	passwd = "C##MVDEMO";
	table = "TEST";
	geom_column = "GEOM";
	id_column = "GID";
	num_dimensions = 2;

	

	/* print out title */
	printf("\nSpatial Cartridge Objects - Write\n");
	printf("(c) Copyright 1997 Oracle Corporation. All rights reserved.\n\n");

	/*if (argc < 3) 
	{
		fprintf(stderr, "Usage: %s user password\n", argv[0]);
		exit(1);
	}*/

	/* connect to oracle */
	mdwconnect(user, passwd);

	write_geometries(table,geom_column,id_column,2);

	/* disconnect from oracle */
	mddisconnect();
}

