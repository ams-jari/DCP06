
/*******************************************************************************

*******************************************************************************/
#ifndef DCP_CALC_HPP
#define DCP_CALC_HPP

struct ams_vector		/* vektori tai piste (x,y,z) */ 
	{
	 double x;
	 double y;
	 double z;
	};

struct sphvect		/* vektori tai piste (r,theta,fii) */
        {		/* pallokoordinaatistossa    	   */
	 double r;
	 double theta;
	 double fii;
	};

struct line 		/* suora eli piste (px,py,pz) ja */
	{		/* yksikk|vektori (ux,uy,uz)     */
	 double px;
	 double py;
	 double pz;
	 double ux;
	 double uy;
	 double uz;
	};

struct plane 		/* taso eli piste (px,py,pz) ja */
	{		/* yksikk|normaalivektori (nx,ny,nz) */
	 double px;
	 double py;
	 double pz;
	 double nx;
	 double ny;
	 double nz;
	};


// *******************************
// Function prototypes
// *******************************

double angle(struct ams_vector *a,struct ams_vector *b);
short cartsph(double p0[4], double *disty, double *thetax,double *thetaz);
short cartsph_left(double p0[4], double *disty, double *thetax,double *thetaz);
double degtorad(double a);
short dir_vector(double pa[4], double pb[4], double (*dv)[4]);
double dot_product(struct ams_vector *a,struct ams_vector *b);
void equation_of_line(struct ams_vector *a,struct ams_vector *b,struct line *c);
short equation_of_plane(struct ams_vector *a, struct ams_vector *b, struct ams_vector *c, struct plane *d);
void rotate_about_x_axis(double a[4][4], double b, double (*w)[4][4]);
void rotate_about_y_axis(double a[4][4], double b, double (*w)[4][4]);
void rotate_about_z_axis(double a[4][4], double b, double (*w)[4][4]);
short htrans6(double p0[4],
		double p1[4],
		double p2[4],
		double p3[4],
		double p4[4],
		double p5[4],
		double (*hm)[4][4]);
short htransm(double p0[4],
		double p1[4],
		double p2[4],
		short iyz,
		double (*hm)[4][4]);

void matcopy_n (double *a,short rows, short cols, double *b);
short matinv_n (double *a,short rows, short cols, double *b);
void matmul_n(double *a, short a_rows, short a_cols,
				  double *b, short b_rows, short b_cols,
				  double *c);
void matsum_n (double *a, short a_rows, short a_cols, double  *b, double *c);
void mattra_n (double *a,short a_rows,short a_cols, double *b);
short matvec(double  a[4][4],double v[4],double (*w)[4]);
short matvecmul_n(	double *a, short a_rows, short a_cols,
				double *b,
				double *c);
short matinv4x4(double a[4][4], double (*b)[4][4]);
short matmul4x4(double a[4][4],double b[4][4], double (*c)[4][4]);
short mattra(double a[4][4], double (*b)[4][4]);
double calc_point_dist_from_line(struct ams_vector *a,struct line *b);
double calc_point_dist_from_line2(struct ams_vector *a,struct line *b, struct ams_vector *dest_point);
double calc_point_dist_from_plane(struct ams_vector *a,struct plane *b);
double calc_point_dist_from_point(struct ams_vector *a,struct ams_vector *b);
short pminuso(double v[4], double u[4], double (*w)[4]);
short ppluso(double  v[4], double u[4], double (*w)[4]);
void ptrans(double v[4],double a[4][4], double (*w)[4]);
double radtodeg(double a);
short ranglz(double p1[4], double p2[4], double *s);
void reset_matrix (double *a, short a_rows, short a_cols);
void sphcart(struct sphvect *a, struct ams_vector *b);
short unit_vector(double pa[4], double (*uv)[4]);
double vector_length(struct ams_vector *a);
int vecmat(double v[4], double a[4][4],double (*w)[4]);
void vec_product(double u[4], double v[4], double (*w)[4]);
double vector_product(struct ams_vector *a,struct ams_vector *b,struct ams_vector *c);

short center_of_circle(double x1,double y1,double x2,double y2,double x3,double y3,double *radius,double *cx,double *cy);

#endif
