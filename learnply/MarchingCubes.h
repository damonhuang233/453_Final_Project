#ifndef MARCHING_CUBES
#define MARCHING_CUBES

#include "polyhedron.h"

class Cube {
public:
	Vertex* verts[8];
	int edges;
};

class MarchingCubes
{
	private:
		float iso_value = 0;

		int num_of_cubes;
		Cube* cubes;

	public:
		MarchingCubes();
		MarchingCubes(int);
		~MarchingCubes();

		void SetIsoValue(float);

		void Init(Polyhedron *);
		void Generate();

		void free_cubes();
};

#endif