#include "spline.h"
#include "widget.h"

CSpline::CSpline(double x[100], double y[100], int grain, double tension, int n)
{
//x[100], y[100]用来存放特征控制点坐标,n 为控制点数量
//grain 控制每两个相邻控制点之间样条插值后的添加点数,一般 grain<20 可以满足大部分应用要求
// tension ∈[0.0,1.]控制样条插值曲线通过控制点处的曲率,实验中应改考察 tension 数值变化对拟合曲线形状带来什么影响;
    int i,np; n0=n; np=n0; CPt jd[100];
    CPt *knots;
    knots0=knots; //将控制点坐标赋值到数组 jd 中
    for(i=1;i<=np;i++)
    {
        jd[i].x = x[i-1];
        jd[i].y = y[i-1];
    }
    jd[0].x=x[0];
    jd[0].y=y[0];
    jd[np+1].x = x[np-1]; //思考:为什么这里这样赋值?
    jd[np+1].y = y[np-1];
    np=np+2;
    knots=jd;
    CubicSpline(np, knots, grain, tension);
}

//析构函数
CSpline::~CSpline()
{
} /////////////////////////////////////////////////////////////////////////////
// CSpline message handlers


void CSpline::CubicSpline(int n, CPt *knots, int grain, double tension)
{
    CPt *s, *k0, *kml, *k1, *k2;
    int i,j;
    double alpha[50];

    GetCardinalMatrix(tension);


    for(i=0; i<grain; i++)
        alpha[i] =((double)i)/grain;


    s = Spline;
    kml = knots;
    k0=kml+1; k1=k0+1; k2=k1+1;



    for(i=1; i<n-1; i++)
        {
            for(j=0; j<grain;j++) {
                s->x = Matrix(kml->x,k0->x,k1->x,k2->x,alpha[j]);
                s->y = Matrix(kml->y,k0->y,k1->y,k2->y,alpha[j]);
                s++;
            }
            k0++; kml++; k1++; k2++;
        }
}


void CSpline::GetCardinalMatrix(double a1)
{
    m[0]=-a1; m[1]=2.-a1;m[2]=a1-2.; m[3]=a1;
    m[4]=2.*a1; m[5]=a1-3.; m[6]=3.-2*a1;m[7]=-a1;
    m[8]=-a1; m[9]=0.; m[10]=a1;m[11]=0.;
    m[12]=0.; m[13]=1.;m[14]=0.; m[15]=0.;
}

double CSpline::Matrix(double a, double b, double c, double d, double alpha)
{
    double p0,p1,p2,p3;
    p0=m[0]*a+m[1]*b+m[2]*c+m[3]*d;
    p1=m[4]*a+m[5]*b+m[6]*c+m[7]*d;
    p2=m[8]*a+m[9]*b+m[10]*c+m[11]*d;
    p3=m[12]*a+m[13]*b+m[14]*c+m[15]*d;


    return(p3+alpha*(p2+alpha*(p1+alpha*p0)));
}

