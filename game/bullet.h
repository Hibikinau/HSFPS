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
	
	float damage, speed;
	modelInf _modelInf;
	Rserver* RS;
};