Texture2D g_Texture			: register(t0);
SamplerState g_SamLinear	: register(s0);

// グローバル.
cbuffer global : register(b0)
{
	matrix g_mWVP;
};

struct VS_INPUT
{
	float4 Pos	: POSITION;
	float4 Norm : NORMAL; // 法線.
	float2 Tex	: TEXCOORD; // テクスチャ座標.
};
struct VS_OUTPUT
{
	float4 Pos	: SV_Position;	// WVPでの座標.
	float2 Tex	: TEXCOORD0;	// ワールド座標.
	float  Height : TEXCOORD1;	// テクスチャ座標.
};

// 頂点シェーダー.
VS_OUTPUT VS_Main( VS_INPUT input )
{
	VS_OUTPUT Out	= (VS_OUTPUT) 0;
	input.Pos.w		= 1.0f;
	Out.Pos			= mul(input.Pos, g_mWVP);
	Out.Height = input.Pos.y <= 0.0f ? 0.0f : input.Pos.y;
	Out.Tex			= input.Tex;
	return Out;
}

// ピクセルシェーダー.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);
	color = lerp(color, float4(1.0f, 1.0f, 1.0f, 1.0f), input.Height);
	return color;
}