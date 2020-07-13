#ifndef CHARACTER_H
#define CHARACTER_H

#include "..\Actor.h"

#define IS_TEMP_MODEL_RENDER	// 仮モデル表示.

class CCharacter : public CActor
{
protected:
	const float INIT_POSITION_ADJ_HEIGHT = 5.0f;	// 調整用座標の高さ.

	// キャラクターパラメータ.
	struct stParameter
	{
		float	Life;			// 体力.
		float	AttackPower;	// 攻撃力.
		float	MoveSpeed;		// 移動速度.
		int		InvincibleTime;	// 無敵時間.
		stParameter()
			: Life				( 1.0f )
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

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// スキンメッシュ.
#ifdef IS_TEMP_MODEL_RENDER
	std::shared_ptr<CDX9StaticMesh>	m_pTempStaticMesh;	// 仮のモデルデータ.
#endif	// #ifdef IS_TEMP_MODEL_RENDER.

	D3DXVECTOR3	m_MoveVector;	// 移動ベクトル.
	int	m_InvincibleCount;		// 無敵カウント.

	bool m_HasFinishedParamSetting;	// パラメーターの設定が終わったか.
};

#endif	// #ifndef CHARACTER_H.