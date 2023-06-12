#pragma once
#include"appframe.h"

class bullet
{
public:
	bullet() {};
	~bullet() {};
	bool Initialize();
	bool Process();
	bool Render();
	bool Terminate();
	void setRS(Rserver* _RS) { RS = _RS; }

	int deadCount;
	float damage, speed, bulletRadius;
	std::string shooterName;
	modelInf _modelInf;
	Rserver* RS;
};