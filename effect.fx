
texture texture0;
texture noise_texture;
texture ground;
float4x4 worldViewProj : WorldViewProjection;
float4x4 world;
float4x4 viewMat;
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

sampler NoiseSampler
{
	Filter = NONE;

    Texture = noise_texture;    

    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    AddressU = CLAMP;
    AddressV = CLAMP;
    AddressW = CLAMP;            
};

sampler GroundSampler
{
	Filter = NONE;

    Texture = ground;    

    MipFilter = NONE;
    MinFilter = NONE;
    MagFilter = NONE;
    AddressU = WRAP;
    AddressV = WRAP;
    AddressW = WRAP;            
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

VS_OUTPUT FlatVertex ( VS_INPUT IN )
{
	VS_OUTPUT OUT;
	OUT.pos.x = IN.position.x;//(IN.position.x-0.5)*2;
	OUT.pos.y = IN.position.y;//(IN.position.y-0.5)*2;
	//OUT.pos.x = IN.position.x / 4.0 + 0.75;
	//OUT.pos.y = IN.position.y / 4.0 + 0.75;
	OUT.pos.z = IN.position.z;//0.0;
	OUT.pos.w = 1.f;
	OUT.normal = IN.normal;
	OUT.tex0 = IN.tex0;
	OUT.color = IN.color;
	return OUT;
}

float4 FlatPixel( VS_OUTPUT IN ) : COLOR
{
	return IN.color;
}

float4 NoisePixel( VS_OUTPUT IN ) : COLOR
{
	//return IN.color;
	float4 texColor = tex2D( NoiseSampler , IN.tex0 );
	return texColor;
}

VS_OUTPUT SimpleVertex( VS_INPUT IN )
{
    VS_OUTPUT OUT;
	
	OUT.pos = mul( float4(IN.position, 1) , mul( world, worldViewProj ) );
	OUT.normal = normalize( mul( float4( IN.normal, 0 ), world ) ).xyz;
	OUT.tex0 = IN.tex0;
	//OUT.color = IN.color;

	
	float3 lightpos = float3( 15.0, 100.0, 0.0 );
	float3 diff = lightpos - IN.position;
	diff = normalize( diff );
	float scale = max( dot( diff, OUT.normal ), 0.1f );
	
	/*
	float3 lightvec = float3( 1.0, 0.5, 1.5 );
	normalize( lightvec );
	float scale = max( dot( lightvec, OUT.normal ), 0.1f );
	*/

	OUT.color = IN.color * scale;
	OUT.color.a = 1.0;

	return OUT;
}

float4 SimplePixel( VS_OUTPUT IN ) : COLOR
{
	return IN.color;
	//return float4( 1.0, 0.0, 0.0, 1.0 );

	/*
	float offset;
	float2 tcoord;
	offset = abs( min( 0.f, IN.tex0.y ) );
	tcoord.x = IN.tex0.x;
	tcoord.y = fmod( IN.tex0.y + 0.5, 0.5 ) - offset + 0.5;


	float4 texColor = tex2D( GroundSampler , tcoord );
	texColor.x *= IN.color.x;
	texColor.xyz *= IN.color.y;
	return texColor;
	return IN.color;
	float mag = dot( IN.normal.xyz, float3( 1.0, 1.0, 1.0 ) );
	mag = max( mag, 0.2 );
	return IN.color * mag * 1.5;
	*/
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
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;
		//CullMode = NONE;
		//FillMode = WireFrame;
		//ZFunc = LESSEQUAL;
    }
}

technique Flat
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 FlatVertex();
		PixelShader = compile ps_2_0 FlatPixel();
		CullMode = NONE;
		//FillMode = WireFrame;
	}
}

technique Noise
{
	pass Pass0
	{
		VertexShader = compile vs_2_0 FlatVertex();
		PixelShader = compile ps_2_0 NoisePixel();
		//CullMode = NONE;
		//FillMode = WireFrame;
		ZFunc = ALWAYS;
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
		CullMode = NONE;
		FillMode = WireFrame;
    }
}
