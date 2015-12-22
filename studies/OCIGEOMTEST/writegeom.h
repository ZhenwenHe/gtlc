#ifndef read81_ORACLE
# define read81_ORACLE

#ifndef OCI_ORACLE
#include <oci.h>
#include <oci1.h>
#endif

#include "readgeom.h"
/*---------------------------------------------------------------------------
                     PUBLIC TYPES AND CONSTANTS
  ---------------------------------------------------------------------------*/

#define TYPE_OWNER                 "MDSYS"
#define SDO_ORDINATE_ARRAY         TYPE_OWNER".SDO_ORDINATE_ARRAY"
#define SDO_ELEM_INFO_ARRAY        TYPE_OWNER".SDO_ELEM_INFO_ARRAY"
#define SDO_GEOMETRY               TYPE_OWNER".SDO_GEOMETRY"

/*************************************************************************/
/* Type definitions                                                      */
/*************************************************************************/


/*************************************************************************/
/* Public Functions                                                      */
/*************************************************************************/

#endif  /* read81_ORACLE */
