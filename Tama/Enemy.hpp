#pragma once

// インクルードガード
#ifndef ENEMY_HPP
#define ENEMY_HPP

// ここにパーソンから派生した敵のクラスを記述する
class Enemy : public PersonBase
{
public:
	Enemy() : Enemy(nullptr, Texture(), 0, 0, 0, Vec2(0, 0))
	{
	}


	Enemy(WeaponBase* wp, Texture image, const int width, const int height, const int hp, const Vec2 pos) : PersonBase(wp, hp, pos), image_(image.resized(width, height)), collision_(Rect(pos.asPoint(), width, height))
	{
	}
	

	Enemy(WeaponBase* wp, TextureRegion resized_image, const int hp, const Vec2 pos) : PersonBase(wp, hp, pos), image_(resized_image), collision_(Rect(pos.asPoint(), resized_image.size.x, resized_image.size.y))
	{
	}

	// デストラクタ(未実装)
	~Enemy()
	{
	}


	void update()
	{
		
	}
	

	void draw() const override final
	{
		/*Print(this->get_hp());
		Print(this->get_weapon());
		Print(this->get_x());
		Print(this->get_y());*/
		image_.drawAt(get_pos());
	}

private:
	TextureRegion image_;
	Rect collision_;
};


#endif
