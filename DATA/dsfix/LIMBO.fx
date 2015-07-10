texture2D frameTex2D;

sampler frameSampler = sampler_state
{
	texture = <frameTex2D>;
	AddressU = CLAMP;
	AddressV = CLAMP;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VSOUT
{
	float4 vertPos : POSITION;
	float2 UVCoord : TEXCOORD0;
};

struct VSIN
{
	float4 vertPos : POSITION0;
	float2 UVCoord : TEXCOORD0;
};

VSOUT FrameVS(VSIN IN)
{
	VSOUT OUT;
	OUT.vertPos = IN.vertPos;
	OUT.UVCoord = IN.UVCoord;
	return OUT;
}

float4 ScaleDepth(VSOUT IN) : COLOR0
{
	float4 color = tex2D(frameSampler, IN.UVCoord);
	float posZ = ((color.z) + (color.y)*255.0 + (color.x)*(255.0*256.0)) / (256.0*256.0);
  float depth = (2.0 * ZNEAR) / ( -(ZFAR - ZNEAR) * posZ + (ZFAR + ZNEAR) );	
  
	return float4(depth, depth, depth, 1.0);
}

technique t0
{
	pass P0
	{
		VertexShader = compile vs_3_0 FrameVS();
		PixelShader = compile ps_3_0 ScaleDepth();
        ZEnable = false;
        AlphaBlendEnable = false;
	}
}
