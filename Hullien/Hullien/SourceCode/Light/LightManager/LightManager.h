#ifndef LIGHT_MANAGER_H
#define LIGHT_MANAGER_H

#include "..\..\Global.h"

class CLightBase;

class CLightManager
{
public:
	CLightManager();
	~CLightManager();

	// ライトクラスの設定.
	static void SetLgiht( std::shared_ptr<CLightBase> pLight );

	// 座標の取得.
	static D3DXVECTOR3 GetPosition();
	// 方向の取得.
	static D3DXVECTOR3 GetDirection();
	// 回転行列の取得.
	static D3DXMATRIX GetRorarionMatrix();
	// 強さの取得.
	static float GetIntensity();

private:
	std::shared_ptr<CLightBase> m_pLight;

private:
	// インスタンスの取得.
	static CLightManager* GetInstance();
};

#endif	// #ifndef LIGHT_MANAGER_H.