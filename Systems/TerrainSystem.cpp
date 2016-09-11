#include "TerrainSystem.h"
#include "../Utilities/VoxelMesh.h"
#include "../Config/ConfigData.h"

TerrainSystem::TerrainSystem(void)
{
}

void TerrainSystem::InitializeDemo(LPDIRECT3DDEVICE9 d3ddev)
{
	_chunks = new Chunk[MAX_CHUNKS];
	for (unsigned int x = 0; x < 8; ++x)
	{
		for (unsigned int y = 0; y < 8; ++y)
		{
			unsigned int index = y * 8 + x;
			_chunks[index].Initilize(
				x*CHUNK_WIDTH, 0, y*CHUNK_WIDTH,
				CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH,
				0);
			char * vdp = new char[(CHUNK_WIDTH + 2)*(CHUNK_WIDTH + 2)*(CHUNK_HEIGHT + 2)];
			char * vd = new char[CHUNK_WIDTH*CHUNK_WIDTH*CHUNK_HEIGHT];
			unsigned int nt = GenerateTerrainWithPadding(
				x*CHUNK_WIDTH, 0, y*CHUNK_WIDTH,
				CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH,
				10, vdp);

			/* fill vd (real) from vdp (temp).
			- vd has a 1 block blank border on all sides for calculations in other algorithms.
			*/
			for (unsigned int _z = 0; _z < CHUNK_WIDTH; ++_z)
				for (unsigned int _y = 0; _y < CHUNK_HEIGHT; ++_y)
					for (unsigned int _x = 0; _x < CHUNK_WIDTH; ++_x)
					{
				vd[_z*CHUNK_WIDTH*CHUNK_HEIGHT + _y*CHUNK_WIDTH + _x] = vdp[(_z + 1)*(CHUNK_WIDTH + 2)*(CHUNK_HEIGHT + 2) + (_y + 1)*(CHUNK_WIDTH + 2) + _x + 1];
					}
			delete vdp; /* erase vdp because we are done with it. (pre-load structure) */

			_chunks[index].SetVoxelData(vd);
			_chunks[index].GenerateVertexBuffer(d3ddev);

			//elist[index].GraphicsComponent = new MeshComponent();
			//((MeshComponent*)elist[index].GraphicsComponent)->NumTriangles = _chunks[index].num_triangles;//nt;
			//((MeshComponent*)elist[index].GraphicsComponent)->Vertices = _chunks[index].vert_data;
			//elist[index].GraphicsComponent->Shader = shader;
			//elist[index].Active = true;
			//D3DXMatrixIdentity(&elist[index].Rotation);
		}
	}
}