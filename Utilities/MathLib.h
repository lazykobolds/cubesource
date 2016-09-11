#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "..\DirectX\VertexTypes.h"

void BuildVQSMatrix( D3DXMATRIX & out, const D3DXQUATERNION & rotation, const D3DXVECTOR3 & translation );
D3DXVECTOR3 GetTranslation( D3DXMATRIX & mat );
void PrintMatrix( D3DXMATRIX & mat );
void SetMatrix( D3DXMATRIX & out, D3DXVECTOR3 & v0, D3DXVECTOR3 & v1, D3DXVECTOR3 & v2 );

void RotateMatrix( D3DXMATRIX & mat, D3DXVECTOR3 & rotation_axix, float radians );
void AdjustMatrix( D3DXMATRIX & mat );
void TransformPoints( D3DXVECTOR3 * new_points, D3DXVECTOR3 * points, unsigned int num_points, D3DXMATRIX & old_mat, D3DXMATRIX & new_mat );


void ThickLine( Vertex * verts, unsigned int & vert_count, D3DXVECTOR3 & p0, D3DXVECTOR3 & p1, float width );

void SubDivide( Vertex triangle[3], D3DXPLANE & plane, Vertex outside[6], Vertex inside[6], unsigned int & out_count, unsigned int & in_count );
Vertex * SubDivideRiver( Vertex * vlist, unsigned int & num_verts, Vertex * rlist, unsigned int & num_rverts, D3DXVECTOR3 * river, unsigned int num_river, D3DXVECTOR3 & up_vec, float width );
void SubDividePlaneArea( Vertex ** vlist, unsigned int & num_verts, Vertex ** inner_verts, unsigned int & num_inner_verts, D3DXPLANE * planes, unsigned int num_planes );
void RemovePolygon( Vertex ** in_list, unsigned int & in_count, Vertex ** removed_list, unsigned int & removed_count, D3DXVECTOR3 * polygon, unsigned int poly_count );

void MapToSphereNormalized( D3DXVECTOR3 & vec );
void MapToSphere( Vertex & vec );
void MapToSphere( D3DXVECTOR3 & vec );
D3DXVECTOR3 MapToSphere( double x, double y, double z );
void MapToSphere( LPDIRECT3DVERTEXBUFFER9 verts, unsigned int num_verts );
void FixNormals( LPDIRECT3DVERTEXBUFFER9 verts, unsigned int num_verts );

bool PointsAreEqual( D3DXVECTOR3 & p0, D3DXVECTOR3 & p1 );
bool VertsAreEqual( Vertex & v0, Vertex v1 );
bool InsidePlanePointTest2D( D3DXVECTOR3 & plane, D3DXVECTOR3 & plane_point, D3DXVECTOR3 & point );

bool IntersectSphereQuad( const D3DXVECTOR3 & pos, const float radius, const D3DXVECTOR3 * quad, D3DXVECTOR3 & displacement );
bool IntersectSweptSphereQuad( const D3DXVECTOR3 & pos, const float radius, const D3DXVECTOR3 & velocity, const D3DXVECTOR3 * quad, D3DXVECTOR3 & norm, float & t );

void AddQuad( D3DXVECTOR3 & tl, D3DXVECTOR3 & bl, D3DXVECTOR3 & br, D3DXVECTOR3 & tr, Vertex * vert_data, unsigned int & vert_count, D3DXVECTOR3 * quad_data, unsigned int & quad_count );

#define T_TO_R_MAT ROTATION_MATS::R_n90_Z
#define T_TO_L_MAT ROTATION_MATS::R_90_Z
#define T_TO_B_MAT ROTATION_MATS::R_n90_X
#define T_TO_F_MAT ROTATION_MATS::R_90_X

#define R_TO_F_MAT ROTATION_MATS::R_n90_Y
#define R_TO_B_MAT ROTATION_MATS::R_90_Y
#define R_TO_T_MAT ROTATION_MATS::R_90_Z
#define R_TO_O_MAT ROTATION_MATS::R_n90_Z

#define F_TO_R_MAT ROTATION_MATS::R_90_Y
#define F_TO_L_MAT ROTATION_MATS::R_n90_Y
#define F_TO_T_MAT ROTATION_MATS::R_n90_X
#define F_TO_O_MAT ROTATION_MATS::R_90_X

#define O_TO_F_MAT ROTATION_MATS::R_n90_X
#define O_TO_B_MAT ROTATION_MATS::R_90_X
#define O_TO_R_MAT ROTATION_MATS::R_90_Z
#define O_TO_L_MAT ROTATION_MATS::R_n90_Z

#define L_TO_F_MAT ROTATION_MATS::R_90_Y
#define L_TO_B_MAT ROTATION_MATS::R_n90_Y
#define L_TO_T_MAT ROTATION_MATS::R_n90_Z
#define L_TO_O_MAT ROTATION_MATS::R_90_Z

#define B_TO_R_MAT ROTATION_MATS::R_n90_Y
#define B_TO_L_MAT ROTATION_MATS::R_90_Y
#define B_TO_T_MAT ROTATION_MATS::R_90_X
#define B_TO_O_MAT ROTATION_MATS::R_n90_X

class ROTATION_MATS
{
public:
	static D3DXMATRIX R_90_X;
	static D3DXMATRIX R_n90_X;
	static D3DXMATRIX R_90_Y;
	static D3DXMATRIX R_n90_Y;
	static D3DXMATRIX R_90_Z;
	static D3DXMATRIX R_n90_Z;
	static D3DXMATRIX R_1_Y;
	static D3DXMATRIX R_n1_Y;
};