#ifndef RAY_H_INCLUDED
#define RAY_H_INCLUDED
#include "vec3.h"

std::function<float()> rn = std::bind(std::uniform_real_distribution<float>(), std::default_random_engine());

class ray {
public:
   ray() {}
   ray(const vec3& a, const vec3& b) : A(a), B(b) {}
   vec3 origin() const {return A;}
   vec3 direction() const {return B;}
   vec3 point_at_parameter(float t) const {return A + t*B;}

   vec3 A, B;
};

#endif // RAY_H_INCLUDED
