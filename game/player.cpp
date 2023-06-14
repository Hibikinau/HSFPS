/*****************************************************************//**
 * \file   player.cpp
 * \brief  プレイヤクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"player.h"

 //プレイヤのrun状態での移動速度
#define runSpd 40.f

//各攻撃のダメージ設定
#define jakuNum 0
#define kyouNum 1
#define counterNum 2
#define debugNum 3
#define kirinukeNum 4
#define charge1Num 5
#define charge2Num 6
#define charge3Num 7
#define finishNum 8

//武器追従フレーム番号設定
#define rWeponParentFrame 192
#define lWeponParentFrame 167

typedef ExclusiveState _estate;
using namespace model;


bool PL::Initialize()
{
	//各変数の初期化
	spd = runSpd;
	type = 1;
	g = 6.f;
	Estate = _estate::NORMAL;
	_statusInf.maxHitPoint = _statusInf.hitPoint = 200;
	_statusInf.maxBloodPoint = 1500;
	_statusInf.bloodPoint = 0;

	_modelInf.pos = VGet(210.0f, 1100.0f, 3100.f);
	_modelInf.dir = VGet(0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);

	//モデルの読み込み
	modelImport("res/Player01/Player1.mv1", 1.5f, &_modelInf, RS);
	modelImport("res/ウェブリーフォスベリー/ウェブリーフォスベリーオートマチックリボルバー.pmx", 2.f, &gun, RS);

	return true;
}


bool	PL::Terminate()
{
	CB::Terminate();
	//音声データのメモリ解放
	for (auto handle : soundHandle) { DeleteSoundMem(handle); }
	return true;
}

bool	PL::Process()
{
	if (_imputInf->_gTrgb[KEY_INPUT_SPACE] && airDashCoolTime <= 0) { _modelInf.vec.y = 210, airDashCoolTime = 70; }
	isGround ? airDashCoolTime = 0 : airDashCoolTime--;

	if (_imputInf->_gTrgb[KEY_INPUT_LSHIFT])
	{
		/*float radian = *_cameraDirX * DX_PI_F / 180.0f;
		_modelInf.vec.x += sin(radian) * 200;
		_modelInf.vec.z += cos(radian) * 200;
		radian = *_cameraDirY * DX_PI_F / 180.0f;
		_modelInf.vec.y += radian * 80;*/

		charMove(300, *_cameraDirX, true);
	}

	for (auto ite = bulletData.begin(); ite != bulletData.end();)
	{
		if (_valData->bulletTime > 0) { break; }
		if (!ite->get()->Process())
		{
			ite->get()->Terminate();
			ite = bulletData.erase(ite);
		}
		else { ++ite; }
	}//lClickTrgCheck

	charMove(10, *_cameraDirX, true);

	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);
	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0) {
		_valData->bulletTime = 10;
	}
	else { _valData->bulletTime--; }
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		if (!lClickTrgCheck)
		{
			float radian = *_cameraDirX * DX_PI_F / 180.0f
				, radian2 = *_cameraDirY * ((DX_PI_F / 180.0f) * 2.f);
			int randRange = 120;

			VECTOR buVec = VScale(VGet(sin(radian), radian2, cos(radian)), 800)
				, buPos = VAdd(_modelInf.pos, _modelInf.vec);

			for (int i = 0; i < 40; i++)
			{
				auto bu = std::make_unique<bullet>();
				bu->Initialize();
				bu->_modelInf.vec = VAdd(buVec, VGet(rand() % randRange - (randRange / 2), rand() % randRange - (randRange / 2), rand() % randRange - (randRange / 2)));
				bu->_modelInf.pos = buPos;
				bulletData.emplace_back(std::move(bu));
			}
			lClickTrgCheck = true;
		}
	}
	else { lClickTrgCheck = false; }

	auto insVec = VScale(_modelInf.vec, 0.05);
	_modelInf.vec = VSub(_modelInf.vec, VGet(insVec.x, 0, insVec.z));

	//gun.pos = VAdd(_modelInf.pos, VGet(0, 0, 0));
	//gun.dir.y = *_cameraDirX;

	return true;
}

bool	PL::Render(float timeSpeed)
{
	isAnimEnd = modelRender(&_modelInf, animSpd, timeSpeed);
	//modelRender(&gun, animSpd, timeSpeed);

	for (int i = 0; i < bulletData.size(); i++) { bulletData[i]->Render(); }

	return true;
}

void PL::charMove(float Speed, float _DirX, bool isAnimChange)
{
	bool inputCheck = false;
	int vecX = 0, vecY = 0;
	if (_imputInf->_gKeyb[KEY_INPUT_W]) { inputCheck = true; }
	if (!_imputInf->_gKeyb[KEY_INPUT_A] && _imputInf->_gKeyb[KEY_INPUT_D]
		|| _imputInf->_gKeyb[KEY_INPUT_A] && !_imputInf->_gKeyb[KEY_INPUT_D])
	{
		if (_imputInf->_gKeyb[KEY_INPUT_A])
		{
			inputCheck ? _DirX -= 45 : _DirX -= 90;
			if (_imputInf->_gKeyb[KEY_INPUT_S]) { _DirX -= 45; }
		}
		if (_imputInf->_gKeyb[KEY_INPUT_D])
		{
			inputCheck ? _DirX += 45 : _DirX += 90;
			if (_imputInf->_gKeyb[KEY_INPUT_S]) { _DirX += 45; }
		}
		inputCheck = true;
	}
	else if (_imputInf->_gKeyb[KEY_INPUT_S] && !inputCheck) { _DirX += 180; inputCheck = true; }
	if (!inputCheck) { return; }

	float radian = _DirX * DX_PI_F / 180.0f;
	_modelInf.vec.x += sin(radian) * Speed;
	_modelInf.vec.z += cos(radian) * Speed;

	return;
}

bool PL::HPmath(float math, float Stan) { return false; }

bool PL::BPmath(float math) { return false; }

pushButton PL::setAction()
{

	return pushButton::F;
}

bool PL::CA_change(std::string name, const char* XorY)
{

	return true;
}

bool PL::CA_debugAttack(PL* insPL)
{
	//	insPL->setRecastTime = 60;
	//	auto insDir = insPL->getMoveDir();
	//	if (insDir != 0) { insPL->_modelInf.dir.y = insDir; }
	//	animChange(PL_motion_hissatsu, &insPL->_modelInf, false, false, true);//アニメーションを覚醒時必殺技モーションに変更
	//	insPL->animSpd = 1.f;
	//	insPL->makeAttackCap(VGet(0.f, 0.f, 0.f), VGet(0.f, 0.f, 200.f), 20.f, 0.f, insPL->getAnimPlayTotalTime(), insPL->animSpd, true, 200.f + insPL->atkBuff, 5.f, rWeponParentFrame, VGet(0, 0, 0), 1);
	//
	//	auto a = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y - 90, 300));
	//	auto b = VAdd(insPL->_modelInf.pos, getDirVecP(insPL->_modelInf.dir.y + 90, 300));
	//	auto bz = getDirVecP(insPL->_modelInf.dir.y, 30);
	//	a.y = b.y = insPL->_modelInf.pos.y + 200;
	//
	//	insPL->makeAttackCap(a, b, 40.f, 10.f, 50 - 10.f, insPL->animSpd, false, insPL->_valData->plAtkNum[debugNum] + insPL->atkBuff, 5.f, -1, bz, 0);
	//
	//	int a2 = PlayEffekseer3DEffect(insPL->impactEfcHandle);
	//	SetPosPlayingEffekseer3DEffect(a2, insPL->_modelInf.pos.x, insPL->_modelInf.pos.y + 120.f, insPL->_modelInf.pos.z);
	//	SetRotationPlayingEffekseer3DEffect(a2, insPL->_modelInf.dir.x * (DX_PI_F / 180), insPL->_modelInf.dir.y * (DX_PI_F / 180), insPL->_modelInf.dir.z * (DX_PI_F / 180));
	//	PlaySoundMem(insPL->soundHandle[6], DX_PLAYTYPE_BACK);
	return true;
}

bool PL::CA_noData(PL* insPL) { return false; }