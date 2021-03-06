#ifndef ULTEMATESIGN_H
#define ULTEMATESIGN_H

#include "..\..\SceneWidget.h"

class CGameActorManager;

/*************************************************
*	アルティメット出現サインクラス.
**/
class CUltemateSing : public CSceneWidget
{
private:
	const char* SPRITE_NAME1 = "warning1";	//スプライトの名前.
	const char* SPRITE_NAME2 = "warning2";	//スプライトの名前.
	const float DISPTIME_MAX = 250.0f;		//最大表示時間.
	const float ALPHA_SPEED = 0.03f;		//透過速度.

public:
	CUltemateSing();
	virtual ~CUltemateSing();

	// 初期化関数.
	virtual bool Init() override;

	// 更新関数.
	virtual void Update() override;

	// 描画関数.
	virtual void Render() override;

	// アルティメットが出現しているか.
	void IsAppUltemate(CGameActorManager* pActor);
	// アルティメットの警告を出現されるか.
	inline void SetAppUltemate( const bool& isAppUltemate ){ m_IsAppUltemate = isAppUltemate; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;
	// サイン表示関数.
	void DispSign();

private:
	std::shared_ptr<CSprite>	m_pWarnigSprite;
	bool						m_IsAppUltemate;	// アルティメットが出現しているか.
	unsigned int				m_ObjCount;			// オブジェクトのカウント.
	float						m_DispTime;			// 表示時間.
};


#endif // #ifndef ULTEMATESIGN_H.