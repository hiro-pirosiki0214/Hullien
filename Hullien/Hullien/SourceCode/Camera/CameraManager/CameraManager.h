#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "..\..\Global.h"

class CCamera;	// カメrラベースクラス.

/****************************************
* カメラ管理クラス.
*	カメラベースを継承したクラスをセットし使用する.
**/
class CCameraManager
{
public:
	CCameraManager();
	~CCameraManager();

	// カメラの設定.
	static void SetCamera( std::shared_ptr<CCamera> pCamera );

	// 更新関数.
	static void Update();
	// ビュー行列、プロジェクション行列の初期化.
	static void InitViewProj();

	// ビュー行列取得関数.
	static D3DXMATRIX GetViewMatrix();
	// プロジェクション行列取得関数.
	static D3DXMATRIX GetProjMatrix();
	// カメラ座標取得関数.
	static D3DXVECTOR3 GetPosition();
	// カメラ注視座標取得関数.
	static D3DXVECTOR3 GetLookPosition();

private:
	std::shared_ptr<CCamera> m_pCamera;	// カメラクラス.

private:
	// インスタンスの取得.
	static CCameraManager* GetInstance();
};

#endif	// #ifndef CAMERA_MANAGER_H.