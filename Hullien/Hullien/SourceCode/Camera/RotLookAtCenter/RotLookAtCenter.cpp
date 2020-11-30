#include "RotLookAtCenter.h"
#include "..\..\Utility\FileManager\FileManager.h"
#include "..\..\Utility\MyVector\MyVector.h"

CRotLookAtCenter::CRotLookAtCenter()
	: m_vDegree			{ 0.0f, 0.0f }
	, m_Length			( DEFAULT_LENGTH )
	, m_HorizontalMax	( DEFAULT_HORIZONTAL_MAX )
	, m_HorizontalMin	( DEFAULT_HORIZONTAL_MIN )
	, m_VerticalMax		( DEFAULT_VERTICAL_MAX )
	, m_VerticalMin		( DEFAULT_VERTICAL_MIN )
	, m_RightMoveSpeed	( DEFAULT_MOVE_SPEED )
	, m_LeftMoveSpeed	(-DEFAULT_MOVE_SPEED )
{
}

CRotLookAtCenter::CRotLookAtCenter( const bool& isLoad )
	: CRotLookAtCenter	()
{
	if( isLoad == false ) return;
	SConfigState conState;
	if( CFileManager::BinaryReading( CONFIG_FILE_PATH, conState ) == false ) return;
	// �J�����𔻒肷�邩.
	if( conState.IsReverse == true ){
		// ���].
		m_RightMoveSpeed	= -conState.MoveSpeed;
		m_LeftMoveSpeed		=  conState.MoveSpeed;
	} else {
		// �ʏ�.
		m_RightMoveSpeed	=  conState.MoveSpeed;
		m_LeftMoveSpeed		= -conState.MoveSpeed;
	}
}

CRotLookAtCenter::~CRotLookAtCenter()
{
}

// �X�V.
void CRotLookAtCenter::Update()
{
}

//-------------------------------.
// �I�u�W�F�N�g���𒍎��ɉ�].
//-------------------------------.
void CRotLookAtCenter::RotationLookAtObject( const D3DXVECTOR3& vLookPos, const float& attenRate )
{
	// �����ʒu���擾.
	m_vLookPosition = vLookPos;

	D3DXVECTOR3 pos;	// �{���̍��W.
	// �J�����ʒu���Z�o.
	pos.x = m_vLookPosition.x + ( sinf(m_vDegree.x) * m_Length );
	pos.y = m_vPosition.y;
	pos.z = m_vLookPosition.z + ( cosf(m_vDegree.x) * m_Length );
	// lerp����. ( out, ���݂̍��W, �{�����B���Ă�����W, �␳�l ).
	D3DXVec3Lerp( &m_vPosition, &m_vPosition, &pos, attenRate );
}

//-------------------------------.
// �������̉�].
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalMove( const float& movePower )
{
	// �������ɑ������킹��.
	m_vDegree.x += movePower;

	// �I�[�o�[�t���[�΍�.
	CVector::OverflowDegree( m_vDegree.x, 
		 m_HorizontalMax, m_HorizontalMin, 
		-m_HorizontalMax, m_HorizontalMin );
}

//-------------------------------.
// �������̉E��].
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalRightMove()
{
	DegreeHorizontalMove( m_RightMoveSpeed );
}

//-------------------------------.
// �������̍���].
//-------------------------------.
void CRotLookAtCenter::DegreeHorizontalLeftMove()
{
	DegreeHorizontalMove( m_LeftMoveSpeed );
}

//-------------------------------.
// �c�����̉�].
//-------------------------------.
void CRotLookAtCenter::DegreeVerticalMove( const float& movePower )
{
	// �c�����ɑ������킹��.
	m_vDegree.y += movePower;

	// �I�[�o�[�t���[�΍�.
	CVector::OverflowDegree( m_vDegree.y, 
		m_VerticalMax, m_VerticalMax, 
		m_VerticalMin, m_VerticalMin );
}

//-------------------------------.
// �J�����ƒ����_�̒����ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetLength( const float& length )
{
	m_Length = length;
}

//-------------------------------.
// �����̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetHeight( const float& height )
{
	m_vPosition.y = height;
}

//-------------------------------.
// �������̍ő�A�ŏ��̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetHorizontalDegree( const float& max, const float& min )
{
	m_HorizontalMax	= max;
	m_HorizontalMin	= min;
}

//-------------------------------.
// �c�����̍ő�A�ŏ��̐ݒ�.
//-------------------------------.
void CRotLookAtCenter::SetVerticalDegree( const float& max, const float& min )
{	
	m_VerticalMax = max;
	m_VerticalMin = min;
}