#include "mesh.h"
#include "plane.h"
#include <fstream>
#include <string>
#include <limits>

// Consider a triangle to intersect a ray if the ray intersects the plane of the
// triangle with barycentric weights in [-weight_tolerance, 1+weight_tolerance]
static const double weight_tolerance = 1e-4;

// Read in a mesh from an obj file.  Populates the bounding box and registers
// one part per triangle (by setting number_parts).
void Mesh::Read_Obj(const char* file)
{
    std::ifstream fin(file);
    if(!fin)
    {
        exit(EXIT_FAILURE);
    }
    std::string line;
    ivec3 e;
    vec3 v;
    box.Make_Empty();
    while(fin)
    {
        getline(fin,line);

        if(sscanf(line.c_str(), "v %lg %lg %lg", &v[0], &v[1], &v[2]) == 3)
        {
            vertices.push_back(v);
            box.Include_Point(v);
        }

        if(sscanf(line.c_str(), "f %d %d %d", &e[0], &e[1], &e[2]) == 3)
        {
            for(int i=0;i<3;i++) e[i]--;
            triangles.push_back(e);
        }
    }
    number_parts=triangles.size();
}

// Check for an intersection against the ray.  See the base class for details.
Hit Mesh::Intersection(const Ray& ray, int part) const
{
    Hit result;
    result.object = nullptr;
    result.dist = 0;
    result.part = 0;

    if (part >= 0) {
        double dist;
        if (Intersect_Triangle(ray, part, result.dist)) 
        {
           return{this, dist, part};
        }
        
    } 
    
    else {
     // similar to render_worlds intersect 
        result.dist = std::numeric_limits<double>::max();

        for (unsigned int i = 0; i < triangles.size(); i++) {
            double dist;
            if (Intersect_Triangle(ray, i, dist)) {
                
                if (dist < result.dist) 
                {
                    result.object = this;
                    result.dist = dist;
                    result.part = i;
                }
            }
        }        
    }

   
    return result;
}

// Compute the normal direction for the triangle with index part.
vec3 Mesh::Normal(const vec3& point, int part) const
{
    assert(part>=0);
    // THREE VERTICES, A, B, C. 
    // NORMAL = crossproduct (A-B) (C-B)
   
    vec3 vertA = vertices.at(triangles[part][0]);
    vec3 vertB = vertices.at(triangles[part][1]);
    vec3 vertC = vertices.at(triangles[part][2]);
    return cross(vertA - vertB, vertB - vertC).normalized();
}

// This is a helper routine whose purpose is to simplify the implementation
// of the Intersection routine.  It should test for an intersection between
// the ray and the triangle with index tri.  If an intersection exists,
// record the distance and return true.  Otherwise, return false.
// This intersection should be computed by determining the intersection of
// the ray and the plane of the triangle.  From this, determine (1) where
// along the ray the intersection point occurs (dist) and (2) the barycentric
// coordinates within the triangle where the intersection occurs.  The
// triangle intersects the ray if dist>small_t and the barycentric weights are
// larger than -weight_tolerance.  The use of small_t avoid the self-shadowing
// bug, and the use of weight_tolerance prevents rays from passing in between
// two triangles.
bool Mesh::Intersect_Triangle(const Ray& ray, int tri, double& dist) const
{
    // Triangle vertices
    vec3 vertA = vertices.at(triangles[tri][0]);
    vec3 vertB = vertices.at(triangles[tri][1]);
    vec3 vertC = vertices.at(triangles[tri][2]);
    
    
    Hit t_inter = Plane(vertA, Normal(vertA, tri)).Intersection(ray, tri);

    if (!t_inter.object || t_inter.dist <= small_t) 
    {
        return false;
    }
    
    // https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
    
    vec3 inter = ray.Point(dist);
    vec3 a = ray.direction; 
    vec3 b = vertB - vertA; 
    vec3 c = vertC - vertA;  
    vec3 d = inter - vertA; 
    

    double gamma = dot(cross(a, b), d) / dot(cross(a, b), c);
    double beta = dot(cross(c, a), d) / dot(cross(a, b), c);
    double alpha = 1.0 - gamma - beta;

    if (gamma > -weight_tolerance && beta > -weight_tolerance && alpha > -weight_tolerance) 
    {
        dist = t_inter.dist;
        return true;
    }
    
    
    return false;
}

// Compute the bounding box.  Return the bounding box of only the triangle whose
// index is part.
Box Mesh::Bounding_Box(int part) const
{
    Box b;
    TODO;
    return b;
}
