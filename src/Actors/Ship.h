#pragma once
#include "Actor.h"

// 宇宙船クラス
class Ship : public Actor {
public:
    Ship(class Game* game);
    void UpdateActor(float deltaTime) override; // アクタ更新
    void ProcessKeyboard(const uint8_t* state); // キーボード入力

private:
    float mRightSpeed; // 右方向への移動速度
    float mDownSpeed;  // 下方向への移動速度
    const float ShipSpeed = 420.0f; // 移動速度

    bool mIsCanShot;      // ミサイルを撃てるかどうか？
    float mShotDeltaTime; // ミサイルを撃ってからの時間
    const float CanShotTime = 0.15f; // ミサイルを撃てるようになるまでの時間
};
