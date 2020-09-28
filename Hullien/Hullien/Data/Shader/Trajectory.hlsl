//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_Texture : register(t0);
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState g_SamLinear : register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global : register(b0)
{
	matrix g_mWVP; // ﾜｰﾙﾄﾞ行列.
};

//構造体.
struct VS_OUTPUT
{
	float4 Pos : SV_Position;
	float2 Tex : TEXCOORD;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mWVP);
	output.Tex = Tex;

	return output;
}

// ピクセルシェーダ.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	float4 color = g_Texture.Sample( g_SamLinear, input.Tex );

	return color;
}