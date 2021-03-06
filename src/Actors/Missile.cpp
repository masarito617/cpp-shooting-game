#include "Missile.h"
#include "Enemy.h"
#include "BombEffect.h"
#include "../Game.h"
#include "../Components/SpriteComponent.h"
#include "../Components/ColliderComponent.h"

Missile::Missile(class Game *game)
:Actor(game)
{
    // スプライト追加
    auto* sprite = new SpriteComponent(this, 90);
    sprite->SetTexture(GetGame()->LoadTexture(GetGame()->GetAssetsPath() + "missile.png"));
    // コライダ追加
    mCollider = new ColliderComponent(this);
    mCollider->SetRadius(50.0f * GetScale());
}

// アクタ更新
void Missile::UpdateActor(float deltaTime)
{
    // 親のメソッド呼び出し
    Actor::UpdateActor(deltaTime);
    // ミサイルを移動させる
    Vector2 pos = GetPosition();
    pos.y -= mMissileSpeed * deltaTime;
    // 画面上部で破棄
    if (pos.y <= 50.0f)
    {
        SetState(EDead);
    }
    SetPosition(pos);

    // エネミーと衝突したら破壊
    for (auto enemy : GetGame()->GetEnemies())
    {
        if (Intersect(*mCollider, *(enemy->GetCollider())))
        {
            SetState(EDead);
            enemy->SetState(EDead);
            // エネミーの位置で爆発させる
            auto* bomb = new BombEffect(GetGame());
            bomb->SetPosition(Vector2(enemy->GetPosition()));
            break;
        }
    }
}
