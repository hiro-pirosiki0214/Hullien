#ifndef GIRL_EDIT_H
#define GIRL_EDIT_H

#include "..\EditBase.h"
#include "..\..\GameObject\Actor\Character\Girl\Girl.h"

class CGirlEdit : public CEditBase
{
	// パラメータのファイルパス.
	const char* PARAMETER_FILE_PATH = "Data\\GameParam\\Girl\\Girl.bin";
public:
	CGirlEdit();
	virtual ~CGirlEdit();

	// 初期化関数.
	virtual bool Init() override;
	// 更新関数.
	virtual void Update() override;
	// 描画関数.
	virtual void Render() override;
	// モデルの描画.
	virtual void ModelRender() override;
	// エフェクトの描画.
	virtual void EffectRender() override;

private:
	std::unique_ptr<CGirl>	m_pGirl;
	SGirlParam				m_GirlParam;
};


#endif	// #ifndef GIRL_EDIT_H.