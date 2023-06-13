#include"bullet.h"
using namespace model;

bool bullet::Initialize()
{
	bulletRadius = 5.f;
	damage = 2;
	deadCount = 100;

	return true;
}

bool bullet::Process()
{
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);

	deadCount--;
	if (deadCount < 0) { return false; }

	return true;
}

bool bullet::Render()
{
	DrawSphere3D(_modelInf.pos, bulletRadius, 16, GetColor(255, 0, 0), GetColor(255, 50, 50), true);
	DrawLine3D(_modelInf.pos, VSub(_modelInf.pos,VScale(_modelInf.vec, 3)), GetColor(255, 20, 20));

	return true;
}

bool bullet::Terminate()
{

	return true;
}
