
# include <Siv3D.hpp> // OpenSiv3D v0.4.2

//----------------------- 
// 自作クラス
#include "Person.hpp"
#include "User.hpp"
#include "Enemy.hpp"
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

		// ゲーム画面

		// userの移動
		Vec2 move_pos = user.get_pos();
		move_pos += Vec2(KeyD.pressed() - KeyA.pressed(), KeyS.pressed() - KeyW.pressed()).setLength(user_move_speed);
		if (move_pos.intersects(Scene::Rect())) // userをシーンの外に出さない
		{
			user.set_pos(move_pos);
			user_collision.setCenter(user.get_pos().asPoint()); // 当たり判定の移動
		}

		// enemyの移動
		// hogehoge();
		
		// 武器の移動、発射処理
		user_wp->update(user.get_pos());

		// 当たり判定の処理
		// hogehoge();

		
		// 描画処理
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
