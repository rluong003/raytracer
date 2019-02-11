#include "light.h"
#include "phong_shader.h"
#include "ray.h"
#include "render_world.h"
#include "object.h"

vec3 Phong_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color;
  
    vec3 amb; vec3 diff; vec3 spec;
    vec3 Ld, Rd; 
    vec3 Rs, Ls;
    
    // color = I(amb) + I (diff) + I(spec)
    
    
    amb = color_ambient * world.ambient_color * world.ambient_intensity;
  
  // color * intensity * max(vectors)
  
  for (unsigned int i = 0; i < world.lights.size(); i++)
  {
      
      // DIFFUSE = Rd * Ld * max(l * n, 0)
      // SPEC = Rs * Ls * max(cos(theta),0 ) ^ specular_power
      // or Rs * Ls * max( v * r, 0) ^ specular_power
      
      // Reflection = View  - 2 * (View * Normal) * Normal (From Piazza)
      
      // TA answered my question
      // view = intersection point to the camera
      // reflection = light vector & normal

        /*
     double angle = dot(normal.normalized(), -i_to_l.normalized()) / (normal.magnitude() * i_to_l.magnitude());
      
      angle = acos(angle);
      
      double c = cos(angle);
     
     
      if (c < 0)
      {
          c = 0;
      }
      
      */
      
    Ray shadowray;
    shadowray.endpoint = intersection_point;
    shadowray.direction = (world.lights.at(i)->position - intersection_point).normalized();
    vec3 i_to_l = intersection_point - world.lights.at(i)->position;
    Rs = color_specular;
    Rd = color_diffuse;
    Ld = world.lights.at(i)->Emitted_Light(i_to_l);  
    Ls = world.lights.at(i)->Emitted_Light(i_to_l); 
         
    double ln = std::fmax(dot(normal.normalized(), -i_to_l.normalized()),0.0);
    
  //  diff = diff + Rd * Ld * ln;
    
    vec3 v = intersection_point - ray.endpoint;
    vec3 L = (intersection_point - world.lights.at(i)->position).normalized();
       
    vec3 r = L - (2 * dot(L, normal)) * normal;
    double vr = std::fmax(dot(-v.normalized(),r.normalized()),0.0);
      
    //spec = spec + Rs * Ls * pow(vr,specular_power);
      
	    
      
    

   Hit shadows = world.Closest_Intersection(shadowray);
   // if shadows arent enabled,, or shhardows are enables,, but theres no object intersction or theh object distance is greater than the length of the light to intersection point vector   
     if (!world.enable_shadows || (world.enable_shadows && (!shadows.object || shadows.dist >= i_to_l.magnitude()) ))
      {
       diff = diff + Rd * Ld * ln;
 	   spec = spec + Rs * Ls * pow(vr,specular_power);

          
      }
          
      
  }
// if the shadow coniditionals are skipped, then the color will only be ambient, if one of the shadow conditionals pass, then diffuse and specular are added or else they are just 0 
	color = amb + diff + spec;
    return color;
}
