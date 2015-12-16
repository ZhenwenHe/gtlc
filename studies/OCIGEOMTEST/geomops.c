#ifdef RCSID
static char *RCSid =
   "$Header: geomops.c 20-oct-98.06:41:03 pfwang Exp $ ";
#endif /* RCSID */

/* Copyright (c) Oracle Corporation 1998. All Rights Reserved. */

/*

   NAME
     geomops.c - <one-line expansion of the name>

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
   pfwang      10/20/98 -
   jsharma     10/14/98 - Dan's updates
   jsharma     09/28/98 - Sample code for operations on SDO_GEOMETRY type
   jsharma     09/28/98 - Creation

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "geomops.h"

/*******************************************************************************
**                        PUBLIC GLOBALS
*******************************************************************************/
extern OCIEnv 		*envhp;                       /* OCI general handles */
extern OCIError 	*errhp;

/*******************************************************************************
**                        PRIVATE GLOBALS
*******************************************************************************/
static SDO_GEOMETRY_TYPE *global_geom;
static SDO_GEOMETRY_ind  *global_geom_ind;
static int global_num_dimensions;
static int global_elem_index;
static int global_ending_offset = -1;
static int global_nelems;
static int global_nords;
static int global_first_elem;

/*******************************************************************************
**
** Routine:     sc_ops_geom_null
**
** Description: Retruns 1 if the geometry object is NULL.
**
*******************************************************************************/
int
sc_ops_geom_null (void)

{
  return (global_geom_ind->_atomic == OCI_IND_NULL);
}


/*******************************************************************************
**
** Routine:     sc_ops_init_geometry
**
** Description: Initializaton routine.
**              This must be called for a geometry before you call any
**              any other routines in sc_ops...
**
*******************************************************************************/
void
sc_ops_init_geometry (SDO_GEOMETRY_TYPE *geom, 
                      SDO_GEOMETRY_ind  *geom_ind,
                      int               num_dimensions)

{
  global_geom = geom;
  global_geom_ind = geom_ind;
  
  if (!sc_ops_geom_null())
  {
    /* Get the size of the sdo_elem_info array */
    checkerr(errhp, OCICollSize(envhp, errhp, 
	  	(		OCIColl *)(global_geom->sdo_elem_info), 
				&global_nelems));

    /* Get the size of the ordinates array */
    checkerr(errhp, OCICollSize(envhp, errhp, 
				(OCIColl *)(global_geom->sdo_ordinates), 
				&global_nords));

    global_elem_index = 0;
    global_first_elem = 1;
    global_ending_offset = -1;
    global_num_dimensions = num_dimensions;
  }
}


/*******************************************************************************
**
** Routine:     sc_ops_get_gtype
**
** Description: Prints the gtype field.
**
*******************************************************************************/
void
sc_ops_get_gtype (void)

{
  int gtype;

  checkerr(errhp, OCINumberToInt(errhp, &(global_geom->sdo_gtype),
				 (uword)sizeof(int), OCI_NUMBER_SIGNED,
				 (dvoid *)&gtype));

  printf("-- gtype: %d \n", gtype);
}


/*******************************************************************************
**
** Routine:     sc_ops_get_sdo_point
**
** Description: Prints the SDO_POINT field.
**              Routine checks for NULL sdo_point field and 
**              NULL x, y and z fields.
**
*******************************************************************************/
void
sc_ops_get_sdo_point (void)

{
  double x, y, z;

  if (global_geom_ind->sdo_point._atomic == OCI_IND_NOTNULL)
  {
    if (global_geom_ind->sdo_point.x == OCI_IND_NOTNULL)
    {
      checkerr(errhp, OCINumberToReal(errhp, &(global_geom->sdo_point.x), 
                                     (uword)sizeof(double),
			 	     (dvoid *)&x));

        printf("-- sdo_point.X: %.9lf \n", x);
    }

    if (global_geom_ind->sdo_point.y == OCI_IND_NOTNULL)
    {
      checkerr(errhp, OCINumberToReal(errhp, &(global_geom->sdo_point.y), 
                                     (uword)sizeof(double),
			 	     (dvoid *)&y));

      printf("-- sdo_point.Y: %.9lf \n", y);
    }

    if (global_geom_ind->sdo_point.z == OCI_IND_NOTNULL)
    {
      checkerr(errhp, OCINumberToReal(errhp, &(global_geom->sdo_point.z), 
                                     (uword)sizeof(double),
			 	     (dvoid *)&z));

      printf("-- sdo_point.Z: %.9lf \n", z);
    }
  }
  else
    printf ("-- sdo_point IS NULL\n");
}


/*******************************************************************************
**
** Routine:     sc_ops_next_elem
**
** Description: Go to the next element in this geometry. 
**              Returns 1 if there are more elements.
**              Returns 0 if there are no more elements.
**
*******************************************************************************/
int
sc_ops_next_elem (void)

{
  int more_elems = 1;

  if (global_first_elem)
    global_first_elem = 0;
  else
    global_elem_index = global_elem_index + 3;

  return global_elem_index < global_nelems;
}

/*******************************************************************************
**
** Routine:     sc_ops_get_element_type
**
** Description: Prints the element_type for current element.
**
*******************************************************************************/
ub4
sc_ops_get_element_type ()

{
  boolean        exists;
  OCINumber      *oci_number;
  ub4            element_type;

  checkerr(errhp, OCICollGetElem(envhp, errhp, 
                                 (OCIColl *)(global_geom->sdo_elem_info), 
			         (sb4)(global_elem_index + 1), 
                                 (boolean *)&exists, 
			         (dvoid **)&oci_number, (dvoid **)0));
  checkerr(errhp, OCINumberToInt(errhp, oci_number, 
				 (uword)sizeof(ub4), OCI_NUMBER_UNSIGNED,
				 (dvoid *)&element_type));

  printf("-- element type: %u \n", element_type); 
  fflush (stdout);

  return element_type;
}


/*******************************************************************************
**
** Routine:     get_interpretation
**
** Description: Prints the interpretation field for current element.
**
*******************************************************************************/
static ub4
get_interpretation (void)

{
  boolean        exists;
  OCINumber      *oci_number;
  ub4            interpretation;

  checkerr(errhp, OCICollGetElem(envhp, errhp, 
                                 (OCIColl *)(global_geom->sdo_elem_info), 
			         (sb4)(global_elem_index + 2), 
                                 (boolean *)&exists, 
			         (dvoid **)&oci_number, (dvoid **)0));

  checkerr(errhp, OCINumberToInt(errhp, oci_number, 
				 (uword)sizeof(ub4), OCI_NUMBER_UNSIGNED,
				 (dvoid *)&interpretation));

  printf("-- interpretation: %u \n", interpretation); 
  fflush(stdout);

  return interpretation;
}


/*******************************************************************************
**
** Routine:     get_ordinates
**
** Description: Prints ordinates out of the ordinates array.
**
*******************************************************************************/
static void 
get_ordinates (int starting_offset, int ending_offset)

{
  int col;
  boolean        exists;
  OCINumber      *oci_number;
  double         double_val;

  /* 
  ** Get ordinates for the current element.
  */
  for (col = starting_offset; col <= ending_offset; col++)
  {
    checkerr(errhp, OCICollGetElem(envhp, errhp, 
				   (OCIColl *)(global_geom->sdo_ordinates), 
				   (sb4)col, (boolean *)&exists, 
				   (dvoid **)&oci_number, (dvoid **)0));
    checkerr(errhp, OCINumberToReal(errhp, oci_number, (uword)sizeof(double),
					(dvoid *)&double_val));
	
    if ((col % global_num_dimensions) == 0)
      printf("%.9lf", double_val);
    else
      printf(", %.9lf", double_val);

    if ((col % global_num_dimensions) == (global_num_dimensions - 1))
      printf("\n");
  } /* end for */

  printf("\n");
}


/*******************************************************************************
**
** Routine:     get_next_ending_offset
**
** Description: Returns the ending offset of current element.
**
*******************************************************************************/
static int
get_next_ending_offset (int processing_type_4or5)

{
  boolean        exists;
  OCINumber      *oci_number;
  int            ending_offset;

  /* last element? */
  if (global_elem_index == global_nelems - 3)
    ending_offset = global_nords;
  else
  {
    checkerr(errhp, OCICollGetElem(envhp, errhp, 
			           (OCIColl *)(global_geom->sdo_elem_info), 
			           (sb4)(global_elem_index + 3), 
                                   (boolean *)&exists, 
			           (dvoid **)&oci_number, (dvoid **)0));
    checkerr(errhp, OCINumberToInt(errhp, 
			           oci_number, 
			           (uword)sizeof(int), OCI_NUMBER_SIGNED,
			           (dvoid *)&ending_offset));

    if (processing_type_4or5)
    {
      /* Add 1 to next elements starting offset */
      ending_offset++;
    }
    else
    {
      /* Subtract 1 from next elements starting offset */
      ending_offset--;
    }
  }

  /* shift offsets from 1..n to 0..n-1 */
  ending_offset--;

  printf("-- ending offset: %d \n", ending_offset+1);

  return ending_offset;
}

/*******************************************************************************
**
** Routine:     get_next_starting_offset
**
** Description: Returns the starting offset of the current element.
**
*******************************************************************************/
static int
get_next_starting_offset (int read_varray)

{
  boolean        exists;
  OCINumber      *oci_number;
  int            starting_offset;


  /* get the element info structure */
  if (read_varray)
  {
    /* This is the first element, get the starting offset */
    checkerr(errhp, OCICollGetElem(envhp, errhp, 
			           (OCIColl *)(global_geom->sdo_elem_info), 
			           (sb4)(global_elem_index), (boolean *)&exists,
			           (dvoid **)&oci_number, (dvoid **)0));

    /* get starting and ending offsets in ordinate array */
    checkerr(errhp, OCINumberToInt(errhp, oci_number, 
				   (uword)sizeof(ub4), OCI_NUMBER_SIGNED,
				   (dvoid *)&starting_offset));

    /* shift offsets from 1..n to 0..n-1 */
    starting_offset--;
  }
  else
    starting_offset = global_ending_offset + 1;
      
  printf("-- starting offset: %d \n", starting_offset+1);

  return starting_offset;
}


/*******************************************************************************
**
** Routine:     sc_ops_get_type1
**
** Description: Print the ordinates for a type1 element.
**
*******************************************************************************/
void
sc_ops_get_type1 (void)

{
   int i,
       interp,
       start,
       end;

   start = get_next_starting_offset (global_ending_offset == -1);
   end = get_next_ending_offset (0); 
   interp = get_interpretation ();

   for (i = 0; i < interp; i++)
   {
     get_ordinates (start, 
                    start + global_num_dimensions - 1);
     start = start + global_num_dimensions;
   }
}


/*******************************************************************************
**
** Routine:     sc_ops_get_type2
**
** Description: Print the ordinates for a type2 element.
**
*******************************************************************************/
void
sc_ops_get_type2 (void)

{
   int interp = get_interpretation ();

   get_ordinates (get_next_starting_offset(global_ending_offset == -1), 
                  get_next_ending_offset (0));
}

/*******************************************************************************
**
** Routine:     sc_ops_get_type3
**
** Description: Print the ordinates for a type3 element.
**
*******************************************************************************/
void
sc_ops_get_type3 (void)

{
   int interp = get_interpretation (),
       start  = get_next_starting_offset(global_ending_offset == -1),
       end    = get_next_ending_offset(0);

   if (interp == 3) /* Rectangle, 2 po;ints */
     get_ordinates (start,
                    start + global_num_dimensions * 2 - 1);

   else if (interp == 4) /* Circle, 3 points */
     get_ordinates (start,
                    start + global_num_dimensions * 3 - 1);

   else if (interp == 1 || interp == 2) /* interpretation 1 or 2 */
     get_ordinates (start, end);
}


/*******************************************************************************
**
** Routine:     sc_ops_get_type4
**
** Description: Print the ordinates for a type4 element.
**
*******************************************************************************/
void
sc_ops_get_type4 (void)

{
  int interp = get_interpretation (),
      start,
      end,
      i;

  for (i = 0; i < interp; i++)
  {
     global_elem_index = global_elem_index + 3;
     start  = get_next_starting_offset(-1);
 
     if (i == interp - 1)
       end = get_next_ending_offset(0);
     else
       end = get_next_ending_offset(1);

     get_ordinates (start, end);
  }
}


/*******************************************************************************
**
** Routine:     sc_ops_get_type5
**
** Description: Print the ordinates for a type5 element.
**
*******************************************************************************/
void
sc_ops_get_type5 (void)

{
  int interp = get_interpretation (),
      start,
      end,
      i;

  for (i = 0; i < interp; i++)
  {
     global_elem_index = global_elem_index + 3;
     start  = get_next_starting_offset(-1);
 
     if (i == interp - 1)
       end = get_next_ending_offset(0);
     else
       end = get_next_ending_offset(1);

     get_ordinates (start, end);
  }
}




/* end of file geomops.c */

