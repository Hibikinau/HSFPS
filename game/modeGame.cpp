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
	charPoint->Initialize();
	charPoint->setCB(&insMG->charBox);
	charPoint->setName(nameA);
	charPoint->setGroundInf(&insMG->stage);
	charPoint->allColl = &insMG->mAllColl;
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
	SetUseLighting(true);
	SetUseZBuffer3D(true);// Ｚバッファを有効にする
	SetWriteZBuffer3D(true);// Ｚバッファへの書き込みを有効にする
	SetUseBackCulling(false);
	//SetUseASyncLoadFlag(true);
	SetAlwaysRunFlag(true);
	SetMouseDispFlag(false);
	Effekseer_StartNetwork(60000);// ネットワーク機能を有効にする

	_valData = &_modeServer->_valData;
	modelImport("res/ZENRYOKUstage/tsStage.mv1", 10.f, &stage, &_modeServer->RS);
	makeChar(this, &_modeServer->RS, std::make_unique<PL>(), Char_PL);

	countTime = GetNowCount();


	//ここまで非同期ロード-------------------------------------------------------------------
	ASyncLoadAnim();

	// シャドウマップハンドルの作成
	ShadowMapHandle = MakeShadowMap(16384, 16384);
	// シャドウマップが想定するライトの方向もセット
	VECTOR lightDir = VGet(-3.0f, -4.0f, 0.0f);
	SetShadowMapLightDirection(ShadowMapHandle, lightDir);
	ChangeLightTypeDir(lightDir);
	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(ShadowMapHandle, VGet(-5000.0f, -1.0f, -5000.0f), VGet(5000.0f, 1000.0f, 5000.0f));

	//ChangeVolumeSoundMem(255 * (0.01 * _valData->soundMasterValume), BGM);

	//読み込んだ3dモデルのサイズ調整
	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		changeScale(&i->second->_modelInf);
	}
	changeScale(&stage);
	changeScale(&skyDoom);
	changeScale(&sakuHandle);

	//ステージの当たり判定作成
	//MV1SetupCollInfo(stage.modelHandle, -1, 32, 6, 32);
	isLockon = true;

	return true;
}

bool	modeG::Process()
{
	plStatus = { 0.f };
	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		if (i->second->getType() == 1)
		{
			i->second->Process();
			i->second->gravity();
			plMI = i->second->getInf();
			plStatus = i->second->getStatus();
			plRecastTimeX = i->second->caRecastX;
			plRecastTimeY = i->second->caRecastY;
			plSetRecastTime = i->second->setRecastTime;
		}
		else
		{
			i->second->Process();
			bossMI = i->second->getInf();
			i->second->gravity();
			bossStatus = i->second->getStatus();
			if (i->second->isDead == 2)
			{
				i->second->Terminate();
				charBox.erase(i);
			}
		}

	}


	//コマンド呼び出し部分
	useCommand();

	//カメラ制御---------------------------------------------
	int XBuf, YBuf;

	GetMousePoint(&XBuf, &YBuf);
	SetMousePoint(640, 360);
	cameraDir -= (640 - XBuf) * 0.05;
	cameraHigh += (360 - YBuf) * 0.05;

	cameraMove();
	SetCameraPositionAndTarget_UpVecY(cameraPos, cameraFor);
	Effekseer_Sync3DSetting();

	//影の明るさ調整-----------------------------------------
	if (CheckHitKey(KEY_INPUT_UP)) { bright += .01f; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { bright -= .01f; }
	if (bright < 0) { bright = 0; }
	if (bright > 1) { bright = 1; }
	SetGlobalAmbientLight(GetColorF(bright, bright, bright, 0.0f));
	debugWardBox.emplace_back("影の明るさ  = " + std::to_string(bright));
	debugWardBox.emplace_back("自機のHP = " + std::to_string(plStatus.hitPoint));
	debugWardBox.emplace_back("自機のBP = " + std::to_string(plStatus.bloodPoint));
	debugWardBox.emplace_back(std::to_string(
		(std::atan2(-_imputInf.lStickX, _imputInf.lStickY) * 180.f) / DX_PI_F));
	debugWardBox.emplace_back("入れ替え技Xのリキャスト = " + std::to_string(plRecastTimeX));
	debugWardBox.emplace_back("入れ替え技Yのリキャスト = " + std::to_string(plRecastTimeY));
	debugWardBox.emplace_back("現在のFPS値/" + std::to_string(FPS));
	debugWardBox.emplace_back("弱攻撃1のフレーム数/" + std::to_string(_valData->plAtkSpd1));
	debugWardBox.emplace_back("弱攻撃2のフレーム数/" + std::to_string(_valData->plAtkSpd2));
	debugWardBox.emplace_back("弱攻撃3のフレーム数/" + std::to_string(_valData->plAtkSpd3));
	debugWardBox.emplace_back("弱攻撃4のフレーム数/" + std::to_string(_valData->plAtkSpd4));
	debugWardBox.emplace_back("ガード出だしのモーションスピード/" + std::to_string(_valData->counterSpd));
	debugWardBox.emplace_back("カウンターの総受付時間/" + std::to_string(_valData->_counterTime));
	debugWardBox.emplace_back("残りのカウンター受付時間/" + std::to_string(_valData->plCTimeN));
	debugWardBox.emplace_back("x." + std::to_string(static_cast<int>(plMI->pos.x))
		+ "/y." + std::to_string(static_cast<int>(plMI->pos.y))
		+ "/z." + std::to_string(static_cast<int>(plMI->pos.z)));

	//当たり判定計算呼び出し
	collHitCheck();

	//メニュー画面呼び出し
	if (_imputInf._gTrgb[KEY_INPUT_M] || _imputInf._gTrgp[XINPUT_BUTTON_START])
	{
		//_modeServer->Add(std::make_unique<modeM>(_modeServer), 1, MODE_MENU);
	}

	if (_imputInf._gTrgb[KEY_INPUT_H])
	{
		_valData->hitstopF = 10;
	}

	if (_imputInf._gTrgb[KEY_INPUT_E]) { makeChar(this, &_modeServer->RS, std::make_unique<EN>(), "enemy"); }
	if (_imputInf._gTrgb[KEY_INPUT_A]) { swordGlitchAnimNum = 0; }
	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();
	return true;
}

bool	modeG::Render()
{
	MV1DrawModel(skyDoom.modelHandle);

	//シャドウマップココカラ-----------------------------------------
	ShadowMap_DrawSetup(ShadowMapHandle);
	//3dモデルの描画
	MV1DrawModel(stage.modelHandle);
	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		//if (i->second->drawStopF > 0) { i->second->drawStopF--; i->second->Render(0); }
		i->second->Render(1);
	}

	ShadowMap_DrawEnd();
	SetUseShadowMap(0, ShadowMapHandle);
	//影用の3dモデルの描画
	MV1DrawModel(stage.modelHandle);
	for (auto i = charBox.begin(); i != charBox.end(); ++i)
	{
		i->second->Render(0);
	}

	SetUseShadowMap(0, -1);
	//シャドウマップここまで-----------------------------------------

	int nowTime = GetNowCount();
	if (countTime + 1000 <= nowTime) { FPS = FPScount, FPScount = 0, countTime += 1000; }
	else { FPScount++; }

	DrawEffekseer3D();// Effekseerにより再生中のエフェクトを描画する。

	SetUseZBuffer3D(FALSE);
	if (isLockon)
	{
		LOMarkerNum < 29 ? LOMarkerNum++ : LOMarkerNum = 0;
		auto a = DrawBillboard3D(VAdd(cameraFor, VGet(0, 170, 0)), .5, .5, 100, 0, lockOnMarkerHandle[LOMarkerNum], true);
	}

	if (plStatus.hitPoint < 70)
	{//nearDeadHandle
		DrawGraph(0, 0, nearDeadHandle, true);
	}

	drawUI();
	SetUseZBuffer3D(TRUE);

	debugWardBox.emplace_back(std::to_string(plMI->playTime));
	debugWardBox.emplace_back(std::to_string(plMI->playTimeOld));
	//debugWardBox.emplace_back("-------武器セット一覧-------");
	debugWardBox.emplace_back("-------コマンド一覧-------");
	debugWardBox.emplace_back("/debug(デバッグモードの切り替え)");
	debugWardBox.emplace_back("/menu(メニュー画面表示)");
	debugWardBox.emplace_back("/atkF1 ~ 4^フレーム数^(自機の1 ~ 4番目の攻撃モーションの総フレーム数変更)");
	debugWardBox.emplace_back("/atkFall^フレーム数^(自機のすべての攻撃モーションの総フレーム数変更)");
	debugWardBox.emplace_back("/GSpd^フレーム数^(ガード出だしのモーションの速さ)");
	debugWardBox.emplace_back("/CTime^フレーム数^(カウンターの受付時間、標準で40)");
	debugWardBox.emplace_back("/effectChange^ファイル名^^スケール^(Eキーで再生されるエフェクトの変更、拡張子不要/resからの相対パス必要)");
	debugWardBox.emplace_back("/csv(csvファイル更新)");
	for (int i = 0; i < debugWardBox.size() && debugMode; i++)
	{
		int sizeX, sizeY, lineCount;
		GetDrawStringSize(&sizeX, &sizeY, &lineCount, debugWardBox[i].c_str(), debugWardBox[i].length());
		DrawBox(10, 10 + 20 * i, 10 + sizeX, 10 + 20 * i + sizeY, GetColor(0, 0, 0), true);
		DrawString(10, 10 + 20 * i, debugWardBox[i].c_str(), GetColor(255, 255, 255));
	}
	debugWardBox.clear();

	return true;
}

bool	modeG::collHitCheck()
{
	for (int i = 0; i < mAllColl.size(); i++)
	{//
		if (mAllColl.at(i).nonActiveTimeF > 0) { mAllColl.at(i).nonActiveTimeF--; }
		else if (mAllColl.at(i).activeTimeF > 0) { mAllColl.at(i).activeTimeF--; }
		else
		{
			atkEfc.emplace_back(mAllColl.at(i).rightingEfc);
			mAllColl.erase(mAllColl.begin() + i);
		}
	}

	for (auto i = charBox.begin(); i != charBox.end(); i++)
	{
		VECTOR hitPos = { -1 }, hitDir = { -1 };
		float _damage;
		if (i->second->hitCheck(i->first.c_str(), &hitPos, &hitDir, &_damage))
		{
			popDamageInf insDamage;
			insDamage.pos = hitPos;
			insDamage.damage = _damage;
			insDamage.isPl = i->first == Char_PL;
			damageNumPopList.emplace_back(insDamage);

			auto a = PlayEffekseer3DEffect(_valData->efcHandle);
			SetPosPlayingEffekseer3DEffect(a, hitPos.x, hitPos.y, hitPos.z);
			auto D = 45;
			SetRotationPlayingEffekseer3DEffect(a, hitDir.x - D, hitDir.y - D, hitDir.z - D);
			SetScalePlayingEffekseer3DEffect(a, 2, 2, 2);
		}
	}

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
	mAllColl.clear();
	charBox.clear();
	debugWardBox.clear();
	DeleteLightHandleAll();
	modeT::save("game/res/save.csv", _valData);
	return true;
}

void modeG::cameraMove()
{
	float radian = cameraDir * DX_PI_F / 180.0f;
	VECTOR InsV = VGet(sin(radian) * 10, 0, cos(radian) * 10);
	auto _pos = cameraPos = VAdd(plMI->pos, VGet(0, 0, 0));
	cameraFor = VAdd(plMI->pos, InsV);
	cameraFor.y += (cameraHigh * DX_PI_F / 180.0f) * 10;

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
			//if (data == "menu") { _modeServer->Add(std::make_unique<modeM>(_modeServer), 1, MODE_MENU); }
			if (data.find("atkF1") != std::string::npos) { _valData->plAtkSpd1 = getNum(data, 1); }
			if (data.find("atkF2") != std::string::npos) { _valData->plAtkSpd2 = getNum(data, 1); }
			if (data.find("atkF3") != std::string::npos) { _valData->plAtkSpd3 = getNum(data, 1); }
			if (data.find("atkF4") != std::string::npos) { _valData->plAtkSpd4 = getNum(data, 1); }
			if (data.find("GSpd") != std::string::npos) { _valData->counterSpd = getNum(data, 1); }
			if (data.find("CTime") != std::string::npos) { _valData->_counterTime = getNum(data, 1); }
			if (data.find("atkFall") != std::string::npos)
			{
				auto a = getNum(data, 1);
				_valData->plAtkSpd1 = a, _valData->plAtkSpd2 = a, _valData->plAtkSpd3 = a, _valData->plAtkSpd4 = a;
			}
			if (data.find("effectChange") != std::string::npos)
			{
				auto comEfcDir = "game/res/" + getChar(data, 1) + ".efkefc";
				auto efcScale = getNum(data, 2);
				_valData->efcHandle = LoadEffekseerEffect(comEfcDir.c_str(), efcScale);
			}
			if (data == "kill")
			{
				charBox[Char_PL]->_statusInf.hitPoint = 0;
			}
			if (data == "csv")
			{
				modeT::loadData("game/res/save.csv", &_modeServer->_valData);
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
