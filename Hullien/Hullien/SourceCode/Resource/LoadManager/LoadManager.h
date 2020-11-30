#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER_H

#include "..\..\Global.h"
#include <thread>
#include <mutex>

#include <memory>
#include <vector>

/*********************************
*	�e���\�[�X�ނ̓ǂݍ��݂��܂Ƃ߂�N���X.
*		Load�摜�Ȃǂ̕\���������ɓ���Ă���������.
*/
class CLoadManager
{
	const char* LOAD_BACK_IMAGE = "Data\\Texture\\_LoadTexture\\LoadBackImage.png";
	const char* LOAD_ANIM_IMAGE = "Data\\Texture\\_LoadTexture\\LoadAnim.png";
	const char* LOAD_TEXT_IMAGE = "Data\\Texture\\_LoadTexture\\LoadText.png";

public:
	CLoadManager();
	~CLoadManager();

	// �ǂݍ���.
	void LoadResource( 
		HWND hWnd, 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11, 
		LPDIRECT3DDEVICE9 pDevice9  );
	// �X���b�h�̉��.
	bool ThreadRelease();

	// �`��.
	void Render();
	// ���[�h���s.
	inline bool IsLoadFailed() const { return m_isLoadFailed; }

private:
	// �X�v���C�g�̎擾.
	HRESULT GetSprite( 
		ID3D11Device* pDevice11, 
		ID3D11DeviceContext* pContext11 );

private:
	std::thread m_Thread;		// �X���b�h.
	std::mutex	m_Mutex;
	std::vector<std::shared_ptr<CSprite>>	m_Sprites;
	
	bool m_isLoadEnd;			// ���[�h���I��������.
	bool m_isThreadJoined;		// �X���b�h��������ꂽ��.
	bool m_isLoadFailed;		// ���[�h���s.
	bool m_isLoadSpriteFailed;	// ���[�h�X�v���C�g�̓ǂݍ��ݎ��s.
};

#endif	// #ifndef LOAD_MANAGER_H.