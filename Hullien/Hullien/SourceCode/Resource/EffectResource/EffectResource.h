#ifndef EFFECT_RESOURCE_H
#define EFFECT_RESOURCE_H

#include "..\..\Global.h"

#include <unordered_map>
#include <string>
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

class CEffect;

/**********************************************
*	�G�t�F�N�g���\�[�X�N���X (�V���O���g��).
*		
*/
class CEffectResource
{
	const char* FILE_PATH = "Data\\Effect";	// �G�t�F�N�g�t�@�C���p�X.
	typedef std::unordered_map<std::string, std::shared_ptr<CEffect>> effect_map;
public:
	CEffectResource();
	~CEffectResource();
	// �C���X�^���X�̎擾.
	static CEffectResource* GetInstance();

	// �ǂݍ���(���b�p�[).
	static void Load( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );
	// ���.
	static void Release();
	// �G�t�F�N�g�擾�֐�.
	static std::shared_ptr<CEffect> GetEffect( const std::string& fileName );

private:
	// �G�t�F�N�g�̓ǂݍ���.
	void EffectLoad( ID3D11Device* pDevice11, ID3D11DeviceContext* pContext11 );

private:
	effect_map m_EffectList;	// �G�t�F�N�g���X�g.
	bool m_HasFinishedLoading;	// �Ǎ����I�������.

private:
	// �R�s�[�E���[�u�R���X�g���N�^, ������Z�q�̍폜.
	CEffectResource( const CEffectResource & )					= delete;
	CEffectResource& operator = ( const CEffectResource & )		= delete;
	CEffectResource( CEffectResource && )						= delete;
	CEffectResource& operator = ( CEffectResource && )			= delete;
};

#endif	// #ifndef EFFECT_RESOURCE_H.