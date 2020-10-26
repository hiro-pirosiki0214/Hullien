#ifndef EVENT_CHARACTER_H
#define EVENT_CHARACTER_H

#include "..\Actor.h"
#include "..\Character\AnimationParamInfo.h"

class CCollisionManager;

/***************************************
*	イベント用キャラクタクラス.
**/
class CEventCharacter : public CActor
{
	const float ROTATIONAL_SPEED = 0.05f;	// 回転速度.
	const float TOLERANCE_RADIAN = static_cast<float>(D3DXToRadian(10.0));	// 回転の許容範囲.

protected:
	const float MODEL_ALPHA_MAX = 1.0f;	// モデルアルファの最大値.
	const float MODEL_SCALE_MAX = 1.0f;	// モデルの大きさ最大値.

public:
	// 任意で設定する情報.
	struct stOptionalState
	{
		D3DXVECTOR3	vPosition;			// 位置.
		D3DXVECTOR3	vRotation;			// 回転値.
		D3DXVECTOR3	vScale;				// 大きさ.
		float		ModelAlpha;			// モデルの透過値.
		float		MoveSpeed;			// 移動速度.
		float		RotationalSpeed;	// 回転速度.
		float		ScaleSpeed;			// 拡大速度.
		float		AlphaSpeed;			// 透過速度.
		bool		IsDisp;				// 描画するか.

		stOptionalState()
			: vPosition			( {0.0f,0.0f,0.0f} )
			, vRotation			( {0.0f,0.0f,0.0f} )
			, vScale			( {1.0f,1.0f,1.0f} )
			, ModelAlpha		( 1.0f )
			, MoveSpeed			( 0.1f )
			, RotationalSpeed	( 0.01f )
			, ScaleSpeed		( 0.03f )
			, AlphaSpeed		( 0.03f )
			, IsDisp			( true )
		{}
	}typedef SOptionalState;

protected:

	// キャラクタの情報.
	struct stCharacterParam : public SOptionalState
	{
		float			ResearchLenght;		// 再検査する際の距離.
		D3DXVECTOR3		SphereAdjPos;		// スフィアの調整座標.
		float			SphereAdjRadius;	// スフィアの調整半径.

		stCharacterParam()
			: ResearchLenght(0.0f)
			, SphereAdjPos(0.0f, 0.0f, 0.0f)
			, SphereAdjRadius(0.0f)
		{}
	}typedef SCharacterParam;

public:
	CEventCharacter();
	virtual ~CEventCharacter();

	// 移動関数.
	virtual void Move() = 0;

	// 情報設定関数.
	virtual void SetOptionalState(const SOptionalState& state);

	// 座標別位置設定関数.
	void SetPositionX(const float& vPos_x);
	void SetPositionY(const float& vPos_y);
	void SetPositionZ(const float& vPos_z);

	// 回転値取得関数.
	D3DXVECTOR3 GetRotation() const;
	// 回転値設定関数.
	void SetRotation(const D3DXVECTOR3& vRot);
	void SetRotationX(const float& vRot_x);
	void SetRotationY(const float& vRot_y);
	void SetRotationZ(const float& vRot_z);

	// 大きさ取得関数.
	D3DXVECTOR3 GetScale() const;
	// 大きさ設定関数.
	void SetScale(const D3DXVECTOR3& vScale);
	void SetScaleX(const float& vScale_x);
	void SetScaleY(const float& vScale_y);
	void SetScaleZ(const float& vScale_z);

	// 右回転の設定.
	float RotationMoveRight(const float& rotValue, 
		const float& rotSpeed, bool IsRightRot = true);
	// アニメーション設定.
	void SetAnimation(const int& animNo);
	// アニメーションをブレンドして設定.
	void SetAnimationBlend(const int& animNo);
	// アニメーション速度の設定.
	void SetAnimSpeed( const double& speed ){ m_AnimSpeed = speed; }

protected:
	// メッシュの表示関数.
	void MeshRender();
	// モデルの取得関数.
	bool GetModel(const char* modelName);
	// 足音.
	void FootStep(const char* rightfoot, const char* leftfoot);
	// 足音用当たり判定の設定.
	bool FootStepCollisionSetting();

protected:
	std::shared_ptr<CDX9SkinMesh>	m_pSkinMesh;		// スキンメッシュ.
	anim::AAnimFrameList			m_AnimFrameList;	// アニメーションフレームのリスト.
	int				m_NowAnimNo;				// 現在のアニメーション番号.
	int				m_OldAnimNo;				// 過去のアニメーション番号.
	double			m_AnimSpeed;				// アニメーション速度.
	SCharacterParam	m_Parameter;				// パラメータ.

private:
	std::vector<std::shared_ptr<CCollisionManager>> m_pFootCollision;
	std::shared_ptr<CCollisionManager>				m_pGroundCollision;
	D3DXVECTOR3 m_vGroundPosition;	// 地面の位置.
	D3DXVECTOR3 m_vRightPosition;	// 右足の位置.
	D3DXVECTOR3 m_vLeftPosition;	// 左足の位置.
};

#endif	//#ifndef EVENT_CHARACTER_H.
