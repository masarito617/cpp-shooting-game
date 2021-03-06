#include "Ship.h"
#include <SDL.h>
#include "Enemy.h"
#include "Missile.h"
#include "BombEffect.h"
#include "../Game.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ColliderComponent.h"
#include "../Scenes/Scene.h"
#include "../Scenes/EndScene.h"

Ship::Ship(Game* game)
:Actor(game)
,mRightMove(0.0f)
,mDownMove(0.0f)
,mIsCanShot(false)
,mDeltaShotTime(0.0f)
{
    // スプライト設定
    auto* sprite = new SpriteComponent(this);
    sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "ship.png"));
    // コライダ追加
    mCollider = new ColliderComponent(this);
    mCollider->SetRadius(70.0f * GetScale());
}

// アクタ更新
void Ship::UpdateActor(float deltaTime)
{
    // 親のメソッド呼び出し
    Actor::UpdateActor(deltaTime);
    // ゲーム終了していたら動かさない
    if (GetGame()->GetScene()->GetSceneName().compare("END") != 0)
    {
        // 宇宙船を移動させる
        Vector2 pos = GetPosition();
        pos.x += mRightMove * deltaTime;
        pos.y += mDownMove * deltaTime;
        if (pos.x < 25.0f)
        {
            pos.x = 25.0f;
        }
        else if (pos.x > Game::ScreenWidth - 25.0f)
        {
            pos.x = Game::ScreenWidth - 25.0f;
        }
        if (pos.y < 25.0f)
        {
            pos.y = 25.0f;
        }
        else if (pos.y > Game::ScreenHeight - 25.0f)
        {
            pos.y = Game::ScreenHeight - 25.0f;
        }
        SetPosition(pos);
    }

    // エネミーと衝突したら死亡
    for (auto enemy : GetGame()->GetEnemies())
    {
        if (Intersect(*mCollider, *(enemy->GetCollider())))
        {
            // ゲーム終了
            GetGame()->SetNextScene(new EndScene(GetGame()));
            SetState(EDead);
            // 宇宙船の位置で爆発エフェクト
            auto* bomb = new BombEffect(GetGame());
            bomb->SetPosition(Vector2(GetPosition()));
            return;
        }
    }

    // ミサイルを撃つ間隔を開ける
    if (!mIsCanShot)
    {
        mDeltaShotTime += deltaTime;
        if (mDeltaShotTime > CanShotTime)
        {
            mIsCanShot = true;
            mDeltaShotTime = 0.0f;
        }
    }
}

// キーボード入力
void Ship::ProcessKeyboard(const uint8_t* state)
{
    mRightMove = 0.0f;
    mDownMove = 0.0f;
    // キー入力で上下左右に移動させる
    if (state[SDL_SCANCODE_D])
    {
        mRightMove += ShipSpeed;
    }
    if (state[SDL_SCANCODE_A])
    {
        mRightMove -= ShipSpeed;
    }
    if (state[SDL_SCANCODE_S])
    {
        mDownMove += ShipSpeed;
    }
    if (state[SDL_SCANCODE_W])
    {
        mDownMove -= ShipSpeed;
    }
    // ミサイルを撃つ
    if (state[SDL_SCANCODE_K])
    {
        if (mIsCanShot)
        {
            // 撃つ間隔を開けるためフラグを変更
            mIsCanShot = false;
            mDeltaShotTime = 0.0f;
            // ミサイル生成
            auto* missile = new Missile(GetGame());
            Vector2 pos = GetPosition();
            missile->SetPosition(Vector2(pos.x, pos.y - 30.0f));
        }
    }
}
