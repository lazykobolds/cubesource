#include "GraphicsEngine.h"
#include "..\DirectX\VertexTypes.h"
#include "..\Utilities\MathLib.h"
#include "..\Graphics\Primatives.h"
#include "..\Config\ConfigData.h"

#include <assert.h>

GraphicsEngine::GraphicsEngine( void )
{
	d3d = NULL;
	d3ddev = NULL;
	unsigned int ScreenWidth = 800;
	unsigned int ScreenHeight = 600;	
	D3DXMatrixIdentity( &IdentityMatrix );
}

GraphicsEngine::~GraphicsEngine( void )
{
}

void GraphicsEngine::RenderScene( void )
{	
	Camera.CalculateMatricies();

	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	d3ddev->Clear(0, NULL, D3DCLEAR_ZBUFFER, 0, 1000.f, 0);
	d3ddev->BeginScene();

	if (!RenderQueue.empty())
	{
		LPD3DXEFFECT shader = 0;
		RenderFcn renderFcn = 0;
		RenderQueueType::iterator current = RenderQueue.begin();
		RenderQueueType::iterator end = RenderQueue.end();
		for (; current != end; ++current)
		{
			if (current->second->GraphicsComponent->Shader != shader)
			{
				shader = current->second->GraphicsComponent->Shader;
				renderFcn = RenderFunctions.find(shader)->second;
			}

			if (renderFcn)
				renderFcn(d3ddev, shader, &Camera, current->second, CurrentTransformation);
		}
	}

	RenderTerrain();

	d3ddev->EndScene();
	d3ddev->Present(NULL, NULL, NULL, NULL);
}

void GraphicsEngine::RenderTerrain()
{
	d3ddev->SetFVF(Vertex::VertexFVF);
	_basicShader->SetTechnique(0);
	_basicShader->SetMatrix("worldViewProj", &Camera.ViewProjMatrix);
	_basicShader->SetMatrix("viewMat", &Camera.ViewMatrix);
	_basicShader->SetMatrix("world", &IdentityMatrix);
	_basicShader->CommitChanges();


	unsigned int passes = 0;
	_basicShader->Begin(&passes, 0);

	for (unsigned int i = 0; i < passes; ++i)
	{
		_basicShader->BeginPass(i);

		for (unsigned int chunkIndex = 0; chunkIndex < MAX_CHUNKS; ++chunkIndex)
		{
			bool intersectsCamera = Camera.IntersectRect(
				float(_terrainSystem->_chunks[chunkIndex].xstart), float(_terrainSystem->_chunks[chunkIndex].xstart + CHUNK_WIDTH),
				float(_terrainSystem->_chunks[chunkIndex].zstart), float(_terrainSystem->_chunks[chunkIndex].zstart + CHUNK_WIDTH));

			if (intersectsCamera)
			{
				d3ddev->SetStreamSource(0, _terrainSystem->_chunks[chunkIndex].vert_data, 0, sizeof(Vertex));
				d3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, _terrainSystem->_chunks[chunkIndex].num_triangles);
			}
		}

		_basicShader->EndPass();
	}

	_basicShader->End();
}

void GraphicsEngine::Initilize( HWND hwnd )
{
	ScreenWidth = 800;
	ScreenHeight = 600;

	Camera.Position = D3DXVECTOR3( 0.f, 0.f, -10.f );

	InitilizeDirectX( hwnd );
	SetRenderStates();

	sphere = CreateGeoSphere( d3ddev, 2, 1.f );
	shader = LoadEffect( "effect.fx", d3ddev );


	_basicShader = LoadEffect("effect.fx", d3ddev);
}

void GraphicsEngine::SetRenderStates( void )
{
    d3ddev->SetRenderState( D3DRS_ZENABLE, TRUE );			// turn on the z-buffer
    d3ddev->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );  // ambient light
    d3ddev->SetRenderState( D3DRS_LIGHTING, FALSE );		// ambient light
    d3ddev->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESS );		// ambient light
}

void GraphicsEngine::InitilizeDirectX( HWND hwnd )
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);		// create the Direct3D interface

    D3DPRESENT_PARAMETERS d3dpp;				// create a struct to hold various device information

    ZeroMemory(&d3dpp, sizeof(d3dpp));			// clear out the struct for use
    d3dpp.Windowed = TRUE;						// program fullscreen, not windowed
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// discard old frames
    d3dpp.hDeviceWindow = hwnd;					// set the window to be used by Direct3D
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// set the back buffer format to 32-bit
    d3dpp.BackBufferWidth = ScreenWidth;		// set the width of the buffer
    d3dpp.BackBufferHeight = ScreenHeight;		// set the height of the buffer
    d3dpp.EnableAutoDepthStencil = TRUE;
    d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
	//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;


    // create a device class using this information and the info from the d3dpp stuct
    HRESULT err = d3d->CreateDevice( D3DADAPTER_DEFAULT,
                       D3DDEVTYPE_HAL,
                       hwnd,
                       D3DCREATE_HARDWARE_VERTEXPROCESSING,
					   //D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                       &d3dpp,
                       &d3ddev);
	if( err != 0 )
		printf(" create device error: %i\n", err );
}

void GraphicsEngine::RegisterShader( LPD3DXEFFECT shader, RenderFcn func )
{
	assert( RenderFunctions.find( shader ) == RenderFunctions.end() && "Error: Shader exists" );
	
	RenderFunctions.insert(std::pair< LPD3DXEFFECT, RenderFcn >(shader, func));
}

void GraphicsEngine::QueueObject( GameEntity * entity )
{
	if( entity->GraphicsComponent == 0 ) return;

	RenderQueue.insert(RenderQueuePair(entity->GraphicsComponent->RenderKey, entity));
}

void GraphicsEngine::SetTransformation( D3DXMATRIX * matrix )
{
	CurrentTransformation = matrix;
}

void GraphicsEngine::SetIdenityTransformation( void )
{
	CurrentTransformation = &IdentityMatrix;
}

void GraphicsEngine::BeginFrame(void)
{
	RenderQueue.clear();
}

void GraphicsEngine::SetTerrainSystem(TerrainSystem * terrainSystem)
{
	_terrainSystem = terrainSystem;
}