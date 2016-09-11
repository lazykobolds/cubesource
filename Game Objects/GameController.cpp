#include "GameController.h"

#include "..\Utilities\VoxelMesh.h"
#include "..\Components\Chunk.h"

/*   CHUNK_WIDTH * 8 chunks => total grid width:
	 8 - 64
	16 - 128
	32 - 256
*/
#define CHUNK_WIDTH 16
#define CHUNK_HEIGHT 128

Chunk chunks[64];
GameEntity elist[64];

GameEntity esphere;

void GameController::Initilize( HWND hWnd, HINSTANCE hInstance )
{	
	G.Initilize( hWnd );
	IB.Initilize( hWnd, hInstance );

	obj.GraphicsComponent = new MeshComponent();
	obj.Active = true;

	((MeshComponent*)obj.GraphicsComponent)->Vertices = CreateGeoSphere( G.d3ddev, 2, 1.8f );
	((MeshComponent*)obj.GraphicsComponent)->NumTriangles = 64 * 2;
	D3DXMatrixIdentity( &obj.Rotation );
	obj.Position.x = 0.f;
	obj.Position.y = 0.f;
	obj.Position.z = 0.f;

	obj.GraphicsComponent->Shader = LoadEffect("effect.fx", G.d3ddev);
	shader = obj.GraphicsComponent->Shader;
	G.RegisterShader( shader, RenderFunctions::SampleRender );
	D3DXMatrixIdentity( &obj.Rotation );
	D3DXMatrixIdentity( &G.Camera.ViewMatrix );	
	G.Camera.Position = D3DXVECTOR3( 0.f, 0.f, 0.f );

	D3DXMatrixLookAtLH(
		&G.Camera.ViewMatrix,
		&D3DXVECTOR3( 0.f, 0.f, 0.f ),		//eye
		&D3DXVECTOR3( 0.f, -1.f, -1.f ),	//target
		&D3DXVECTOR3( 0.f, 1.f, 0.f ) );	//up vector

	unsigned int tri_diff[2] = { 0, 0 };

	TerrainSystem * terrainSystem = new TerrainSystem();
	terrainSystem->InitializeDemo(G.d3ddev);
	G.SetTerrainSystem(terrainSystem);
}

void GameController::Update( unsigned int i )
{

#pragma region Update user input

	IB.UpdateInputBuffer();
	if( IB.KeyDown( DIK_W ) ) G.Camera.Position.z += 0.1f;
	if( IB.KeyDown( DIK_S ) ) G.Camera.Position.z -= 0.1f;
	if( IB.KeyDown( DIK_D ) ) G.Camera.Position.x += 0.1f;
	if( IB.KeyDown( DIK_A ) ) G.Camera.Position.x -= 0.1f;
	if( IB.KeyDown( DIK_E ) ) G.Camera.Position.y += 0.1f;
	if( IB.KeyDown( DIK_Q ) ) G.Camera.Position.y -= 0.1f;

#pragma endregion 
		
	G.Camera.ViewMatrix._41 = G.Camera.Position.x;
	G.Camera.ViewMatrix._42 = G.Camera.Position.y;
	G.Camera.ViewMatrix._43 = G.Camera.Position.z;	
		
	//G.Camera.Position = obj.Position + ::D3DXVECTOR3( 0.f, 2.f, 10.f );
	G.Camera.right.x = 1.0f;
	G.Camera.right.y = 0.0f;
	G.Camera.right.z = 0.0f;
	D3DXVec3Normalize( &G.Camera.dir, &D3DXVECTOR3( 0.f, -2.f, -10.f ) );
	
	//right = D3DXVECTOR3( ViewMatrix._11, ViewMatrix._12, ViewMatrix._13 );
	//up = D3DXVECTOR3( ViewMatrix._21, ViewMatrix._22, ViewMatrix._23 );
	//dir = D3DXVECTOR3( ViewMatrix._31, ViewMatrix._32, ViewMatrix._33 );

	G.Camera.CalculateMatricies();
	G.SetIdenityTransformation();
	G.BeginFrame();

	G.QueueObject( &obj );

	G.RenderScene();
}


void GameController::CollisionStep( void )
{
	
}

void GameController::DetectObjectCollision( GameEntity & ge )
{

}