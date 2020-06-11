/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
*	Auther		: an.
**/
//定義.
#define MAX_BONE_MATRICES (255)

//グローバル.
Texture2D		g_Texture	: register( t0 );	//テクスチャーは レジスターt(n).
SamplerState	g_Sampler	: register( s0 );	//サンプラーはレジスターs(n).


//コンスタントバッファ(メッシュごと).
cbuffer per_mesh		: register( b0 )
{
	matrix g_mW;		//ワールド行列.
	matrix g_mWVP;		//ワールドから射影までの変換行列.
};
//コンスタントバッファ(マテリアルごと).
cbuffer per_material	: register( b1 )
{
	float4 g_vAmbient	= float4( 0, 0, 0, 0 );	//アンビエント光.
	float4 g_vDiffuse	= float4( 1, 0, 0, 0 );	//拡散反射(色）
	float4 g_vSpecular	= float4( 1, 1, 1, 1 );	//鏡面反射.
};
//コンスタントバッファ(フレームごと).
cbuffer per_frame		: register( b2 )
{
    float4 g_vCamPos; //ｶﾒﾗ位置.
    float4 g_vLightPos; //ﾗｲﾄ位置.
    float4 g_vLightDir; //ﾗｲﾄ方向.
    matrix g_mLightRot; //ﾗｲﾄ回転行列.
    float4 g_fIntensity; //ﾗｲﾄ強度(明るさ). ※xのみ使用する.
    float4 g_Color;     // 色.
};
//ボーンのポーズ行列が入る.
cbuffer per_bones		: register( b3 )
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
	float4 Pos		: POSITION;		//位置.  
	float3 Norm		: NORMAL;		//頂点法線.
	float2 Tex		: TEXCOORD;		//テクスチャー座標.
	uint4  Bones	: BONE_INDEX;	//ボーンのインデックス.
	float4 Weights	: BONE_WEIGHT;	//ボーンの重み.
};

//ピクセルシェーダーの入力（バーテックスバッファーの出力）　
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};


//指定した番号のボーンのポーズ行列を返す.
//サブ関数（バーテックスシェーダーで使用）.
matrix FetchBoneMatrix( uint iBone )
{
	return g_mConstBoneWorld[iBone];
}


//頂点をスキニング（ボーンにより移動）する.
//サブ関数（バーテックスシェーダーで使用）.
Skin SkinVert( VSSkinIn Input )
{
	Skin Output = (Skin)0;

	float4 Pos = float4(Input.Pos);
	float3 Norm = Input.Norm;
	//ボーン0.
	uint iBone	=Input.Bones.x;
	float fWeight= Input.Weights.x;
	matrix m	=  FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );
	//ボーン1.
	iBone	= Input.Bones.y;
	fWeight	= Input.Weights.y;
	m		= FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );
	//ボーン2.
	iBone	= Input.Bones.z;
	fWeight	= Input.Weights.z;
	m		= FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );
	//ボーン3.
	iBone	= Input.Bones.w;
	fWeight	= Input.Weights.w;
	m		= FetchBoneMatrix( iBone );
	Output.Pos	+= fWeight * mul( Pos, m );
	Output.Norm	+= fWeight * mul( Norm, (float3x3)m );

	return Output;
}

// バーテックスシェーダ.
VS_OUTPUT VS_Main(VSSkinIn input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert( input);

	output.Pos	= mul( vSkinned.Pos, g_mWVP );
    output.Normal = normalize(mul(vSkinned.Norm, (float3x3) g_mW));
	output.Tex	= input.Tex;
    output.Light = normalize(g_vLightDir).xyz;
    float3 PosWorld = mul(input.Pos, g_mW).xyz;
    output.EyeVector = normalize(g_vCamPos.xyz - PosWorld);

	return output;
}


// ピクセルシェーダ.
float4 PS_Main(VS_OUTPUT input) : SV_Target
{
	//環境光　①.
    float4 ambient = g_vAmbient;

	//拡散反射光 ②.
    float NL = saturate(dot(input.Normal, input.Light));
    float4 diffuse =
		(g_vDiffuse / 2 + g_Texture.Sample(g_Sampler, input.Tex) / 2) * NL;

	//鏡面反射光 ③.
    float3 reflect = normalize(2 * NL * input.Normal - input.Light);
    float4 specular =
		pow(saturate(dot(reflect, input.EyeVector)), 4) * g_vSpecular;

	//ﾌｫﾝﾓﾃﾞﾙ最終色　①②③の合計.
    float4 Color = ambient + diffuse + specular;

	//ﾗｲﾄ強度を反映.
    Color *= g_fIntensity.x * g_Color;
    Color.a = g_Color.a;
    
    return Color;
}
