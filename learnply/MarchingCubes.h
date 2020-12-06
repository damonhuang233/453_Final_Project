#include "polyhedron.h"

class Cube {
public:
	Vertex* verts[8];
};

class Cubes {

public:
	int num_of_cubes;

	Cube* cubes;
};

Cubes *init_cubes(Polyhedron* p)
{
	Cubes* _cubes = new Cubes;

	int num_of_vertexs = p->nverts;
	
	int num_of_lines_per_axis = (int)(pow((float)num_of_vertexs,1.0/3.) + 0.5) - 1;

	int num_of_cubes = num_of_lines_per_axis * num_of_lines_per_axis * num_of_lines_per_axis;

	_cubes->num_of_cubes = num_of_cubes;

	_cubes->cubes = (Cube*)malloc(num_of_cubes * sizeof(class Cube));
	
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

		_cubes->cubes[i].verts[0] = p->vlist[idx_0];
		_cubes->cubes[i].verts[1] = p->vlist[idx_1];
		_cubes->cubes[i].verts[2] = p->vlist[idx_2];
		_cubes->cubes[i].verts[3] = p->vlist[idx_3];
		_cubes->cubes[i].verts[4] = p->vlist[idx_4];
		_cubes->cubes[i].verts[5] = p->vlist[idx_5];
		_cubes->cubes[i].verts[6] = p->vlist[idx_6];
		_cubes->cubes[i].verts[7] = p->vlist[idx_7];
	}

	return _cubes;
}