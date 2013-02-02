#ifndef _NEUBERT_GEN_INCLUDE_
#define _NEUBERT_GEN_INCLUDE_

#include <TooN/TooN.h>
#include <cvd/image_ref.h>
#include <TooN/helpers.h>
#include <TooN/so3.h>

template<class T>
inline double sq(const T &x) { return (double)(x*x);}

template<>
inline double sq(const CVD::ImageRef &x) { return (double)(x[0]*x[0] + x[1]*x[1]);}

inline double HUBER_W(const double &x, const double &K){return (x>K?(K/x):1.0);}

template<class T>
inline T ABS(T x){return (x>0?x:-x);}

template<class T>
inline T MIN(T X,T Y){X<Y?X:Y;}
/*
inline TooN::Vector<2> make_vector(const CVD::ImageRef &p)
{  
   double els[]={(double)p.x, (double)p.y}; 
   return TooN::Vector<2>(els);
}
*/
/*
//TooN::Vector<6> inline makeVector(double x, double y, double z, double wx, double wy, double wz) { double els[]={x,y,z,wx,wy,wz};  return TooN::Vector<6>(els); }
TooN::Vector<12> inline makeVector(double x, double y, double z, double wx, double wy, double wz,
			     double vx, double vy, double vz, double vwx, double vwy, double vwz) { 
  double els[]={x,y,z,wx,wy,wz,vx,vy,vz,vwx,vwy,vwz};    return TooN::Vector<12>(els); 
}
*/
inline TooN::Matrix<3> get_skew_symetric(const TooN::Vector<3> &t )
{
   TooN::Matrix<3> M(TooN::Zeros);
   //TooN::Zero(M);
   M[1][0] = t[2];M[0][1] =-t[2];
   M[2][0] =-t[1];M[0][2] = t[1];
   M[2][1] = t[0];M[1][2] =-t[0];
   return M;
}

inline TooN::SO3<> getSO3( TooN::Vector<3> axis, double angle )
{
   double ST = sin(angle), CT = cos(angle), vT = 1.0 - CT, R[9];

   R[0] = axis[0]*axis[0]*vT + CT;             
   R[1] = axis[0]*axis[1]*vT - axis[2]*ST;
   R[2] = axis[0]*axis[2]*vT + axis[1]*ST;
   R[3] = axis[0]*axis[1]*vT + axis[2]*ST;     
   R[4] = axis[1]*axis[1]*vT + CT;           
   R[5] = axis[1]*axis[2]*vT - axis[0]*ST;
   R[6] = axis[0]*axis[2]*vT - axis[1]*ST;     
   R[7] = axis[1]*axis[2]*vT + axis[0]*ST;   
   R[8] = axis[2]*axis[2]*vT + CT;

   TooN::Matrix<3> my_R(R);
   
   return TooN::SO3<>(my_R);
}   
 




/*
template<int L>
TooN::Matrix<L> get_skew_symetric(const TooN::Vector<L> &t )
{

   TooN::Matrix<L> M;
   for
   

}
*/

#endif
