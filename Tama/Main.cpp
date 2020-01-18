
# include <Siv3D.hpp> // OpenSiv3D v0.4.2

/*
 * やること
 *
 *	シーンUIの実装は知らん...
 *
 * 1. シーン遷移とゲーム画面の分離
 * 1-2. 描画の分離
 * 2. ゲームステートをうまく使ってシーンを分離
 * 3. シーンごとにswitchブロックを生成(ロジックのみ, 描画は後程って感じで)
 * 4. 各ステージのロジックを実装.
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
	int game_state = 0;

	constexpr int user_move_speed = 10;
	constexpr int user_size = 50;
	
	WeaponBase* user_wp = new PredatorCannon(Scene::Center());
	User user = User(user_wp, 100, Scene::Center() ,0);
	Rect user_collision = Rect(Scene::Center(), user_size);
	const Texture user_image = Texture(U"Path");

	Vec2 spawn_pos = RandomVec2(Scene::Rect());
	Array<Enemy> enemies;
	WeaponBase* temp_wp = new SampleWeapon(spawn_pos, Vec2(0, 5));
	EnemyAIBase* temp_ai = new EnemyAISample(temp_wp);

	enemies.push_back(Enemy(temp_wp, temp_ai, Texture(U"").resized(200, 200), 1000, spawn_pos));
	
	const Font title = Font(40);
	
	
	while (System::Update())
	{
		// 制御ロジック

		// タイトル画面
		if (game_state == 0)
		{
			if (SimpleGUI::ButtonAt(U"ガトリング砲を試す", Scene::Center(), 250))
			{
				game_state = 1;
				user_wp = new PredatorCannon(Scene::Center());
			}
			if (SimpleGUI::ButtonAt(U"レールガンを試す", Scene::Center() + Vec2(0, 50), 250))
			{
				game_state = 1;
				user_wp = new PlasmaRailGun(Scene::Center());
			}
			if (SimpleGUI::ButtonAt(U"フリーガーハマーを試す", Scene::Center() + Vec2(0, 100), 250))
			{
				game_state = 1;
				user_wp = new Fliegerhummer(Scene::Center());
			}
			if (SimpleGUI::ButtonAt(U"アプリを終了する", Scene::Center() + Vec2(0, 150), 250))
			{
				System::Exit();
			}
			title(U"プロトタイプ１").drawAt(Scene::Center() - Vec2(0, 100));
			continue; // ゲーム画面の処理をスキップ
		}


		if (user.get_hp() <= 0)
		{
			if (SimpleGUI::ButtonAt(U"アプリを終了する", Scene::Center() + Vec2(0, 100), 250))
			{
				System::Exit();
			}
			title(U"ゲームオーバー").drawAt(Scene::Center() - Vec2(0, 50));
			continue;
		}

		// ゲーム画面

		// userの移動
		Vec2 move_pos = user.get_pos();
		move_pos += Vec2(KeyD.pressed() - KeyA.pressed(), KeyS.pressed() - KeyW.pressed()).setLength(user_move_speed);
		if (move_pos.intersects(Scene::Rect())) // userをシーンの外に出さない
		{
			user.set_pos(move_pos);
			user_collision.setCenter(user.get_pos().asPoint()); // 当たり判定の移動
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
		
		// enemyの移動
		for (int i = 0; i < enemies.size(); i++)
		{
			enemies[i].update();
		}
		
		// 武器の移動、発射処理
		user_wp->update(user.get_pos());

		// 当たり判定の処理
		// 自機の弾丸が敵に当たっているかどうか
		for (int i = 0; i < user_wp->getBullets().size(); i++)
		{
			for (int j = 0; j < enemies.size(); j++)
			{
				if (user_wp->getBullets()[i].getCollision().intersects(enemies[j].get_collision()))
				{
					enemies[j].set_hp(enemies[j].get_hp() - user_wp->getBullets()[i].hit());
				}

				if(enemies[j].get_hp() <= 0)
				{
					enemies.pop_front();
				}
			}
		}

		// 敵の弾丸が自機に当たっているかどうか
		for (int i = 0; i < enemies.size(); i++)
		{
			for (int j = 0; j < enemies[i].get_weapon()->getBullets().size(); j++)
			{
				if (enemies[i].get_weapon()->getBullets()[j].getCollision().intersects(user_collision))
				{
					user.set_hp(user.get_hp() - enemies[i].get_weapon()->getBullets()[j].hit());
				}
			}
		}

		
		// 描画処理
		for (int i = 0; i < enemies.size(); i++)
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
