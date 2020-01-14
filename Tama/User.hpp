#pragma once

// �C���N���[�h�K�[�h
#ifndef USER_HPP
#define USER_HPP

#include "Person.hpp"

// �����Ƀp�[�\������h���������[�U�[�̃N���X���L�q����
class User : public PersonBase
{
public:
	User() : User(nullptr, 0, 0, 0, 0)
	{
	}

	User(WeaponBase* wp, const int hp, const int x, const int y, const int item): PersonBase(wp, hp, x, y), item_(item)
	{
	}

	// �f�X�g���N�^(������)
	~User()
	{
	}

	void draw() const override final
	{
		// �������ł������܂�
		Print(this->get_hp());
		Print(this->get_weapon());
		Print(this->get_x());
		Print(this->get_y());
		Print(this->get_item());
	}

	///////////////////////
	// �A�N�Z�T�[
	void set_item(const int item)
	{
		this->item_ = item;
	}

	int get_item() const
	{
		return this->item_;
	}

private:
	int item_;
};

#endif
