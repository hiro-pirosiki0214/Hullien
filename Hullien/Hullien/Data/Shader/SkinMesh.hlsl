/***************************************************************************************************
*	SkinMeshCode Version 2.00
*	LastUpdate	: 2019/10/09
*	Auther		: an.
**/
//��`.
#define MAX_BONE_MATRICES (255)

//-------------------------------------------------.
// �O���[�o���ϐ�.
//-------------------------------------------------.
// �e�N�X�`��.
Texture2D g_Texture		: register(t0); //�e�N�X�`���[�� ���W�X�^�[t(n).
Texture2D g_ShadowMap1	: register(t1);	// �V���h�E�e�N�X�`��.
Texture2D g_ShadowMap2	: register(t2);	// �V���h�E�e�N�X�`��.
Texture2D g_ShadowMap3	: register(t3);	// �V���h�E�e�N�X�`��.
Texture2D g_ShadowMap4	: register(t4);	// �V���h�E�e�N�X�`��.
Texture2D g_ToonMap		: register(t5); // toon�V�F�[�_�[�p�̃e�N�X�`��.
Texture2D g_FogTexture	: register(t6); // �t�H�O�p�̃e�N�X�`��.
// �T���v��.
SamplerState g_SamLinear		: register(s0); //�T���v���[�̓��W�X�^�[s(n).
SamplerState g_ToonSamLinear	: register(s1);
SamplerState g_ShadowSamLinear	: register(s2);

struct FOG
{
	float4  FogTex;		// �t�H�O�̃e�N�X�`�����W.
	float4  FogColor;		// �t�H�O�̐F.
	float	MinHeight;	// �t�H�O�̍ŏ�����.
	float	MaxHeight;	// �t�H�O�̍ő卂��.
	float	TexScale;		// �t�H�O�̃e�N�Z���X�P�[��.
};

//�R���X�^���g�o�b�t�@(���b�V������).
cbuffer per_mesh : register(b0)
{
	matrix g_mW; //���[���h�s��.
	matrix g_mWVP; //���[���h����ˉe�܂ł̕ϊ��s��.
};
//�R���X�^���g�o�b�t�@(�}�e���A������).
cbuffer per_material : register(b1)
{
	float4 g_vAmbient = float4(0, 0, 0, 0); //�A���r�G���g��.
	float4 g_vDiffuse = float4(1, 0, 0, 0); //�g�U����(�F�j
	float4 g_vSpecular = float4(1, 1, 1, 1); //���ʔ���.
};
//�R���X�^���g�o�b�t�@(�t���[������).
cbuffer per_frame : register(b2)
{
	float4 g_vCamPos; //��׈ʒu.
	float4 g_vLightPos; //ײĈʒu.
	float4 g_vLightDir; //ײĕ���.
	matrix g_mLightRot; //ײĉ�]�s��.
	float4 g_fIntensity; //ײċ��x(���邳). ��x�̂ݎg�p����.
	float4 g_Color; // �F.
	matrix g_LightWVP[4];
	float4 g_SpritPos;
	float4 g_IsShadow;
	FOG	g_Fog;
};
//�{�[���̃|�[�Y�s�񂪓���.
cbuffer per_bones : register(b3)
{
	matrix g_mConstBoneWorld[MAX_BONE_MATRICES];
};

//�X�L�j���O��̒��_�E�@��������.
struct Skin
{
	float4 Pos;
	float3 Norm;
};
//�o�[�e�b�N�X�o�b�t�@�[�̓���.
struct VSSkinIn
{
	float4 Pos : POSITION; //�ʒu.  
	float3 Norm : NORMAL; //���_�@��.
	float2 Tex : TEXCOORD; //�e�N�X�`���[���W.
	uint4 Bones : BONE_INDEX; //�{�[���̃C���f�b�N�X.
	float4 Weights : BONE_WEIGHT; //�{�[���̏d��.
};

//�s�N�Z���V�F�[�_�[�̓��́i�o�[�e�b�N�X�o�b�t�@�[�̏o�́j�@
struct VS_OUTPUT
{
	float4 Pos : SV_Position; // WVP�ł̍��W.
	float4 PosW : Position; // W�ł̍��W.
	float3 LightDir : TEXCOORD0; // ���C�g����.
	float3 Normal : TEXCOORD1; // �@��.
	float3 EyeVector : TEXCOORD2; // ���_�x�N�g��.
	float2 Tex : TEXCOORD3; // �e�N�X�`�����W.
	float4 ZDepth[4] : TEXCOORD4;
	float SpritPos[4] : TEXCOORD8;
};

//�w�肵���ԍ��̃{�[���̃|�[�Y�s���Ԃ�.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
matrix FetchBoneMatrix(uint iBone)
{
	return g_mConstBoneWorld[iBone];
}

//���_���X�L�j���O�i�{�[���ɂ��ړ��j����.
//�T�u�֐��i�o�[�e�b�N�X�V�F�[�_�[�Ŏg�p�j.
Skin SkinVert(VSSkinIn Input)
{
	Skin Output = (Skin) 0;

	float4 Pos = float4(Input.Pos);
	float3 Norm = Input.Norm;
	//�{�[��0.
	uint iBone = Input.Bones.x;
	float fWeight = Input.Weights.x;
	matrix m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//�{�[��1.
	iBone = Input.Bones.y;
	fWeight = Input.Weights.y;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//�{�[��2.
	iBone = Input.Bones.z;
	fWeight = Input.Weights.z;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);
	//�{�[��3.
	iBone = Input.Bones.w;
	fWeight = Input.Weights.w;
	m = FetchBoneMatrix(iBone);
	Output.Pos += fWeight * mul(Pos, m);
	Output.Norm += fWeight * mul(Norm, (float3x3) m);

	return Output;
}

// �o�[�e�b�N�X�V�F�[�_.
VS_OUTPUT VS_Main(VSSkinIn input)
{
	VS_OUTPUT output = (VS_OUTPUT) 0;
	Skin vSkinned = SkinVert(input);
	
	output.Pos = mul(vSkinned.Pos, g_mWVP); // WVP���W.
	output.PosW = mul(vSkinned.Pos, g_mW); // world���W.
	output.Normal = normalize(mul(vSkinned.Norm.xyz, (float3x3) g_mW)); // �@��.
	output.LightDir = normalize(g_vLightDir).xyz; // ���C�g����.
	output.EyeVector = normalize(g_vCamPos - output.PosW).xyz; // ���_�x�N�g��.
	output.Tex = input.Tex; //�@�e�N�X�`�����W.

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
	// R�������o
	float R = round(depth * 256.0f) / 256.0f;
	// G�������o
	float Def = depth - R;
	float G = round(Def * 65536.0f) / 256.0f;
	// B�������o
	Def -= G / 256.0f;
	float B = round(Def * 65536.0f * 256.0f) / 256.0f;
	// A�������o
	Def -= B / 65536.0f;
	float A = round(Def * 65536.0f * 65536.0f) / 256.0f;
	
	return float4(R, G, B, A);
}

// �s�N�Z���V�F�[�_.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);
	color *= g_Color;
	
	// �e�s�N�Z���ʒu�܂ł̋���.
	float dist = input.Pos.w; // �r���[��Ԃł�Z���W.
	float shadowColor = 1.0f;
	float zValue = 0.0f; // �[�x�l.
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
	if (g_IsShadow.x >= 1.0f) color.rgb *= shadowColor;
	
	//-----�g�D�[������------.
	// �n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
	float p = dot(input.Normal, input.LightDir);
	p = p * 0.5f + 0.5f;
	p = p * p;
	// �v�Z���ʂ��g�D�[���V�F�[�_�[�p�̃e�N�X�`������F���t�F�b�`����
	float4 toonColor = g_ToonMap.Sample(g_ToonSamLinear, float2(p, 0.0f));
	color.rgb *= toonColor.rgb * g_fIntensity.x;
	
	//-----�����t�H�O����------.
	// fog�e�N�X�`���̍��W���擾�A�v�Z.
	const float fogColor =
		(g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.xy).r +
		 g_FogTexture.Sample(g_SamLinear, input.PosW.xz * g_Fog.TexScale + g_Fog.FogTex.zw).r) * 0.5f;
	float alpha = clamp((input.PosW.y - g_Fog.MinHeight) / (g_Fog.MaxHeight - g_Fog.MinHeight), 0.0f, 1.0f);
	float alphas = 1.0f - (1.0f - alpha) * fogColor;

	color.rgb = color.rgb * alphas + g_Fog.FogColor.rgb * (1.0f - alphas);
	
	PS_OUTPUT output = (PS_OUTPUT) 0;
	output.Color = color;
	output.Normal = float4(input.Normal, 1.0f);
	output.ZDepth = input.Pos.z / input.Pos.w;
	return output;
}
