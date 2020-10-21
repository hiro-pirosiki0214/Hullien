#ifndef VOLUME_BAR_H
#define VOLUME_BAR_H

#include "..\..\..\Widget.h"
#include <vector>

class CVolumeBar : public CWidget
{
	const char* SPRITE_NAME = "VolumeBar";
public:
	CVolumeBar();
	virtual ~CVolumeBar();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;

	// ボリュームの設定.
	void SetVolume( const float& volume ){ m_Volume = volume; }

	// 現在のカーソルの座標の取得.
	float GetNowCursorPosition(){ return m_CursorPositionX; }
	// Y座標の設定.
	void SetPositionY( const float& posY ){ m_vPosition.y = posY; }

private:
	// スプライト設定関数.
	virtual bool SpriteSetting() override;

private:
	std::shared_ptr<CSprite>	m_pSprite;	// スプライトクラス.
	float	m_CursorPositionX;
	float	m_ImageWidthSize;
	float	m_Volume;
};

#endif	// #ifndef VOLUME_BAR_H.