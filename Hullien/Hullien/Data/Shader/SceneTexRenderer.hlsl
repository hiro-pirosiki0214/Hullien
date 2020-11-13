#include "FXAA.hlsl"

//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_TextureColor	: register(t0);	// 色情報.
Texture2D g_TextureNormal	: register(t1);	// 法線情報.
Texture2D g_TextureDepth	: register(t2);	// 深度情報.
Texture2D g_TextureTrans	: register(t3);	// アルファ情報.
Texture2D g_TextureLast		: register(t4);	// 輪郭線などを描画したテクスチャ.
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState g_SamLinear : register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer cBuffer : register(b0)
{
	matrix g_mW			: packoffset(c0); // ﾜｰﾙﾄﾞ行列.
	float2 g_vViewPort	: packoffset(c4); // ビューポート幅.
};

//構造体.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;
	float4 Color		: COLOR;
	float2 Tex			: TEXCOORD0;
	float4 OutLineColor : TEXCOORD1;
};

VS_OUTPUT VS_Main(
	float4 Pos : POSITION,
	float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	output.Pos = mul(Pos, g_mW);

	// スクリーン座標に合わせる計算,
	output.Pos.x = (output.Pos.x / g_vViewPort.x) * 2.0f - 1.0f;
	output.Pos.y = 1.0f - (output.Pos.y / g_vViewPort.y) * 2.0f;

	output.Tex = Tex;
//	output.OutLineColor = float4(output.Pos.x + 0.7f, 0.0f, output.Pos.y + 0.7f, 1.0f);
	output.OutLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	
	return output;
}

// ピクセルシェーダ.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	// 魚眼レンズ.
	/*
	const float f = 1.0f;	// 強さ.
	float2 pos = input.Tex * 2.0f - 1.0f;
	float p = length(pos);
	pos = (1 + f * p * p) / (1 + 2 * f) * pos;
	float4 color = g_TextureColor.Sample(g_SamLinear, pos*0.5+0.5);
	*/
	
	// 描画されたモデルなどの色情報を取得.
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	//----------------------------------------------------------------.
	// 輪郭線.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	// テクスチャのサイズを取得.
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	const float s = 0.25f;	// サンプリングする強さ.
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	const float px = s * dx, py = s * dx;
	
	//----------------------------------------------------------------.
	// 法線の情報を取得.
	float3 normColor = float3(0.0f, 0.0f, 0.0f);
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).xyz; // 左上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).xyz; // 上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).xyz; // 右上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).xyz; // 左.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px, 0 * py)).xyz * -8.0; // 自分.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).xyz; // 右.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).xyz; // 左下.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).xyz; // 下.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).xyz; // 右下.
	
	//----------------------------------------------------------------.
	// 深度値の情報を取得.
	float z = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(0 * dx, 0 * dy)).x; // 自分.
	float depthColor11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	 py)).x; // 左上.
	float depthColor10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px, 0 * py)).x; // 上.
	float depthColor1_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	 px,	-py)).x; // 右上.
	float depthColor01		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	 py)).x; // 左.
	float depthColor0_1		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * px,	-py)).x; // 右.
	float depthColor_11		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	 py)).x; // 左下.
	float depthColor_10		= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px, 0 * py)).x; // 下.
	float depthColor_1_1	= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(	-px,	-py)).x; // 右下.
	
	// Horizontal.
	float h = (depthColor11	 *  1.0f + depthColor10	  *  2.0f)
			+ (depthColor1_1 *  1.0f + depthColor_11  * -1.0f)
			+ (depthColor_10 * -2.0f + depthColor_1_1 * -1.0f);
	// vertical.
	float v = (depthColor11	 *  1.0f + depthColor01	  *  2.0f)
			+ (depthColor_11 *  1.0f + depthColor1_1  * -1.0f)
			+ (depthColor0_1 * -2.0f + depthColor_1_1 * -1.0f);
	float depth = 1.0f - clamp(abs(h + v), 0.0f, 1.0f);
	
	//----------------------------------------------------------------.
	// エフェクト(パーティクル)を描画したテクスチャの情報を取得.
	float4 transColor = g_TextureTrans.Sample(g_SamLinear, input.Tex);
	// グレースケール化.
	float4 grayScale = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	grayScale = 1.0f - saturate(grayScale);
	
	// 法線情報と、深度値の情報が一定以上なら輪郭線を表示.
	if (length(normColor) >= 0.72f || abs(depth) < 0.8f)
	{
		grayScale *= 1.0f;
	}
	else
	{
		grayScale *= 0.0f;
	}
	return lerp( color, input.OutLineColor, grayScale);
}

// ピクセルシェーダ.
float4 PS_LastMain(VS_OUTPUT input) : SV_Target
{
	FxaaTex tex = { g_SamLinear, g_TextureLast };
	return float4( FxaaPixelShader( input.Tex, tex, float2( 1.0f/g_vViewPort.x, 1.0f/g_vViewPort.y ) ), 1.0f );
}