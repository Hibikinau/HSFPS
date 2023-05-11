/*****************************************************************//**
 * \file   modeTitle.cpp
 * \brief  タイトル画面のクラスの定義
 *
 * \author 山本莉久
 * \date   March 2023
 *********************************************************************/
#include"allMode.h"
#include <fstream>
#include <sstream>

bool modeT::save(const char* dir, valData* _val)
{
	std::vector<std::string> _data;
	int points = _val->points;
	fileIO::loadCSV(&_data, dir, false);

	for (int i = 0; i < _data.size(); i++)
	{
		if (_data[i].find("//") != std::string::npos)
		{
			continue;
		}

		if (_data[i] == "入れ替え技X") { i++; _data[i] = _val->plChangeAttackX; }
		if (_data[i] == "入れ替え技Y") { i++; _data[i] = _val->plChangeAttackY; }
		if (_data[i] == "所持ポイント") { i++; _data[i] = std::to_string(points); }
		if (_data[i] == "撃破済みBoss") {
			i++;  while (_data[i] != "ここまで") { _data.erase(_data.begin() + i); }
			for (auto boss : _val->deadBoss) { _data.emplace(_data.begin() + i, boss), i++; };
		}
	}
	std::string insStr = "";
	for (auto insData : _data) { insStr += insData + "\n"; }
	std::ofstream ofs(dir);
	ofs << insStr;
	ofs.close();

	return true;
}

bool modeT::loadData(const char* dir, valData* _val)
{
	std::vector<std::string> _data;
	fileIO::loadCSV(&_data, dir, false);

	for (int i = 0; i < _data.size(); i++)
	{
		if (_data[i].find("//") != std::string::npos)
		{
			continue;
		}

		//例
		//if (_data[i] == "撃破済みBoss") { i++, _val->deadBoss.clear(); while (_data[i] != "ここまで") { _val->deadBoss.emplace_back(_data[i]); i++; } }
		//if (_data[i] == "所持ポイント") { i++; _val->points = std::atoi(_data[i].c_str()); }
		//if (_data[i] == "開発者モード") { _val->isDebug = true; }
		//if (_data[i] == "ボス1依頼情報")
		//{
		//	_val->boss1Inf = ""; i++;
		//	while (_data[i] != "ここまで")
		//	{
		//		if (_data[i] == "内容") { _val->boss1Inf += "\n"; }
		//		if (_data[i] == "概要") { _val->boss1Inf += "\n"; }
		//		if (_data[i] == "報酬") { _val->boss1Inf += "\n"; }
		//		_val->boss1Inf += _data[i] + "\n";
		//		i++;
		//	}
		//}
	}

	return true;
}

bool	modeT::Initialize()
{
	_cg = _modeServer->RS.loadGraphR("game/res/タイトル.png");

	return true;
}

bool	modeT::Process()
{
	if ((_imputInf._gTrgb[KEY_INPUT_RETURN] || _imputInf._gTrgp[XINPUT_BUTTON_A]))
	{
		//_modeServer->Add(std::make_unique<modeE>(_modeServer), 1, MODE_END);
		//_modeServer->Add(std::make_unique<modeMM>(_modeServer), 1, MODE_MM);
		return false;
	}
	return true;
}

bool	modeT::Render()
{

	return true;
}

bool	modeT::Terminate()
{
	StopMusic();
	return true;
}