#pragma once
#include"charBase.h"

class EN : public CB
{
public:
	EN() {};
	~EN() {};

	/**
	 * @brief ‰Šú‰»ˆ—
	 * @return true
	 */
	virtual bool	Initialize();

	/**
	 * @brief íœˆ—
	 * @return true
	 */
	virtual bool	Terminate();

	/**
	 * @brief XVˆ—
	 * @return true
	 */
	virtual bool	Process();

	/**
	 * @brief •`‰æˆ—
	 * @param Ä¶‘¬“x
	 * @return true
	 */
	virtual bool	Render(float timeSpeed);
	void setRS(Rserver* _RS) { RS = _RS; }
	bool hitCheck(VECTOR _pos, VECTOR _vec, float _damage, float r) override;

private:
	float charSize;
	unsigned int charColor;
};