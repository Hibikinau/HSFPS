#include"allMode.h"
#include <fstream>
#include <sstream>

bool	modeR::Initialize()
{
	//_cg = _modeServer->RS.loadGraphR("res/title.png");
	_valData = &_modeServer->_valData;
	DefFontSize = GetFontSize();

	return true;
}

bool	modeR::Process()
{
	if (_imputInf._gTrgb[KEY_INPUT_RETURN])
	{
		_modeServer->Add(std::make_unique<modeT>(_modeServer), 1, MODE_TITLE);
		return false;
	}
	return true;
}

bool	modeR::Render()
{
	SetFontSize(70);
	std::string insStr = "SCORE : " + std::to_string(_valData->score);
	DrawString(80, 100, insStr.c_str(), GetColor(255, 255, 255));
	insStr = "MAXSCORE : " + std::to_string(_valData->maxScore);
	DrawString(0, 200, insStr.c_str(), GetColor(255, 255, 255));

	SetFontSize(40);
	DrawString(0, 630, "Enterキーでタイトルに戻る", GetColor(255, 255, 255));
	DrawString(0, 680, "Escapeキーでゲーム終了", GetColor(255, 255, 255));

	return true;
}

bool	modeR::Terminate()
{
	SetFontSize(DefFontSize);

	return true;
}