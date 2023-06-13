/*****************************************************************//**
 * \file   charBase.cpp
 * \brief  キャラベースクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"charBase.h"
using namespace model;

VECTOR CB::getDirVecP(float dir, int powar)
{
	float _Dir = dir - 180.f;
	float radian = _Dir * DX_PI_F / 180.0f;
	VECTOR dirZ = VGet(0, 0, 0);
	dirZ.x = sin(radian) * powar;
	dirZ.z = cos(radian) * powar;
	dirZ.y = 0;
	return dirZ;
}

bool	CB::gravity()
{
		if (_modelInf.pos.y < 1100)
		{
			_modelInf.pos.y = 1090;
			isGround = true;
			if (_modelInf.vec.y < 0) { _modelInf.vec.y = 0; }
		}
		else
		{
			_modelInf.vec.y -= g;
			isGround = false;
		}

		//マップ(円)の中から出ないように
		auto a = _modelInf.pos;
		float c = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
		if (c > 20000.f)
		{
			_modelInf.pos = VScale(VNorm(a), 20000.f);
		}
	return true;
}

bool	CB::makeAttackCap(VECTOR _underPos, VECTOR _overPos, float r
	, int nonActiveTimeF, int activeTimeF, int timeSpeed, bool isUseMat, float damage, float stan, int frameNum, VECTOR _dir, int _efcNum)
{
	attackColl acoll;
	acoll.isUseMat = isUseMat;
	acoll.capColl.parentModelHandle = _modelInf.modelHandle;
	acoll.capColl.frameNum = frameNum;
	acoll.capColl.underPos = _underPos;
	acoll.capColl.overPos = _overPos;
	acoll.capColl.r = r;
	acoll.attackChar = name;
	activeTimeF == 0 || timeSpeed == 0 ? acoll.activeTimeF = activeTimeF : acoll.activeTimeF = activeTimeF / timeSpeed;
	nonActiveTimeF == 0 || timeSpeed == 0 ? acoll.nonActiveTimeF = nonActiveTimeF : acoll.nonActiveTimeF = nonActiveTimeF / timeSpeed;
	acoll.damage = damage;
	acoll.stan = stan;
	acoll.capCollOld.r = -1;
	acoll.Vec = _dir;
	acoll.efcNum = _efcNum;
	allColl->emplace_back(acoll);

	return true;
}