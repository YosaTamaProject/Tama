#pragma once

// �C���N���[�h�K�[�h
#ifndef EXTENSION_AI_HPP
#define EXTENSION_AI_HPP

#include <Siv3D.hpp>
#include "EnemyAI.hpp"


class EnemyAISample : EnemyAIBase
{
public:
	EnemyAISample() : EnemyAIBase()
	{

	}

	// enemy�̓��������L�q����֐�
	Vec2 update(Vec2 last_pos) override
	{
		if (last_pos != Scene::Center())
		{
			next_pos_ = last_pos;
			next_pos_ += get_direction_to_center(last_pos, 5);
		}
		return next_pos_;
	}

private:
	Vec2 next_pos_;

	// pos�����ʂ̒��S�Ɍ������Aspeed�̒����̃x�N�g�����擾����
	static Vec2 get_direction_to_center(const Vec2 pos, const int speed)
	{
		auto move = (Scene::Center() - pos).setLength(speed);
		return move;
	}
	
};

#endif