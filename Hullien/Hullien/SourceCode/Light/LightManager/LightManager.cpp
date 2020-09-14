#include "LightManager.h"
#include "..\LightBase.h"

CLightManager::CLightManager()
	: m_pLight	( nullptr )
{
}
CLightManager::~CLightManager()
{
}

//-----------------------------------.
// ライトクラスの設定.
//-----------------------------------.
void CLightManager::SetLgiht( std::shared_ptr<CLightBase> pLight )
{
	GetInstance()->m_pLight = pLight;
}

//-----------------------------------.
// カスケード分のシャドウ行列を取得する.
//-----------------------------------.
D3DXMATRIX* CLightManager::GetShadowVP()
{
	if( GetInstance()->m_pLight == nullptr ) 
		return nullptr;
	return GetInstance()->m_pLight->GetShadowVP();
}

//-----------------------------------.
// カスケード分割座標を取得.
//-----------------------------------.
float* CLightManager::GetSpritPos()
{
	if( GetInstance()->m_pLight == nullptr ) 
		return nullptr;
	return GetInstance()->m_pLight->GetSpritPos();
}

//-----------------------------------.
// 座標の取得.
//-----------------------------------.
D3DXVECTOR3 CLightManager::GetPosition()
{
	if( GetInstance()->m_pLight == nullptr ) 
		return { 0.0f, 5.0f, 0.0f };
	return GetInstance()->m_pLight->GetPosition();
}

//-----------------------------------.
// 方向の取得.
//-----------------------------------.
D3DXVECTOR3 CLightManager::GetDirection()
{
	if( GetInstance()->m_pLight == nullptr ) 
		return { 1.5f, 1.0f, -1.0f };
	return GetInstance()->m_pLight->GetDirection();
}

//-----------------------------------.
// 回転行列の取得.
//-----------------------------------.
D3DXMATRIX CLightManager::GetRorarionMatrix()
{
	if( GetInstance()->m_pLight == nullptr ){
		D3DXMATRIX mRot;
		D3DXMatrixIdentity( &mRot );
		return mRot;
	}
	return GetInstance()->m_pLight->GetRorarionMatrix();
}

//-----------------------------------.
// 強さの取得.
//-----------------------------------.
float CLightManager::GetIntensity()
{ 
	if( GetInstance()->m_pLight == nullptr ) return 1.0f;
	return GetInstance()->m_pLight->GetIntensity(); 
}

//-----------------------------------.
// インスタンスの取得.
//-----------------------------------.
CLightManager* CLightManager::GetInstance()
{
	static std::unique_ptr<CLightManager> pInstance =
		std::make_unique<CLightManager>();
	return pInstance.get();
};