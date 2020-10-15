#ifndef ANIMATION_PARAM_H
#define ANIMATION_PARAM_H

#include <vector>

namespace anim
{

// アニメーションフレーム構造体.
struct stAnimFrame
{
	double NowFrame;	// 経過フレーム.
	double EndFrame;	// 終了フレーム.

	stAnimFrame() : NowFrame ( 0.0 ), EndFrame ( 0.0 )
	{}
	stAnimFrame( const double& nowFrame, const double& endFrame ) 
		: NowFrame	( nowFrame )
		, EndFrame	( endFrame )
	{}

	// 経過フレームが終了フレームを超えているか.
	bool IsNowFrameOver() const { return NowFrame >= EndFrame; }

} typedef SAnimFrame;

// アニメーションのフレーム情報のリスト.
typedef std::vector<SAnimFrame> AAnimFrameList;	

};

#endif	// #ifndef ANIMATION_PARAM_H.