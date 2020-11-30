#ifndef OBJECT_LIST_H
#define OBJECT_LIST_H

enum class enObjectTagList : unsigned char
{
	None,

	Player,				// �v���C���[.
	Girl,				// ���̎q.

	Alien_A,			// �F���lA.
	Alien_B,			// �F���lB.
	Alien_C,			// �F���lC.
	Alien_D,			// �F���lD.

	LifeRecoveryItem,	// �̗͉񕜃A�C�e��.
	SPEffectTimeItem,	// ����\�͉񕜑��xUP�A�C�e��.
	AttackUpItem,		// �U����UP�A�C�e��.
	MoveSpeedUpItem,	// �ړ����xUP�A�C�e��.

	MotherShipUFO,		// �}�U�[�V�b�vUFO.

	LaserBeam,			// ���[�U�[�r�[��.
	Explosion,			// ����.
	Bariier,			// �o���A.

	//-------------------------------------.
	// ��STG�p��.
	STG_Player,		// �v���C���[.
	STG_Enemy,		// �G.
	STG_Bullet,		// �e.

	Max,

} typedef EObjectTag;

#endif	// #ifndef OBJECT_LIST_H.