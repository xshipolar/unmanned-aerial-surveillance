#ifndef _NEUBERT_PLANE_H_
#define _NEUBERT_PLANE_H_

#include <vector>
#include <my_utils.h>
#include <TooN/SVD.h>

inline TooN::Vector<4> uvq2xyz1(const Vector<3> &v)
{
   return makeVector(v[0], v[1], 1.0, v[2]);
}

inline TooN::Vector<3> xyz12uvq(const Vector<4> &v)
{
   return makeVector(v[0]/v[2], v[1]/v[2], v[3]/v[2]);
}

inline TooN::Vector<4> xyz12uvq1(const Vector<4> &v)
{
   return makeVector(v[0], v[1], v[3], v[2]);
}
struct PlaneSolver 
{
   const static int min_num_points = 3;
   inline TooN::Vector<3> get_hypothesis(const std::vector< TooN::Vector<3> > &verts, bool &valid) const
   {
      Vector<3> norm_vec = (verts[0]-verts[2])^(verts[1]-verts[2]);
      double d = -verts[2]*(verts[0]^verts[1]);
      if( sq(norm_vec) < 1e-10 || sq(d) < 1e-10 ){valid = false; return norm_vec;}
      valid = true;
      norm_vec /= d; 
      return norm_vec;
   }
};

struct PlaneOutlierDetector
{
   double tol;
   Vector<3> plane_norm;
   PlaneOutlierDetector():tol(1e-3){}
   PlaneOutlierDetector(double t):tol(t){}

   inline bool operator()(const Vector<3> &loc) const
   {
      return is_outlier(loc, plane_norm);
   }
   inline void set_solution(const TooN::Vector<3> &n){ plane_norm = n; }
   inline bool is_outlier(const TooN::Vector<3> &p1, const TooN::Vector<3> &p2) const
   {
      return (sq(p1*p2+1.0)>tol);
   }

   template<class ITERATOR>
   inline int num_inliers(ITERATOR p_begin, ITERATOR p_end, const TooN::Vector<3> &p2) const
   {
      int num_in = 0;
      for(;p_begin != p_end; ++p_begin)
            if( sq((*p_begin)*p2+1)<tol ) ++num_in;
      return num_in;
   }

};


inline double get_depth_from_plane(const TooN::Vector<2> &p, const TooN::Vector<4> norm)
{
   double tmp = p*norm.slice<0,2>();
   return -norm[3]/( tmp + norm[2] );
}

inline double dist_of_cam_from_plane(TooN::Vector<4> norm)
{
   TooN::Vector<3> p;
   p = 0, 0, 1.0;
   
   if( sq(p*(norm.slice<0,3>())+norm[3]) < 1.0e-7 ) return -1;
   double t = norm[3]/ ( p*(norm.slice<0,3>()));
   
   std::cout << "p = "    << p.as_row()    << std::endl;
   std::cout << "norm = " << norm.as_row() << std::endl;
   std::cout << "t = "    << t             << std::endl;
   p *= t;
   std::cout << "plane p = " << p.as_row() << std::endl;
   TooN::Vector<3> norm3 = project(norm);
   normalize(norm3);
   double dist = p*norm3;
   std::cout << "Dist = " << -dist << std::endl;
   
   return -dist;
}



inline double get_depth_from_plane(const TooN::Vector<2> &p, const TooN::Vector<3> &norm)
{
   double tmp = p*norm.slice<0,2>();
   return -1/( tmp + norm[2] );
}
inline TooN::Vector<3> estimate_plane_norm(const std::vector<Vector<3> > &verts)
{
   int idx = 0;
   Matrix<> A(verts.size(), 4);
   std::vector<Vector<3> >::const_iterator it;

   for( it = verts.begin(); it != verts.end(); ++it, ++idx)
   {
	  A.slice( idx, 0, 1, 3) = (*it).as_row();
	  A[idx][3] = 1;
   }

   TooN::SVD<> Solution(A);

   double min = 10;
   idx = -1;
   for( int i = 0; i < 4; ++i)
      if( Solution.get_diagonal()[i] < min )
	  {
	     idx = i;
		 min = Solution.get_diagonal()[i];
      }

   if(idx < 0 ){std::cerr<<"No NULL space found!!"<<std::endl; throw("Help no subspace found");}
  
   TooN::Vector<3> ans;
   ans.as_row() = Solution.get_VT().slice(idx,0,1,3) / Solution.get_VT()[idx][3];
   return ans;
}
inline TooN::Matrix<3> getHomography(const TooN::SE3 &T_diff, const TooN::Vector<3> &norm)
{
   return  ((T_diff.get_rotation()).get_matrix()
                 - T_diff.get_translation().as_col() * norm.as_row() );
   //              \-----------------   t x n  -----------------------/ 
}

inline TooN::Matrix<3> getHomography(const TooN::SE3 &T_diff, const TooN::Vector<4> &norm)
{
   assert(norm[3]*norm[3] > 1.0e-10); 
   return  ((T_diff.get_rotation()).get_matrix()
                 - T_diff.get_translation().as_col() * TooN::project(norm).as_row() );
   //              \-----------------   t x n  --------------------------------------/ 
}

#endif
