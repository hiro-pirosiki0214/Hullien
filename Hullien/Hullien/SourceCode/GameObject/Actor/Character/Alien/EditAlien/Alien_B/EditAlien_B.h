#ifndef EDIT_ALIEN_B_H
#define EDIT_ALIEN_B_H

#include "..\EditAlien.h"

class CEditAlienB : public CEditAlien
{
	const char* MODEL_NAME = "b_s";
public:
	CEditAlienB();
	virtual	~CEditAlienB();

	// ‰Šú‰»ŠÖ”.
	virtual bool Init() override;
	// XVŠÖ”.
	virtual void Update() override;
	// •`‰æŠÖ”.
	virtual void Render() override;
	// “–‚½‚è”»’èŠÖ”.
	virtual void Collision( CActor* pActor ) override;

	// UŒ‚‚ÌÄ¶.
	virtual void PlayAttack() override;

private:
	// ƒXƒ|[ƒ“.
	virtual void Spawning() override;
	// ˆÚ“®.
	virtual void Move() override;
	// ‰û‚¤.
	virtual void Abduct() override;
	// ‹¯‚İ.
	virtual void Fright() override;
	// €–S.
	virtual void Death() override;
	// “¦‚°‚é.
	virtual void Escape() override;

	// UŒ‚ŠÖ”.
	void Attack();

	// “–‚½‚è”»’è‚Ìİ’è.
	bool ColliderSetting();

private:
	float	m_RotAccValue;		// ‰ñ“]‰Á‘¬’l.
	bool	m_IsAttackSE;		// UŒ‚SE‚ğ–Â‚ç‚·‚©.
};

#endif	// #ifndef EDIT_ALIEN_B_H.
