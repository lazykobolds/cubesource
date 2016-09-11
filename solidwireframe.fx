
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

struct VS_OUTPUT_SIMPLE
{
	float4 pos : POSITION;
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
	//OUT.color = posvec;
	OUT.color = IN.color;
	return OUT;
}

VS_OUTPUT_SIMPLE SolidVertex( VS_INPUT IN )
{ 
    VS_OUTPUT_SIMPLE OUT;
	OUT.pos = mul( float4(IN.position, 1) , mul( world, worldViewProj ) );
	OUT.color.a = 1.0;
	OUT.color.r = 0.0;
	OUT.color.g = 0.0;
	OUT.color.b = 0.0;
	return OUT;
}

float4 SolidPixel( VS_OUTPUT_SIMPLE IN ) : COLOR
{
	return IN.color;
}

float4 SimplePixel( VS_OUTPUT IN ) : COLOR
{
	//return IN.color;
	float mag = dot( IN.normal.xyz, float3( 1.0, 1.0, 1.0 ) );
	return IN.color * mag;
	return float4( IN.normal.z,IN.normal.z,IN.normal.z,IN.normal.z );
	return IN.color * mag;
	return colorset * mag;
	return float4( mag, mag, mag, 1.0 );
	return float4( IN.normal.x, IN.normal.y, IN.normal.z, 1.0 );
	return IN.color.x * colorset;
}

float4 TexPixel( VS_OUTPUT IN ) : COLOR
{
	float3 normal = normalize(IN.normal);	
	float NL = saturate(dot( normal , lightDir.xyz));
	float4 texColor = tex2D( Sampler , IN.tex0 );
	return NL * texColor;
}

VS_OUTPUT SkinVertex( VS_INPUT_SKIN IN , uniform int NumBones )
{
    VS_OUTPUT OUT;
    
    float3 Pos = float3(0,0,0);
    float3 Normal = float3(0,0,0);	
   
    float BlendWeightsArray[4] = (float[4])IN.BlendWeights;
    int   IndexArray[4]        = (int[4])IN.BlendIndices;
	    
	for (int iBone = 0; iBone < NumBones ; iBone++)
    {       
        Pos += mul( IN.position , WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
        Normal += mul(IN.normal , WorldMatrixArray[IndexArray[iBone]]) * BlendWeightsArray[iBone];
    }
        
	Normal = normalize(Normal);
	OUT.pos = mul( float4( Pos , 1.0f) , world*worldViewProj);	
	OUT.normal = mul( float4(Normal, 0) , world );	
	OUT.tex0 = IN.tex0;
	OUT.color = float4( 0, 0, 0, 0 );
	return OUT;
}

technique Technique0
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 SimpleVertex();
        PixelShader  = compile ps_2_0 SimplePixel();
        //ZEnable = false;
        AlphaBlendEnable = false;
		//CullMode = NONE;
		//FillMode = Both;
    }

	pass Pass1
	{
        VertexShader = compile vs_2_0 SolidVertex();
        PixelShader  = compile ps_2_0 SolidPixel();
        ZEnable = false;
        AlphaBlendEnable = false;
		//CullMode = NONE;
		FillMode = WireFrame;
	}
}

technique TechniqueSkin
{
    pass Pass0
    {
        VertexShader = compile vs_2_0 SkinVertex( 4 );
        PixelShader  = compile ps_2_0 TexPixel();
        ZEnable = true;
        AlphaBlendEnable = false;
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
