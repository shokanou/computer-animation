#ifndef SPLINE_H
#define SPLINE_H

// Spline.h : header file
#define CPt CPT
struct CPT { double x; double y;};
extern CPT pointdraw[100];
extern CPT draw[1024];


void Spline(double x[],double y[],int,int);


void ArcLengthPoint(float);

void SetLengths(int);
float ArcLength(float,float);
float ArcIntegrand(float);

void SegCoeff(float[][2],int);
void GetCardinalMatrix(double a1);
double Matrix(double a, double b, double c, double d, double alpha);

void GetPointOnSpline(float, CPT *);
void ControlPoint(int,int);

#endif // SPLINE_H
