#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"
#include "AnimationParamInfo.h"

class CCharacter : public CActor
{
protected:
	const double DEFAULT_ANIM_SPEED = 0.01;	// デフォルトアニメーション速度.

public:
	// キャラクターパラメータ.
	struct stParameter
	{
		float	LifeMax;		// 体力.
		float	AttackPower;	// 攻撃力.
		float	MoveSpeed;		// 移動速度.
		int		InvincibleTime;	// 無敵時間.
		stParameter()
			: LifeMax			( 1.0f )
			, AttackPower		( 0.5f )
			, MoveSpeed			( 0.5f )
			, InvincibleTime	( 0 )
		{}
	} typedef SParameter;

public:
	CCharacter();
	virtual ~CCharacter();

	// 移動関数.
	virtual void Move() = 0;
	// スプライトの描画.
	virtual void SpriteRender(){};

	// アニメーションを止める.
	virtual void StopAnimation() override { m_AnimSpeed = 0.0; }

protected:
	// メッシュの表示.
	void MeshRender();
	// 無敵時間かどうか.
	bool IsInvincibleTime( const int& invincibleTime );
	// モデルの取得.
	bool GetModel( const char* modelName );
	// 当たり判定の設定.
	bool ColliderSetting();
	// パラメータの設定.
	template<class inParam>
	bool ParameterSetting( const char* fileName, inParam& param )
	{
		// 既にパラメータが設定済みなら終了.
		if( m_HasFinishedParamSetting == true ) return true;

		// 読み込みが正常に行えてなければ終了.
		if( CFileManager::BinaryReading( fileName, param ) == false ) return false;

		m_HasFinishedParamSetting = true;
		return true;
	}
	// 目的の座標へ回転.
	bool TargetRotation( const D3DXVECTOR3& targetVec, const float& rotSpeed, const float& ToleranceRadian );
	// アニメーション設定.
	void SetAnimation( const int& animNo, LPD3DXANIMATIONCONTROLLER	pAc = nullptr );
	// アニメーションをブレンドして設定.
	void SetAnimationBlend( const int& animNo );

	// アニメーションフレームの設定.
	virtual bool SetAnimFrameList() = 0;

	// 足音.
	void FootStep(const char* rightfoot, const char* leftfoot);
	// 足音用当たり判定の設定.
	bool FootStepCollisionSetting();

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// スキンメッシュ.
	anim::AAnimFrameList			m_AnimFrameList;	// アニメーションフレームのリスト.
	int		m_NowAnimNo;	// 現在のアニメーション番号.
	int		m_OldAnimNo;	// 過去のアニメーション番号.
	double	m_AnimSpeed;	// アニメーション速度.

	D3DXVECTOR3	m_MoveVector;				// 移動ベクトル.
	int			m_InvincibleCount;			// 無敵カウント.
	bool		m_HasFinishedParamSetting;	// パラメーターの設定が終わったか.

private:
	std::vector<std::shared_ptr<CCollisionManager>> m_pFootCollision;
	std::shared_ptr<CCollisionManager>				m_pGroundCollision;
	D3DXVECTOR3 m_vGroundPosition;	// 地面の位置.
	D3DXVECTOR3 m_vRightPosition;	// 右足の位置.
	D3DXVECTOR3 m_vLeftPosition;	// 左足の位置.
};

#endif	// #ifndef CHARACTER_H.