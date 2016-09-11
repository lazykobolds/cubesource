#include "../Components/Chunk.h"

class TerrainSystem
{
public:
	TerrainSystem(void);
	void InitializeDemo(LPDIRECT3DDEVICE9 d3ddev);
	Chunk * _chunks;

private:
};