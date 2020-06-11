#include "Alien.h"
#include "..\..\..\..\Resource\MeshResource\MeshResource.h"
#include "..\..\..\..\Common\Mesh\Dx9SkinMesh\Dx9SkinMesh.h"

CAlien::CAlien()
	: m_GirlPosition	( 0.0f, 0.0f, 0.0f )
	, m_TargetRotation	( 0.0f, 0.0f, 0.0f )
	, m_NowState		( EAlienState::None )
{
}

CAlien::~CAlien()
{
}

// 相手座標の設定.
void CAlien::SetTargetPos( CActor& actor )
{
	// 女の子じゃなければ終了.
	// 今は女の子がいないのでplyaerで対処.
	if( actor.GetObjectTag() != EObjectTag::Player ) return;
	m_GirlPosition = actor.GetPosition();	// 女の子の座標を取得.
}

// 現在の状態の更新関数.
void CAlien::CurrentStateUpdate()
{
	switch( m_NowState )
	{
	case EAlienState::Spawn:
		this->Spawning();
		break;
	case EAlienState::Move:
		this->Move();
		break;
	case EAlienState::Abduct:
		this->Abduct();
		break;
	case EAlienState::Fright:
		this->Fright();
		break;
	case EAlienState::Death:
		this->Death();
		break;
	case EAlienState::Escape:
		this->Escape();
		break;
	default:
		break;
	}
}

// 移動ベクトル設定関数.
void CAlien::SetMoveVector( const D3DXVECTOR3& targetPos )
{
	// 目的の回転軸を取得.
	m_TargetRotation.y = atan2f( 
		targetPos.x - m_vPosition.x,
		targetPos.z - m_vPosition.z );

	// 移動用ベクトルを取得.
	m_MoveVector.x = sinf( m_TargetRotation.y );
	m_MoveVector.z = cosf( m_TargetRotation.y );
}

// 目的の座標へ回転.
void CAlien::TargetRotation()
{
	// 自身のベクトルを用意.
	D3DXVECTOR3 myVector = { 0.0f, 0.0f ,0.0f };
	myVector.x = sinf( m_vRotation.y );
	myVector.z = cosf( m_vRotation.y );

	// ベクトルの長さを求める.
	float myLenght = sqrtf(myVector.x*myVector.x + myVector.z*myVector.z);
	float targetLenght = sqrtf(m_MoveVector.x*m_MoveVector.x + m_MoveVector.z*m_MoveVector.z);

	// 目的のベクトルと、自分のベクトルの外積を求める.
	float cross = myVector.x * m_MoveVector.z-myVector.z * m_MoveVector.x;
	float dot = myVector.x * m_MoveVector.x+myVector.z * m_MoveVector.z;
	dot = acosf( dot /( myLenght * targetLenght ));

	// 外積が0.0より少なければ 時計回り : 反時計回り に回転する.
	m_vRotation.y += cross < 0.0f ? ROTATIONAL_SPEED : -ROTATIONAL_SPEED;

	// 内積が許容範囲なら.
	if( -TOLERANCE_RADIAN < dot && dot < TOLERANCE_RADIAN ){
		m_vRotation.y = m_TargetRotation.y;	// ターゲットへの回転取得.
		// 移動用ベクトルを取得.
		m_MoveVector.x = sinf( m_vRotation.y );
		m_MoveVector.z = cosf( m_vRotation.y );
	}
}

// 移動関数.
void CAlien::VectorMove( const float& moveSpeed )
{
	float lenght = D3DXVec3Length( &D3DXVECTOR3(m_GirlPosition - m_vPosition) );

	if( lenght <= 0.05f ) return;

	m_vPosition.x -= sinf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;
	m_vPosition.z -= cosf( m_vRotation.y+static_cast<float>(D3DX_PI) ) * moveSpeed;

}