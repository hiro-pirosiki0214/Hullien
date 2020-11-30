#ifndef SLIDER_H
#define SLIDER_H

#include "..\Widget.h"
#include <vector>

class CSlinder : public CWidget
{
	const char* SPRITE_BACK_BAR_NAME	= "BackBar";
	const char* SPRITE_BAR_NAME			= "Bar";
	const char* SPRITE_ICON_NAME		= "Icon";

	const float DEFALUT_VALUE_MAX = 1.0f;
	const float DEFALUT_VALUE_MIN = 0.0f;
	const float VALUE_MAX;
	const float VALUE_MIN;

	enum enSpriteNo
	{
		ESpriteNo_None = -1,

		ESpriteNo_BackBar,	// ���̃o�[.
		ESpriteNo_Bar,		// �o�[.
		ESpriteNo_Icon,		// �A�C�R��.

		ESpriteNo_Max,
	} typedef ESpriteNo;
public:
	CSlinder();
	CSlinder( const float& valueMax, const float& valueMin );
	virtual ~CSlinder();

	// �������֐�.
	virtual bool Init() override;
	// �X�V�֐�.
	virtual void Update() override;
	// �`��֐�.
	virtual void Render() override;

	// �l�𑝂₷.
	void AddValue( const float& value );
	// �l�����炷.
	void SubValue( const float& value );
	// �l�̐ݒ�.
	inline void SetValue( const float& value ){ m_Value = value; }
	// �l�̎擾.
	inline float GetValue() const { return m_Value; }

	// ���W�ݒ�֐�.
	virtual void SetPosition( const D3DXVECTOR3& vPos ) override;
	// �A�C�R���̍��W�̎擾.
	inline D3DXVECTOR3 GetIconPosition() const { return m_IconPosition; }

private:
	// �A�C�R���̍��W�̐ݒ�.
	void SetIconPosition();

	// �X�v���C�g�ݒ�֐�.
	virtual bool SpriteSetting() override;

private:
	std::vector<std::shared_ptr<CSprite>>	m_pSprites;	// �X�v���C�g�N���X.
	float		m_Value;				// �l.
	D3DXVECTOR3	m_IconPosition;			// �A�C�R���̍��W.
	float		m_BackBarImageWidthSize;// �o�[�̉摜�T�C�Y��.
	D3DXVECTOR3	m_BarImageWidthSize;	// �o�[�̉摜�T�C�Y��.
};

#endif	// #ifndef SLIDER_H.
