cbuffer per_mesh : register(b0)
{
	matrix g_LightWVP; // ライトのワールド、ビュー、プロジェクション.
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
};

//-----------------------------------------.
// スタティックメッシュ.
//-----------------------------------------.
// 頂点シェーダーの入力パラメータ.
struct VS_INPUT
{
	float4 Pos : POSITION;
	float4 Norm : NORMAL;
	float2 Tex : TEXCOORD;
};

// スタティックメッシュのメイン関数.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	
	// ライト行列と座標を計算.
	output.Pos = mul(input.Pos, g_LightWVP);
	
	return output;
}
//-----------------------------------------.

//-----------------------------------------.
// スキンメッシュ.
//-----------------------------------------.
//定義.
#define MAX_BONE_MATRICES (255)	// ボーン最大数.
// ボーンのポーズ行列が入る.
cbuffer per_bones : register(b1)
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
VS_OUTPUT VS_SkinMain(VSSkinIn input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert(input);

	// ライト行列と座標を計算.
	output.Pos = mul(vSkinned.Pos, g_LightWVP);
	
	return output;
}
//-----------------------------------------.

// 深度値を計算.
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

// ピクセルシェーダー.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	return input.Pos.z / input.Pos.w;
	return outRBGA(input.Pos.z / input.Pos.w);
}