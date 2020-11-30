#include "GameObject.h"
#include "..\Camera\CameraManager\CameraManager.h"
#include "..\Common\SceneTexRenderer\SceneTexRenderer.h"


/****************************************
*	�Q�[���I�u�W�F�N�g���N���X.
**/
CGameObject::CGameObject()
	: m_ObjectTag	( EObjectTag::None )
	, m_vPosition	( 0.0f, 0.0f, 0.0f )
	, m_vRotation	( 0.0f, 0.0f, 0.0f )
	, m_vScale		( 1.0f, 1.0f, 1.0f )
{
}

CGameObject::~CGameObject()
{
}

//-----------------------------.
// ��ʂ̊O�ɏo�Ă��邩.
//-----------------------------.
bool CGameObject::IsDisplayOut( const float& adjSize )
{
	// �r���[�|�[�g�s��.
	D3DXMATRIX mViewport;
	D3DXMatrixIdentity( &mViewport );	// �P�ʍs��쐬.
	mViewport._11 = static_cast<float>( WND_W ) *  0.5f;
	mViewport._22 = static_cast<float>( WND_H ) * -0.5f;
	mViewport._41 = static_cast<float>( WND_W ) *  0.5f;
	mViewport._42 = static_cast<float>( WND_H ) *  0.5f;

	// ���[���h, �r���[, �v���W�F, �r���[�|�[�g�̌v�Z.
	D3DXMATRIX m = CCameraManager::GetViewMatrix() * CCameraManager::GetProjMatrix() * mViewport;
	D3DXVECTOR3 screenPos = { 0.0f, 0.0f, 0.0f };	// �X�N���[�����W.
	// D3DXVECTOR3�Ɗ|�����킹.
	D3DXVec3TransformCoord( &screenPos, &m_vPosition, &m );

	if( -adjSize < screenPos.x && screenPos.x < WND_W + adjSize &&
		-adjSize < screenPos.y && screenPos.y < WND_H + adjSize ) return false;	// ��ʓ�.
	// ��ʊO�ł��V���h�E�͕`�悷��.
	if( CSceneTexRenderer::GetRenderPass() == CSceneTexRenderer::ERenderPass::Shadow ) return false;
	
	return true;	// ��ʊO.
}