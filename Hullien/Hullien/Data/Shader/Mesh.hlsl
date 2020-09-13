//-------------------------------------------------.
// グローバル変数.
//-------------------------------------------------.
// テクスチャ.
Texture2D		g_Texture	: register(t0);	// モデルのテクスチャ.
Texture2D		g_ToonMap	: register(t1);	// toonシェーダー用のテクスチャ.
// サンプラ.
SamplerState	g_SamLinear	: register(s0);

//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
// メッシュ単位.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;	// ワールド行列.
	matrix	g_mWVP;	// ワールド,ビュー,プロジェクションの合成行列.
};
// マテリアル単位.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		// アンビエント色(環境色).
	float4	g_vDiffuse;		// ディヒューズ色(拡散反射色).
	float4	g_vSpecular;	// スペキュラ色(鏡面反射色).
};
// フレーム単位.
cbuffer per_frame : register(b2)
{
    float4 g_vCamPos;		// カメラ位置.
    float4 g_vLightPos;		// ライト位置.
    float4 g_vLightDir;		// ライト方向.
    matrix g_mLightRot;		// ライト回転行列.
    float4 g_fIntensity;	// ライト強度(明るさ). ※xのみ使用する.
    float4 g_Color;			// 色.
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点インプット.
struct VS_INPUT
{
	float4 Pos : POSITION;	// 座標.
	float4 Norm: NORMAL;	// 法線.
	float2 Tex : TEXCOORD;	// テクスチャ座標.
};
// 頂点アウトプット.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;	// WVPでの座標.
	float4	PosW		: Position;		// Wでの座標.
	float3	LightDir	: TEXCOORD0;	// ライト方向.
	float3	Normal		: TEXCOORD1;	// 法線.
	float3	EyeVector	: TEXCOORD2;	// 視点ベクトル.
	float2	Tex			: TEXCOORD3;	// テクスチャ座標.
};

//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main( VS_INPUT input )
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;

    Out.Pos		= mul( input.Pos, g_mWVP );	// WVP座標.
	Out.PosW	= mul( input.Pos, g_mW );	// world座標.
    Out.Normal	= mul( input.Norm.xyz, (float3x3) g_mW );		// 法線.
    Out.LightDir	= normalize( g_vLightDir ).xyz;				// ライト方向.
    Out.EyeVector	= normalize( g_vCamPos - Out.PosW ).xyz;	// 視点ベクトル.
    Out.Tex			= input.Tex;	//　テクスチャ座標.

    return Out;
}

//-------------------------------------------------.
//	ピクセルシェーダー.
//-------------------------------------------------.
float4 PS_Main( VS_OUTPUT input ) : SV_Target
{
	// モデルのテクスチャ色を取得.
    float4 color = g_Texture.Sample( g_SamLinear, input.Tex );
	
	//-----トゥーン処理------.
	// ハーフランバート拡散照明によるライティング計算
    float p = dot( input.Normal, input.LightDir );
	p = p * 0.5f + 0.5f;
	p = p * p;
	// 計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
	float4 toonColor = g_ToonMap.Sample( g_SamLinear, float2( p, 0.0f ) );
    color *= toonColor * g_fIntensity.x;
	
	//-----高さフォグ処理------.
	const float4 fogColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float4 fogTColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float minHeight = -5.0f;
	const float maxHeight = 20.0f;
	float alpha = clamp((input.PosW.y - minHeight) / (maxHeight - minHeight), 0.0f, 1.0f );
	float4 alphas = 1.0f - ( 1.0f - alpha ) * fogTColor;

	color = color * alphas + fogColor * ( 1.0f - alpha );
	color *= g_Color;
	
	return color;
}

//========= テクスチャ無し用 ========================================
//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

    Out.Pos		= mul( Pos, g_mWVP );	// WVP座標.
	Out.PosW	= mul( Pos, g_mW );		// world座標.
    Out.Normal	= mul( Norm.xyz, (float3x3) g_mW );				// 法線.
    Out.LightDir	= normalize( g_vLightDir ).xyz;				// ライト方向.
    Out.EyeVector	= normalize( g_vCamPos - Out.PosW ).xyz;	// 視点ベクトル.

    return Out;
}

//-------------------------------------------------.
//	ピクセルシェーダー.
//-------------------------------------------------.
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	// モデルのテクスチャ色を取得.
    float4 color = float4( 1.0f, 1.0f, 1.0f, 1.0f );
	
	//-----トゥーン処理------.
	// ハーフランバート拡散照明によるライティング計算
    float p = dot( input.Normal, input.LightDir );
	p = p * 0.5f + 0.5f;
	p = p * p;
	// 計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
	float4 toonColor = g_ToonMap.Sample( g_SamLinear, float2( p, 0.0f ) );
    color *= toonColor * g_fIntensity.x;
	
	//-----高さフォグ処理------.
	const float4 fogColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float4 fogTColor = float4( 0.5f, 0.5f, 0.5f, 1.0f );
	const float minHeight = -5.0f;
	const float maxHeight = 20.0f;
	float alpha = clamp((input.PosW.y - minHeight) / (maxHeight - minHeight), 0.0f, 1.0f );
	float4 alphas = 1.0f - ( 1.0f - alpha ) * fogTColor;

	color = color * alphas + fogColor * ( 1.0f - alpha );
	color *= g_Color;
	
	return color;
}