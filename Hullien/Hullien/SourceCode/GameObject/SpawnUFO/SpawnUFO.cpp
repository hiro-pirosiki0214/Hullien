#include "SpawnUFO.h"
#include "..\..\Common\Mesh\Dx9StaticMesh\Dx9StaticMesh.h"
#include "..\..\Resource\MeshResource\MeshResource.h"
#include "..\UFOLight\UFOLight.h"
#include "..\Actor\Character\Alien\Alien_A\Alien_A.h"
#include "..\Actor\Character\Alien\Alien_B\Alien_B.h"
#include "..\Actor\Character\Alien\Alien_C\Alien_C.h"
#include "..\Actor\Character\Alien\Alien_D\Alien_D.h"

#include "..\..\Collider\CollsionManager\CollsionManager.h"

CSpawnUFO::CSpawnUFO()
	: m_pStaticMesh				( nullptr )
	, m_pCollManager			( nullptr )
	, m_pUFOLight				( nullptr )
	, m_AilenList				()
	, m_SpawnParameter			()
	, m_SpawnPoint				{ 0.0f, 0.0f, 0.0f }
	, m_pAbductUFOPosition		( nullptr )
	, m_pAlienParamList			( nullptr )
	, m_FrameCount				( 0 )
	, m_SpawnCount				( 0 )
	, m_AlienIndex				( 0 )
	, m_AlienSpawnCount			( 0 )
	, m_IsDisp					( true ) 
	, m_RandomSeed				()
{
	// ランダムシードの初期化.
	std::random_device rd;
	m_RandomSeed = std::mt19937( rd() );
	m_pUFOLight = std::make_shared<CUFOLight>( 1.65f, 1.4f );
}

CSpawnUFO::~CSpawnUFO()
{
}

// 初期化関数.
bool CSpawnUFO::Init()
{
	if( GetModel() == false ) return false;
	if( CollisionSetting() == false ) return false;
	if( m_pUFOLight->Init() == false ) return false;
	return true;
}

// 当たり判定(イベントで使用).
D3DXVECTOR3 CSpawnUFO::Collision(CActor * pActor)
{
	// 対象オブジェクトじゃなければ終了.
	if ((pActor->GetObjectTag() != EObjectTag::Player) &&
		(pActor->GetObjectTag() != EObjectTag::Girl)) return pActor->GetPosition();

	if (m_pCollManager->IsShereToShere(pActor->GetCollManager()) == false) return pActor->GetPosition();

	D3DXVECTOR3 pos = pActor->GetPosition();
	pos.y += ADD_POS_POWER;	// 座標を上にあげる.

	return pos;
}

// 更新関数.
void CSpawnUFO::Update()
{
	m_SpawnCount++;
	m_pUFOLight->Update();
}

// 描画関数.
void CSpawnUFO::Render()
{
	// 画面外なら終了.
	if( IsDisplayOut() == true ) return;
	if( m_pStaticMesh == nullptr ) return;
	if( m_IsDisp == false ) return;

	m_pStaticMesh->SetPosition( m_vPosition );
	m_pStaticMesh->SetScale( m_vScale );
	m_pStaticMesh->SetRasterizerState( ERS_STATE::Back );
	m_pStaticMesh->Render();
	m_pStaticMesh->SetRasterizerState( ERS_STATE::None );

	m_pUFOLight->SetPosition( m_vPosition );
	m_pUFOLight->Render();
}

// 宇宙人をスポーンさせる.
void CSpawnUFO::SpawnAlien( std::vector<std::shared_ptr<CAlien>>& alienList )
{
	// 宇宙人リストのサイズが最大数より多ければ終了.
	if( (int)alienList.size() >= m_SpawnParameter.MaxAlienCount ) return;

	Update();	// 更新関数.

	if( m_SpawnCount < m_SpawnParameter.SpawnTime*FPS ) return;
	if( m_pAbductUFOPosition == nullptr ) return;

	alienList.emplace_back( m_AilenList[m_AlienSpawnCount] );	// 宇宙人の追加.

	// リストにあったパラメータとスポーン座標を設定し、スポーンさせる.
	alienList.back()->Spawn( m_SpawnPoint );
	// 連れ去るUFOの座標を設定.
	alienList.back()->SetAbductUFOPosition( m_pAbductUFOPosition );
	// アイテムの設定.
	alienList.back()->SetItem( ProbabilityGetItem( static_cast<EAlienList>(m_AlienIndex) == EAlienList::D ) );
	m_SpawnCount = 0;
	m_AlienSpawnCount++;
}

// 宇宙人のパラメータリストを設定する.
void CSpawnUFO::SetAlienParameterList( std::vector<SAlienParam>* alienParamList )
{
	m_pAlienParamList = alienParamList;
	m_AilenList.reserve( m_SpawnParameter.MaxAlienCount );
	for( int i = 0; i < m_SpawnParameter.MaxAlienCount; i++ ){
		m_AilenList.emplace_back( AlienFactory() );	// 宇宙人の追加.
		m_AilenList.back()->Init();
	}
}

// スポーンパラメータの設定.
void CSpawnUFO::SetSpawnParameter( const SSpawnUFOParam& param )
{ 
	m_SpawnParameter = param;
	m_SpawnPoint	= m_vPosition = m_SpawnParameter.Position;
	m_SpawnPoint.y	= m_SpawnParameter.SpawnPointHight;
	m_SpawnCount = m_SpawnParameter.SpawnTime*FPS/2;
}

// ライトを取り出す.
void CSpawnUFO::LightDischarge()
{
	m_pUFOLight->Discharge();
}

// ライトをしまう.
void CSpawnUFO::LightCleanUP()
{
	m_pUFOLight->CleanUP();
}

// 光を完全に放出する.
void CSpawnUFO::DischargePreparation()
{
	m_pUFOLight->DischargePreparation();
}

// 光を片づける.
void CSpawnUFO::CleanUPPreparation()
{
	m_pUFOLight->CleanUPPreparation();
}

// 宇宙人の作成.
std::shared_ptr<CAlien> CSpawnUFO::AlienFactory()
{
	// 宇宙人番号の作成.
	const EAlienList alienNo = static_cast<EAlienList>(GetAlienNo());

	switch( alienNo )
	{
	case EAlienList::A:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienA>( &m_pAlienParamList->at(m_AlienIndex) );

	case EAlienList::Ada:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienA>( &m_pAlienParamList->at(m_AlienIndex) );

	case EAlienList::B:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienB>( &m_pAlienParamList->at(m_AlienIndex) );

	case EAlienList::Bda:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienB>( &m_pAlienParamList->at(m_AlienIndex) );

	case EAlienList::C:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienC>( &m_pAlienParamList->at(m_AlienIndex) );

	case EAlienList::D:
		m_AlienIndex = static_cast<int>(alienNo);
		return std::make_shared<CAlienD>( &m_pAlienParamList->at(m_AlienIndex) );
	default:
		break;
	}
	return nullptr;
}

// 宇宙人番号の作成.
int CSpawnUFO::GetAlienNo()
{
	if( (rand()%100) < m_SpawnParameter.ProbabilityD ){
		// 宇宙人Dを含める.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::D),		// 最大値.
			static_cast<int>(EAlienList::C) );		// 除外する値.
	} else if( (rand()%100) < m_SpawnParameter.ProbabilityC ){
		// 宇宙人Cを含める.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::C),		// 最大値.
			static_cast<int>(EAlienList::D) );		// 除外する値.
	} else {
		// 宇宙人C、Dを含めない.
		return CreateAlienNo( 
			static_cast<int>(EAlienList::First),	// 最小値.
			static_cast<int>(EAlienList::Bda),		// 最大値.
			static_cast<int>(EAlienList::C) );		// 除外する値.
	}
}

int CSpawnUFO::CreateAlienNo( const int& min, const int& max, const int& outVal )
{
	// ランダム値の最大と最小の値を設定.
	std::uniform_int_distribution<int> alienNoMaxMin( min, max );
	int outAlienNo = alienNoMaxMin( m_RandomSeed );
	if( outVal == outAlienNo ){
		// 除外する値になった場合、再度作り直す.
		outAlienNo = CreateAlienNo( min, max, outVal );
	}
	return outAlienNo;
}

// アイテムを取得する.
EItemList CSpawnUFO::ProbabilityGetItem( const bool& isAlienD )
{
	// 乱数が確率値より少ない場合　または　宇宙人Dの場合.
	if(( (rand()%100) < m_SpawnParameter.ProbabilityItem ) || ( isAlienD == true )){
		// ランダム値の最大と最小の値を設定.
		std::uniform_int_distribution<int> itemNoMaxMin( 
			static_cast<int>(EItemList::First),
			static_cast<int>(EItemList::Last) );
		// アイテム番号を取得する.
		return static_cast<EItemList>(itemNoMaxMin( m_RandomSeed ));
	} else {
		return EItemList::None;	// アイテムなし.
	}
}

// モデルの取得.
bool CSpawnUFO::GetModel()
{
	if( m_pStaticMesh != nullptr ) return false;
	CMeshResorce::GetStatic( m_pStaticMesh, MODEL_NAME );
	if( m_pStaticMesh == nullptr ) return false;
	return true;
}

// 当たり判定の設定.
bool CSpawnUFO::CollisionSetting()
{
	if (m_pCollManager == nullptr) {
		m_pCollManager = std::make_shared<CCollisionManager>();
	}
	if (FAILED(m_pCollManager->InitSphere(
		&m_vPosition,
		&m_vRotation,
		&m_vScale.x,
		{ 0.0f, 0.0f, 0.0f },
		COLLISION_RADIUS))) {
		return false;
	}
	return true;
}