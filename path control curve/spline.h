#ifndef SPLINE
#define SPLINE
// Spline.h : header file
struct CPT { double x; double y;};
#define CPt CPT

class CSpline //: public CEdit
{
// Construction
public:
    CSpline(double x[100], double y[100],int grain, double tension, int n);
// Attributes
public:
// Operations
public:

// Overrides
// ClassWizard generated virtual function overrides
//{{AFX_VIRTUAL(CSpline)
//}}AFX_VIRTUAL


// Implementation
public:
    int last;
    CPt Spline[1024];

void CubicSpline(int n, CPt *knots, int grain, double tension);

virtual ~CSpline();
// Generated message map functions protected:
//{{AFX_MSG(CSpline)
// NOTE - the ClassWizard will add and remove member functions here.
//}}AFX_MSG
//DECLARE_MESSAGE_MAP()

private:
    CPt *knots0;
    int n0;
    double m[16];
    double Matrix(double a, double b, double c, double d, double alpha);
    void GetCardinalMatrix(double a1);
};
//end of head file

#endif // SPLINE

