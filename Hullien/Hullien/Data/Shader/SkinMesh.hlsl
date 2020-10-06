/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
*	Auther		: an.
**/
//定義.
#define MAX_BONE_MATRICES (255)

//-------------------------------------------------.
// グローバル変数.
//-------------------------------------------------.
// テクスチャ.
Texture2D g_Texture		: register(t0); //テクスチャーは レジスターt(n).
Texture2D g_ShadowMap1	: register(t1);	// シャドウテクスチャ.
Texture2D g_ShadowMap2	: register(t2);	// シャドウテクスチャ.
Texture2D g_ShadowMap3	: register(t3);	// シャドウテクスチャ.
Texture2D g_ShadowMap4	: register(t4);	// シャドウテクスチャ.
Texture2D g_ToonMap		: register(t5); // toonシェーダー用のテクスチャ.
Texture2D g_FogTexture	: register(t6); // フォグ用のテクスチャ.
// サンプラ.
SamplerState g_SamLinear		: register(s0); //サンプラーはレジスターs(n).
SamplerState g_ShadowSamLinear	: register(s1);

struct FOG
{
	float4  FogTex;		// フォグのテクスチャ座標.
	float4  FogColor;		// フォグの色.
	float	MinHeight;	// フォグの最小高さ.
	float	MaxHeight;	// フォグの最大高さ.
	float	TexScale;		// フォグのテクセルスケール.
};

//コンスタントバッファ(メッシュごと).
cbuffer per_mesh : register(b0)
{
	matrix g_mW; //ワールド行列.
	matrix g_mWVP; //ワールドから射影までの変換行列.
};
//コンスタントバッファ(マテリアルごと).
cbuffer per_material : register(b1)
{
	float4 g_vAmbient = float4(0, 0, 0, 0); //アンビエント光.
	float4 g_vDiffuse = float4(1, 0, 0, 0); //拡散反射(色）
	float4 g_vSpecular = float4(1, 1, 1, 1); //鏡面反射.
};
//コンスタントバッファ(フレームごと).
cbuffer per_frame : register(b2)
{
	float4 g_vCamPos; //ｶﾒﾗ位置.
	float4 g_vLightPos; //ﾗｲﾄ位置.
	float4 g_vLightDir; //ﾗｲﾄ方向.
	matrix g_mLightRot; //ﾗｲﾄ回転行列.
	float4 g_fIntensity; //ﾗｲﾄ強度(明るさ). ※xのみ使用する.
	float4 g_Color; // 色.
	matrix g_LightWVP[4];
	float4 g_SpritPos;
	float4 g_IsShadow;
	FOG	g_Fog;
};
//ボーンのポーズ行列が入る.
cbuffer per_bones : register(b3)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//スキニング後の頂点・法線が入る.
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//バーテックスバッファーの入力.
struct VSSkinIn
{
	float4 Pos : POSITION; //位置.  
	float3 Norm : NORMAL; //頂点法線.
	float2 Tex : TEXCOORD; //テクスチャー座標.
	uint4 Bones : BONE_INDEX; //ボーンのインデックス.
	float4 Weights : BONE_WEIGHT; //ボーンの重み.
};

//ピクセルシェーダーの入力（バーテックスバッファーの出力）　
struct VS_OUTPUT
{
	float4 Pos : SV_Position; // WVPでの座標.
	float4 PosW : Position; // Wでの座標.
	float3 LightDir : TEXCOORD0; // ライト方向.
	float3 Normal : TEXCOORD1; // 法線.
	float3 EyeVector : TEXCOORD2; // 視点ベクトル.
	float2 Tex : TEXCOORD3; // テクスチャ座標.
	float4 ZDepth[4] : TEXCOORD4;
	float SpritPos[4] : TEXCOORD8;
};

//指定した番号のボーンのポーズ行列を返す.
//サブ関数（バーテックスシェーダーで使用）.
matrix FetchBoneMatrix(uint iBone)
{
	return g_mConstBoneWorld[iBone];
}

//頂点をスキニング（ボーンにより移動）する.
//サブ関数（バーテックスシェーダーで使用）.
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin) 0;

	float4 Pos = float4(Input.Pos);
	float3 Norm = Input.Norm;
	//ボーン0.
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//ボーン1.
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//ボーン2.
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//ボーン3.
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);

	return Output;
}

// バーテックスシェーダ.
VS_OUTPUT VS_Main(VSSkinIn input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert(input);
	
	output.Pos = mul(vSkinned.Pos, g_mWVP); // WVP座標.
	output.PosW = mul(vSkinned.Pos, g_mW); // world座標.
	output.Normal = normalize(mul(vSkinned.Norm.xyz, (float3x3) g_mW)); // 法線.
	output.LightDir = normalize(g_vLightDir).xyz; // ライト方向.
	output.EyeVector = normalize(g_vCamPos - output.PosW).xyz; // 視点ベクトル.
	output.Tex = input.Tex; //　テクスチャ座標.

	output.ZDepth[0] = mul(vSkinned.Pos, g_LightWVP[0]);
	output.ZDepth[1] = mul(vSkinned.Pos, g_LightWVP[1]);
	output.ZDepth[2] = mul(vSkinned.Pos, g_LightWVP[2]);
	output.ZDepth[3] = mul(vSkinned.Pos, g_LightWVP[3]);
	output.SpritPos[0] = g_SpritPos.x;
	output.SpritPos[1] = g_SpritPos.y;
	output.SpritPos[2] = g_SpritPos.z;
	output.SpritPos[3] = g_SpritPos.w;
	
	return output;
}

struct PS_OUTPUT
{
	float4 Color : SV_Target0;
	float4 Normal : SV_Target1;
	float4 ZDepth : SV_Target2;
};

float4 outRBGA(float depth)
{
	// R成分抽出
	float R = round(depth * 256.0f) / 256.0f;
	// G成分抽出
	float Def = depth - R;
	float G = round(Def * 65536.0f) / 256.0f;
	// B成分抽出
	Def -= G / 256.0f;
	float B = round(Def * 65536.0f * 256.0f) / 256.0f;
	// A成分抽出
	Def -= B / 65536.0f;
	float A = round(Def * 65536.0f * 65536.0f) / 256.0f;
	
	return float4(R, G, B, A);
}

// ピクセルシェーダ.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// モデルのテクスチャ色を取得.
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);
	color *= g_Color;
	
	// 各ピクセル位置までの距離.
	float dist = input.Pos.w; // ビュー空間でのZ座標.
	float shadowColor = 1.0f;
	float zValue = 0.0f; // 深度値.
	if (dist < input.SpritPos[0])
	{
		zValue = input.ZDepth[0].z / input.ZDepth[0].w;
		float2 cord;
		cord.x = (1.0f + input.ZDepth[0].x / input.ZDepth[0].w) / 2.0f;
		cord.y = (1.0f - input.ZDepth[0].y / input.ZDepth[0].w) / 2.0f;
		float4 depthColor = g_ShadowMap1.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos[1])
	{
		zValue = input.ZDepth[1].z / input.ZDepth[1].w;
		float2 cord;
		cord.x = (1.0f + input.ZDepth[1].x / input.ZDepth[1].w) / 2.0f;
		cord.y = (1.0f - input.ZDepth[1].y / input.ZDepth[1].w) / 2.0f;
		float4 depthColor = g_ShadowMap2.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos[2])
	{
		zValue = input.ZDepth[2].z / input.ZDepth[2].w;
		float2 cord;
		cord.x = (1.0f + input.ZDepth[2].x / input.ZDepth[2].w) / 2.0f;
		cord.y = (1.0f - input.ZDepth[2].y / input.ZDepth[2].w) / 2.0f;
		float4 depthColor = g_ShadowMap3.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos[3])
	{
		zValue = input.ZDepth[3].z / input.ZDepth[3].w;
		float2 cord;
		cord.x = (1.0f + input.ZDepth[3].x / input.ZDepth[3].w) / 2.0f;
		cord.y = (1.0f - input.ZDepth[3].y / input.ZDepth[3].w) / 2.0f;
		float4 depthColor = g_ShadowMap4.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm) ? 0.5f : 1.0f;
	}
	if (g_IsShadow.x >= 1.0f) color.xyz *= shadowColor;
	
	//-----トゥーン処理------.
	// ハーフランバート拡散照明によるライティング計算
	float p = dot(input.Normal, input.LightDir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	// 計算結果よりトゥーンシェーダー用のテクスチャから色をフェッチする
	float4 toonColor = g_ToonMap.Sample(g_SamLinear, float2(p, 0.0f));
	color *= toonColor * g_fIntensity.x;
	
	//-----高さフォグ処理------.
	// fogテクスチャの座標を取得、計算.
	const float fogColor =
		(g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.xy).r +
		 g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.zw).r) * 0.5f;
	float alpha = clamp((input.PosW.y - g_Fog.MinHeight) / (g_Fog.MaxHeight - g_Fog.MinHeight), 0.0f, 1.0f);
	float alphas = 1.0f - (1.0f - alpha) * fogColor;

	color = color * alphas + g_Fog.FogColor * (1.0f - alphas);
	
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color = color;
	output.Normal = float4(input.Normal, 1.0f);
	output.ZDepth = outRBGA(zValue);
	return output;
}
