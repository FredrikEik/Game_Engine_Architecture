#ifndef INTERPRATE_H
#define INTERPRATE_H
inline double CubicInterp (double n0, double n1, double n2, double n3,
   double a)
 {
     double p = (n3 - n2) - (n0 - n1);
     double q = (n0 - n1) - p;
     double r = n2 - n0;
     double s = n1;
     return p * a * a * a + q * a * a + r * a + s;
 }

inline double LinearInterp (double n0, double n1, double a)
  {
    return ((1.0 - a) * n0) + (a * n1);
  }

inline double SCurve3 (double a)
  {
    return (a * a * (3.0 - 2.0 * a));
  }

inline double SCurve5 (double a)
  {
    double a3 = a * a * a;
    double a4 = a3 * a;
    double a5 = a4 * a;
    return (6.0 * a5) - (15.0 * a4) + (10.0 * a3);
  }
#endif // INTERPRATE_H
