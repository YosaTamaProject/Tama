#pragma once

// インクルードガード
#ifndef EXTENSION_WEAPONS_HPP
#define  EXTENSION_WEAPONS_HPP

#include "Weapon.hpp"


#define FPS 60


class PredatorCannon : WeaponBase
{
public:
	PredatorCannon() : WeaponBase()
	{

	}


	PredatorCannon(Vec2 pos) : WeaponBase()
	{
		gun_image = Texture(U"path");
		bullet_image = Texture(U"path");
		firing_rate = 15; // 決め打ち
		bullet_list.clear();
	}


	void update(Vec2 gun_pos) override
	{
		pos = gun_pos;

		if (KeyEnter.pressed())
		{
			if (Scene::FrameCount() % (FPS / firing_rate) == 0)
			{
				shot();
			}
		}

		for (int i = 0; i < bullet_list.size(); i++)
		{
			bullet_list[i].update();
			if (bullet_list[i].isDead())
			{
				bullet_list.remove_at(i);
				i--;
			}
		}


	}


	void draw() const
	{
		for (int i = 0; i < bullet_list.size(); i++)
		{
			bullet_list[i].draw();
		}
		gun_image.resized(50, 100).drawAt(pos);
	}




private:
	Vec2 pos;
	Texture gun_image;
	Texture bullet_image;
	Array<Bullet> bullet_list;

	int damage;
	int firing_rate;

	void shot() override
	{
		// 発射エフェクトの描画のフラグを立てるならここに書くとよいかもしれない
		bullet_list.push_back(Bullet(bullet_image, 5, 10, pos, Vec2(0, -10), damage, U"player"));
	}
};


#endif
