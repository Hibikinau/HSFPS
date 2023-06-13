#include"enemy.h"

using namespace model;

bool EN::Initialize()
{
	_modelInf.pos = charBox->find(Char_PL)->second->_modelInf.pos;
	for (int i = 0; i < 200; i++)
	{
		VECTOR insVec = VGet(rand() % 40000 - 20000.f, 0.f, rand() % 40000 - 20000.f);
		float c = sqrt(insVec.x * insVec.x + insVec.y * insVec.y + insVec.z * insVec.z);
		if (c < 20000.f)
		{
			_modelInf.pos = insVec;
			break;
		}
	}
	_modelInf.pos.y = 400.f;
	charSize = 300.f;
	charColor = GetColor(50, 0, 0);
	_statusInf.hitPoint = 2;

	return true;
}

bool EN::Process()
{

	return true;
}

bool EN::Render(float timeSpeed)
{
	DrawCapsule3D(_modelInf.pos, VAdd(_modelInf.pos, VGet(0, 800, 0)), charSize, 12, charColor, charColor, true);

	return true;
}

bool	EN::Terminate()
{
	CB::Terminate();

	return true;
}

bool EN::hitCheck(VECTOR _pos, VECTOR _vec, float _damage, float r)
{
	float bulletDis = Segment_Segment_MinLength(_pos, VAdd(_pos, _vec), _modelInf.pos, VAdd(_modelInf.pos, VGet(0, 400, 0)));
	if (bulletDis < (r + charSize))
	{
		isDead = 2;
		return true;
	}
	return false;
}
