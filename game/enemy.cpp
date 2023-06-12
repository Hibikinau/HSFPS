#include"enemy.h"

using namespace model;

bool EN::Initialize()
{
	_modelInf.pos = VGet(0, 0, 0);
	charSize = 60.f;
	charColor = GetColor(255, 0, 0);
	_statusInf.hitPoint = 2;

	_modelInf.pos = charBox->find(Char_PL)->second->_modelInf.pos;
	return true;
}

bool EN::Process()
{

	return true;
}

bool EN::Render(float timeSpeed)
{
	DrawSphere3D(_modelInf.pos, charSize, 12, charColor, charColor, true);
	//modelRender(&_modelInf, animSpd, timeSpeed);

	return true;
}

bool	EN::Terminate()
{
	CB::Terminate();

	return true;
}

bool EN::hitCheck(VECTOR _pos, VECTOR _vec, float _damage, float r)
{
	float bulletDis = Segment_Point_MinLength(_pos, VAdd(_pos, _vec), _modelInf.pos);
	if (bulletDis < (r + charSize))
	{
		isDead = 2;
		return true;
	}
	return false;
}
