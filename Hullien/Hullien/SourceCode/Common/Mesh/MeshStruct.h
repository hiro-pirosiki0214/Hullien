#ifndef MESH_STRUCT_H
#define MESH_STRUCT_H

#include "..\..\Global.h"
#include "..\Fog\Fog.h"

//======================================
//	�\����.
//======================================
// ���b�V���P�ʂœn�����.
struct CBUFFER_PER_MESH
{
	D3DXMATRIX	mW;		// ���[���h�s��.
	D3DXMATRIX	mWVP;	// ���[���h,�r���[,�v���W�F�N�V�����̍����ϊ��s��.	
};
// �}�e���A���P�ʂœn�����.
struct CBUFFER_PER_MATERIAL
{
	D3DXVECTOR4 vAmbient;	// �A���r�G���g�F(���F).
	D3DXVECTOR4	vDiffuse;	// �f�B�q���[�Y�F(�g�U���ːF).
	D3DXVECTOR4	vSpecular;	// �X�y�L�����F(���ʔ��ːF).
};
// �t���[���P�ʂœn�����.
struct CBUFFER_PER_FRAME
{
	D3DXVECTOR4	vCamPos;	//��׈ʒu(���_�ʒu).
	D3DXVECTOR4	vLightPos;	//ײĈʒu.
	D3DXVECTOR4	vLightDir;	//ײĕ���.
	D3DXMATRIX	mLightRot;	//ײĉ�]�s��.
	D3DXVECTOR4	fIntensity;	//ײċ��x(���邳). ��x�̂ݎg�p����.
	D3DXVECTOR4 vColor;		//�F.
	D3DXMATRIX	mLightWVP[4];
	D3DXVECTOR4	SpritPos;
	D3DXVECTOR4 IsShadow;
	SFog Fog;				// �t�H�O�̃e�N�X�`�����W.
};

#endif	// #ifndef MESH_STRUCT_H.