/*****************************************************************//**
 * \file   modeGame.h
 * \brief  ゲーム画面のクラス
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"
#include <sstream>

using namespace model;

bool makeChar(modeG* insMG, Rserver* _rs, std::shared_ptr<CB> charPoint, const char* nameA)
{
	charPoint->_valData = insMG->_valData;
	charPoint->setRS(&insMG->_modeServer->RS);
	charPoint->setCB(&insMG->charBox);
	charPoint->setName(nameA);
	charPoint->Initialize();
	charPoint->setGroundInf(&insMG->stage);
	charPoint->getInputKey(&insMG->_imputInf, &insMG->cameraDir, &insMG->cameraHigh);
	insMG->charBox.emplace(nameA, std::move(charPoint));
	return true;
}

bool	modeG::popBoss(int bossType, const char* _nameA)
{
	//if (bossType == 1) { makeChar(this, &_modeServer->RS, std::shared_ptr<BossKnight>(), _nameA); }

	return true;
}

bool	modeG::ASyncLoadAnim()
{
	SetUseASyncLoadFlag(false);
	SetDrawScreen(DX_SCREEN_BACK);
	int i = 0;
	while (GetASyncLoadNum() > 0)
	{
		ProcessMessage();
		ClearDrawScreen();
		i++;
		DrawBox(0, 0, i, 20, GetColor(255, 255, 255), true);

		ScreenFlip();
	}

	return GetASyncLoadNum();
}

bool	modeG::Initialize()
{
	SetUseLighting(false);
	SetUseZBuffer3D(true);// Ｚバッファを有効にする
	SetWriteZBuffer3D(true);// Ｚバッファへの書き込みを有効にする
	SetUseBackCulling(false);
	//SetUseASyncLoadFlag(true);
	SetAlwaysRunFlag(true);
	SetMouseDispFlag(false);
	SetupCamera_Perspective(1.5f);

	_valData = &_modeServer->_valData;
	modelImport("res/レトロフューチャーステージ/レトロフューチャーステージ.pmx", 200.f, &stage, &_modeServer->RS);
	modelImport("res/skyDoom/incskies_029_16k.x", 70.f, &skyDoom, &_modeServer->RS);
	makeChar(this, &_modeServer->RS, std::make_unique<PL>(), Char_PL);
	reticleHandle = _modeServer->RS.loadGraphR("res/reticle.png");
	countTime = GetNowCount();

	//ここまで非同期ロード-------------------------------------------------------------------
	ASyncLoadAnim();

	//ライトの方向
	VECTOR lightDir = VGet(-3.0f, -4.0f, 0.0f);
	ChangeLightTypeDir(lightDir);

	//読み込んだ3dモデルのサイズ調整
	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		changeScale(&i->second->_modelInf);
	}
	changeScale(&stage);
	stage.pos = VGet(0, 0, 0);
	changeScale(&skyDoom);
	changeScale(&sakuHandle);
	defFontSize = GetFontSize();
	SetFontSize(50);

	score = 0.f;
	isLockon = true;

	return true;
}

bool	modeG::Process()
{
	int nowTime = GetNowCount();//180000
	if (countTime + 60000 <= nowTime)
	{
		_valData->score = score;
		if (_valData->maxScore < score) { _valData->maxScore = score; }
		_modeServer->Add(std::make_unique<modeR>(_modeServer), 1, MODE_RESULT);

		return false;
	}
	else { FPScount++; }
	debugWardBox.emplace_back("残り時間 : " + std::to_string((countTime + 60000 - nowTime) / 1000) + "s");
	debugWardBox.emplace_back("現在のスコア : " + std::to_string(score));

	plStatus = { 0.f };
	std::vector<std::string> deadEnemyList;
	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		if (i->second->getType() == 1)
		{
			i->second->Process();
			i->second->gravity();
			plMI = i->second->getInf();
			plStatus = i->second->getStatus();
		}
		else
		{
			i->second->Process();
			bossMI = i->second->getInf();

			std::vector<std::unique_ptr<bullet> >* _bulletData = &charBox.find(Char_PL)->second->bulletData;
			auto dataSize = _bulletData->size();
			for (int j = 0; j < dataSize; j++)
			{
				if (i->second->hitCheck(_bulletData->at(j)->_modelInf.pos, _bulletData->at(j)->_modelInf.vec, _bulletData->at(j)->damage, _bulletData->at(j)->bulletRadius))
				{
					deadEnemyList.emplace_back(i->first);
					break;
				}
			}
		}
	}
	for (std::string deadEnemyName : deadEnemyList)
	{
		charBox.find(deadEnemyName)->second->Terminate();
		charBox.erase(deadEnemyName);
		score += 200;
	}
	if (charBox.size() <= 1) { score += 10; }

	//コマンド呼び出し部分
	//useCommand();

	//カメラ制御---------------------------------------------
	int XBuf, YBuf;

	GetMousePoint(&XBuf, &YBuf);
	SetMousePoint(640, 360);
	cameraDir -= (640 - XBuf) * 0.07;
	cameraHigh += (360 - YBuf) * 0.07;

	cameraMove();
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraFor);
	Effekseer_Sync3DSetting();

	SetGlobalAmbientLight(GetColorF(bright, bright, bright, 0.0f));

	if (enemyPopCoolTime <= 0)
	{
		auto popNum = 7 + rand() % 5;
		for (int i = 0; i < popNum; i++)
		{
			std::string insName = "enemy";
			insName += std::to_string(enemyNum);
			enemyNum++;
			makeChar(this, &_modeServer->RS, std::make_shared<EN>(), insName.c_str());
		}

		enemyPopCoolTime = 240 + rand() % 60;
	}
	else { enemyPopCoolTime--; }
	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
	return true;
}

bool	modeG::Render()
{
	SetCameraNearFar(100.f, 90000.f);
	MV1DrawModel(skyDoom.modelHandle);
	//3dモデルの描画
	MV1DrawModel(stage.modelHandle);

	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		i->second->Render(1);
	}

	DrawEffekseer3D();// Effekseerにより再生中のエフェクトを描画する。

	SetUseZBuffer3D(FALSE);

	drawUI();
	SetUseZBuffer3D(TRUE);

	for (int i = 0; i < debugWardBox.size() && debugMode; i++)
	{
		int sizeX, sizeY, lineCount;
		GetDrawStringSize(&sizeX, &sizeY, &lineCount, debugWardBox[i].c_str(), debugWardBox[i].length());
		DrawBox(10, 10 + 50 * i, 10 + sizeX, 10 + 50 * i + sizeY, GetColor(0, 0, 0), true);
		DrawString(10, 10 + 50 * i, debugWardBox[i].c_str(), GetColor(255, 255, 255));
	}
	debugWardBox.clear();

	DrawGraph(0, 0, reticleHandle, true);
	//DrawCircle(640, 360, 5, GetColor(255, 255, 255), true);

	return true;
}

bool	modeG::Terminate()
{
	StopSoundMem(BGM);
	//MV1TerminateCollInfo(stage.modelHandle, -1);
	MV1DeleteModel(stage.modelHandle);
	MV1DeleteModel(skyDoom.modelHandle);
	MV1DeleteModel(sakuHandle.modelHandle);

	for (auto i = charBox.begin(); i != charBox.end(); ++i) { i->second->Terminate(); i->second.reset(); }
	charBox.clear();
	debugWardBox.clear();
	DeleteLightHandleAll();
	SetFontSize(defFontSize);

	modeT::save("data.csv", _valData);
	return true;
}

void modeG::cameraMove()
{
	float radian = cameraDir * DX_PI_F / 180.0f;
	VECTOR InsV = VGet(sin(radian) * 10, 0, cos(radian) * 10);
	auto _pos = cameraPos = VAdd(plMI->pos, VGet(0, 0, 0));
	cameraFor = VAdd(plMI->pos, InsV);
	cameraFor.y += cameraHigh * 0.35f;

}

float getNum(std::string data, int Num)
{
	std::string input;
	std::stringstream b{ data };
	for (int i = 0; i < Num; i++)
	{
		std::getline(b, input, '^');
		std::getline(b, input, '^');
	}

	return std::stof(input);
}

std::string getChar(std::string data, int Num)
{
	std::string input;
	std::stringstream b{ data };
	for (int i = 0; i < Num; i++)
	{
		std::getline(b, input, '^');
		std::getline(b, input, '^');
	}

	return input;
}

int modeG::useCommand()
{
	if (!_imputInf._gTrgb[KEY_INPUT_RETURN]) { return 1; }
	DrawBox(10, 700, 1270, 730, GetColor(0, 0, 0), true);
	KeyInputString(10, 700, 141, _imputInf.wardBox, true);
	std::string _wardBox = _imputInf.wardBox;
	if (_wardBox.size() == 0) { return 1; }
	std::stringstream a{ _wardBox };
	std::string data, input;
	auto commandNum = std::count(_wardBox.cbegin(), _wardBox.cend(), '/');

	try
	{
		std::getline(a, data, '/');
		for (int i = 0; i < commandNum; i++)
		{
			std::getline(a, data, '/');

			if (data == "debug") { debugMode ? debugMode = false : debugMode = true;	return 2; }
			if (data == "kill")
			{
				charBox[Char_PL]->_statusInf.hitPoint = 0;
			}
			if (data == "allKill")
			{
				std::vector<std::string> nameList;
				for (auto _name : charBox)
				{
					if (_name.first == Char_PL) { continue; }
					nameList.emplace_back(_name.first);
				}
				for (auto _name : nameList) { charBox.erase(_name); }
			}
			if (data == "csv")
			{
				modeT::loadData("data.csv", &_modeServer->_valData);
			}
			if (data == "test")
			{
				OutputDebugString("succes");
				return 2;
			}
		}
	}
	catch (std::exception) { return -1; }
	return -1;
}

bool modeG::drawUI()
{

	return true;
}
