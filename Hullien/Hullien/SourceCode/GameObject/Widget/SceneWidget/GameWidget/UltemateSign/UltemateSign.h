#ifndef ULTEMATESIGN_H
#define ULTEMATESIGN_H

#include "..\..\SceneWidget.h"

class CActor;

/*************************************************
*	アルティメット出現サインクラス.
**/
class CUltemateSing : public CSceneWidget
{
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
	void IsAppUltemate(CActor* pActor);

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	bool m_IsAppUltemate;	//アルティメットが出現しているか.
};


#endif // #ifndef ULTEMATESIGN_H.