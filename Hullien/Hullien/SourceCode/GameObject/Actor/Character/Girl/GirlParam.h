#ifndef GIRL_PARAM_H
#define GIRL_PARAM_H

#include "..\Character.h"

// ���̎q�p�����[�^�[.
struct stGirlParam : public CCharacter::SParameter
{
	float		RotatlonalSpeed;	// ��]���x.
	D3DXVECTOR3 InitPosition;		// �������W.
	float		InitPosLenght;		// �������W�̋��e�̒���.
	float		SearchCollRadius;	// ���G�p�̃X�t�B�A�̔��a.
	D3DXVECTOR3 SphereAdjPos;		// �X�t�B�A�̒������W.
	float		SphereAdjRadius;	// �X�t�B�A�̒������a.

	stGirlParam()
		: RotatlonalSpeed	( 0.0f )
		, InitPosition		( 0.0f, 0.0f, 0.0f )
		, InitPosLenght		( 1.0f )
		, SearchCollRadius	( 10.0f )
		, SphereAdjPos		( 0.0f, 0.0f, 0.0f )
		, SphereAdjRadius	( 0.0f )
	{}
} typedef SGirlParam;

namespace girl
{
	enum enAnimNo : char
	{
		EAnimNo_None = -1,

		EAnimNo_Wait,	// �ҋ@.
		EAnimNo_Move,	// �ړ�.
		EAnimNo_Abduct,	// �A�ꋎ���Ă���.

		EAnimNo_Max,

		EAnimNo_Begin	= EAnimNo_Wait,
		EAnimNo_End		= EAnimNo_Max,

		Abduct_Max,
	} typedef EAnimNo;
}

#endif	// #ifndef GIRL_PARAM_H.