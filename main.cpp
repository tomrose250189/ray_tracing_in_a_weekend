#include <fstream>
#include <iostream>
#include <functional>
#include <random>
#include "sphere.h"
#include "hitable_list.h"
#include "camera.h"
#include "material.h"

#define MAXFLOAT 1.99999988079071044921875e127

//Chapter 11
hitable *random_scene() {
   int n = 500;
   hitable **list = new hitable*[n+1];
   list[0] = new sphere(vec3(0.0, -1000.0, 0.0), 1000.0, new lambertian(vec3(0.5, 0.5, 0.5)));
   int i = 1;
   for(int a = -11; a < 11; a++){
      for(int b = -11; b < 11; b++){
         float choose_mat = rn();
         vec3 center(a + 0.9*rn(), 0.2, b+0.9*rn());
         if((center-vec3(4.0, 0.2, 0.0)).length() > 0.9){
            if(choose_mat < 0.8) { // diffuse
               list[i++] = new sphere(center, 0.2, new lambertian(vec3(rn()*rn(), rn()*rn(), rn()*rn())));
            }
            else if(choose_mat < 0.95) { // metal
               list[i++] = new sphere(center, 0.2, new metal(vec3(0.5*(1.0 + rn()), 0.5*(1.0 + rn()), 0.5*(1.0 + rn())), 0.5*rn()));
            }
            else{
               list[i++] = new sphere(center, 0.2, new dielectric(1.5));
            }
         }
      }
   }
   list[i++] = new sphere(vec3(0.0, 1.0, 0.0), 1.0, new dielectric(1.5));
   list[i++] = new sphere(vec3(-4.0, 1.0, 0.0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
   list[i++] = new sphere(vec3(4.0, 1.0, 0.0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));
   return new hitable_list(list, i);
}

vec3 color(const ray& r, hitable *world, int depth) {
   hit_record rec;
   if(world->hit(r, 0.001, MAXFLOAT, rec)){
      ray scattered;
      vec3 attenuation;
      if((depth < 50) && (rec.mat_ptr->scatter(r, rec, attenuation, scattered))) {
         return attenuation*color(scattered, world, depth+1);
      }
      else{
         return vec3(0, 0, 0);
      }
   }
   else{
      vec3 unit_direction = unit_vector(r.direction());
      float t = 0.5*(unit_direction.y() + 1.0);
      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
   }
}

int main(){
   std::ofstream fo("img012.ppm");
   int nx = 2000;
   int ny = 1000;
   int ns = 100;
   fo << "P3\n" << nx << " " << ny << "\n255\n";
   hitable *world = random_scene();
   vec3 lookfrom(10.0, 10.0, 10.0);
   vec3 lookat(0.0, 0.0, 0.0);
   float dist_to_focus = (lookfrom - lookat).length();
   float aperture = 0.2;
   camera cam(lookfrom, lookat, vec3(0.0, 1.0, 0.0), 20, float(nx)/float(ny), aperture, dist_to_focus);
   for(int j = ny-1; j >= 0; j--){
      for(int i = 0; i < nx; ++i){
         vec3 col(0, 0, 0);
         for(int s = 0; s < ns; ++s){
            float u = float(i + rn())/float(nx);
            float v = float(j + rn())/float(ny);
            ray r = cam.get_ray(u, v);
            col += color(r, world, 0);
         }
         col /= float(ns);
         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
         int ir = int(255.99*col[0]);
         int ig = int(255.99*col[1]);
         int ib = int(255.99*col[2]);

         fo << ir << " " << ig << " " << ib << "\n";
      }
      std::cout << (float(ny-j)*100.0)/float(ny) << "%\n";
   }
//   delete dynamic_cast<sphere*>(list[0])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[1])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[2])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[3])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[4])->mat_ptr;
//   delete list[0];
//   delete list[1];
//   delete list[2];
//   delete list[3];
//   delete list[4];
//   delete world;
   fo.close();
}

//Chapter 10
//vec3 color(const ray& r, hitable *world, int depth) {
//   hit_record rec;
//   if(world->hit(r, 0.001, MAXFLOAT, rec)){
//      ray scattered;
//      vec3 attenuation;
//      if((depth < 50) && (rec.mat_ptr->scatter(r, rec, attenuation, scattered))) {
//         return attenuation*color(scattered, world, depth+1);
//      }
//      else{
//         return vec3(0, 0, 0);
//      }
//   }
//   else{
//      vec3 unit_direction = unit_vector(r.direction());
//      float t = 0.5*(unit_direction.y() + 1.0);
//      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//   }
//}
//
//int main(){
//   std::ofstream fo("img010.ppm");
//   int nx = 1000;
//   int ny = 500;
//   int ns = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   hitable *list[5];
//   list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
//   list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
//   list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
//   list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
//   list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
//   hitable *world = new hitable_list(list, 5);
//   vec3 lookfrom(3.0, 3.0, 2.0);
//   vec3 lookat(0.0, 0.0, -1.0);
//   float dist_to_focus = (lookfrom - lookat).length();
//   float aperture = 2.0;
//   camera cam(lookfrom, lookat, vec3(0.0, 1.0, 0.0), 20, float(nx)/float(ny), aperture, dist_to_focus);
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         vec3 col(0, 0, 0);
//         for(int s = 0; s < ns; ++s){
//            float u = float(i + rn())/float(nx);
//            float v = float(j + rn())/float(ny);
//            ray r = cam.get_ray(u, v);
//            col += color(r, world, 0);
//         }
//         col /= float(ns);
//         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   delete dynamic_cast<sphere*>(list[0])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[1])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[2])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[3])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[4])->mat_ptr;
//   delete list[0];
//   delete list[1];
//   delete list[2];
//   delete list[3];
//   delete list[4];
//   delete world;
//   fo.close();
//}

////Chapter 9
//vec3 color(const ray& r, hitable *world, int depth) {
//   hit_record rec;
//   if(world->hit(r, 0.001, MAXFLOAT, rec)){
//      ray scattered;
//      vec3 attenuation;
//      if((depth < 50) && (rec.mat_ptr->scatter(r, rec, attenuation, scattered))) {
//         return attenuation*color(scattered, world, depth+1);
//      }
//      else{
//         return vec3(0, 0, 0);
//      }
//   }
//   else{
//      vec3 unit_direction = unit_vector(r.direction());
//      float t = 0.5*(unit_direction.y() + 1.0);
//      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//   }
//}
//
//int main(){
//   std::ofstream fo("img009.ppm");
//   int nx = 1000;
//   int ny = 500;
//   int ns = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   hitable *list[5];
//   list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.1, 0.2, 0.5)));
//   list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
//   list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
//   list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
//   list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
//   hitable *world = new hitable_list(list, 5);
//   camera cam;
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         vec3 col(0, 0, 0);
//         for(int s = 0; s < ns; ++s){
//            float u = float(i + rn())/float(nx);
//            float v = float(j + rn())/float(ny);
//            ray r = cam.get_ray(u, v);
//            col += color(r, world, 0);
//         }
//         col /= float(ns);
//         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   delete dynamic_cast<sphere*>(list[0])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[1])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[2])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[3])->mat_ptr;
//   delete list[0];
//   delete list[1];
//   delete list[2];
//   delete list[3];
//   delete world;
//   fo.close();
//}

//Chapter 8
//vec3 color(const ray& r, hitable *world, int depth) {
//   hit_record rec;
//   if(world->hit(r, 0.001, MAXFLOAT, rec)){
//      ray scattered;
//      vec3 attenuation;
//      if((depth < 50) && (rec.mat_ptr->scatter(r, rec, attenuation, scattered))) {
//         return attenuation*color(scattered, world, depth+1);
//      }
//      else{
//         return vec3(0, 0, 0);
//      }
//   }
//   else{
//      vec3 unit_direction = unit_vector(r.direction());
//      float t = 0.5*(unit_direction.y() + 1.0);
//      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//   }
//}
//
//int main(){
//   std::ofstream fo("img008.ppm");
//   int nx = 1000;
//   int ny = 500;
//   int ns = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   hitable *list[4];
//   list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
//   list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
//   list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
//   list[3] = new sphere(vec3(-1, 0, -1), 0.5, new metal(vec3(0.8, 0.8, 0.8), 1.0));
//   hitable *world = new hitable_list(list, 4);
//   camera cam;
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         vec3 col(0, 0, 0);
//         for(int s = 0; s < ns; ++s){
//            float u = float(i + rn())/float(nx);
//            float v = float(j + rn())/float(ny);
//            ray r = cam.get_ray(u, v);
//            col += color(r, world, 0);
//         }
//         col /= float(ns);
//         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   delete dynamic_cast<sphere*>(list[0])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[1])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[2])->mat_ptr;
//   delete dynamic_cast<sphere*>(list[3])->mat_ptr;
//   delete list[0];
//   delete list[1];
//   delete list[2];
//   delete list[3];
//   delete world;
//   fo.close();
//}

//Chapter 7
//vec3 random_in_unit_sphere(){
//   vec3 p;
//   do {
//      p = 2.0*vec3(rn(), rn(), rn()) - vec3(1, 1, 1);
//   } while(p.squared_length() >= 1.0);
//   return p;
//}
//
//vec3 color(const ray& r, hitable *world) {
//   hit_record rec;
//   if(world->hit(r, 0.001, MAXFLOAT, rec)){
//      vec3 target = rec.p + rec.normal + random_in_unit_sphere();
//      return 0.5*color(ray(rec.p, target-rec.p), world);
//   }
//   else{
//      vec3 unit_direction = unit_vector(r.direction());
//      float t = 0.5*(unit_direction.y() + 1.0);
//      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//   }
//}
//
//int main(){
//   std::ofstream fo("img007.ppm");
//   int nx = 800;
//   int ny = 400;
//   int ns = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   hitable *list[2];
//   list[0] = new sphere(vec3(0, 0, -1), 0.5);
//   list[1] = new sphere(vec3(0, -100.5, -1), 100);
//   hitable *world = new hitable_list(list, 2);
//   camera cam;
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         vec3 col(0, 0, 0);
//         for(int s = 0; s < ns; ++s){
//            float u = float(i + rn())/float(nx);
//            float v = float(j + rn())/float(ny);
//            ray r = cam.get_ray(u, v);
//            col += color(r, world);
//         }
//         col /= float(ns);
//         col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   delete list[0];
//   delete list[1];
//   delete world;
//   fo.close();
//}

//Chapter 6
//vec3 color(const ray& r, hitable *world) {
//   hit_record rec;
//   if(world->hit(r, 0.0, MAXFLOAT, rec)){
//      return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
//   }
//   else{
//      vec3 unit_direction = unit_vector(r.direction());
//      float t = 0.5*(unit_direction.y() + 1.0);
//      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//   }
//}
//
//int main(){
//   std::ofstream fo("img006.ppm");
//   int nx = 2000;
//   int ny = 1000;
//   int ns = 100;
//   auto rn = std::bind(std::uniform_real_distribution<float>(), std::default_random_engine());
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   hitable *list[2];
//   list[0] = new sphere(vec3(0.2, 0.8, -1.0), 0.5);
//   list[1] = new sphere(vec3(0, -100.5, -1), 100);
//   hitable *world = new hitable_list(list, 2);
//   camera cam;
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         vec3 col(0, 0, 0);
//         for(int s = 0; s < ns; ++s){
//            float u = float(i + rn())/float(nx);
//            float v = float(j + rn())/float(ny);
//            ray r = cam.get_ray(u, v);
//            col += color(r, world);
//         }
//         col /= float(ns);
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   delete list[0];
//   delete list[1];
//   delete world;
//   fo.close();
//}

//Chapter 5
//vec3 color(const ray& r, hitable *world) {
//   hit_record rec;
//   if(world->hit(r, 0.0, MAXFLOAT, rec)){
//      return 0.5*vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1);
//   }
//   else{
//      vec3 unit_direction = unit_vector(r.direction());
//      float t = 0.5*(unit_direction.y() + 1.0);
//      return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//   }
//}
//
//int main(){
//   std::ofstream fo("img005.ppm");
//   int nx = 200;
//   int ny = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   vec3 lower_left_corner(-2.0, -1.0, -1.0);
//   vec3 horizontal(4.0, 0.0, 0.0);
//   vec3 vertical(0.0, 2.0, 0.0);
//   vec3 origin(0.0, 0.0, 0.0);
//   hitable *list[2];
//   list[0] = new sphere(vec3(0, 0, -1), 0.5);
//   list[1] = new sphere(vec3(0, -100.5, -1), 100);
//   hitable *world = new hitable_list(list, 2);
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         float u = float(i)/float(nx);
//         float v = float(j)/float(ny);
//         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
//         vec3 col = color(r, world);
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   delete list[0];
//   delete list[1];
//   delete world;
//   fo.close();
//}

//Chapter 4
//float hit_sphere(const vec3& center, float radius, const ray& r){
//   vec3 oc = r.origin() - center;
//   float a = dot(r.direction(), r.direction());
//   float b = 2.0*dot(oc, r.direction());
//   float c = dot(oc, oc) - radius*radius;
//   float discriminant = b*b - 4*a*c;
//   if(discriminant < 0){
//      return -1.0;
//   }
//   else{
//      return (-b-sqrt(discriminant))/(2.0*a);
//   }
//}
//
//vec3 color(const ray& r) {
//   vec3 s_center = vec3(0, 0, -1);
//   float t = hit_sphere(s_center, 0.5, r);
//   if(t > 0.0){
//      vec3 N = unit_vector(r.point_at_parameter(t) - s_center);
//      return 0.5*vec3(N.x() + 1, N.y() + 1, N.z() + 1);
//   }
//   vec3 unit_direction = unit_vector(r.direction());
//   t = 0.5*(unit_direction.y() + 1.0);
//   return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//}
//
//int main(){
//   std::ofstream fo("img004.ppm");
//   int nx = 200;
//   int ny = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   vec3 lower_left_corner(-2.0, -1.0, -1.0);
//   vec3 horizontal(4.0, 0.0, 0.0);
//   vec3 vertical(0.0, 2.0, 0.0);
//   vec3 origin(0.0, 0.0, 0.0);
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         float u = float(i)/float(nx);
//         float v = float(j)/float(ny);
//         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
//         vec3 col = color(r);
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   fo.close();
//}

//Chapter 4
//bool hit_sphere(const vec3& center, float radius, const ray& r){
//   vec3 oc = r.origin() - center;
//   float a = dot(r.direction(), r.direction());
//   float b = 2.0 * dot(oc, r.direction());
//   float c = dot(oc, oc) - radius*radius;
//   float discriminant = b*b - 4*a*c;
//   return(discriminant > 0);
//}
//
//vec3 color(const ray& r) {
//   if(hit_sphere(vec3(0, 0, -1), 0.5, r)){
//      return vec3(1, 0, 0);
//   }
//   vec3 unit_direction = unit_vector(r.direction());
//   float t = 0.5*(unit_direction.y() + 1.0);
//   return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//}
//
//int main(){
//   std::ofstream fo("img003.ppm");
//   int nx = 200;
//   int ny = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   vec3 lower_left_corner(-2.0, -1.0, -1.0);
//   vec3 horizontal(4.0, 0.0, 0.0);
//   vec3 vertical(0.0, 2.0, 0.0);
//   vec3 origin(0.0, 0.0, 0.0);
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         float u = float(i)/float(nx);
//         float v = float(j)/float(ny);
//         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
//         vec3 col = color(r);
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   fo.close();
//}

//Chapter 3
//vec3 color(const ray& r) {
//   vec3 unit_direction = unit_vector(r.direction());
//   float t = 0.5*(unit_direction.y() + 1.0);
//   return (1.0-t)*vec3(1.0, 1.0, 1.0) + t*vec3(0.5, 0.7, 1.0);
//}
//
//int main(){
//   std::ofstream fo("img002.ppm");
//   int nx = 200;
//   int ny = 100;
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   vec3 lower_left_corner(-2.0, -1.0, -1.0);
//   vec3 horizontal(4.0, 0.0, 0.0);
//   vec3 vertical(0.0, 2.0, 0.0);
//   vec3 origin(0.0, 0.0, 0.0);
//   for(int j = ny-1; j >= 0; j--){
//      for(int i = 0; i < nx; ++i){
//         float u = float(i)/float(nx);
//         float v = float(j)/float(ny);
//         ray r(origin, lower_left_corner + u*horizontal + v*vertical);
//         vec3 col = color(r);
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   fo.close();
//}

//Chapter 1
//int main() {
//   int nx = 200;
//   int ny = 100;
//   std::ofstream fo("img001.ppm");
//   fo << "P3\n" << nx << " " << ny << "\n255\n";
//   for(int j = ny-1; j>=0; j--){
//      for(int i = 0; i < nx; i++){
//         vec3 col(float(i)/float(nx), float(j)/float(ny), 0.2);
//         int ir = int(255.99*col[0]);
//         int ig = int(255.99*col[1]);
//         int ib = int(255.99*col[2]);
//         fo << ir << " " << ig << " " << ib << "\n";
//      }
//   }
//   fo.close();
//}
