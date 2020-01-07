#pragma once

// インクルードガード
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Bullet.hpp"

// ここに武器の基底クラスを記述する
class WeaponBase
{
public:
	WeaponBase()
	{

	}

	virtual void update() = 0;
	virtual void draw() const = 0;
	virtual Array<Bullet> getBullets() = 0;

private:
	virtual void shot() = 0;

};

#endif