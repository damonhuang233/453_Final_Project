#include "MarchingCubes.h"

Cube::Cube()
{
	verts = 0b00000000;
	edges = 0b000000000000;
}
Cube::~Cube()
{
	
}

MarchingCubes::MarchingCubes()
{
	num_of_cubes = 0;
	iso_value = 0;

	cubes = NULL;
}
MarchingCubes::MarchingCubes(int iso_value)
{
	num_of_cubes = 0;
	this->iso_value = iso_value;

	cubes = NULL;
}
MarchingCubes::~MarchingCubes()
{
	free_cubes();
}

void MarchingCubes::free_cubes()
{
	if (cubes)
		delete [] cubes;
}

void MarchingCubes::Generate(Polyhedron* p)
{
	free_cubes();

	int num_of_vertexs = p->nverts;

	int num_of_lines_per_axis = (int)(pow((float)num_of_vertexs, 1.0 / 3.) + 0.5) - 1;

	int num_of_cubes = num_of_lines_per_axis * num_of_lines_per_axis * num_of_lines_per_axis;

	num_of_cubes = num_of_cubes;

	cubes = new Cube[num_of_cubes];

	/* Order in Cube
		0 --- 1
		/|    /|
	   3 --- 2 |
	   | |   | |
	   | 4 --- 5
		/    |/
	   7 --- 6
	*/

	for (int i = 0; i < num_of_cubes; i++)
	{
		int tmp = i;
		int count_level = 0;
		if (tmp >= (num_of_lines_per_axis * num_of_lines_per_axis))
		{
			tmp -= num_of_lines_per_axis * num_of_lines_per_axis;
			count_level++;
		}
		int idx_0 = count_level * (num_of_lines_per_axis + 1) * (num_of_lines_per_axis + 1);

		int count_y = 0;
		if (tmp >= num_of_lines_per_axis)
		{
			tmp -= num_of_lines_per_axis;
			count_y++;
		}
		idx_0 += count_y * (num_of_lines_per_axis + 1) + tmp;

		int idx_1 = idx_0 + 1;
		int idx_2 = idx_1 + (num_of_lines_per_axis + 1);
		int idx_3 = idx_2 - 1;
		int idx_4 = idx_0 + (num_of_lines_per_axis + 1) * (num_of_lines_per_axis + 1);
		int idx_5 = idx_4 + 1;
		int idx_6 = idx_5 + (num_of_lines_per_axis + 1);
		int idx_7 = idx_6 - 1;

		Vertex* vert0 = p->vlist[idx_0];
		Vertex* vert1 = p->vlist[idx_1];
		Vertex* vert2 = p->vlist[idx_2];
		Vertex* vert3 = p->vlist[idx_3];
		Vertex* vert4 = p->vlist[idx_4];
		Vertex* vert5 = p->vlist[idx_5];
		Vertex* vert6 = p->vlist[idx_6];
		Vertex* vert7 = p->vlist[idx_7];

		cubes[i].points[0] = vert0;
		cubes[i].points[1] = vert1;
		cubes[i].points[2] = vert2;
		cubes[i].points[3] = vert3;
		cubes[i].points[4] = vert4;
		cubes[i].points[5] = vert5;
		cubes[i].points[6] = vert5;
		cubes[i].points[7] = vert6;

		/*
		 * Store crossing information
		 */

		// Vertices
		if (vert0->scalar >= iso_value)
			cubes[i].verts |= v0;
		if (vert1->scalar >= iso_value)
			cubes[i].verts |= v1;
		if (vert2->scalar >= iso_value)
			cubes[i].verts |= v2;
		if (vert3->scalar >= iso_value)
			cubes[i].verts |= v3;
		if (vert4->scalar >= iso_value)
			cubes[i].verts |= v4;
		if (vert5->scalar >= iso_value)
			cubes[i].verts |= v5;
		if (vert6->scalar >= iso_value)
			cubes[i].verts |= v6;
		if (vert7->scalar >= iso_value)
			cubes[i].verts |= v7;

		// Edges
		int intersections = edgeTable[cubes[i].verts];

		// if there are no intersections
		if (intersections == 0b000000000000 || intersections == 0b111111111111)
			continue;


	}

	printf("Value for isosurface: %f \n", iso_value);

	// For coloring
	for (int i = 0; i < p->nverts; i++)
	{
		Vertex* temp_v = p->vlist[i];
		if (temp_v->scalar >= iso_value)
		{
			// 0 for < iso_value, 1 for >= iso_value, 2 for new vertexs add by linear interpolation
			temp_v->type = 1;
			temp_v->R = 1.;
			temp_v->B = 0.;
		}
		else
		{
			temp_v->type = 0;
			temp_v->R = 0.;
			temp_v->B = 1.;
		}
	}
}

void MarchingCubes::Render()
{

}

void MarchingCubes::SetIsoValue(float iso_value)
{
	printf("Enter new value for isosurface: \n");
	scanf("%f", &this->iso_value);
}