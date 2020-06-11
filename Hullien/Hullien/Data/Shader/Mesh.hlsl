//ｸﾞﾛｰﾊﾞﾙ変数.

//ﾃｸｽﾁｬは ﾚｼﾞｽﾀ t(n).
Texture2D		g_Texture	: register(t0);
//ｻﾝﾌﾟﾗは ﾚｼﾞｽﾀ s(n).
SamplerState	g_SamLinear	: register(s0);

//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer per_mesh : register(b0)
{
	matrix	g_mW;			//ﾜｰﾙﾄﾞ行列.
	matrix	g_mWVP;			//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝの合成行列.
};
//ﾏﾃﾘｱﾙ単位.
cbuffer per_material : register(b1)
{
	float4	g_vAmbient;		//ｱﾝﾋﾞｴﾝﾄ色(環境色).
	float4	g_vDiffuse;		//ﾃﾞｨﾌｭｰｽﾞ色(拡散反射色).
	float4	g_vSpecular;	//ｽﾍﾟｷｭﾗ色(鏡面反射色).
};
//ﾌﾚｰﾑ単位.
cbuffer per_frame : register(b2)
{
    float4 g_vCamPos; //ｶﾒﾗ位置.
    float4 g_vLightPos; //ﾗｲﾄ位置.
    float4 g_vLightDir; //ﾗｲﾄ方向.
    matrix g_mLightRot; //ﾗｲﾄ回転行列.
    float4 g_fIntensity; //ﾗｲﾄ強度(明るさ). ※xのみ使用する.
    float4 g_Color; // 色.
};

//頂点ｼｪｰﾀﾞの出力ﾊﾟﾗﾒｰﾀ.
struct VS_OUTPUT
{
	float4	Pos			: SV_Position;
	float4	Color		: COLOR;
	float3	Light		: TEXCOORD0;
	float3	Normal		: TEXCOORD1;
	float3	EyeVector	: TEXCOORD2;
	float2	Tex			: TEXCOORD3;
};

//-------------------------------------------------
//	頂点(ﾊﾞｰﾃｯｸｽ)ｼｪｰﾀﾞ.
//-------------------------------------------------
VS_OUTPUT VS_Main(float4 Pos : POSITION,
				  float4 Norm: NORMAL,
				  float2 Tex : TEXCOORD)
{
    VS_OUTPUT Out = (VS_OUTPUT) 0;

	//ﾌﾟﾛｼﾞｪｸｼｮﾝ変換(ﾜｰﾙﾄﾞ→ﾋﾞｭｰ→ﾌﾟﾛｼﾞｪｸｼｮﾝ).
    Out.Pos = mul(Pos, g_mWVP);

	//法線をﾓﾃﾞﾙの姿勢に合わせる.
	// (ﾓﾃﾞﾙが回転すれば法線も回転させる必要があるため).
    Out.Normal = mul(Norm.xyz, (float3x3) g_mW);

	//ﾗｲﾄ方向:
	// ﾃﾞｨﾚｸｼｮﾅﾙﾗｲﾄは、どこでも同じ方向.位置は無関係.
    Out.Light = normalize(g_vLightDir).xyz;

    float3 PosWorld = mul(Pos, g_mW).xyz;

	//視線ﾍﾞｸﾄﾙ:
	// ﾜｰﾙﾄﾞ空間上での頂点から頂点へ向かうﾍﾞｸﾄﾙ.
    Out.EyeVector = normalize(g_vCamPos.xyz - PosWorld);

	//ﾃｸｽﾁｬ座標.
    Out.Tex = Tex;

    return Out;
}

//*************************************************
//	ﾗｽﾀﾗｲｻﾞ(ｱﾌﾟﾘ側で設定する項目。ここでは触れない).
//*************************************************

//-------------------------------------------------
//	ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//-------------------------------------------------
float4 PS_Main( VS_OUTPUT In ) : SV_Target
{
	//環境光　①.
    float4 ambient = g_vAmbient;

	//拡散反射光 ②.
    float NL = saturate(dot(In.Normal, In.Light));
    float4 diffuse =
		(g_vDiffuse / 2 + g_Texture.Sample(g_SamLinear, In.Tex) / 2) * NL;

	//鏡面反射光 ③.
    float3 reflect = normalize(2 * NL * In.Normal - In.Light);
    float4 specular =
		pow(saturate(dot(reflect, In.EyeVector)), 4) * g_vSpecular;

	//ﾌｫﾝﾓﾃﾞﾙ最終色　①②③の合計.
    float4 Color = ambient + diffuse + specular;

	//ﾗｲﾄ強度を反映.
    Color *= g_fIntensity.x * g_Color;
    Color.a = g_Color.a;

    return Color;
}

//========= ﾃｸｽﾁｬ無し用 ========================================
//-------------------------------------------------
//	頂点(ﾊﾞｰﾃｯｸｽ)ｼｪｰﾀﾞ.
//-------------------------------------------------
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm: NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	//法線をﾜｰﾙﾄﾞ空間に.
	Norm.w = 0;	//w=0で移動成分を反映させない.
	output.Normal = mul(Norm, g_mW).xyz;
	output.Pos = mul(Pos, g_mWVP);
	//ﾗｲﾄ方向.
	output.Light = g_vLightDir.xyz;
	//視線ﾍﾞｸﾄﾙ.
	float3 PosWorld = mul(Pos, g_mW).xyz;
	output.EyeVector = g_vCamPos.xyz - PosWorld;

	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector);
	float NL = saturate(dot(Normal, LightDir));

	float3 Reflect = normalize(2 * NL*Normal - LightDir);
	float4 Specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	output.Color = g_vDiffuse * NL + Specular * g_vSpecular;

	return output;
}

//-------------------------------------------------
//	ﾋﾟｸｾﾙｼｪｰﾀﾞ.
//-------------------------------------------------
float4 PS_NoTex(VS_OUTPUT input) : SV_Target
{
	float4 Color = input.Color;
	return Color;
}

//=====================================================================
//	以降、板ﾎﾟﾘｺﾞﾝ用ｺｰﾄﾞ.
//=====================================================================
//ｺﾝｽﾀﾝﾄﾊﾞｯﾌｧ.
cbuffer Ita_Global : register(b4)	//ﾚｼﾞｽﾀを数分追加.
{
	matrix g_WVP;	//ﾜｰﾙﾄﾞ,ﾋﾞｭｰ,ﾌﾟﾛｼﾞｪｸｼｮﾝまでの変換行列.
	float4 g_Color2;	//色.
	float4 g_UV;	//UV座標.
};

//構造体.
struct VS_ITAOUT
{
	float4	Pos : SV_Position;
	float2	Tex	: TEXCOORD;
};

//頂点ｼｪｰﾀﾞ.
VS_ITAOUT VS_Ita(float4 Pos : POSITION,
				 float2 Tex : TEXCOORD)
{
	VS_ITAOUT output = (VS_ITAOUT)0;
	output.Pos = mul(Pos, g_WVP);
	output.Tex = Tex;

	//ﾃｸｽﾁｬのUV座標を操作する.
	output.Tex.x += g_UV.x;

	return output;
}

//ﾋﾟｸｾﾙｼｪｰﾀﾞ.
float4 PS_Ita( VS_ITAOUT input ) : SV_Target
{
	return g_Texture.Sample(g_SamLinear, input.Tex);//色を返す.
}