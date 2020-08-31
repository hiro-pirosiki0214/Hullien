#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

class CPlayerManager
{
public:
	CPlayerManager();
	~CPlayerManager();

	// ‰Šú‰»ŠÖ”.
	bool Init();
	// XVŠÖ”.
	void Update();
	// •`‰æŠÖ”.
	void Render();
	// UI‚Ì•`‰æŠÖ”.
	void RenderUI();
};
 
#endif	// #ifndef PLAYER_MANAGER_H.