#include "FXAA.hlsl"

//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_TextureColor	: register(t0);
Texture2D g_TextureNormal	: register(t1);
Texture2D g_TextureDepth	: register(t2);
Texture2D g_TextureTrans	: register(t3);
Texture2D g_TextureLast		: register(t4);
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
	float4 Pos		: SV_Position;
	float4 Color	: COLOR;
	float2 Tex		: TEXCOORD;
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
	float4 color = g_TextureColor.Sample(g_samLinear, pos*0.5+0.5);
	*/
	
	// 描画されたモデルなどの色情報を取得.
	float4 color = g_TextureColor.Sample(g_SamLinear, input.Tex);
	
	//----------------------------------------------------------------.
	// 輪郭線.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	// テクスチャのサイズを取得.
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	const float s = 0.2f;	// サンプリングする強さ.
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	
	//----------------------------------------------------------------.
	// 法線の情報を取得.
	float3 normColor = float3(0.0f, 0.0f, 0.0f);
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(-s * dx, -s * dy)).xyz;			// 左上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * dx, -s * dy)).xyz;			// 上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( s * dx, -s * dy)).xyz;			// 右上.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(-s * dx,  0 * dy)).xyz;			// 左.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * dx,  0 * dy)).xyz * -8.0;	// 自分.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( s * dx,  0 * dy)).xyz;			// 右.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2(-s * dx,  s * dy)).xyz;			// 左下.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( 0 * dx,  s * dy)).xyz;			// 下.
	normColor += g_TextureNormal.Sample(g_SamLinear, input.Tex + float2( s * dx,  s * dy)).xyz;			// 右下.
	
	//----------------------------------------------------------------.
	// 深度値の情報を取得.
	float4 zColor = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(0 * dx, 0 * dy)); // 自分.
	float z = zColor.r + (zColor.g + (zColor.b + zColor.a / 256.0f) / 256.0f) / 256.0f;
	
	float4 depthColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	depthColor	 = g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(-s * dx, -s * dy)); // 左上.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * dx, -s * dy)); // 上.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( s * dx, -s * dy)); // 右上.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(-s * dx,  0 * dy)); // 左.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( s * dx,  0 * dy)); // 右.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2(-s * dx,  s * dy)); // 左下.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( 0 * dx,  s * dy)); // 下.
	depthColor	+= g_TextureDepth.Sample(g_SamLinear, input.Tex + float2( s * dx,  s * dy)); // 右下.
	float depth = 0.0f;
	depth = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
	depth /= 8.0f;	// 深度値の平均を計算.
	
	//----------------------------------------------------------------.
	// エフェクト(パーティクル)を描画したテクスチャの情報を取得.
	float4 transColor = g_TextureTrans.Sample(g_SamLinear, input.Tex);
	// グレースケール化.
	float grayScale = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	grayScale = 1.0f - saturate(grayScale);
	
	float4 outLineColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
	// 法線情報と、深度値の情報が一定以上なら輪郭線を表示.
	if (length(normColor) >= 0.62f || abs(depth-z) > 0.00097f)
	{
		outLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		grayScale *= 0.0f;
	}
	else
	{
		outLineColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		grayScale *= 1.0f;
	}
	color *= lerp(color, outLineColor, grayScale);
	
	return color;
}

// ピクセルシェーダ.
float4 PS_LastMain(VS_OUTPUT input) : SV_Target
{
	FxaaTex tex = { g_SamLinear, g_TextureLast };
	return float4( FxaaPixelShader( input.Tex, tex, float2( 1.0f/g_vViewPort.x, 1.0f/g_vViewPort.y ) ), 1.0f );
}