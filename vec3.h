#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

#include <math.h>
#include <stdlib.h>
#include <iostream>

class vec3 {
public:
   vec3() {}
   vec3(float e0, float e1, float e2) : e{e0, e1, e2} {}
   float x() const {return e[0];}
   float y() const {return e[1];}
   float z() const {return e[2];}
   float r() const {return e[0];}
   float g() const {return e[1];}
   float b() const {return e[2];}

   const vec3& operator+() const {return *this;}
   vec3 operator-() const {return vec3(-e[0], -e[1], -e[2]);}
   float operator[](int i) const {return e[i];}
   float& operator[](int i) {return e[i];}

   vec3& operator +=(const vec3& v2);
   vec3& operator -=(const vec3& v2);
   vec3& operator *=(const vec3& v2);
   vec3& operator /=(const vec3& v2);
   vec3& operator *=(const float t);
   vec3& operator /=(const float t);

   float length() const{
      return sqrt(e[0]*e[0] + e[1]*e[1] +e[2]*e[2]);
   }
   float squared_length() const{
      return e[0]*e[0] + e[1]*e[1] +e[2]*e[2];
   }
   void make_unit_vector();
   float e[3];
};

std::istream& operator>>(std::istream& is, vec3 &t);
std::ostream& operator>>(std::ostream& os, const vec3& t);
vec3 operator+(const vec3& v1, const vec3& v2);
vec3 operator-(const vec3& v1, const vec3& v2);
vec3 operator*(const vec3& v1, const vec3& v2);
vec3 operator/(const vec3& v1, const vec3& v2);
vec3 operator*(float t, const vec3& v);
vec3 operator*(const vec3& v, float t);
vec3 operator/(const vec3& v, float t);
float dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);
vec3 unit_vector(vec3 v);

#endif // VEC3_H_INCLUDED
