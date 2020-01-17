#pragma once

// インクルードガード
#ifndef ENEMY_AI_HPP
#define ENEMY_AI_HPP

#include <Siv3D.hpp>

class EnemyAIBase
{
public:
	EnemyAIBase()
	{
	}

	virtual Vec2 update(Vec2 last_pos) = 0;

private:
};

#endif