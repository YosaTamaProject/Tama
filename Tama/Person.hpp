#pragma once

// インクルードガード
#ifndef PERSON_HPP
#define PERSON_HPP

#include "ExtensionWeapons.hpp"

// ここにパーソンの基底クラスを記述する
class PersonBase
{
public:
	// コンストラクタ
	PersonBase() : PersonBase(nullptr, 0, 0, 0)
	{
	}

	PersonBase(const int hp, const int x, const int y) : PersonBase(nullptr, hp, x, y)
	{
	}

	PersonBase(WeaponBase* wp, const int hp, const int x, const int y) : wp_(wp), hp_(hp), x_(x), y_(y)
	{
	}

	// デストラクタ(未実装)
	~PersonBase()
	{
	}

	// 描画
	virtual void draw() const = 0;

	//////////////////////////////
	// アクセサー
	void set_weapon(WeaponBase* wp)
	{
		this->wp_ = wp;
	}

	WeaponBase* get_weapon() const
	{
		return this->wp_;
	}

	void set_hp(const int hp)
	{
		this->hp_ = hp;
	}

	int get_hp() const
	{
		return this->hp_;
	}

	void set_x(const int x)
	{
		this->x_ = x;
	}

	int get_x() const
	{
		return this->x_;
	}

	void set_y(const int y)
	{
		this->y_ = y;
	}

	int get_y() const
	{
		return this->y_;
	}

private:
	WeaponBase* wp_;
	int hp_;
	int x_;
	int y_;
};

#endif
