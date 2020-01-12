#pragma once

// インクルードガード
#ifndef EXTENSION_WEAPONS_HPP
#define  EXTENSION_WEAPONS_HPP

#include "Weapon.hpp"


#define FPS 60


class PredatorCannon : public WeaponBase
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
		damage = 10;
		this->pos = pos;
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


	void draw() const override
	{
		for (int i = 0; i < bullet_list.size(); i++)
		{
			bullet_list[i].draw();
		}
		gun_image.resized(50, 100).drawAt(pos);
	}


	Array<Bullet>&getBullets() override
	{
		return bullet_list;
	}
	

private:
	Vec2 pos;
	Texture gun_image;
	Texture bullet_image;
	Array<Bullet> bullet_list;

	int damage;
	int firing_rate;

	const Rect bullet_base = Rect(0, 0, 5, 10);
	const int bullet_visible_frame = 120;

	void shot() override
	{
		// 発射エフェクトの描画のフラグを立てるならここに書くとよいかもしれない
		bullet_list.push_back(Bullet(bullet_image, bullet_base.w, bullet_base.h, pos, Vec2(0, -10), damage, bullet_visible_frame, U"predator"));
	}
};



class PlasmaRailGun : public WeaponBase
{
public:
	PlasmaRailGun() : WeaponBase()
	{

	}


	PlasmaRailGun(Vec2 pos) : WeaponBase()
	{
		gun_image = Texture(U"path");
		bullet_image = Texture(U"path");
		firing_rate =3; // 決め打ち
		damage = 2;
		this->pos = pos;
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
			if (bullet_list[i].getVisibleFrame() <= 0)
			{
				bullet_list.remove_at(i);
				i--;
			}
		}
	}


	void draw() const override
	{
		for (int i = 0; i < bullet_list.size(); i++)
		{
			bullet_list[i].draw();
		}
		gun_image.resized(50, 100).drawAt(pos);
	}


	Array<Bullet>& getBullets() override
	{
		return bullet_list;
	}


private:
	Vec2 pos;
	Texture gun_image;
	Texture bullet_image;
	Array<Bullet> bullet_list;
	
	int damage;
	int firing_rate;

	const Vec2 bullet_size = Vec2(10, Scene::Height());
	const int bullet_visible_frame = 30;

	void shot() override
	{
		// 発射エフェクトの描画のフラグを立てるならここに書くとよいかもしれない
		bullet_list.push_back(Bullet(bullet_image, bullet_size.x, bullet_size.y, pos - Vec2(0, bullet_size.y / 2) , Vec2(0, 0), damage, bullet_visible_frame, U"railgun"));
	}
};


class Fliegerhummer : public WeaponBase
{
public:
	Fliegerhummer() : WeaponBase()
	{

	}


	Fliegerhummer(Vec2 pos) : WeaponBase()
	{
		gun_image = Texture(U"path");
		bullet_image = Texture(U"path");
		firing_rate = 10; // 決め打ち
		damage = 1;
		this->pos = pos;
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
			if (bullet_list[i].isDead() && bullet_list[i].getDamage() == 0)
			{
				Vec2 hypocenter = bullet_list[i].getCollision().pos + Vec2(bullet_list[i].getCollision().w / 2, bullet_list[i].getCollision().h / 2);
				Circle explosion_range = Circle(hypocenter, explosion_radius);
				for (int j = 0; j < explosion_density; j++)
				{
					// 小爆発をexplosion_range中のランダムな場所に配置する、次いでに小爆発の範囲自体もランダム
					explode_small(RandomVec2(explosion_range), Random<int>(explosion_radius / 4, explosion_radius / 2));
				}
				bullet_list.remove_at(i);
				i--;
			}
			else if(bullet_list[i].getVisibleFrame() <= 0)
			{
				bullet_list.remove_at(i);
				i--;
			}
		}
	}


	void draw() const override
	{
		for (int i = 0; i < bullet_list.size(); i++)
		{
			bullet_list[i].draw();
		}
		gun_image.resized(50, 100).drawAt(pos);
	}


	Array<Bullet>& getBullets() override
	{
		return bullet_list;
	}


private:
	Vec2 pos;
	Texture gun_image;
	Texture bullet_image;
	Texture explosion_image;
	Array<Bullet> bullet_list;

	int damage;
	int firing_rate;
	const int explosion_radius = 50; // 爆発の範囲の半径
	const int explosion_density = 10; // 爆発の密度、爆発の範囲にどれだけ弾(小爆発)を敷き詰めるか

	const Rect bullet_base = Rect(0, 0, 10, 15);
	const int bullet_visible_frame = 120;
	const int explosion_visible_frame = 20;

	void shot() override
	{
		// 発射エフェクトの描画のフラグを立てるならここに書くとよいかもしれない
		bullet_list.push_back(Bullet(bullet_image, bullet_base.w, bullet_base.h, pos - bullet_base.center(), Vec2(Random<double>(-0.4, 0.4), -10), 0, bullet_visible_frame, U"flieger"));
	}
	

	void explode_small(Vec2 pos_center, int radius)
	{
		bullet_list.push_back(Bullet(explosion_image, radius * 2, radius * 2, pos_center, Vec2(0, 0), damage, explosion_visible_frame,U"explosion"));
	}


	
};
#endif
