/*
 * $Header: readgeom.h 28-sep-98.10:20:57 jsharma Exp $
 */

/* Copyright (c) Oracle Corporation 1998. All Rights Reserved. */ 
 
 
/* 
   NAME 
     <<<LOWERCASE_FILENAME>>>.h - <one-line expansion of the name> 

   DESCRIPTION 
     <short description of component this file declares/defines> 

   RELATED DOCUMENTS 
 
   INSPECTION STATUS 
     Inspection date: 
     Inspection status: 
     Estimated increasing cost defects per page: 
     Rule sets: 
 
   ACCEPTANCE REVIEW STATUS 
     Review date: 
     Review status: 
     Reviewers: 
 
   PUBLIC FUNCTION(S) 
     <list of external functions declared/defined - with one-line descriptions>

   PRIVATE FUNCTION(S)
     <list of static functions defined in .c file - with one-line descriptions>

   EXAMPLES

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   jsharma     09/28/98 - Sample code to read an SDO_GEOMETRY type
   jsharma     09/28/98 - Creation

*/


#ifndef readgeom_ORACLE
# define readgeom_ORACLE


#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif

/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

#define ARRAY_SIZE                 32
#define TYPE_OWNER                 "MDSYS"
#define SDO_GEOMETRY               TYPE_OWNER".SDO_GEOMETRY"

/*************************************************************************/
/* Type definitions                                                      */
/*************************************************************************/
struct sdo_point_type
{
   OCINumber x;
   OCINumber y;
   OCINumber z;
};
typedef struct sdo_point_type sdo_point_type;

typedef OCIArray sdo_elem_info_array;
typedef OCIArray sdo_ordinate_array;

struct sdo_geometry
{
   OCINumber      sdo_gtype;
   OCINumber      sdo_srid;
   sdo_point_type sdo_point;
   OCIArray       *sdo_elem_info;
   OCIArray       *sdo_ordinates;
};

typedef struct sdo_geometry SDO_GEOMETRY_TYPE;

/***
** Indicator structures for SDO_GEOMETRY_TYPE
***/

struct sdo_point_type_ind
{
   OCIInd _atomic;
   OCIInd x;
   OCIInd y;
   OCIInd z;
};
typedef struct sdo_point_type_ind sdo_point_type_ind;

struct SDO_GEOMETRY_ind
{
   OCIInd                    _atomic;
   OCIInd                    sdo_gtype;
   OCIInd                    sdo_srid;
   struct sdo_point_type_ind sdo_point;
   OCIInd                    sdo_elem_info;
   OCIInd                    sdo_ordinates;
};
typedef struct SDO_GEOMETRY_ind SDO_GEOMETRY_ind;

/*************************************************************************/
/* Public Functions                                                      */
/*************************************************************************/
void checkerr(OCIError *, sword);


#endif                                              /* readgeom_ORACLE */
