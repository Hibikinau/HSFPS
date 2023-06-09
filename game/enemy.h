#pragma once
#include"charBase.h"

class EN : public CB
{
public:
	EN() {};
	~EN() {};

	/**
	 * @brief 初期化処理
	 * @return true
	 */
	virtual bool	Initialize();

	/**
	 * @brief 削除処理
	 * @return true
	 */
	virtual bool	Terminate();

	/**
	 * @brief 更新処理
	 * @return true
	 */
	virtual bool	Process();

	/**
	 * @brief 描画処理
	 * @param 再生速度
	 * @return true
	 */
	virtual bool	Render(float timeSpeed);
	void setRS(Rserver* _RS) { RS = _RS; }
	bool hitCheck(VECTOR _pos, VECTOR _vec, float _damage, float r) override;

private:
	float charSize;
	unsigned int charColor;
};