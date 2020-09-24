//ｸﾞﾛｰﾊﾞﾙ変数.
//ﾃｸｽﾁｬは、ﾚｼﾞｽﾀ t(n).
Texture2D g_TextureColor	: register(t0);
Texture2D g_TextureNormal	: register(t1);
Texture2D g_TextureDepth	: register(t2);
Texture2D g_TextureTrans	: register(t3);
//ｻﾝﾌﾟﾗは、ﾚｼﾞｽﾀ s(n).
SamplerState g_samLinear : register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer global : register(b0)
{
	matrix g_mW			: packoffset(c0); // ﾜｰﾙﾄﾞ行列.
	float2 g_vViewPort	: packoffset(c4); // ビューポート幅.
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
	float4 color = g_TextureColor.Sample(g_samLinear, input.Tex);
	
	float4 transColor =  g_TextureTrans.Sample(g_samLinear, input.Tex);
	float4 gray = transColor.r * 0.299 + transColor.g * 0.587 + transColor.b * 0.114;
	gray = 1.0f - saturate(gray);
	gray.a = 1.0f;
	
	
	//----------------------------------------------------------------.
	// 輪郭線.
	//----------------------------------------------------------------.
	float imageSizeW, imageSizeh, levels;
	g_TextureNormal.GetDimensions(0, imageSizeW, imageSizeh, levels);
	
	const float s = 0.2f;
	const float dx = 1.0f / imageSizeW;
	const float dy = 1.0f / imageSizeh;
	
	// 法線の情報を取得.
	float3 norm = float3(0.0f, 0.0f, 0.0f);
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(-s * dx, -s * dy)).xyz; // 左上.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(0 * dx, -s * dy)).xyz; // 上.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(s * dx, -s * dy)).xyz; // 右上.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(-s * dx, 0 * dy)).xyz; // 左.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(0 * dx, 0 * dy)).xyz * -8.0; // 自分.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(s * dx, 0 * dy)).xyz; // 右.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(-s * dx, s * dy)).xyz; // 左下.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(0 * dx, s * dy)).xyz; // 下.
	norm += g_TextureNormal.Sample(g_samLinear, input.Tex + float2(s * dx, s * dy)).xyz; // 右下.
	
	// 深度値の情報を取得.
	float4 z4 = g_TextureDepth.Sample(g_samLinear, input.Tex + float2(0 * dx, 0 * dy)); // 自分.
	float z = z4.r + (z4.g + (z4.b + z4.a / 256.0f) / 256.0f) / 256.0f;
	
	float4 depth4 = g_TextureDepth.Sample(g_samLinear, input.Tex + float2(-s * dx, -s * dy)); // 左上.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(0 * dx, -s * dy)); // 上.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(s * dx, -s * dy)); // 右上.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(-s * dx, 0 * dy)); // 左.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(s * dx, 0 * dy)); // 右.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(-s * dx, s * dy)); // 左下.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(0 * dx, s * dy)); // 下.
	depth4 += g_TextureDepth.Sample(g_samLinear, input.Tex + float2(s * dx, s * dy)); // 右下.
	float depth = 0.0f;
	depth = depth4.r + (depth4.g + (depth4.b + depth4.a / 256.0f) / 256.0f) / 256.0f;
	depth /= 8.0f;
	
	float4 outLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	if (length(norm) >= 0.58f || abs(z - depth) > 0.0011f )
	{
		outLineColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
		gray *= 0.0f;
	}
	else
	{
		outLineColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
		gray *= 1.0f;
	}
	color = lerp(color, outLineColor, gray);
	
	return color;
}