#include "sphere.h"
#include "ray.h"

// Determine if the ray intersects with the sphere
Hit Sphere::Intersection(const Ray& ray, int part) const
{
    
    Hit result;
    
    vec3 v = ray.endpoint - this->center;
    vec3 u = ray.direction;
    
    double a = dot(u,u);
    double b = 2 * dot(u,v);
    double c = dot (v, v) - pow(this->radius, 2);
    double t1 = 0.0;
    double t2 = 0.0;
    
    double det = (b*b) - (4 * a * c);
    
   
   
   
   if(det > 0)
   {
       result.part = part;
        t1 = ((-b + sqrt(det)) / (2 * a));
        t2 = ((-b - sqrt(det)) / (2 * a));
        result.object = this;
        if (t1 >= small_t && t1 < t2)
        {
            //result.dist = t1;
            return {this, t1, part};
        }
        else if (t2 >= small_t && t2 < t1)
        {
            //result.dist = t2;
            return {this, t2 , part};
        }
        
   }
   else if (det == 0)
   {
       result.part = part;
       t1 = (-b / (2 * a));
       if (t1 >= small_t)
       {
            result.dist = t1;
	    result.object = this;
	return {this, t1, part};
       }
       else
       {
           result.dist = 0;
	   result.object = nullptr;
	return {nullptr, 0, part};
       }
       
      
       
   }
   else
   {
      result.dist = 0;
      result.part = 0;
      result.object = nullptr;
	return {nullptr, 0, part};

   }
   
   
    return result;
    
    
}

vec3 Sphere::Normal(const vec3& point, int part) const
{
    vec3 normal;
    normal = (point - this->center) / this->radius; // compute the normal direction
    return normal;
}

Box Sphere::Bounding_Box(int part) const
{
    Box box;
    TODO; // calculate bounding box
    return box;
}
