#ifndef C_XAUDIO2_H
#define C_XAUDIO2_H

#include <xaudio2.h>
#include <xaudio2fx.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"xaudio2.lib")

//--------------------------
// セーフ開放マクロ.

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=nullptr; } }
#endif  
#ifndef SAVE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=nullptr; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=nullptr; } }
#endif

#endif//#ifndef C_XAUDIO2_H.