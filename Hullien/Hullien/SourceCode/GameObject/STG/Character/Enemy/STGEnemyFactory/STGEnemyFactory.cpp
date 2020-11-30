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

// �G�̍쐬.
bool STG::CEnemyFactory::Create( std::vector<std::shared_ptr<STG::CEnemy>>& enemys )
{
	std::fstream fileStream( FILE_NAME );
	// �t�@�C�����J���Ȃ��ꍇ.
	if( !fileStream.is_open() ){
		ERROR_MESSAGE( "STG�G�t�@�C���ǂݍ��ݎ��s" );
		return false;
	}

	float posY = 0.0f;
	std::string line;	// 1�s����ǂݍ��ނ��߂̕���.
	while( std::getline( fileStream, line ) ){
		std::string buff = "";		// �������ꎞ�I�Ɋi�[����.
		std::istringstream stream( line );

		int no = STG::EEnemyParamNo::EEnemyParamNo_Start;
		STG::SEnemyParam enemyParam;
		// �f�t�H���g ',' ��؂�ŕ������擾.
		while( std::getline( stream, buff, ',' ) ){
			switch( no )
			{
			case EEnemyParamNo_SpawnTime:
				enemyParam.SpawnTime = std::stof(buff) * FPS;
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
				enemyParam.BulletSpeed = std::stof(buff) * 2.0f;
				break;
			case EEnemyParamNo_BulletCountMax:
				enemyParam.BulletCountMax = std::stoi(buff);
				break;
			case EEnemyParamNo_AnyBulletCountMax:
				enemyParam.AnyBulletCountMax = std::stoi(buff);
				break;
			case EEnemyParamNo_BulletAngle:
				enemyParam.BulletAngle =  static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EEnemyParamNo_ShotAngle:
				enemyParam.ShotAngle = static_cast<float>( D3DXToRadian( std::stof(buff) ));
				break;
			case EEnemyParamNo_ShotIntervalFrame:
				enemyParam.ShotIntervalFrame = std::stoi(buff);
				break;
			case EEnemyParamNo_BulletCollDisappear:
				enemyParam.BulletCollDisappear = std::stoi(buff);
				break;
			default:
				break;
			}
			no++;
		}
		enemys.emplace_back( std::make_shared<STG::CEnemy>(enemyParam) );
		enemys.back()->SetPositionY( posY );
		posY += 0.001f;
	}
	return true;
}
