/*****************************************************************//**
 * \file   modeTitle.cpp
 * \brief  �^�C�g����ʂ̃N���X�̒�`
 *
 * \author �R�{介v
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

		if (_data[i] == "����ւ��ZX") { i++; _data[i] = _val->plChangeAttackX; }
		if (_data[i] == "����ւ��ZY") { i++; _data[i] = _val->plChangeAttackY; }
		if (_data[i] == "�����|�C���g") { i++; _data[i] = std::to_string(points); }
		if (_data[i] == "���j�ς�Boss") {
			i++;  while (_data[i] != "�����܂�") { _data.erase(_data.begin() + i); }
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

		//��
		//if (_data[i] == "���j�ς�Boss") { i++, _val->deadBoss.clear(); while (_data[i] != "�����܂�") { _val->deadBoss.emplace_back(_data[i]); i++; } }
		//if (_data[i] == "�����|�C���g") { i++; _val->points = std::atoi(_data[i].c_str()); }
		//if (_data[i] == "�J���҃��[�h") { _val->isDebug = true; }
		//if (_data[i] == "�{�X1�˗����")
		//{
		//	_val->boss1Inf = ""; i++;
		//	while (_data[i] != "�����܂�")
		//	{
		//		if (_data[i] == "���e") { _val->boss1Inf += "\n"; }
		//		if (_data[i] == "�T�v") { _val->boss1Inf += "\n"; }
		//		if (_data[i] == "��V") { _val->boss1Inf += "\n"; }
		//		_val->boss1Inf += _data[i] + "\n";
		//		i++;
		//	}
		//}
	}

	return true;
}

bool	modeT::Initialize()
{
	_cg = _modeServer->RS.loadGraphR("game/res/�^�C�g��.png");

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