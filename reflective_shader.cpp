#include "reflective_shader.h"
#include "ray.h"
#include "render_world.h"

vec3 Reflective_Shader::
Shade_Surface(const Ray& ray,const vec3& intersection_point,
    const vec3& normal,int recursion_depth) const
{
    vec3 color = shader->Shade_Surface(ray, intersection_point, normal, recursion_depth);
    
    if ( recursion_depth >= world.recursion_depth_limit)
    {
        color = (1 - reflectivity ) * color;
        return color;
    }
    
    Ray reflect(intersection_point, (ray.direction - (2 * dot(ray.direction, normal)) * normal) );
    
    color = (1 - reflectivity) * color + reflectivity * world.Cast_Ray(reflect, ++recursion_depth);
     // determine the color
    return color;
}
