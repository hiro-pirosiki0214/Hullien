#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include "Effect.h"

#include <memory>

class CEffectManager
{
public:
	CEffectManager();
	~CEffectManager();

	void Play( const D3DXVECTOR3 vPos );

private:
	std::shared_ptr<CEffect>	m_pEffect;	// エフェクトクラス.
	CEffect::Handle				m_Handle;	// エフェクトハンドル.
};

#endif	// #ifndef EFFECT_MANAGER_H.