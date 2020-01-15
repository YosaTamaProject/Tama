#pragma once

// インクルードガード
#ifndef ENEMY_HPP
#define ENEMY_HPP

// ここにパーソンから派生した敵のクラスを記述する
class Enemy : public PersonBase
{
public:
	Enemy() : Enemy(nullptr, 0, Vec2(0, 0))
	{
	}

	Enemy(WeaponBase* wp, const int hp, const Vec2 pos) : PersonBase(wp, hp, pos)
	{
	}

	// デストラクタ(未実装)
	~Enemy()
	{
	}

	void draw() const override final
	{
		/*Print(this->get_hp());
		Print(this->get_weapon());
		Print(this->get_x());
		Print(this->get_y());*/
	}

private:

};


#endif
