//-------------------------------------------------.
// �O���[�o���ϐ�.
//-------------------------------------------------.
// �e�N�X�`��.
Texture2D g_Texture		: register(t0); // ���f���̃e�N�X�`��.
Texture2D g_ShadowMap1	: register(t1); // �V���h�E�e�N�X�`��.
Texture2D g_ShadowMap2	: register(t2); // �V���h�E�e�N�X�`��.
Texture2D g_ShadowMap3	: register(t3); // �V���h�E�e�N�X�`��.
Texture2D g_ShadowMap4	: register(t4); // �V���h�E�e�N�X�`��.
Texture2D g_ToonMap		: register(t5); // toon�V�F�[�_�[�p�̃e�N�X�`��.
Texture2D g_FogTexture	: register(t6); // �t�H�O�p�̃e�N�X�`��.
// �T���v��.
SamplerState g_SamLinear : register(s0);
SamplerState g_ToonSamLinear : register(s1);
SamplerState g_ShadowSamLinear : register(s2);

struct FOG
{
	float4  FogTex;		// �t�H�O�̃e�N�X�`�����W.
	float4  FogColor;		// �t�H�O�̐F.
	float	MinHeight;	// �t�H�O�̍ŏ�����.
	float	MaxHeight;	// �t�H�O�̍ő卂��.
	float	TexScale;		// �t�H�O�̃e�N�Z���X�P�[��.
};

//-------------------------------------------------.
// �R���X�^���g�o�b�t�@.
//-------------------------------------------------.
// ���b�V���P��.
cbuffer per_mesh : register(b0)
{
	matrix g_mW;	// ���[���h�s��.
	matrix g_mWVP;	// ���[���h,�r���[,�v���W�F�N�V�����̍����s��.
};
// �}�e���A���P��.
cbuffer per_material : register(b1)
{
	float4 g_vAmbient;	// �A���r�G���g�F(���F).
	float4 g_vDiffuse;	// �f�B�q���[�Y�F(�g�U���ːF).
	float4 g_vSpecular; // �X�y�L�����F(���ʔ��ːF).
};
// �t���[���P��.
cbuffer per_frame : register(b2)
{
	float4 g_vCamPos;		// �J�����ʒu.
	float4 g_vLightPos;		// ���C�g�ʒu.
	float4 g_vLightDir;		// ���C�g����.
	matrix g_mLightRot;		// ���C�g��]�s��.
	float4 g_fIntensity;	// ���C�g���x(���邳). ��x�̂ݎg�p����.
	float4 g_Color;			// �F.
	matrix g_LightWVP[4];	// ���C�g�s��4��.
	float4 g_SpritPos;		// ���C�g�̋�؂���W.
	float4 g_IsShadow;		// ���C�g���g�p���邩.
	FOG	g_Fog;
};

//-------------------------------------------------.
// �\����.
//-------------------------------------------------.
// ���_�C���v�b�g.
struct VS_INPUT
{
	float4 Pos	: POSITION;	// ���W.
	float4 Norm : NORMAL;	// �@��.
	float2 Tex	: TEXCOORD;	// �e�N�X�`�����W.
};
// ���_�A�E�g�v�b�g.
struct VS_OUTPUT
{
	float4 Pos			: SV_Position;	// WVP�ł̍��W.
	float4 PosW			: Position;		// W�ł̍��W.
	float4 Color		: COLOR;		// �F.
	float3 LightDir		: TEXCOORD0;	// ���C�g����.
	float3 Normal		: TEXCOORD1;	// �@��.
	float3 EyeVector	: TEXCOORD2;	// ���_�x�N�g��.
	float2 Tex			: TEXCOORD3;	// �e�N�X�`�����W.
	float4 SpritPos		: TEXCOORD4;	// ��؂���W.
	float4 ZDepth[4]	: TEXCOORD5;	// �e�[�x�l.
};

//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_Main(VS_INPUT input)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

	Out.Pos			= mul(input.Pos, g_mWVP);	// WVP���W.
	Out.PosW		= mul(input.Pos, g_mW);		// world���W.
	Out.Normal		= mul(input.Norm.xyz, (float3x3) g_mW); // �@��.
	Out.LightDir	= normalize(g_vLightDir).xyz; // ���C�g����.
	Out.EyeVector	= normalize(g_vCamPos - Out.PosW).xyz; // ���_�x�N�g��.
	Out.Tex = input.Tex; //�@�e�N�X�`�����W.
	// �e���C�g����̐[�x�l�̎擾.
	Out.ZDepth[0] = mul(input.Pos, g_LightWVP[0]);
	Out.ZDepth[1] = mul(input.Pos, g_LightWVP[1]);
	Out.ZDepth[2] = mul(input.Pos, g_LightWVP[2]);
	Out.ZDepth[3] = mul(input.Pos, g_LightWVP[3]);
	Out.SpritPos = g_SpritPos;
	
	return Out;
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

//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
PS_OUTPUT PS_Main(VS_OUTPUT input) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
	float4 color = g_Texture.Sample(g_SamLinear, input.Tex);
	color *= g_Color;
	
	// �e�s�N�Z���ʒu�܂ł̋���.
	float dist = input.Pos.w; // �r���[��Ԃł�Z���W.
	float shadowColor = 1.0f;
	float zValue = 0.0f; // �[�x�l.
	float baias = 0.01f;
	float2 cord = float2(0.0f, 0.0f);
	if (dist < input.SpritPos.x)
	{
		zValue = input.ZDepth[0].z / input.ZDepth[0].w;
		cord.x = (1.0f + input.ZDepth[0].x / input.ZDepth[0].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[0].y / input.ZDepth[0].w) * 0.5f;
		float4 depthColor = g_ShadowMap1.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.y)
	{
		zValue = input.ZDepth[1].z / input.ZDepth[1].w;
		cord.x = (1.0f + input.ZDepth[1].x / input.ZDepth[1].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[1].y / input.ZDepth[1].w) * 0.5f;
		float4 depthColor = g_ShadowMap2.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.z)
	{
		zValue = input.ZDepth[2].z / input.ZDepth[2].w;
		cord.x = (1.0f + input.ZDepth[2].x / input.ZDepth[2].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[2].y / input.ZDepth[2].w) * 0.5f;
		float4 depthColor = g_ShadowMap3.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else
	{
		zValue = input.ZDepth[3].z / input.ZDepth[3].w;
		cord.x = (1.0f + input.ZDepth[3].x / input.ZDepth[3].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[3].y / input.ZDepth[3].w) * 0.5f;
		float4 depthColor = g_ShadowMap4.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
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

//========= �e�N�X�`�������p ========================================
//-------------------------------------------------.
//	���_�V�F�[�_�[.
//-------------------------------------------------.
VS_OUTPUT VS_NoTex(float4 Pos : POSITION,
				   float4 Norm : NORMAL)
{
	VS_OUTPUT Out = (VS_OUTPUT) 0;

	Out.Pos			= mul(Pos, g_mWVP); // WVP���W.
	Out.PosW		= mul(Pos, g_mW);	// world���W.
	Out.Normal		= mul(Norm.xyz, (float3x3) g_mW); // �@��.
	Out.LightDir	= normalize(g_vLightDir).xyz; // ���C�g����.
	Out.EyeVector	= normalize(g_vCamPos - Out.PosW).xyz; // ���_�x�N�g��.

	// �e���C�g����̐[�x�l�̎擾.
	Out.ZDepth[0] = mul(Pos, g_LightWVP[0]);
	Out.ZDepth[1] = mul(Pos, g_LightWVP[1]);
	Out.ZDepth[2] = mul(Pos, g_LightWVP[2]);
	Out.ZDepth[3] = mul(Pos, g_LightWVP[3]);
	Out.SpritPos[0] = g_SpritPos.x;
	Out.SpritPos[1] = g_SpritPos.y;
	Out.SpritPos[2] = g_SpritPos.z;
	Out.SpritPos[3] = g_SpritPos.w;
	
	return Out;
}

//-------------------------------------------------.
//	�s�N�Z���V�F�[�_�[.
//-------------------------------------------------.
PS_OUTPUT PS_NoTex(VS_OUTPUT input) : SV_Target
{
	// ���f���̃e�N�X�`���F���擾.
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	color *= g_Color;
	
	// �e�s�N�Z���ʒu�܂ł̋���.
	float dist = input.Pos.w; // �r���[��Ԃł�Z���W.
	float shadowColor = 1.0f;
	float zValue = 0.0f; // �[�x�l.
	float baias = 0.01f;
	float2 cord = float2(0.0f, 0.0f);
	if (dist < input.SpritPos.x)
	{
		zValue = input.ZDepth[0].z / input.ZDepth[0].w;
		cord.x = (1.0f + input.ZDepth[0].x / input.ZDepth[0].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[0].y / input.ZDepth[0].w) * 0.5f;
		float4 depthColor = g_ShadowMap1.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.y)
	{
		zValue = input.ZDepth[1].z / input.ZDepth[1].w;
		cord.x = (1.0f + input.ZDepth[1].x / input.ZDepth[1].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[1].y / input.ZDepth[1].w) * 0.5f;
		float4 depthColor = g_ShadowMap2.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else if (dist < input.SpritPos.z)
	{
		zValue = input.ZDepth[2].z / input.ZDepth[2].w;
		cord.x = (1.0f + input.ZDepth[2].x / input.ZDepth[2].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[2].y / input.ZDepth[2].w) * 0.5f;
		float4 depthColor = g_ShadowMap3.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
	}
	else
	{
		zValue = input.ZDepth[3].z / input.ZDepth[3].w;
		cord.x = (1.0f + input.ZDepth[3].x / input.ZDepth[3].w) * 0.5f;
		cord.y = (1.0f - input.ZDepth[3].y / input.ZDepth[3].w) * 0.5f;
		float4 depthColor = g_ShadowMap4.Sample(g_ShadowSamLinear, cord);
		float sm = depthColor.r + (depthColor.g + (depthColor.b + depthColor.a / 256.0f) / 256.0f) / 256.0f;
		shadowColor = (zValue > sm - baias) ? 0.5f : 1.0f;
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