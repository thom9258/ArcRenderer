#pragma once

#include <vector>


namespace BasicGeometry {
	
using Indice = unsigned int;

struct Pos {
	float x;
	float y;
	float z;
};
	
struct UV {
	float u;
	float v;
};

struct Normal {
	float x;
	float y;
	float z;
};

struct PlaneData {
	std::vector<Pos> positions;
	std::vector<Indice> indices;
	std::vector<UV> uvs;
};
	

struct CubeData {
	std::vector<Pos> positions;
	std::vector<Normal> normals;
	std::vector<Indice> indices;
	std::vector<UV> uvs;
};

struct SphereData {
};
	
PlaneData create_unit_plane();

CubeData create_unit_cube();
	
/**
 * @todo implement sphere: http://www.songho.ca/opengl/gl_sphere.html
 */
SphereData create_sphere(float radius);

/** *************************************************************
 *  Implementation defined below,
 *  enable by defining exactly once, before include.
 *
 *    #define BASICGEOMETRY_IMPLEMENTATION
 *    #include "path/to/BasicGeometry.hpp"
 */
   
#ifdef BASICGEOMETRY_IMPLEMENTATION

PlaneData create_unit_plane()
{
	PlaneData plane{};
	plane.positions = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        {0.5f, 0.5f, 0.0f},
        {-0.5f, 0.5f, 0.0f},
    };

    plane.indices = {
        0, 1, 2, 2, 3, 0
    };

	plane.uvs = {
		{1.0f, 0.0f},
		{0.0f, 0.0f},
		{0.0f, 1.0f},
		{1.0f, 1.0f}
	};
    return plane;
}

    
CubeData create_unit_cube()
{
	CubeData cube{};
    cube.positions = {
		{-0.5f, -0.5f, -0.5f},  // A 0
		{0.5f, -0.5f, -0.5f},   // B 1
		{0.5f,  0.5f, -0.5f},   // C 2
		{-0.5f,  0.5f, -0.5f},  // D 3
		{-0.5f, -0.5f,  0.5f},  // E 4
		{0.5f, -0.5f,  0.5f},   // F 5
		{0.5f,  0.5f,  0.5f},   // G 6
		{-0.5f,  0.5f,  0.5f},  // H 7
		
		{-0.5f,  0.5f, -0.5f},  // D 8
		{-0.5f, -0.5f, -0.5f},  // A 9
		{-0.5f, -0.5f,  0.5f},  // E 10
		{-0.5f,  0.5f,  0.5f},  // H 11
		{0.5f, -0.5f, -0.5f},   // B 12
		{0.5f,  0.5f, -0.5f},   // C 13
		{0.5f,  0.5f,  0.5f},   // G 14
		{0.5f, -0.5f,  0.5f},   // F 15
		
		{-0.5f, -0.5f, -0.5f},  // A 16
		{0.5f, -0.5f, -0.5f},   // B 17
		{0.5f, -0.5f,  0.5f},   // F 18
		{-0.5f, -0.5f,  0.5f},  // E 19
		{0.5f,  0.5f, -0.5f},   // C 20
		{-0.5f,  0.5f, -0.5f},  // D 21
		{-0.5f,  0.5f,  0.5f},  // H 22
		{0.5f,  0.5f,  0.5f},   // G 23
    };
	
	cube.normals = {
		{0.0f,  0.0f, -1.0f},
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 
		{0.0f,  0.0f, -1.0f}, 

		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},
		{0.0f,  0.0f, 1.0f},

		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},

		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},
		{1.0f,  0.0f,  0.0f},

		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},
		{0.0f, -1.0f,  0.0f},

		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f},
		{0.0f,  1.0f,  0.0f}
	};

    cube.indices = {
        // front and back
        0, 3, 2,
        2, 1, 0,
        4, 5, 6,
        6, 7 ,4,
        // left and right
        11, 8, 9,
        9, 10, 11,
        12, 13, 14,
        14, 15, 12,
        // bottom and top
        16, 17, 18,
        18, 19, 16,
        20, 21, 22,
        22, 23, 20
    };
	
    cube.uvs = {
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},    
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 0.0f},
		{1.0f, 1.0f},
		{0.0f, 1.0f}
	};
    return cube;
}
	

SphereData create_sphere(float)
{
	throw std::runtime_error("Spheres not supported!");
}

#endif
}
