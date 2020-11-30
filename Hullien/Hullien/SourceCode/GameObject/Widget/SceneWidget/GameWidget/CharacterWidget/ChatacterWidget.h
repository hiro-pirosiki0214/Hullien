#ifndef CHARACTERWIDGET_H
#define CHARACTERWIDGET_H

#include "..\..\SceneWidget.h"
class CCharacter;

/*****************************************
*	�L�����N�^UI�N���X.
**/
class CCharacterWidget : public CSceneWidget
{
public:
	// �L�����N�^�̃p�����[�^.
	struct stCharacterParam
	{
		float Life;						//���C�t.
		float LifeMax;					//���C�t�ő吔.
		float SpecialAbility;			//����Q�[�W
		float SpecialAbilityMax;		//����Q�[�W�ő吔.
		stCharacterParam()
			: Life				( 0.0f )
			, LifeMax			( 0.0f )
			, SpecialAbility	( 0.0f )
			, SpecialAbilityMax	( 0.0f )
		{}

	}typedef SCharacterParam;


	// �Q�[�W�̏��.
	struct stGaugeState
	{
		D3DXVECTOR3 vPosition;	//�ʒu.
		D3DXVECTOR3	vScale;		//�傫��.
		int						AnimNum;	//�A�j���[�V�����ԍ�.

		stGaugeState()
			: vPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f))
			, vScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f))
			, AnimNum(0)
		{}
	}typedef SGaugeState;

public:
	CCharacterWidget();
	virtual ~CCharacterWidget();

	// �p�����[�^�ݒ�֐�.
	virtual void SetParameter( SCharacterParam& pChara ) {m_Parameter = pChara;}

protected:
	SCharacterParam m_Parameter;							//�p�����[�^.

private:
};

#endif	//#ifndef CHARACTERWIDGET_H.
