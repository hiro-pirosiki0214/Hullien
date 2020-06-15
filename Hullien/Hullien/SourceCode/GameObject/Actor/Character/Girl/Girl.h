#ifndef GIRL_H
#define GIRL_H

#include "..\Character.h"

class CGirl : public CCharacter
{
	const char* MODEL_NAME = "Towa_s";

	// プレイヤーパラメータ.
	struct stGirlParam : public stParameter
	{
		D3DXVECTOR3 SphereAdjPos;	// スフィアの調整座標.
		float		SphereAdjRadius;			// スフィアの調整半径.

		stGirlParam()
			: SphereAdjPos		( 0.0f, 0.0f, 0.0f )
			, SphereAdjRadius	( 0.7f )
		{}
	} typedef SGirlParam;
public:
	CGirl();
	~CGirl();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// 当たり判定関数.
	virtual void Collision(CActor* pActor) override;
	// 相手座標の設定関数.
	virtual void SetTargetPos(CActor& actor) override;

private:
	// 移動関数.
	virtual void Move() override;
	// 当たり判定の作成.
	bool  ColliderSetting();

private:
	SGirlParam m_Parameter;
};

#endif	// #ifndef GIRL_H.