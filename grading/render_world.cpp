#include "render_world.h"
#include "flat_shader.h"
#include "object.h"
#include "light.h"
#include "ray.h"
#include <iostream>

extern bool disable_hierarchy;

Render_World::Render_World()
    :background_shader(0),ambient_intensity(0),enable_shadows(true),
    recursion_depth_limit(3)
{}

Render_World::~Render_World()
{
    delete background_shader;
    for(size_t i=0;i<objects.size();i++) delete objects[i];
    for(size_t i=0;i<lights.size();i++) delete lights[i];
}

// Find and return the Hit structure for the closest intersection.  Be careful
// to ensure that hit.dist>=small_t.
Hit Render_World::Closest_Intersection(const Ray& ray)
{
    double min_t = std::numeric_limits<int>::max();
    Hit result;
    result = {nullptr, 0, 0};
    for(unsigned int i = 0; i < this->objects.size(); i++)
    {
        Hit temp = this->objects.at(i)->Intersection(ray, -1);
        if (temp.dist > small_t && temp.dist < min_t && temp.object)
        {
            min_t = temp.dist;
            result = temp;
        }
    }
   
    return result;
}

// set up the initial view ray and call
void Render_World::Render_Pixel(const ivec2& pixel_index)
{
     // set up the initial view ray here
    Ray ray;
    ray.endpoint = camera.position;
    
    
    ray.direction = (camera.World_Position(pixel_index) - ray.endpoint).normalized();
    
    vec3 color=Cast_Ray(ray,1);
    camera.Set_Pixel(pixel_index,Pixel_Color(color));
}

void Render_World::Render()
{
    if(!disable_hierarchy)
        Initialize_Hierarchy();

    for(int j=0;j<camera.number_pixels[1];j++)
        for(int i=0;i<camera.number_pixels[0];i++)
            Render_Pixel(ivec2(i,j));
}

// cast ray and return the color of the closest intersected surface point,
// or the background color if there is no object intersection
vec3 Render_World::Cast_Ray(const Ray& ray,int recursion_depth)
{
    vec3 color;
    Hit close = this->Closest_Intersection(ray);
    vec3 inter_point;
    vec3 norm;
    vec3 trash;
    
    if(recursion_depth > recursion_depth_limit)
    {
      color = background_shader->Shade_Surface(ray, trash, trash, recursion_depth);

    }
    
   else if(close.dist != 0)
    {
	
        inter_point = ray.Point(close.dist);
        norm = close.object->Normal(inter_point, close.part);
        color = close.object->material_shader->Shade_Surface(ray, inter_point, norm, recursion_depth);
        
    }
    else
    {
        color = background_shader->Shade_Surface(ray, trash, trash, recursion_depth);
    }
    // determine the color here
    
    return color;
}

void Render_World::Initialize_Hierarchy()
{
    TODO; // Fill in hierarchy.entries; there should be one entry for
    // each part of each object.

    hierarchy.Reorder_Entries();
    hierarchy.Build_Tree();
}
