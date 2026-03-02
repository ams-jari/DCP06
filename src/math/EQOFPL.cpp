/*
Calculate equation of plane

Function:		equation_of_plane(struct ams_vector *a, struct ams_vector *b,struct ams_vector *c, struct plane *d)
Call:			equation_of_plane(&a,&b,&c,&d)
Input:			struct ams_vector a,b,c
Output:			struct plane d

*/
#include "stdafx.h"
#include "calc.h"
/*
 double vector_product(struct ams_vector *a, struct ams_vector *b, struct ams_vector *c);
*/
short equation_of_plane(struct ams_vector *a,
				  struct ams_vector *b,
				  struct ams_vector *c,
				  struct plane 	*d)

/*
struct ams_vector *a;
struct ams_vector *b;
struct ams_vector *c;
struct plane *d;
*/
 {
  struct ams_vector dif1;
  struct ams_vector dif2;
  struct ams_vector norm;

  /* point in plane*/

  d->px = a->x;
  d->py = a->y;
  d->pz = a->z;

  /* calculate norm of the plane */

  dif1.x = b->x - a->x;
  dif1.y = b->y - a->y;
  dif1.z = b->z - a->z;

  dif2.x = c->x - a->x;
  dif2.y = c->y - a->y;
  dif2.z = c->z - a->z;

  vector_product(&dif1,&dif2,&norm);

  d->nx = norm.x;
  d->ny = norm.y;
  d->nz = norm.z;

  return 1;
 }
