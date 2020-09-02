#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.h"

#include <memory>

class CEffectManager
{
public:
	CEffectManager();
	~CEffectManager();

	// エフェクトの設定.
	bool SetEffect( const char* effectName );

	// 描画.
	void Render();

	// 再生.
	void Play( const D3DXVECTOR3 vPos );
	// 停止.
	void Stop();
	// 全て停止.
	void StopAll();
	// 一時停止.
	void Pause( const bool bFlag );
	// 位置を指定する.
	void SetLocation( const D3DXVECTOR3 vPos );
	// サイズを指定する.
	void SetScale( const D3DXVECTOR3& vScale );
	void SetScale( const float& scale );
	// 回転を指定する.
	void SetRotation( const D3DXVECTOR3& vRot );
	// 再生速度を設定する.
	void SetSpeed( const float fSpeed );

private:
	std::shared_ptr<CEffect>	m_pEffect;	// エフェクトクラス.
	CEffect::Handle				m_Handle;	// エフェクトハンドル.
};

#endif	// #ifndef EFFECT_MANAGER_H.