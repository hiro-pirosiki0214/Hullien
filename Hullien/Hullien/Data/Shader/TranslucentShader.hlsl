//-------------------------------------------------.
// コンスタントバッファ.
//-------------------------------------------------.
// メッシュ単位.
cbuffer per_mesh : register(b0)
{
	matrix g_mWVP; // ワールド,ビュー,プロジェクションの合成行列.
};
// フレーム単位.
cbuffer per_frame : register(b2)
{
	float4 g_Color; // 色.
};

//-------------------------------------------------.
// 構造体.
//-------------------------------------------------.
// 頂点アウトプット.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;	// WVPでの座標.
};

//-----------------------------------------.
// スタティックメッシュ.
//-----------------------------------------.
// 頂点インプット.
struct VS_INPUT
{
	float4 Pos	: POSITION; // 座標.
	float4 Norm	: NORMAL; // 法線.
	float2 Tex	: TEXCOORD; // テクスチャ座標.
};
//-------------------------------------------------.
//	頂点シェーダー.
//-------------------------------------------------.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

	Out.Pos = mul(input.Pos, g_mWVP); // WVP座標.
	
	return Out;
}
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
	output.Pos = mul(vSkinned.Pos, g_mWVP);
	return output;
}
//-----------------------------------------.

// ピクセルシェーダー.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	return float4( 0.0f, 0.0f, 0.0f, 1.0f );
}