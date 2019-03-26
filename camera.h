#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED

#include "ray.h"
#define M_PI 3.14159265359

vec3 random_in_unit_disk(){
   vec3 p;
   do{
      p = 2.0*vec3(rn(), rn(), 0.0) - vec3(1.0, 1.0, 0.0);
   } while(p.squared_length() >= 1.0);
   return p;
}

class camera {
public:
   camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focus_dist){ // vfov is top to bottom in degrees
      lens_radius = aperture/2.0;
      float theta = vfov*M_PI/180.0;
      float half_height = tan(theta/2);
      float half_width = aspect*half_height;
      origin = lookfrom;
      w = unit_vector(lookfrom - lookat);
      u = unit_vector(cross(vup, w));
      v = cross(w, u);
      lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
      horizontal = 2.0*half_width*focus_dist*u;
      vertical = 2.0*half_height*focus_dist*v;
   }
   ray get_ray(float s, float t) {
      vec3 rd = lens_radius*random_in_unit_disk();
      vec3 offset = u*rd.x() + v*rd.y();
      return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
   }
   vec3 origin, lower_left_corner, horizontal, vertical, u, v, w;
   float lens_radius;
};

#endif // CAMERA_H_INCLUDED
