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

void MarchingCubes::Init(Polyhedron* p)
{
	free_cubes();

	int num_of_vertexs = p->nverts;

	int num_of_lines_per_axis = (int)(pow((float)num_of_vertexs, 1.0 / 3.) + 0.5) - 1;

	int num_of_cubes = num_of_lines_per_axis * num_of_lines_per_axis * num_of_lines_per_axis;

	num_of_cubes = num_of_cubes;

	cubes = new Cube[num_of_cubes];

	/* Order in Cube
		0 --- 1
		/     /
	   3 --- 2 |
	   | |   |
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

		cubes[i].points[0] = p->vlist[idx_0];
		cubes[i].points[1] = p->vlist[idx_1];
		cubes[i].points[2] = p->vlist[idx_2];
		cubes[i].points[3] = p->vlist[idx_3];
		cubes[i].points[4] = p->vlist[idx_4];
		cubes[i].points[5] = p->vlist[idx_5];
		cubes[i].points[6] = p->vlist[idx_6];
		cubes[i].points[7] = p->vlist[idx_7];
	}

	printf("Value for isosurface: %f \n", iso_value);

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

void MarchingCubes::Generate()
{

}

void MarchingCubes::SetIsoValue(float iso_value)
{
	printf("Enter new value for isosurface: \n");
	scanf("%f", &this->iso_value);
}