#include "MarchingCubes.h"


Triangle::Triangle()
{
	verts[0] = Vertex(0, 0, 0);
	verts[1] = Vertex(0, 0, 0);
	verts[2] = Vertex(0, 0, 0);
}
Triangle::Triangle(Vertex a, Vertex b, Vertex c)
{
	verts[0] = a;
	verts[1] = b;
	verts[2] = c;

	Vertex u = b - a;
	Vertex v = c - a;

	normal.x = u.y * v.z - u.z * v.y;
	normal.y = u.z * v.x - u.x * v.z;
	normal.z = u.x * v.y - u.y * v.x;
}

Cube::Cube()
{
	vTable = 0b00000000;
	eTable = 0b000000000000;
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
	Reset();
}

void MarchingCubes::Reset()
{
	if (cubes)
		delete [] cubes;
}

void MarchingCubes::Generate(Polyhedron* p)
{
	Reset();

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

		float s0 = vert0->scalar;
		float s1 = vert1->scalar;
		float s2 = vert2->scalar;
		float s3 = vert3->scalar;
		float s4 = vert4->scalar;
		float s5 = vert5->scalar;
		float s6 = vert6->scalar;
		float s7 = vert7->scalar;

		cubes[i].verts[0] = *vert0;
		cubes[i].verts[1] = *vert1;
		cubes[i].verts[2] = *vert2;
		cubes[i].verts[3] = *vert3;
		cubes[i].verts[4] = *vert4;
		cubes[i].verts[5] = *vert5;
		cubes[i].verts[6] = *vert5;
		cubes[i].verts[7] = *vert6;

		/*
		 * Get crossing information
		 */

		// Vertices
		if (s0 >= iso_value)
			cubes[i].vTable |= v0;
		if (s1 >= iso_value)
			cubes[i].vTable |= v1;
		if (s2 >= iso_value)
			cubes[i].vTable |= v2;
		if (s3 >= iso_value)
			cubes[i].vTable |= v3;
		if (s4 >= iso_value)
			cubes[i].vTable |= v4;
		if (s5 >= iso_value)
			cubes[i].vTable |= v5;
		if (s6 >= iso_value)
			cubes[i].vTable |= v6;
		if (s7 >= iso_value)
			cubes[i].vTable |= v7;

		std::cout << "Verts for cube " << i << ": " << std::bitset<8>(cubes[i].vTable) << "\n";

		// Edges
		int intersections = edgeTable[cubes[i].vTable];
		cubes[i].eTable = intersections;

		/*
		 * Generate crossing points
		 */

		//@Todo: This can be optimized, as currently most edges (except the outer ones) will be
		//		 calculated 4 times (because 4 adjacent cubes will share the same edge, and that
		//		 edge will be separately calculated for each cube)

		// If there are no intersections
		if (intersections == 0b000000000000 || intersections == 0b111111111111)
			continue;
		
		if (intersections & e01)
			cubes[i].crossing[0] = LERP(*vert0, *vert1, (iso_value - s0) / (s1 - s0));
		if (intersections & e12)
			cubes[i].crossing[1] = LERP(*vert1, *vert2, (iso_value - s1) / (s2 - s1));
		if (intersections & e23)
			cubes[i].crossing[2] = LERP(*vert2, *vert3, (iso_value - s2) / (s3 - s2));
		if (intersections & e30)
			cubes[i].crossing[3] = LERP(*vert3, *vert0, (iso_value - s3) / (s0 - s3));
		if (intersections & e45)
			cubes[i].crossing[0] = LERP(*vert4, *vert5, (iso_value - s4) / (s5 - s4));
		if (intersections & e56)
			cubes[i].crossing[1] = LERP(*vert5, *vert6, (iso_value - s5) / (s6 - s5));
		if (intersections & e67)
			cubes[i].crossing[2] = LERP(*vert6, *vert7, (iso_value - s6) / (s7 - s6));
		if (intersections & e74)
			cubes[i].crossing[3] = LERP(*vert7, *vert4, (iso_value - s7) / (s1 - s7));
		if (intersections & e04)
			cubes[i].crossing[0] = LERP(*vert0, *vert1, (iso_value - s0) / (s4 - s0));
		if (intersections & e15)
			cubes[i].crossing[1] = LERP(*vert1, *vert2, (iso_value - s1) / (s5 - s1));
		if (intersections & e26)
			cubes[i].crossing[2] = LERP(*vert2, *vert3, (iso_value - s2) / (s6 - s2));
		if (intersections & e37)
			cubes[i].crossing[3] = LERP(*vert3, *vert0, (iso_value - s3) / (s7 - s3));


		/*
		 * Generate triangles
		 */

		triangles.clear();

		 // The loop should run at most 5 times, since there is a
		 // maximum of 5 triangles that can be generated per cube.
		for (int i = 0; triTable[cubes[i].vTable][i] != -1 && i < 15; i += 3)
		{
			Vertex a = cubes[i].crossing[i];
			Vertex b = cubes[i].crossing[i + 1];
			Vertex c = cubes[i].crossing[i + 2];

			triangles.push_back(Triangle(a, b, c));
		}
	}

	printf("\nNumber of triangles: %d\n", triangles.size());

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

			if (i == 0 || i == 1)
			{
				temp_v->R = 1;
				temp_v->G = .5;
				temp_v->B = .5;
			}
		}
		else
		{
			temp_v->type = 0;
			temp_v->R = 0.;
			temp_v->B = 1.;

			if (i == 0 || i == 1)
			{
				temp_v->R = 0;
				temp_v->G = 0;
				temp_v->B = .5;
			}
		}
	}
}

Vertex MarchingCubes::LERP(Vertex a, Vertex b, float alpha, float epsilon)
{
	if (a.x < b.x || a.y < b.y || a.z < b.z)
	{
		Vertex tmp = a;
		a = b;
		b = tmp;
	}

	Vertex v;

	if (fabs(a.scalar - b.scalar) > epsilon)
		v = a + (b - a) / (b.scalar - a.scalar) * (alpha - a.scalar);
	else
		v = a;

	return v;
}

void MarchingCubes::Render(bool showPoints, bool showTriangles)
{
	if (showPoints)
	{
		
	}

	if (showTriangles)
	{
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GLfloat mat_diffuse[4] = { 1.0, 1.0, 0.0, 0.0 };
		GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialf(GL_FRONT, GL_SHININESS, 50.0);

		for (int i = 0; i < triangles.size(); i++)
		{
			glBegin(GL_TRIANGLES);
			for (int j = 0; j < 3; j++)
			{
				glNormal3d(triangles[i][j].normal.x, triangles[i][j].normal.y, triangles[i][j].normal.z);
				glVertex3d(triangles[i][j].x, triangles[i][j].y, triangles[i][j].z);
			}
			glEnd();
		}

		CHECK_GL_ERROR();
	}
}

void MarchingCubes::SetIsoValue(float iso_value)
{
	printf("Enter new value for isosurface: \n");
	scanf("%f", &this->iso_value);
}