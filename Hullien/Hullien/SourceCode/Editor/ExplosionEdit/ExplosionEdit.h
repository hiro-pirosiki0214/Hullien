#ifndef EXPLOSION_EDIT_H
#define EXPLOSION_EDIT_H

#include "..\..\Utility\ImGuiManager\ImGuiManager.h"
#include "..\..\GameObject\Actor\Explosion\Explosion.h"
#include <vector>

class CExplosionEdit
{
	const ImVec2 WINDOW_SIZE = { 510.0f, WND_H-70.0f };	// ウィンドウサイズ.
	const ImVec2 RENDER_POSITION = { 0.0f, 70.0f };		// 描画座標.
	const char* FILE_PATH = "Data\\GameParam\\Alien\\Explosion\\Explosion.bin";

public:
	CExplosionEdit();
	~CExplosionEdit();

	// 初期化関数.
	bool Init();
	// 描画関数.
	void Render();

private:
	// ファイルの読み込み.
	bool FileReading();
	// ファイルの書き込み.
	bool FileWriting();

private:
	CExplosion::SExplosionParam m_ExplosionParam;	// 爆発パラメーター.
};

#endif	// #ifndef EXPLOSION_EDIT_H.