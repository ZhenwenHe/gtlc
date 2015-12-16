/*
 * $Header: geomops.h 20-oct-98.06:41:56 pfwang Exp $
 */

/* Copyright (c) Oracle Corporation 1998. All Rights Reserved. */ 
 
 
/* 
   NAME 
     <<<LOWERCASE_FILENAME>>>.h - <one-line expansion of the name> 

   DESCRIPTION 
     <short description of component this file declares/defines> 

   RELATED DOCUMENTS 
 
   PUBLIC FUNCTION(S) 
     <list of external functions declared/defined - with one-line descriptions>

   PRIVATE FUNCTION(S)
     <list of static functions defined in .c file - with one-line descriptions>

   EXAMPLES

   NOTES
     <other useful comments, qualifications, etc.>

   MODIFIED   (MM/DD/YY)
   pfwang      10/20/98 -
   jsharma     09/28/98 - Sample code for operations on SDO_GEOMETRY type
   jsharma     09/28/98 - Creation

*/

#ifndef geomops_ORACLE
# define geomops_ORACLE

#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif

#include "readgeom.h"

/*************************************************************************/
/* Public functions                                                      */
/*************************************************************************/
int  sc_ops_geom_null(void);
void sc_ops_init_geometry (SDO_GEOMETRY_TYPE *, SDO_GEOMETRY_ind  *, int);
void sc_ops_get_gtype (void);
void sc_ops_get_sdo_point (void);
int  sc_ops_next_elem (void);
ub4  sc_ops_get_element_type (void);
void sc_ops_get_type1 (void);
void sc_ops_get_type2 (void);
void sc_ops_get_type3 (void);
void sc_ops_get_type4 (void);
void sc_ops_get_type5 (void);

#endif  /* geomops_ORACLE */
