/*****************************************************************//**
 * \file   modeGame.h
 * \brief  ゲーム画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#pragma once
#include"appframe.h"
#include"player.h"
#include"enemy.h"
#include<memory>

struct popDamageInf
{
	VECTOR pos;
	float damage;
	int popTime = 0;
	bool isPl;
};

class modeG : public ModeBase
{
public:
	modeG(ModeServer* MS) { _modeServer = MS; }
	~modeG() {};

	virtual bool	Initialize();
	virtual bool	Terminate();
	virtual bool	Imput(imputInf* iInf) { _imputInf = *iInf; return true; };
	virtual bool	Process();
	virtual bool	Render();
	void cameraMove();
	int useCommand();
	bool ASyncLoadAnim();
	bool drawUI();
	bool	popBoss(int bossType, const char* _nameA)override;

public:
	ModeServer* _modeServer;
	int _x, _y, enemyPopCoolTime, score, defFontSize;
	int reticleHandle = -1;
	float spd, cameraDir, cameraLockDir, cameraNtDir, cameraHigh;
	int startTime, countTime, FPS, FPScount;
	bool isLockon, isGameOver = false, endVoice = false;
	std::map<std::string, std::shared_ptr<CB> > charBox;
	VECTOR cameraPos, cameraFor;
	modelInf* plMI, stage, * bossMI, skyDoom, sakuHandle;
	std::string NS;
	std::vector<std::string> debugWardBox;
	imputInf _imputInf;
	valData* _valData;
	statusInf plStatus;
	std::vector<weaponEffect> atkEfc;
	std::vector<popDamageInf> damageNumPopList;
	int UIkari, BGM, insEfcHandle, efcHandle, efcTime;

	float bright = .6f;
	float Atten0 = 0.8f, Atten1 = 0.f, Atten2 = 0.f;
	bool debugMode = true, plDead;
	int enemyNum = 0;
};