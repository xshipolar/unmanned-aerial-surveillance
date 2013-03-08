#include <UAS_nav.hpp>

using namespace std;
using namespace arma;

gimbal_angle gimbal_calculate(double u, double v, double uc, double vc, double pitch, double yaw)
{
	/* Rotation matrices */
	/* Rci = rotation matrix from {C} to Inertial fram {I} */
	/* Rec = rotation matrix  */
	pitch*=M_PI/18000;
	yaw*=M_PI/18000;
	double e_pitch, e_yaw, a, b;
	a=20*180/M_PI;
	b=20*180/M_PI;
	mat Rci(3,3);
	mat Rec(3,3);
	mat prod(3,3);
	Rci(0,0)=cos(pitch)*cos(yaw); Rci(0,1)=cos(pitch)*sin(yaw); Rci(0,2)=-sin(pitch);
	Rci(1,0)=-sin(yaw); 		  Rci(1,1)=cos(yaw); 			Rci(1,2)=0;
	Rci(2,0)=sin(yaw); 			  Rci(2,1)=sin(yaw)*sin(pitch);	Rci(2,2)=cos(pitch);

	e_pitch = a*(v - vc)/480;
	e_yaw = b*(u - uc)/640;

	Rec(0,0)=cos(e_pitch)*cos(e_yaw); Rec(0,1)=cos(e_pitch)*sin(e_yaw); Rec(0,2)=-sin(e_pitch);
	Rec(1,0)=-sin(e_yaw); 		  	  Rec(1,1)=cos(e_yaw); 				Rec(1,2)=0;
	Rec(2,0)=sin(e_yaw); 			  Rec(2,1)=sin(e_yaw)*sin(e_pitch);	Rec(2,2)=cos(e_pitch);

	prod = Rci * Rec;

	gimbal_angle f;
	f.tilt = (atan2(prod(2,1), prod(2,2))*18000)/M_PI;
	f.pan = (atan2(prod(1,0), prod(0,0))*18000)/M_PI;
	return f;
}
