#include "spline.h"
#include "widget.h"
#include "math.h"
#include <QLabel>
#include <QWidget>
#include <QDebug>

#define SEARCH_TOL 0.01
#define ITERATION_COUNT 50

float spline[100][2];
CPT *cp;
double m[16];
double xcoeff[4],ycoeff[4];

CPT draw[1024];
CPT pointdraw[100];

float s;
float t;
float a;
float v;




float total_length; //弧长总长度
float seg_lengths[100];//累加弧长表


//----from ex1 to get spline curve------
void GetCardinalMatrix(double a1)
{
       m[0]=-a1;   m[1]=2.-a1;  m[2]=a1-2.;   m[3]=a1;
       m[4]=2.*a1; m[5]=a1-3.;  m[6]=3.-2*a1; m[7]=-a1;
       m[8]=-a1;   m[9]=0.;     m[10]=a1;     m[11]=0.;
       m[12]=0.;   m[13]=1.;    m[14]=0.;     m[15]=0.;
}

double Matrix(double a, double b, double c, double d, double alpha)
{
    double p0,p1,p2,p3;
    p0=m[0]*a+m[1]*b+m[2]*c+m[3]*d;
    p1=m[4]*a+m[5]*b+m[6]*c+m[7]*d;
    p2=m[8]*a+m[9]*b+m[10]*c+m[11]*d;
    p3=m[12]*a+m[13]*b+m[14]*c+m[15]*d;


    return(p3+alpha*(p2+alpha*(p1+alpha*p0)));
}
//combine cspline & cubicspline
void Spline(double *x,double *y,int npoints,int grain)
{
    for(int i=1; i <= npoints; i++)
    {
        spline[i][0] = x[i-1];
        spline[i][1] = y[i-1];
    }
    spline[0][0] = x[0];
    spline[0][1] = y[0];
    spline[npoints+1][0] = x[npoints-1];
    spline[npoints+1][1] = y[npoints-1];

    npoints = npoints + 2;

    double alpha[grain];
    for(int i=0; i<grain; i++)
    {
        alpha[i] =((double)i)/grain;
    }

    for(int i=1; i< npoints-1; i++) {
        for(int j=0; j<grain;j++) {
            draw[grain*i+j-grain].x = Matrix(spline[i-1][0],spline[i][0],spline[i+1][0],spline[i+2][0],alpha[j]);
            draw[grain*i+j-grain].y = Matrix(spline[i-1][1],spline[i][1],spline[i+1][1],spline[i+2][1],alpha[j]);
        }
    }

     draw[grain*npoints].x = Matrix(spline[npoints-2][0],spline[npoints-1][0],spline[npoints][0],spline[npoints+1][0],alpha[grain]);
     draw[grain*npoints].y = Matrix(spline[npoints-2][0],spline[npoints-1][0],spline[npoints][0],spline[npoints+1][0],alpha[grain]);

}

void ControlPoint(int npoints,int grain)
{
    cp = pointdraw;

    SetLengths(npoints);

    s = 0.0;t = 0.0;
    a = 2;//jiasudu
    v = 0;
    do
    {
        ArcLengthPoint(s);
        cp++;
        t += 1.0/grain;
        s = 0.5*a*t*t + v*t;//use the formula
    }while( s <= 1.0);

    pointdraw[grain].x = spline[npoints-1][0];
    pointdraw[grain].y = spline[npoints-1][1];
}

//------from reference book---------
void ArcLengthPoint(float s)//返回样条上弧长为s属于[0,1]的点
{
  float uL = 0.0, uR = 1.0, usearch;
  float segment_dist, ssearch;
  int segment_no = 0;

  /* Find Curve segment within which s lies */
  do {
      segment_no++;
  }
  while(seg_lengths[segment_no] < s);

  segment_no--;

  segment_dist = total_length*(s-seg_lengths[segment_no]);
  /*Set up nessary coefficients relevant to that segment*/

  SegCoeff(spline,segment_no);

  //search segment for local parameter tsearch that globally gives us
  //an arc length within SEARCH_TOL of s
  do{
      usearch = (uL + uR)/2;
      ssearch = ArcLength(uL,usearch);
      if(segment_dist < ssearch) uR = usearch;
      else{
          uL = usearch;
          segment_dist -= ssearch;
      }
  }while((uR - uL) > SEARCH_TOL);

  GetPointOnSpline(usearch,cp);
}


float ArcLength(float ustart,float uend)//返回当前曲线段在区间内的弧长
{
  float h, sum, u;
  int i;

  //compute arc length using the extended Simpson's rule
  h = (uend-ustart)/(float)ITERATION_COUNT;
  sum = 0.0;
  u = ustart + h;

  for(i = 2; i <= ITERATION_COUNT; i++){
      if(i%2 == 0) sum += 4.0 * ArcIntegrand(u);
      else sum += 2.0 * ArcIntegrand(u);
      u += h;
  }

  return( h*(ArcIntegrand(ustart) + sum + ArcIntegrand(uend))/3.0);
}


void SetLengths(int npoints) //建立累加弧长表
{
    int i, no_segments;
    float arclength;

    no_segments = npoints -3;
    arclength = 0.0;
    seg_lengths[0] = 0.0;
    for( i = 0; i < no_segments; i++){
        SegCoeff(spline,i);
        arclength += ArcLength(0.0,1.0);
        seg_lengths[i+1] = arclength;
    }
    total_length = arclength;
    for(i = 1; i <= no_segments; i++) seg_lengths[i]/= total_length;
}

void SegCoeff(float spline[100][2],int i)//建立当前曲线段的相关系数
{
    xcoeff[0] = m[0]*spline[i][0]+m[1]*spline[i+1][0]+m[2]*spline[i+2][0]+m[3]*spline[i+3][0];
    xcoeff[1] = m[4]*spline[i][0]+m[5]*spline[i+1][0]+m[6]*spline[i+2][0]+m[7]*spline[i+3][0];
    xcoeff[2] = m[8]*spline[i][0]+m[9]*spline[i+1][0]+m[10]*spline[i+2][0]+m[11]*spline[i+3][0];
    xcoeff[3] = m[12]*spline[i][0]+m[13]*spline[i+1][0]+m[14]*spline[i+2][0]+m[15]*spline[i+3][0];

    ycoeff[0] = m[0]*spline[i][1]+m[1]*spline[i+1][1]+m[2]*spline[i+2][1]+m[3]*spline[i+3][1];
    ycoeff[1] = m[4]*spline[i][1]+m[5]*spline[i+1][1]+m[6]*spline[i+2][1]+m[7]*spline[i+3][1];
    ycoeff[2] = m[8]*spline[i][1]+m[9]*spline[i+1][1]+m[10]*spline[i+2][1]+m[11]*spline[i+3][1];
    ycoeff[3] = m[12]*spline[i][1]+m[13]*spline[i+1][1]+m[14]*spline[i+2][1]+m[15]*spline[i+3][1];

}

float ArcIntegrand(float u)
{
    float f;
    f = sqrt(9*(xcoeff[0]*xcoeff[0]+ycoeff[0]*ycoeff[0])*u*u*u*u+
            12*(xcoeff[0]*xcoeff[1]+ycoeff[0]*ycoeff[1])*u*u*u+
            (6*(xcoeff[0]*xcoeff[2]+ycoeff[0]*ycoeff[2])+4*(xcoeff[1]*xcoeff[1]+ycoeff[1]*ycoeff[1]))*u*u+
             4*(xcoeff[1]*xcoeff[2]+ycoeff[1]*ycoeff[2])*u+xcoeff[2]*xcoeff[2]+ycoeff[2]*ycoeff[2]);
    return f;
}
//----functions from reference book end here----

void GetPointOnSpline(float u,CPT *cp)
{
    cp->x = xcoeff[3]+u*(xcoeff[2]+u*(xcoeff[1]+u*xcoeff[0]));
    cp->y = ycoeff[3]+u*(ycoeff[2]+u*(ycoeff[1]+u*ycoeff[0]));
}

