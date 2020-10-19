#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

enum class enObjectTagList
{
	None,

	Player,				// vC[.
	Girl,				// Ìq.

	Alien_A,			// FlA.
	Alien_B,			// FlB.
	Alien_C,			// FlC.
	Alien_D,			// FlD.

	LifeRecoveryItem,	// ÌÍñACe.
	SPEffectTimeItem,	// Áê\Íñ¬xUPACe.
	AttackUpItem,		// UÍUPACe.
	MoveSpeedUpItem,	// Ú®¬xUPACe.

	LaserBeam,			// [U[r[.
	Explosion,			// ­.
	Bariier,			// oA.

	//-------------------------------------.
	// «STGp«.
	STG_Player,		// vC[.
	STG_Enemy,		// G.
	STG_Bullet,		// e.

	Max,

} typedef EObjectTag;

#endif	// #ifndef OBJECT_LIST_H.