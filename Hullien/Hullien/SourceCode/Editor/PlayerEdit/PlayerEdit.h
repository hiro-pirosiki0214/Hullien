#ifndef PLAYER_EDIT_H
#define PLAYER_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Player\Player.h"

class CPlayerEdit : public CEditBase
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Player\\Player.bin";
public:
	CPlayerEdit();
	virtual ~CPlayerEdit();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// モデルの描画.
	virtual void ModelRender() override;

private:
	std::unique_ptr<CPlayer> m_pPlayer;
};

#endif	// #ifndef PLAYER_EDIT_H.
