
texture texture0;
float4x4 worldViewProj : WorldViewProjection;
float4x4 world;
float3 lightDir = float3(0,0,-1);
static const int MaxMatrices = 80;
float4x3   WorldMatrixArray[MaxMatrices] : WORLDMATRIXARRAY;

float4 colorset;

sampler Sampler
{
    Texture   = texture0;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;            
};

struct VS_INPUT
{
	float3 position	: POSITION;
	float3 normal : NORMAL;
	float2 tex0 : TEXCOORD0;
	float4 color : COLOR0;
};

struct VS_INPUT_SKIN
{
    float4 position	: POSITION;
    float3 normal : NORMAL;
	float2 tex0 : TEXCOORD0;
    float4 BlendWeights : BLENDWEIGHT;
    float4 BlendIndices : BLENDINDICES;
};


struct VS_OUTPUT
{
    float4 pos : POSITION;
	float2 tex0  : TEXCOORD0;
    float3 normal : TEXCOORD1;
	float4 color : COLOR0;
};

VS_OUTPUT SimpleVertex( VS_INPUT IN )
{
	float4 posvec = float4( 0.0, 0.0, 0.0, 1.0 );
    VS_OUTPUT OUT;

	OUT.pos = mul( float4(IN.position, 1) , mul( world, worldViewProj ) );
	OUT.normal = normalize( mul( float4(IN.normal, 0) , world ) );

	OUT.tex0 = IN.tex0;

	posvec = mul( posvec, world );
	OUT.color = IN.color;
	return OUT;
}

float4 SimplePixel( VS_OUTPUT IN ) : COLOR
{
	return float4( 0.5, 0.8, 0.5, 1.0 );
}

float4 BlackPixel( VS_OUTPUT IN ) : COLOR
{
	return float4( 0.0, 0.0, 0.0, 1.0 );
}

technique Technique0
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 SimpleVertex();
        PixelShader  = compile ps_2_0 SimplePixel();
        //ZEnable = false;
        AlphaBlendEnable = true;
		SrcBlend = ONE;
		DestBlend = ONE;
		CullMode = NONE;
		//FillMode = WireFrame;
    }
    pass Pass0
    {
        VertexShader = compile vs_2_0 SimpleVertex();
        PixelShader  = compile ps_2_0 BlackPixel();
        //ZEnable = false;
        AlphaBlendEnable = false;
		CullMode = NONE;
		FillMode = WireFrame;
    }
}

technique outlines
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 SimpleVertex();
        PixelShader  = compile ps_2_0 SimplePixel();
        //ZEnable = false;
        AlphaBlendEnable = false;
		//CullMode = NONE;
		FillMode = WireFrame;
    }
}
