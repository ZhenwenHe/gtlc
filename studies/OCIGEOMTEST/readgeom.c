#ifdef RCSID
static char *RCSid =
"$Header: readgeom.c 22-mar-2005.09:09:58 sravada Exp $ ";
#endif /* RCSID */

/* Copyright (c) 1998, 2005, Oracle. All rights reserved.  */

/*

   NAME
     readgeom.c - <one-line expansion of the name>

   DESCRIPTION
     <short description of component this file declares/defines>

   PUBLIC FUNCTION(S)
     <list of external functions declared/defined - with one-line descriptions>

   PRIVATE FUNCTION(S)
     <list of static functions defined in .c file - with one-line descriptions>

   RETURNS
     <function return values, for .c file with single function>

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   sravada     03/22/05 - bug 4241090 
   sravada     04/12/00 - remove OCI_THREADED
   jsharma     09/28/98 - Sample code to read an SDO_GEOMETRY type
   jsharma     09/28/98 - Creation

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <oci1.h>
#include <oci.h>
#include <math.h>

#include "readgeom.h"
#include "geomops.h"

/*******************************************************************************
**                        PUBLIC GLOBALS
*******************************************************************************/
OCIEnv 		*envhp;                       /* OCI general handles */
OCIError 	*errhp;


/*******************************************************************************
**                        PRIVATE GLOBALS
*******************************************************************************/
static OCIServer 	*srvhp;
static OCISvcCtx 	*svchp;
static OCISession 	*usrhp;
static OCIStmt 		*stmthp;
static OCIDescribe	*dschp = NULL;

static OCINumber         global_gid[ARRAY_SIZE];
static OCIType           *geom_tdo = NULL;
static SDO_GEOMETRY_TYPE *global_geom_obj[ARRAY_SIZE];/* spatial object buffer */
static SDO_GEOMETRY_ind  *global_geom_ind[ARRAY_SIZE]; /* Object indicator */

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

	if (status != OCI_SUCCESS && status != OCI_SUCCESS_WITH_INFO)
		mddisconnect();
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
void mdrconnect(char *username, char *password)
{

	int maxsz = 50 ;  /* 50% increase ? */
	int optsz = 2048000 ;  /* 2000 K bytes */
	int curmaxsz, curoptsz;

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

	geom_tdo = get_tdo(SDO_GEOMETRY);

	printf ("\nConnected to Oracle.\n");
}


/*******************************************************************************
**
** Routine:     process_data
**
** Description: Process the data for current host array fetch.
**
*******************************************************************************/
void process_data(int  num_dimensions,
char *id_column,
int  rows_to_process, 
int  *rows_processed)
{
	int            row;
	double         double_gid;

	for (row = 0; row < rows_to_process; row++, (*rows_processed)++)
	{
		/*
		    ** Get ID_COLUMN
		    */
		checkerr(errhp, OCINumberToReal(errhp, &(global_gid[row]), 
		    (uword)sizeof(double), 
		    (dvoid *)&double_gid));

		printf("\n%s %.5lf\n", id_column, double_gid);

		sc_ops_init_geometry (global_geom_obj[row], 
		    global_geom_ind[row],
		    num_dimensions);

		if (sc_ops_geom_null())
		{
			printf ("OBJECT IS NULL\n");
			fflush (stdout);
		}
		else
		{
			sc_ops_get_gtype ();
			sc_ops_get_sdo_point ();

			/* Loop through and print all the elements for this geometry */
			while (sc_ops_next_elem ())
			{
				switch (sc_ops_get_element_type ())
				{
				case 1: 
					sc_ops_get_type1();
					break;
				case 2: 
					sc_ops_get_type2();
					break;
				case 3: 
					sc_ops_get_type3();
					break;
				case 4: 
					sc_ops_get_type4();
					break;
				case 5: 
					sc_ops_get_type5();
					break;
				}
			}

			/* free the spatial object instance */
			checkerr(errhp, OCIObjectFree(envhp, errhp, (dvoid *)global_geom_obj[row], 
			    (ub2)OCI_OBJECTFREE_FORCE));
			global_geom_obj[row] = NULL;
		}
	} /* end of for-loop: row */
}


/*******************************************************************************
**
** Routine:     read_geometries
**
** Description: 
**
*******************************************************************************/
void read_geometries (char *table, 
char *geom_column, 
char *id_column, 
int   num_dimensions)
{
	int       i, 
	nrows = 0,
	rows_fetched = 0, 
	rows_processed = 0,
	rows_to_process = 0;
	char      query[100];
	sword     status;
	boolean   has_more_data = TRUE;
	OCIDefine *defn1p = NULL, 
	    *defn2p = NULL;

	/* construct query */
	sprintf(query, "SELECT %s, %s FROM %s", id_column, geom_column, table);

	/* parse query */
	checkerr(errhp, OCIStmtPrepare(stmthp, errhp, 
	    (text *)query, (ub4)strlen(query), 
	    (ub4)OCI_NTV_SYNTAX, (ub4)OCI_DEFAULT));

	/* define GID and spatial ADT object */
	checkerr(errhp, OCIDefineByPos(stmthp, &defn1p, errhp, (ub4)1, 
	    (dvoid *)global_gid, 
	    (sb4)sizeof(OCINumber), SQLT_VNU,
	    (dvoid *)0, (ub2 *)0, (ub2 *)0, 
	    (ub4)OCI_DEFAULT));

	checkerr(errhp, OCIDefineByPos(stmthp, &defn2p, errhp, (ub4)2, 
	    (dvoid *)0, (sb4)0, SQLT_NTY, (dvoid *)0,
	    (ub2 *)0, (ub2 *)0, (ub4)OCI_DEFAULT));

	checkerr(errhp, OCIDefineObject(defn2p, errhp, geom_tdo, 
	    (dvoid **)global_geom_obj, (ub4 *)0, 
	    (dvoid **)global_geom_ind, (ub4 *)0));

	/* execute */
	status = OCIStmtExecute(svchp, stmthp, errhp, (ub4)ARRAY_SIZE, (ub4)0, 
	    (OCISnapshot *)NULL, (OCISnapshot *)NULL, 
	    (ub4)OCI_DEFAULT);

	if (status == OCI_SUCCESS_WITH_INFO || status == OCI_NO_DATA)
		has_more_data = FALSE;
		else
	{
		has_more_data = TRUE;
		checkerr(errhp, status);
	}

	/* process data */
	checkerr(errhp, OCIAttrGet((dvoid *)stmthp, (ub4)OCI_HTYPE_STMT,
	    (dvoid *)&rows_fetched, (ub4 *)0, 
	    (ub4)OCI_ATTR_ROW_COUNT, errhp));
	rows_to_process = rows_fetched - rows_processed;

	process_data(num_dimensions, id_column, 
	    rows_to_process, &rows_processed);

	while (has_more_data)
	{
		status = OCIStmtFetch(stmthp, errhp, (ub4)ARRAY_SIZE, 
		    (ub2)OCI_FETCH_NEXT, (ub4)OCI_DEFAULT);

		if (status != OCI_SUCCESS)
			has_more_data = FALSE;

		/* process data */
		checkerr(errhp, OCIAttrGet((dvoid *)stmthp, (ub4)OCI_HTYPE_STMT,
		    (dvoid *)&rows_fetched, (ub4 *)0, 
		    (ub4)OCI_ATTR_ROW_COUNT, errhp));
		rows_to_process = rows_fetched - rows_processed;

		process_data(num_dimensions, id_column, 
		    rows_to_process, &rows_processed);
	}

	if (status != OCI_SUCCESS_WITH_INFO && status != OCI_NO_DATA)
		checkerr(errhp, status);
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

	/* print out title */
	printf("\nSpatial Cartridge Objects - Read/Print\n");
	printf("(c) Copyright 1998 Oracle Corporation. All rights reserved.\n\n");

	/*if( argc < 7 )
	{
		printf("USAGE: %s <username> <password> \n"
		    "              <table_name> <geom_column_name> <id_column_name> \n"
		    "              <num_dimensions>\n\n", argv[0]);
		printf("   <username>         - Oracle user\n");
		printf("   <password>         - Oracle password\n");
		printf("   <table_name>       - Table with geometry column\n");
		printf("   <geom_column_name> - Geometry column name\n");
		printf("   <id_column_name>   - Unique id column name\n");
		printf("   <num_dimensions>   - Number of ordinates per point\n");
		exit( 1 );
	}
	else
	{
		user = argv[1];
		passwd = argv[2];
		table = argv[3];
		geom_column = argv[4];
		id_column = argv[5];
		num_dimensions = atoi(argv[6]);
	}*/

	user = "C##MVDEMO";
	passwd ="C##MVDEMO";
	table = "COUNTIES";
	geom_column = "GEOM";
	id_column = "1";
	num_dimensions = 2;

	/* connect to oracle */
	mdrconnect(user, passwd);

	read_geometries(table, geom_column, id_column, num_dimensions);

	/* disconnect from oracle */
	mddisconnect();
}




/* end of file readgeom.c */

