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

	if (normal.x == 0 && normal.y == 0 && normal.z == 0)
		return;

	float len = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

	normal.x /= -len;
	normal.y /= -len;
	normal.z /= -len;

	//std::cout << "NORMAL: " << normal.x << ", " << normal.y << ", " << normal.z << "\n";
}

Cube::Cube()
{
	vTable = 0b00000000;
	eTable = 0b000000000000;
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

	for (int i = 0; i < triangles.size(); i++)
		if (triangles[i])
			delete triangles[i];

	triangles.clear();
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
		if (s0 < iso_value)
			cubes[i].vTable |= v0;
		if (s1 < iso_value)
			cubes[i].vTable |= v1;
		if (s2 < iso_value)
			cubes[i].vTable |= v2;
		if (s3 < iso_value)
			cubes[i].vTable |= v3;
		if (s4 < iso_value)
			cubes[i].vTable |= v4;
		if (s5 < iso_value)
			cubes[i].vTable |= v5;
		if (s6 < iso_value)
			cubes[i].vTable |= v6;
		if (s7 < iso_value)
			cubes[i].vTable |= v7;

		/*
		std::cout << "Verts for cube " << i << ": " << std::bitset<8>(cubes[i].vTable) << "\n"
			      << "\ts0 >= iso_value: " << (s0 >= iso_value) << "\n"
			      << "\ts1 >= iso_value: " << (s1 >= iso_value) << "\n"
			      << "\ts2 >= iso_value: " << (s2 >= iso_value) << "\n"
			      << "\ts3 >= iso_value: " << (s3 >= iso_value) << "\n"
			      << "\ts4 >= iso_value: " << (s4 >= iso_value) << "\n"
			      << "\ts5 >= iso_value: " << (s5 >= iso_value) << "\n"
			      << "\ts6 >= iso_value: " << (s6 >= iso_value) << "\n"
			      << "\ts7 >= iso_value: " << (s7 >= iso_value) << "\n";
		*/

		// Edges
		int intersections = edgeTable[cubes[i].vTable];
		cubes[i].eTable = intersections;

		//std::cout << "Edges for cube " << i << ": " << std::bitset<12>(intersections) << "\n";

		/*
		 * Generate crossing points
		 */

		//@Todo: This can be optimized, as currently most edges (except the outer ones) will be
		//		 repeatedly calculated 4 times (because 4 adjacent cubes will share the same edge,
		//		 so that edge will be separately calculated for each cube)

		// If there are no intersections
		if (intersections == 0b000000000000)
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
			cubes[i].crossing[4] = LERP(*vert4, *vert5, (iso_value - s4) / (s5 - s4));
		if (intersections & e56)
			cubes[i].crossing[5] = LERP(*vert5, *vert6, (iso_value - s5) / (s6 - s5));
		if (intersections & e67)
			cubes[i].crossing[6] = LERP(*vert6, *vert7, (iso_value - s6) / (s7 - s6));
		if (intersections & e74)
			cubes[i].crossing[7] = LERP(*vert7, *vert4, (iso_value - s7) / (s4 - s7));
		if (intersections & e04)
			cubes[i].crossing[8] = LERP(*vert0, *vert4, (iso_value - s0) / (s4 - s0));
		if (intersections & e15)
			cubes[i].crossing[9] = LERP(*vert1, *vert5, (iso_value - s1) / (s5 - s1));
		if (intersections & e26)
			cubes[i].crossing[10] = LERP(*vert2, *vert6, (iso_value - s2) / (s6 - s2));
		if (intersections & e37)
			cubes[i].crossing[11] = LERP(*vert3, *vert7, (iso_value - s3) / (s7 - s3));


		/*
		std::cout << "Crossings for cube " << i << ": " << std::bitset<12>(intersections) << "\n";

		if (intersections & e01)
			std::cout << "\n\t0 - e01: " << cubes[i].crossing[0].x << ", " << cubes[i].crossing[0].y << ", " << cubes[i].crossing[0].z << "\n";
		if (intersections & e12)
			std::cout << "\t1 - e12: " << cubes[i].crossing[1].x << ", " << cubes[i].crossing[1].y << ", " << cubes[i].crossing[1].z << "\n";
		if (intersections & e23)
			std::cout << "\t2 - e23: " << cubes[i].crossing[2].x << ", " << cubes[i].crossing[2].y << ", " << cubes[i].crossing[2].z << "\n";
		if (intersections & e30)
			std::cout << "\t3 - e30: " << cubes[i].crossing[3].x << ", " << cubes[i].crossing[3].y << ", " << cubes[i].crossing[3].z << "\n";
		if (intersections & e45)
			std::cout << "\t4 - e45: " << cubes[i].crossing[4].x << ", " << cubes[i].crossing[4].y << ", " << cubes[i].crossing[4].z << "\n";
		if (intersections & e56)
			std::cout << "\t5 - e56: " << cubes[i].crossing[5].x << ", " << cubes[i].crossing[5].y << ", " << cubes[i].crossing[5].z << "\n";
		if (intersections & e67)
			std::cout << "\t6 - e67: " << cubes[i].crossing[6].x << ", " << cubes[i].crossing[6].y << ", " << cubes[i].crossing[6].z << "\n";
		if (intersections & e74)
			std::cout << "\t7 - e74: " << cubes[i].crossing[7].x << ", " << cubes[i].crossing[7].y << ", " << cubes[i].crossing[7].z << "\n";
		if (intersections & e04)
			std::cout << "\t8 - e04: " << cubes[i].crossing[8].x << ", " << cubes[i].crossing[8].y << ", " << cubes[i].crossing[8].z << "\n";
		if (intersections & e15)
			std::cout << "\t9 - e15: " << cubes[i].crossing[9].x << ", " << cubes[i].crossing[9].y << ", " << cubes[i].crossing[9].z << "\n";
		if (intersections & e26)
			std::cout << "\t10 - e26: " << cubes[i].crossing[10].x << ", " << cubes[i].crossing[10].y << ", " << cubes[i].crossing[10].z << "\n";
		if (intersections & e37)
			std::cout << "\t11 - e37: " << cubes[i].crossing[11].x << ", " << cubes[i].crossing[11].y << ", " << cubes[i].crossing[11].z << "\n";
		*/

		/*
		if (i == 1)
		{
			std::cout << "Crossings for cube " << i << ": " << std::bitset<12>(intersections) << "\n";

			if (intersections & e01)
				std::cout << "\t0 - e01: " << cubes[i].crossing[0].x << ", " << cubes[i].crossing[0].y << ", " << cubes[i].crossing[0].z << "\n"
										   << "\t\ta: " << (*vert0).x << "," << (*vert0).y << "," << (*vert0).z << " - " << (*vert0).scalar << "\n"
										   << "\t\tb: " << (*vert1).x << "," << (*vert1).y << "," << (*vert1).z << " - " << (*vert1).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s0) / (s1 - s0) << "\n";
			if (intersections & e12)
				std::cout << "\t1 - e12: " << cubes[i].crossing[1].x << ", " << cubes[i].crossing[1].y << ", " << cubes[i].crossing[1].z << "\n"
										   << "\t\ta: " << (*vert1).x << "," << (*vert1).y << "," << (*vert1).z << " - " << (*vert1).scalar << "\n"
										   << "\t\tb: " << (*vert2).x << "," << (*vert2).y << "," << (*vert2).z << " - " << (*vert2).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s1) / (s2 - s1) << "\n";
			if (intersections & e23)
				std::cout << "\t2 - e23: " << cubes[i].crossing[2].x << ", " << cubes[i].crossing[2].y << ", " << cubes[i].crossing[2].z << "\n"
										   << "\t\ta: " << (*vert2).x << "," << (*vert2).y << "," << (*vert2).z << " - " << (*vert2).scalar << "\n"
										   << "\t\tb: " << (*vert3).x << "," << (*vert3).y << "," << (*vert3).z << " - " << (*vert3).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s2) / (s3 - s2) << "\n";
			if (intersections & e30)
				std::cout << "\t3 - e30: " << cubes[i].crossing[3].x << ", " << cubes[i].crossing[3].y << ", " << cubes[i].crossing[3].z << "\n"
										   << "\t\ta: " << (*vert3).x << "," << (*vert3).y << "," << (*vert3).z << " - " << (*vert3).scalar << "\n"
										   << "\t\tb: " << (*vert0).x << "," << (*vert0).y << "," << (*vert0).z << " - " << (*vert0).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s3) / (s0 - s3) << "\n";
			if (intersections & e45)
				std::cout << "\t4 - e45: " << cubes[i].crossing[4].x << ", " << cubes[i].crossing[4].y << ", " << cubes[i].crossing[4].z << "\n"
										   << "\t\ta: " << (*vert4).x << "," << (*vert4).y << "," << (*vert4).z << " - " << (*vert4).scalar << "\n"
										   << "\t\tb: " << (*vert5).x << "," << (*vert5).y << "," << (*vert5).z << " - " << (*vert5).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s4) / (s5 - s4) << "\n";
			if (intersections & e56)
				std::cout << "\t5 - e56: " << cubes[i].crossing[5].x << ", " << cubes[i].crossing[5].y << ", " << cubes[i].crossing[5].z << "\n"
										   << "\t\ta: " << (*vert5).x << "," << (*vert5).y << "," << (*vert5).z << " - " << (*vert5).scalar << "\n"
										   << "\t\tb: " << (*vert6).x << "," << (*vert6).y << "," << (*vert6).z << " - " << (*vert6).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s5) / (s6 - s5) << "\n";
			if (intersections & e67)
				std::cout << "\t6 - e67: " << cubes[i].crossing[6].x << ", " << cubes[i].crossing[6].y << ", " << cubes[i].crossing[6].z << "\n"
										   << "\t\ta: " << (*vert6).x << "," << (*vert6).y << "," << (*vert6).z << " - " << (*vert6).scalar << "\n"
										   << "\t\tb: " << (*vert7).x << "," << (*vert7).y << "," << (*vert7).z << " - " << (*vert7).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s6) / (s7 - s6) << "\n";
			if (intersections & e74)
				std::cout << "\t7 - e74: " << cubes[i].crossing[7].x << ", " << cubes[i].crossing[7].y << ", " << cubes[i].crossing[7].z << "\n"
										   << "\t\ta: " << (*vert7).x << "," << (*vert7).y << "," << (*vert7).z << " - " << (*vert7).scalar << "\n"
										   << "\t\tb: " << (*vert4).x << "," << (*vert4).y << "," << (*vert4).z << " - " << (*vert4).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s7) / (s4 - s7) << "\n";
			if (intersections & e04)
				std::cout << "\t8 - e04: " << cubes[i].crossing[8].x << ", " << cubes[i].crossing[8].y << ", " << cubes[i].crossing[8].z << "\n"
										   << "\t\ta: " << (*vert0).x << "," << (*vert0).y << "," << (*vert0).z << " - " << (*vert0).scalar << "\n"
										   << "\t\tb: " << (*vert4).x << "," << (*vert4).y << "," << (*vert4).z << " - " << (*vert5).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s0) / (s4 - s0) << "\n";
			if (intersections & e15)
				std::cout << "\t9 - e15: " << cubes[i].crossing[9].x << ", " << cubes[i].crossing[9].y << ", " << cubes[i].crossing[9].z << "\n"
										   << "\t\ta: " << (*vert1).x << "," << (*vert1).y << "," << (*vert1).z << " - " << (*vert0).scalar << "\n"
										   << "\t\tb: " << (*vert5).x << "," << (*vert5).y << "," << (*vert5).z << " - " << (*vert6).scalar << "\n"
										   << "\t\talpha: " << (iso_value - s1) / (s5 - s1) << "\n";
			if (intersections & e26)
				std::cout << "\t10 - e26: " << cubes[i].crossing[10].x << ", " << cubes[i].crossing[10].y << ", " << cubes[i].crossing[10].z << "\n"
										    << "\t\ta: " << (*vert2).x << "," << (*vert2).y << "," << (*vert2).z << " - " << (*vert0).scalar << "\n"
										    << "\t\tb: " << (*vert6).x << "," << (*vert6).y << "," << (*vert6).z << " - " << (*vert6).scalar << "\n"
										    << "\t\talpha: " << (iso_value - s2) / (s6 - s2) << "\n";
			if (intersections & e37)
				std::cout << "\t11 - e37: " << cubes[i].crossing[11].x << ", " << cubes[i].crossing[11].y << ", " << cubes[i].crossing[11].z << "\n"
										    << "\t\ta: " << (*vert3).x << "," << (*vert3).y << "," << (*vert3).z << " - " << (*vert0).scalar << "\n"
										    << "\t\tb: " << (*vert7).x << "," << (*vert7).y << "," << (*vert7).z << " - " << (*vert7).scalar << "\n"
										    << "\t\talpha: " << (iso_value - s3) / (s7 - s3) << "\n";
		}
		*/

		/*
		 * Generate triangles
		 */

		// The loop should run at most 5 times, since there is a
		// maximum of 5 triangles that can be generated per cube.
		for (int j = 0; triTable[cubes[i].vTable][j] != -1 && j < 15; j += 3)
		{
			/*
			if (i == 2)
				std::cout << "\tCrossings " << triTable[cubes[i].vTable][j] << ", " << triTable[cubes[i].vTable][j + 1] << ", " << triTable[cubes[i].vTable][j + 2] << "\n";
			*/
			Vertex a = cubes[i].crossing[triTable[cubes[i].vTable][j    ]];
			Vertex b = cubes[i].crossing[triTable[cubes[i].vTable][j + 1]];
			Vertex c = cubes[i].crossing[triTable[cubes[i].vTable][j + 2]];

			triangles.push_back(new Triangle(a, b, c));

			//std::cout << "TRI NORMAL: " << triangles.back().normal.x << ", " << triangles.back().normal.y << ", " << triangles.back().normal.z << "\n";
		}
	}

	/*
	std::cout << "\nTriangles:\n";
	for (int j = 0; j < triangles.size(); j++)
		std::cout << j << ":\n"
					   << "\t" << triangles[j][0].x << ", " << triangles[j][0].y << ", " << triangles[j][0].z << "\n"
					   << "\t" << triangles[j][1].x << ", " << triangles[j][1].y << ", " << triangles[j][1].z << "\n"
					   << "\t" << triangles[j][2].x << ", " << triangles[j][2].y << ", " << triangles[j][2].z << "\n";
	*/

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

	//printf("\nNumber of triangles: %d\n", triangles.size());

	//printf("Value for isosurface: %f \n", iso_value);
}

Vertex MarchingCubes::LERP(Vertex a, Vertex b, float alpha, float epsilon)
{
	/*
	if (a.x < b.x || a.y < b.y || a.z < b.z)
	{
		Vertex tmp = a;
		a = b;
		b = tmp;
	}

	Vertex v;

	if (fabs(a.scalar - b.scalar) > epsilon)
	{
		v = a + (b - a) / (b.scalar - a.scalar) * (alpha - a.scalar);

		//v = (b - a) * alpha + a;
		//v = Vertex(b.x - a.x, b.y - a.y, b.z - a.z) * alpha + a;
	}
	else
		v = a;

	return v;
	*/

	return (b - a) * alpha + a;
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
				//std::cout << "NORMAL: " << triangles[i][j].normal.x << ", " << triangles[i][j].normal.y << ", " << triangles[i][j].normal.z << "\n";

				glNormal3d(triangles[i]->normal.x, triangles[i]->normal.y, triangles[i]->normal.z);
				//glNormal3d(1, 1, 1);
				glVertex3d(triangles[i]->verts[j].x, triangles[i]->verts[j].y, triangles[i]->verts[j].z);
			}
			glEnd();
		}

		CHECK_GL_ERROR();
	}
}

void MarchingCubes::SetIsoValue(float iso_value)
{
	/*
	printf("Enter new value for isosurface: \n");
	scanf("%f", &this->iso_value);
	*/

	this->iso_value = iso_value;
}