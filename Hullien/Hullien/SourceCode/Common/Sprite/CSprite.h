#ifndef CSPRITE_UI_H
#define CSPRITE_UI_H

#include "..\Common.h"
#include "..\Shader\SpriteShader\SpriteShader.h"

// �������\����(float�^).
struct WHDIZE_FLOAT
{
	float w;	// ��.
	float h;	// ����.
};

enum class enLocalPosition : unsigned char
{
	None,

	LeftUp = 0,	// ����.
	Left,		// ��.
	LeftDown,	// ����.
	Down,		// ��.
	RightDown,	// �E��.
	Right,		// �E.
	RightUp,	// �E��.
	Up,			// ��.
	Center,		// ����.

	Max,
} typedef ELocalPosition;

// �X�v���C�g�\����.
struct stSpriteState
{
	int				LocalPosNum;// ���[�J�����W��
	WHDIZE_FLOAT	Disp;		// �\����,����.
	WHDIZE_FLOAT	Base;		// ���摜�̕�,����.
	WHDIZE_FLOAT	Stride;		// 1�R�}������̕�,����.
	D3DXVECTOR3		vPos;		// ���W.
	int				AnimNum;	// �A�j���[�V������.

	enum enSPRITE_STATE : unsigned char
	{
		None,

		enLocalPosNum = 0,
		enDisp_w,
		enDisp_h,
		enBase_w,
		enBase_h,
		enStride_w,
		enStride_h,
		envPos_x,
		envPos_y,
		envPos_z,
		enAnimNum,

		MAX = enAnimNum,
	} typedef ESPRITE_STATE;
} typedef SSpriteState;

class CSprite : public CCommon
{
public:
	CSprite();
	CSprite(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	virtual ~CSprite();

	// ������.
	HRESULT Init(
		ID3D11Device* pDevice11,
		ID3D11DeviceContext* pContext11,
		const char* fileName,
		const SSpriteState& pSs);

	// ���.
	void Release();

	// �����_�����O.
	// 3D��Ԃɔz�u.
	void Render(const bool& isBillboard = false);
	// UI�Ƃ��Ĕz�u.
	void RenderUI();
	// �A�j���[�V�����ԍ��̐ݒ�.
	void SetAnimNumber(const int& animNumber);
	// �t���[���^�C���̐ݒ�(���t���[���ŃA�j���[�V�������邩).
	void SetFrameTime(const int& frameTime) { m_FrameTime = frameTime; }
	// �A�j���[�V�������Đ�����.
	void PlayAnimation() { m_IsAnimation = true; }
	// �\���摜�T�C�Y�̎擾.
	D3DXVECTOR2 GetSpriteSize() { return { m_SState.Disp.w, m_SState.Disp.h }; }
	// �`����W�̎擾.
	D3DXVECTOR3 GetRenderPos() { return m_SState.vPos; }

private:
	// ���f���쐬.
	HRESULT InitModel();
	// �e�N�X�`���쐬.
	HRESULT CreateTexture(const char* fileName, ID3D11ShaderResourceView** pTexture);
	HRESULT InitSample();

	D3DXMATRIX CreateWorldMatrix();
	// UV���W�ɕϊ�.
	D3DXVECTOR2 ConvertIntoUV();
	// �A�j���[�V�����̍X�V.
	void AnimUpdate();

	void CreateVERTEX(
		const float& w, const float& h,
		const float& u, const float& v);


	// �e�N�X�`���̔䗦���擾.
	int myGcd(int t, int t2)
	{
		if (t2 == 0) return t;
		return myGcd(t2, t % t2);
	}
private:
	std::unique_ptr<CSpriteShader> m_pSpriteShader;	// �X�v���C�g�V�F�[�_�[.
	ID3D11InputLayout*		m_pVertexLayout;		// ���_���C�A�E�g.
	ID3D11Buffer*			m_pVertexBuffer;		// ���_�o�b�t�@.
	ID3D11Buffer*			m_pVertexBufferUI;		// ���_�o�b�t�@.

	ID3D11SamplerState*		m_pSampleLinear;		// �T���v��:�e�N�X�`���Ɋe��t�B���^��������.
	ID3D11ShaderResourceView*	m_pTexture;			// �e�N�X�`��.

	SSpriteState	m_SState;			// �X�v���C�g���.
	POINT			m_PatternNo;		// �p�^�[���ԍ�.
	POINT			m_PatternMax;		// �p�^�[���ő吔.
	D3DXVECTOR2		m_UV;				// �e�N�X�`��UV���W.
	int				m_AnimNumber;		// �A�j���[�V�����i���o�[.
	int				m_FrameCount;		// �t���[���J�E���g.
	int				m_FrameCountSpeed;	// �t���[���J�E���g���x.
	int				m_FrameTime;		// �t���[���^�C��.
	bool			m_IsAnimation;		// �A�j���[�V�������邩.
	enLocalPosition m_enLocalPosition;	// ���[�J�����W�̎��.
	CSpriteShader::VERTEX m_VerticesUI[4];
};

#endif	// #ifndef CSPRITE_UI_H.