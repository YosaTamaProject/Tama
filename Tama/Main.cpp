﻿# include <Siv3D.hpp> // OpenSiv3D v0.4.2

// デバッグ用のマクロ
#define DEBUG_SCENE_TRANSITION 

/*
 * やること
 *
 *	シーンUIの実装は知らん...
 *
 * 1. シーン遷移とゲーム画面の分離 O
 * 1-2. 描画の分離 O
 * 2. ゲームステートをうまく使ってシーンを分離 O
 * 3. シーンごとにswitchブロックを生成(ロジックのみ, 描画は後程って感じで) O
 * 4. 各ステージのロジックを実装. <- IAの完成待ち
 * 4-2. 明らかなバグをフィックス
 * 5. テスト!!
 *
 * 描画と上手くいけばいいね. テクスチャなどは今のところいじらない方針でいきます.
 * 
 */

//----------------------- 
// 自作クラス
#include "Person.hpp"
#include "User.hpp"
#include "Enemy.hpp"
#include "EnemyAI.hpp"
#include "SampleEnemyAI1.hpp"
#include "Weapon.hpp"
#include "ExtensionWeapons.hpp"

void Main()
{
	// 初期化
	constexpr auto scene_title = 0;
	constexpr auto scene_select = 1;
	constexpr auto scene_pause = 2;
	constexpr auto scene_game_over = 3;
	constexpr auto scene_game_clear = 4;
	constexpr auto scene_result = 5;
	constexpr auto scene_stage_1 = 6;
	constexpr auto scene_stage_1_b = 7;
	constexpr auto user_move_speed = 10;
	constexpr auto user_size = 50;

	const auto title = Font(40);
	const auto user_image = Texture(U"Path");

	WeaponBase* user_wp = new PredatorCannon(Scene::Center());
	auto user = User(user_wp, 100, Scene::Center(), 0);
	auto user_collision = Rect(Scene::Center(), user_size);

	auto spawn_pos = RandomVec2(Scene::Rect());
	Stopwatch stopwatch;
	Array<Enemy> enemies;
	WeaponBase* temp_wp = new SampleWeapon(spawn_pos, Vec2(0, 5));
	EnemyAIBase* temp_ai = new EnemyAISample(temp_wp);

	enemies.push_back(Enemy(temp_wp, temp_ai, Texture(U"").resized(200, 200), 1000, spawn_pos));

	auto game_state = 0;
	std::stack<int> game_state_carry;

	////////////
	///init
	stopwatch.start();

	while (System::Update())
	{
		////////////////////////////////////////////////////////////////////////////////////
		// 制御ロジック
		////////////////////////////////////////////////////////////////////////////////////

		// 画面の状態遷移について
		/*
		 *   これをgame_stateとswitch文で実装します
		 *
		 *	シーンごとにマクロを用意しました. 
		 *  切り替え時にはこちらを使って下さい.
		 *
		 *  SCENE_TITLE,
		 *	SCENE_SELECT,
		 *	SCENE_PAUSE,
		 *	SCENE_GAME_OVER,
		 *	SCENE_GAME_CLEAR,
		 *	SCENE_RESULT,
		 *	SCENE_STAGE_1,
		 *	SCENE_STAGE_1_B,
		 *
		 *   [-----タイトル画面 (case 0)-----]
		 *   ↓                                 ↑
		 *   [-----セレクト画面 (case 1)-----] |
		 *   ↓                                 |
		 *   [-----第一ステージ (case 6)-----] |
		 *   |   ↓ ↑			↓ ↑            |
		 *   | [ポーズ画面] [ゲームオーバ-] -| |  ポーズ画面がcase 2, ゲームオーバーがcase 4です.
		 *   ↓			                     | |
		 *   [----第一ステージボス(case 7)]  | |
		 *   |   ↓ ↑			↓ ↑          | | 
		 *   | [ポーズ画面] [ゲームオーバ-] -| |
		 *   ↓                               | |
		 *   [----クリア画面(case 3)-------] | |
		 *   ↓                               ↓ |
		 *   [----リザルト画面(case 5)---------]                       
		 */

		switch (game_state)
		{
			// タイトル画面(エンター押して抜けるやつ)
		case scene_title:

			if (SimpleGUI::ButtonAt(U"ゲームスタート", Scene::Center() + Vec2(0, 100), 250))
			{
				game_state = scene_select;
			}
			title(U"😎たま😎").drawAt(Scene::Center() - Vec2(0, 50));
			continue;

			// セレクト画面(自機とか選ぶやつ)
		case scene_select:
			if (SimpleGUI::ButtonAt(U"ステージ1へ", Scene::Center(), 250))
			{
				game_state = scene_stage_1;
				user_wp = new PredatorCannon(Scene::Center());
				stopwatch.restart();
			}
			if (SimpleGUI::ButtonAt(U"ステージ1ボスへ", Scene::Center() + Vec2(0, 50), 250))
			{
				game_state = scene_stage_1_b;
				user_wp = new PlasmaRailGun(Scene::Center());
				stopwatch.restart();
			}
			if (SimpleGUI::ButtonAt(U"アプリを終了する", Scene::Center() + Vec2(0, 150), 250))
			{
				System::Exit();
			}
			title(U"プロトタイプ１").drawAt(Scene::Center() - Vec2(0, 100));
			continue; // ゲーム画面の処理をスキップ

			// ポーズ画面
		case scene_pause:
			if (SimpleGUI::ButtonAt(U"ゲームに戻る", Scene::Center() + Vec2(0, 100), 250) || KeyP.pressed())
			{
				game_state = game_state_carry.top();
				game_state_carry.pop();
			}
			title(U"ポーズ").drawAt(Scene::Center() - Vec2(0, 50));
			continue;

			// クリア画面
		case scene_game_clear:
			if (SimpleGUI::ButtonAt(U"結果を確認する", Scene::Center() + Vec2(0, 100), 250))
			{
				game_state = scene_result;
			}
			title(U"ゲームクリア!!!!").drawAt(Scene::Center() - Vec2(0, 50));
			continue;


			// ゲームオーバー画面
		case scene_game_over:
			if (SimpleGUI::ButtonAt(U"結果を確認する", Scene::Center() + Vec2(0, 100), 250))
			{
				game_state = scene_result;
			}
			title(U"ゲームオーバー").drawAt(Scene::Center() - Vec2(0, 50));
			continue;

			// リザルト画面
		case scene_result:
			if (SimpleGUI::ButtonAt(U"タイトルへ", Scene::Center() + Vec2(0, 100), 250))
			{
				game_state = scene_title;
			}
			title(U"リザルト").drawAt(Scene::Center() - Vec2(0, 50));
			continue;

			///////////////////////////////////////////////////////////////////////////////////
			// 第一ステージのロジック
			///////////////////////////////////////////////////////////////////////////////////
			/*
			タイマーで時間を測って, 1分経てばボスステージに遷移するのように
			時間で制御するのはどうでしょうか.

			メインループの上で初期化をしておいて, セレクト画面でセレクトされたときにタイマーをリセット.
			ゲームステージのswitch文ないで時間計測によるif文で制御をかける感じでどうでしょうか

			以下参考文献
			https://github.com/Siv3D/Reference-JP/wiki/%E7%B5%8C%E9%81%8E%E6%99%82%E9%96%93%E3%81%AE%E6%B8%AC%E5%AE%9A


			とりあえず, ここのブロックではエネミーの発生のみを行う方針で...
			
			*/
		case scene_stage_1:

			// ポーズがあるか確認する
			// 上手くうごかねぇ....
			if (KeyP.pressed())
			{
				game_state_carry.push(game_state);
				game_state = scene_pause;
			}

			// 60秒以内の敵出現と移動パターン
			if (stopwatch.s() < 60)
			{
				/* code */
			}

			// 120秒以内の敵出現と移動パターン
			else if (stopwatch.s() < 120)
			{
				/* code */
			}

			// ボスへ分岐
			else
			{
				game_state = scene_stage_1_b;
			}

			// enemyの発生
			if (enemies.size() == 0)
			{
				delete temp_wp;
				delete temp_ai;
				spawn_pos = RandomVec2(Scene::Rect());
				temp_wp = new SampleWeapon(spawn_pos, Vec2(0, 5));
				temp_ai = new EnemyAISample(temp_wp);
				enemies.push_back(Enemy(temp_wp, temp_ai, Texture(U"").resized(200, 200), 1000, spawn_pos));
			}
			break;

			//////////////////////////////////////////////////////////////////////////
			// 第一ボスのロジック
			///////////////////////////////////////////////////////////////////////////
		case scene_stage_1_b:
			// ポーズがあるか確認する
			if (KeyP.pressed())
			{
				game_state_carry.push(game_state);
				game_state = scene_pause;
			}
			break;

			// 1体出現と体力が削りきれた時点でゲーム終了でどうでしょう?

			/*code*/
			
		default:
			break;
		}

		////////////////////////////////////////////////////////////////////////////////////
		/// 制御, 描画ロジック
		////////////////////////////////////////////////////////////////////////////////////

		// userの移動
		auto move_pos = user.get_pos();
		move_pos += Vec2(KeyD.pressed() - KeyA.pressed(), KeyS.pressed() - KeyW.pressed()).setLength(
			user_move_speed);
		if (move_pos.intersects(Scene::Rect())) // userをシーンの外に出さない
		{
			user.set_pos(move_pos);
			user_collision.setCenter(user.get_pos().asPoint()); // 当たり判定の移動
		}

		// enemyの移動
		for (auto i = 0; i < enemies.size(); i++)
		{
			enemies[i].update();
		}

		// 武器の移動、発射処理
		user_wp->update(user.get_pos());

		// 当たり判定の処理
		// 自機の弾丸が敵に当たっているかどうか
		for (auto i = 0; i < user_wp->getBullets().size(); i++)
		{
			for (auto j = 0; j < enemies.size(); j++)
			{
				if (user_wp->getBullets()[i].getCollision().intersects(enemies[j].get_collision()))
				{
					enemies[j].set_hp(enemies[j].get_hp() - user_wp->getBullets()[i].hit());
				}

				if (enemies[j].get_hp() <= 0)
				{
					enemies.pop_front();
				}
			}
		}

		// 敵の弾丸が自機に当たっているかどうか
		for (auto i = 0; i < enemies.size(); i++)
		{
			for (auto j = 0; j < enemies[i].get_weapon()->getBullets().size(); j++)
			{
				if (enemies[i].get_weapon()->getBullets()[j].getCollision().intersects(user_collision))
				{
					user.set_hp(user.get_hp() - enemies[i].get_weapon()->getBullets()[j].hit());
				}
			}
		}

		// User の生存判定
		if (user.get_hp() <= 0)
		{
			game_state = scene_game_over;
		}

		////////////////////////////////////////////////////////////////////////////////////
		// 描画処理
		////////////////////////////////////////////////////////////////////////////////////
		for (auto i = 0; i < enemies.size(); i++)
		{
			enemies[i].draw();
		}
		user.draw();
		user_wp->draw();
		user_image.resized(user_size).drawAt(user.get_pos());
	}
}

//
// = アドバイス =
// Debug ビルドではプログラムの最適化がオフになります。
// 実行速度が遅いと感じた場合は Release ビルドを試しましょう。
// アプリをリリースするときにも、Release ビルドにするのを忘れないように！
//
// 思ったように動作しない場合は「デバッグの開始」でプログラムを実行すると、
// 出力ウィンドウに詳細なログが表示されるので、エラーの原因を見つけやすくなります。
//
// = お役立ちリンク =
//
// OpenSiv3D リファレンス
// https://siv3d.github.io/ja-jp/
//
// チュートリアル
// https://siv3d.github.io/ja-jp/tutorial/basic/
//
// よくある間違い
// https://siv3d.github.io/ja-jp/articles/mistakes/
//
// サポートについて
// https://siv3d.github.io/ja-jp/support/support/
//
// Siv3D Slack (ユーザコミュニティ) への参加
// https://siv3d.github.io/ja-jp/community/community/
//
// 新機能の提案やバグの報告
// https://github.com/Siv3D/OpenSiv3D/issues
//
