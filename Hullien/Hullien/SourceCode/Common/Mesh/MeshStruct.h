#ifndef MESH_STRUCT_H
#define MESH_STRUCT_H

#include "..\..\Global.h"
#include "..\Fog\Fog.h"

//======================================
//	構造体.
//======================================
// メッシュ単位で渡す情報.
struct CBUFFER_PER_MESH
{
	D3DXMATRIX	mW;		// ワールド行列.
	D3DXMATRIX	mWVP;	// ワールド,ビュー,プロジェクションの合成変換行列.	
};
// マテリアル単位で渡す情報.
struct CBUFFER_PER_MATERIAL
{
	D3DXVECTOR4 vAmbient;	// アンビエント色(環境色).
	D3DXVECTOR4	vDiffuse;	// ディヒューズ色(拡散反射色).
	D3DXVECTOR4	vSpecular;	// スペキュラ色(鏡面反射色).
};
// フレーム単位で渡す情報.
struct CBUFFER_PER_FRAME
{
	D3DXVECTOR4	vCamPos;	//ｶﾒﾗ位置(視点位置).
	D3DXVECTOR4	vLightPos;	//ﾗｲﾄ位置.
	D3DXVECTOR4	vLightDir;	//ﾗｲﾄ方向.
	D3DXMATRIX	mLightRot;	//ﾗｲﾄ回転行列.
	D3DXVECTOR4	fIntensity;	//ﾗｲﾄ強度(明るさ). ※xのみ使用する.
	D3DXVECTOR4 vColor;		//色.
	D3DXMATRIX	mLightWVP[4];
	D3DXVECTOR4	SpritPos;
	D3DXVECTOR4 IsShadow;
	SFog Fog;				// フォグのテクスチャ座標.
};

#endif	// #ifndef MESH_STRUCT_H.