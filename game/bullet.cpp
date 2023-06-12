#include"bullet.h"
using namespace model;

bool bullet::Initialize()
{
	//modelImport("res/ZENRYOKUstage/Stage1.mv1", 10.f, &_modelInf, RS);
	bulletRadius = 30.f;
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
	DrawSphere3D(_modelInf.pos, bulletRadius, 16, GetColor(255, 0, 0), GetColor(0, 0, 0), true);

	return true;
}

bool bullet::Terminate()
{

	return true;
}
