#include"BasicType.hlsli"

cbuffer cbuff0 : register(b0) {
	matrix mat;
    float elapsedTime;
    float2 resolution;
}

BasicType main(float4 pos : POSITION, float2 uv : TEXCOORD)
{
	BasicType output;
    output.position = mul(mat, pos); // HLSL‚Å‚Í—ñ—Dæ
	output.uv = uv;
    output.elapsedTime = elapsedTime;
    output.resolution = resolution;
	return output;
}