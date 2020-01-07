#pragma once

// �C���N���[�h�K�[�h
#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "Bullet.hpp"

// �����ɕ���̊��N���X���L�q����
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