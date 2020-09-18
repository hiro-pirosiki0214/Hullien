#include "EventPlayer.h"

/********************************
*	イベント用プレイヤークラス.
**/
CEventPlayer::CEventPlayer()
{
}

CEventPlayer::~CEventPlayer()
{
}

// 初期化関数
bool CEventPlayer::Init()
{
	if (GetModel(MODEL_NAME) == false) return false;

	return true;
}

// 更新関数.
void CEventPlayer::Update()
{
}

// 描画関数.
void CEventPlayer::Render()
{
	MeshRender();	// メッシュの描画.
	EffectRender();
}

// 当たり判定関数.
void CEventPlayer::Collision(CActor * pActor)
{
}

// 相手座標の設定関数.
void CEventPlayer::SetTargetPos(CActor & actor)
{
}

// スプライトの描画.
void CEventPlayer::SpriteRender()
{
}

// 特殊能力を使っているか.
bool CEventPlayer::IsSpecialAbility()
{
	if (m_HasUsableSP == false) return false;
	// 特殊能力が使えるなら.
	m_HasUsableSP = false;	// 初期化して.
	return true;			// trueを返す.
}

// 攻撃操作関数.
void CEventPlayer::AttackController()
{
}

// 特殊能力操作関数.
void CEventPlayer::SPController()
{
}

// 移動関数.
void CEventPlayer::Move()
{
}

// エフェクト描画関数.
void CEventPlayer::EffectRender()
{
}

// 攻撃の当たり判定.
void CEventPlayer::AttackCollision(CActor * pActor)
{
}

// アニメーション設定.
void CEventPlayer::SetAnimation(const EAnimNo & animNo)
{
}

// 当たり判定の設定.
bool CEventPlayer::ColliderSetting()
{
	return false;
}

// エフェクトの設定.
bool CEventPlayer::EffectSetting()
{
	return false;
}

