#include "STGEnemyFactory.h"
#include "..\STGEnemy.h"
#include "..\STGEnemyParam.h"
#include "..\..\..\..\..\Utility\FileManager\FileManager.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

STG::CEnemyFactory::CEnemyFactory()
{
}

STG::CEnemyFactory::~CEnemyFactory()
{
}

// 敵の作成.
bool STG::CEnemyFactory::Create( std::vector<std::shared_ptr<STG::CEnemy>>& enemys )
{
	std::fstream fileStream( FILE_NAME );
	// ファイルが開けない場合.
	if( !fileStream.is_open() ){
		ERROR_MESSAGE( "STG敵ファイル読み込み失敗" );
		return false;
	}

	std::string line;	// 1行分を読み込むための文字.
	while( std::getline( fileStream, line ) ){
		std::string buff = "";		// 文字を一時的に格納する.
		std::istringstream stream( line );

		int no = STG::EEnemyParamNo::EEnemyParamNo_Start;
		STG::SEnemyParam enemyParam;
		// デフォルト ',' 区切りで文字を取得.
		while( std::getline( stream, buff, ',' ) ){
			switch( no )
			{
			case EEnemyParamNo_SpawnTime:
				enemyParam.SpawnTime = std::stof(buff);
				break;
			case EEnemyParamNo_PositionX:
				enemyParam.PositionX = std::stof(buff);
				break;
			case EEnemyParamNo_Text:
				enemyParam.Text = buff;
				break;
			case EEnemyParamNo_TextSize:
				enemyParam.TextSize = std::stof(buff);
				break;
			case EEnemyParamNo_LifePoint:
				enemyParam.LifePoint = std::stof(buff);
				break;
			case EEnemyParamNo_MoveSpeed:
				enemyParam.MoveSpeed = std::stof(buff);
				break;
			case EEnemyParamNo_BulletSpeed:
				enemyParam.BulletSpeed = std::stof(buff);
				break;
			case EEnemyParamNo_BulletCollDisappear:
				enemyParam.BulletCollDisappear = std::stoi(buff);
				break;
			case EEnemyParamNo_BulletAngle:
				enemyParam.BulletAngle =  static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EEnemyParamNo_BulletCountMax:
				enemyParam.BulletCountMax = std::stoi(buff);
				break;
			case EEnemyParamNo_AnyBulletCountMax:
				enemyParam.AnyBulletCountMax = std::stoi(buff);
				break;
			case EEnemyParamNo_ShotIntervalFrame:
				enemyParam.ShotIntervalFrame = std::stoi(buff);
				break;
			case EEnemyParamNo_ShotBulletCount:
				enemyParam.ShotBulletCount = std::stoi(buff);
				break;
			case EEnemyParamNo_ShotAngle:
				enemyParam.ShotAngle = static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EEnemyParamNo_ShotNumber:
				enemyParam.ShotNumber = std::stoi(buff);
				break;
			default:
				break;
			}
			no++;
		}
		enemys.emplace_back( std::make_shared<STG::CEnemy>(enemyParam) );
	}
	return true;
}
