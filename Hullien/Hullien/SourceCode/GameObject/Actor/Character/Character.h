#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

class CCharacter : public CActor
{
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
	// 足音.
	void FootStep(const char* rightfoot, const char* leftfoot);
	// 足音用当たり判定の設定.
	bool FootStepCollisionSetting();

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// スキンメッシュ.

	D3DXVECTOR3	m_MoveVector;	// 移動ベクトル.
	int	m_InvincibleCount;		// 無敵カウント.

	bool m_HasFinishedParamSetting;	// パラメーターの設定が終わったか.

private:
	std::vector<std::shared_ptr<CCollisionManager>> m_pFootCollision;
	std::shared_ptr<CCollisionManager>				m_pGroundCollision;
	D3DXVECTOR3 m_vGroundPosition;	// 地面の位置.
	D3DXVECTOR3 m_vRightPosition;	// 右足の位置.
	D3DXVECTOR3 m_vLeftPosition;	// 左足の位置.
};

#endif	// #ifndef CHARACTER_H.