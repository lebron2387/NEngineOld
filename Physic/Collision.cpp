
#include "Physic.h"

void CSignedDistanceMap::ConstructMesh(int32 _nx, int32 _ny, int32 _nz, Float32 extra_frac,Triangle *triangles,uint32 triangles_count,CGeometry *geometry)
{
	nx = _nx;
	ny = _ny;
	nz = _nz;

	Float32 big_val = 99999999.0f;
	min_x = min_y = min_z = big_val;
	max_x = max_y = max_z = -big_val;

	for(uint32 i = 0;i < triangles_count;i++)
	{
		min_x = math_min4(min_x, triangles[i].v[0][0], triangles[i].v[1][0], triangles[i].v[2][0]);
		min_y = math_min4(min_y, triangles[i].v[0][1], triangles[i].v[1][1], triangles[i].v[2][1]);
		min_z = math_min4(min_z, triangles[i].v[0][2], triangles[i].v[1][2], triangles[i].v[2][2]);
		max_x = math_max4(max_x, triangles[i].v[0][0], triangles[i].v[1][0], triangles[i].v[2][0]);
		max_y = math_max4(max_y, triangles[i].v[0][1], triangles[i].v[1][1], triangles[i].v[2][1]);
		max_z = math_max4(max_z, triangles[i].v[0][2], triangles[i].v[1][2], triangles[i].v[2][2]);
	}

	Float32 mid_x = 0.5f * (min_x + max_x);
	Float32 mid_y = 0.5f * (min_y + max_y);
	Float32 mid_z = 0.5f * (min_z + max_z);

	min_x = mid_x + extra_frac * (min_x - mid_x);
	min_y = mid_y + extra_frac * (min_y - mid_y);
	min_z = mid_z + extra_frac * (min_z - mid_z);
	max_x = mid_x + extra_frac * (max_x - mid_x);
	max_y = mid_y + extra_frac * (max_y - mid_y);
	max_z = mid_z + extra_frac * (max_z - mid_z);

	dx = (max_x - min_x) / (nx - 1);
	dy = (max_y - min_y) / (ny - 1);
	dz = (max_z - min_z) / (nz - 1);

	Vec3f corners[8];
	corners[0] = Vec3f(max_x, max_y, max_z);
	corners[1] = Vec3f(max_x, max_y, min_z);
	corners[2] = Vec3f(max_x, min_y, max_z);
	corners[3] = Vec3f(max_x, min_y, min_z);
	corners[4] = Vec3f(min_x, max_y, max_z);
	corners[5] = Vec3f(min_x, max_y, min_z);
	corners[6] = Vec3f(min_x, min_y, max_z);
	corners[7] = Vec3f(min_x, min_y, min_z);

	boundingRadius = corners[0].length();

	for(int32 i = 1 ; i < 8 ; i++)
	{
		if(corners[i].length() > boundingRadius) boundingRadius = corners[i].length();
	}

	if(geometry)
		PopulateMesh(geometry);
	else
		PopulateMesh(triangles,triangles_count);
}

Bool CSignedDistanceMap::GetPoint(Vec3f &pos, Vec3f &dir, Float32 &dist, Bool accurate_outside_dist)
{
	if(!accurate_outside_dist)
	{
		if( (pos[0] < min_x) || (pos[0] > max_x) ||
			(pos[1] < min_y) || (pos[1] > max_y) ||
			(pos[2] < min_z) || (pos[2] > max_z) )return false;
	}

	Float32 fi = (pos[0] - min_x) / dx;
	Float32 fj = (pos[1] - min_y) / dy;
	Float32 fk = (pos[2] - min_z) / dz;
	
	int32 ii = (int32)math_floor(fi + 0.5f);
	int32 ij = (int32)math_floor(fj + 0.5f);
	int32 ik = (int32)math_floor(fk + 0.5f);

	if(ii < 0) ii = 0; else if(ii >= nx) ii = nx - 1;
	if(ij < 0) ij = 0; else if(ij >= ny) ij = ny - 1;
	if(ik < 0) ik = 0; else if(ik >= nz) ik = nz - 1;

	SDMData &datum = data(ii, ij, ik);
	dir = datum.dir;
	dist = datum.dist;


	Float32 extra_dist = (dir * (pos - Vec3f(min_x + ii * dx, min_y + ij * dy,min_z + ik * dz)));

	dist -= extra_dist;

	if(dist > 0)
	{
		return true;
	}
	else
	{
		dist = -dist;
		if( (accurate_outside_dist) ||
			(ii == 0) || (ii == (nx - 1)) ||
			(ij == 0) || (ij == (ny - 1)) ||
			(ik == 0) || (ik == (nz - 1)) )
		{
			dist += Vec3f(pos - Vec3f(min_x + ii * dx, min_y + ij * dy, min_z + ik * dz)).length();
		}

		return false;
	}
	return false;
}

Float32 CSignedDistanceMap::GetSquareDistanceToBBox(Vec3f &pos)
{
	Vec3f box_pos;

	if(pos[0] > max_x)
		box_pos[0] = max_x;
	else if(pos[0] < min_x)
		box_pos[0] = min_x;
	else
		box_pos[0] = pos[0];

	if(pos[1] > max_y)
		box_pos[1] = max_y;
	else if(pos[1] < min_y)
		box_pos[1] = min_y;
	else
		box_pos[1] = pos[1];

	if(pos[2] > max_z)
		box_pos[2] = max_z;
	else if(pos[2] < min_z)
		box_pos[2] = min_z;
	else
		box_pos[2] = pos[2];

	return (pos - box_pos).length2();
}

void CSignedDistanceMap::PopulateMesh(CGeometry *geometry)
{
	data.resize(nx, ny, nz);

	for(int32 i = 0 ; i < nx ; i++)
	{
		for(int32 j = 0 ; j < ny ; j++)
		{
			for(int32 k = 0 ; k < nz ; k++)
			{
				Vec3f pos = Vec3f(min_x + i * dx, min_y + j * dy, min_z + k * dz);
				Bool inside = false;
				Vec3f vector_to_surface;
				if(geometry->GetMeshInfoSDM(pos, inside, vector_to_surface))
				{
					SDMData &datum = data(i, j, k);
					Float32 dist = vector_to_surface.length();
					if(dist > 0.00001f)
					{
						if(inside)
						{
							datum.dir = vector_to_surface / dist;
							datum.dist = dist;
						}
						else
						{
							datum.dir = -vector_to_surface / dist;
							datum.dist = -dist;
						}
					}
					else
					{
						datum.dir = pos;
						datum.dir.normalize();
						datum.dist = 0.00001f;
					}
				}
			}
		}
	}
}

Bool CSignedDistanceMap::IntersectSegmentTriangle(Segment seg,Triangle tri,Vec3f pos,Float32 SegP,Bool seg_in_dir_of_tri_normal)
{
	Vec3f edge0 = tri.v[1] - tri.v[0];
	Vec3f edge1 = tri.v[2] - tri.v[0];
	Vec3f tri_normal = (edge0 ^ edge1);
	tri_normal.normalize();
	Float32 tri_dist = -(tri_normal * tri.v[0]);
	Float32 perp_dist_seg0 = (seg.start * tri_normal) + tri_dist;
	Float32 perp_dist_seg1 = ((seg.start + seg.dir) * tri_normal) + tri_dist;
	if(perp_dist_seg0 * perp_dist_seg1 >= 0.0f)return false; 
	SegP = (perp_dist_seg0 / (perp_dist_seg0 - perp_dist_seg1));
	pos = seg.start +   seg.dir*SegP;
	// edge 0
	Vec3f plane_normal = Vec3f(edge0 ^ tri_normal);
	plane_normal.normalize();
	Float32 plane_dist = -(plane_normal * tri.v[0]);
	Float32 point_dist = (pos * plane_normal) + plane_dist;
	if(point_dist > 0.0f)return false;
	// edge 1
	plane_normal = Vec3f(tri_normal ^ edge1);
	plane_normal.normalize();
	plane_dist = -(plane_normal * tri.v[0]);
	point_dist = (pos * plane_normal) + plane_dist;
	if(point_dist > 0.0f)return false;
	Vec3f edge = edge1 - edge0;
	plane_normal = Vec3f(edge^ tri_normal);
	plane_normal.normalize();
	plane_dist = -(plane_normal * (tri.v[0] + edge0));
	point_dist = (pos * plane_normal) + plane_dist;
	if(point_dist > 0.0f)return false;
	seg_in_dir_of_tri_normal = (perp_dist_seg0 < 0.0f);
	return true;
}

uint32 CSignedDistanceMap::NumberTriangleIntersections(Segment seg, Triangle *triangles,uint32 triangles_count)
{
	uint32 hits = 0;
	Vec3f pos;
	Float32 S = 0.0f;
	Bool seg_in_dir = false;
	for(uint32 i = 0 ; i < triangles_count ; i++)
	{
		if(IntersectSegmentTriangle(seg, triangles[i], pos, S, seg_in_dir))
		{
			++hits;
		}
	}
	return hits;
}

void CSignedDistanceMap::PopulateMesh(Triangle *triangles,uint32 triangles_count)
{
	if(triangles_count == 0)return;

	data.resize(nx, ny, nz);

	Vec3f basic_dir = Vec3f(max_x - min_x, max_y - min_y, max_z - min_z);
	basic_dir *= 5.0f;

	uint32 num_outside_dirs = 10;
	Vec3f outside_dirs[10];

	for(uint32 i = 0 ; i < num_outside_dirs ; i++)
	{
		outside_dirs[i] = Vec3f(1.10f,1.10f, 1.10f);
		outside_dirs[i].normalize();
		outside_dirs[i] *= 2.0f * GetBoundingRadius();
	}

	for(int32 i = 0 ; i < nx ; i++)
	{
		for(int32 j = 0 ; j < ny ; j++)
		{
			for(int32 k = 0 ; k < nz ; k++)
			{	
				SDMData &datum = data(i, j, k);
				Vec3f pos = Vec3f(min_x + i * dx, min_y + j * dy, min_z + k * dz);
				Vec3f point_on_triangle;
				int32 triangle_index = 0;
				datum.dir = point_on_triangle - pos;
				datum.dist = datum.dir.length();
				if(datum.dist < 0.0001f)
				{
					datum.dist = 0.0001f;
					datum.dir = ((triangles[triangle_index].v[1] - triangles[triangle_index].v[0]) ^ (triangles[triangle_index].v[2] - triangles[triangle_index].v[0]));
					datum.dir.normalize();
				}
				else
				{
					datum.dir /= datum.dist;

					int32 num_in = 0;
					int32 num_out = 0;
					for(uint32 l = 0 ; l < num_outside_dirs ; l++)
					{
						
						Segment seg(pos, outside_dirs[l]);
						int hits = NumberTriangleIntersections(seg, triangles,triangles_count);
						if(hits % 2)
							num_in++;
						else
							num_out++;
							
					}

					if(num_out > num_in)
					{
						datum.dist = -datum.dist;
						datum.dir = -datum.dir;
					}
         
				}
			}
		}
	}
}



