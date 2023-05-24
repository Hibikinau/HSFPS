#include"bullet.h"
using namespace model;

bool bullet::Initialize()
{
	//modelImport("res/ZENRYOKUstage/Stage1.mv1", 10.f, &_modelInf, RS);

	return true;
}

bool bullet::Process()
{
	_modelInf.pos = VAdd(_modelInf.pos, _modelInf.vec);

	return true;
}

bool bullet::Render()
{
	DrawSphere3D(_modelInf.pos, 60.f, 16, GetColor(255, 0, 0), GetColor(0, 0, 0), true);

	return true;
}

bool bullet::Terminate()
{

	return true;
}
