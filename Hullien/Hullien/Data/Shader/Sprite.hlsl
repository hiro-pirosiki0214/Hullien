//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_Texture : register(t0);
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState g_samLinear : register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global : register(b0)
{
    matrix g_mW; // ﾜｰﾙﾄﾞ行列.
    matrix g_mWVP; // ﾜｰﾙﾄﾞ行列.
    float4 g_Color; // カラー.
    float2 g_vUV; // UV座標.
    float g_fViewPortW; // ビューポート幅.
    float g_fViewPortH; // ビューポート高さ.
};

//構造体.
struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, g_mWVP);
    output.Tex = Tex;
	// UV座標をずらす.
    output.Tex.x += g_vUV.x;
    output.Tex.y += g_vUV.y;

    return output;
}


// 頂点シェーダ.
VS_OUTPUT VS_MainUI(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(Pos, g_mW);

	// スクリーン座標に合わせる計算,
    output.Pos.x = (output.Pos.x / g_fViewPortW) * 2.0f - 1.0f;
    output.Pos.y = 1.0f - (output.Pos.y / g_fViewPortH) * 2.0f;

    output.Tex = Tex;
	
	// UV座標をずらす.
    output.Tex.x += g_vUV.x;
    output.Tex.y += g_vUV.y;

    return output;
}

// ピクセルシェーダ.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    float4 color = g_Texture.Sample(g_samLinear, input.Tex);
    color.a *= g_Color.a;
    return color;
}