// @file   Penguins.cpp
// @author Matheus Vieira Portela
// @date   01/05/2015
//
// @brief Playable game object.

#include "Penguins.h"

Penguins::Penguins(const Point& position) : cannonRotation(0.0)
{
    SetSprite(CFG_GETP("PENGUINS_BODY_SPRITE"));
    SetCenter(position);
    hp = CFG_GETI("PENGUINS_HP");
    linearAcceleration = CFG_GETF("PENGUINS_ACCELERATION");
    angularSpeed = CFG_GETF("PENGUINS_ANGULAR_SPEED");
    maxLinearSpeed = CFG_GETF("PENGUINS_MAX_SPEED");
    cannonSprite = std::unique_ptr<Sprite>(
        new Sprite(CFG_GETP("PENGUINS_CANNON_SPRITE")));

    REGISTER_INPUT_KEY_CALLBACK(Penguins::SpeedUpCallback,
        InputType::KeyDown, KeyboardButton::LowercaseW);
    REGISTER_INPUT_KEY_CALLBACK(Penguins::SlowDownCallback,
        InputType::KeyDown, KeyboardButton::LowercaseS);
    REGISTER_INPUT_KEY_CALLBACK(Penguins::LeftRotationCallback,
        InputType::KeyDown, KeyboardButton::LowercaseA);
    REGISTER_INPUT_KEY_CALLBACK(Penguins::RightRotationCallback,
        InputType::KeyDown, KeyboardButton::LowercaseD);
    REGISTER_INPUT_KEY_CALLBACK(Penguins::Shoot, InputType::MousePress,
        MouseButton::Left);
}

void Penguins::UpdatePosition(float dt)
{
    Vector displacement = GetSpeed();
    displacement.Multiply(dt);

    // Apply the displacement to the penguin front.
    displacement.Rotate(GetRotation());
    
    Point position = GetCenter();
    position.Add(displacement);
    SetCenter(position);
}

void Penguins::UpdateCannonRotation()
{
    Point mousePosition = InputManager::GetInstance().GetMouseScreenPosition();
    Point cannonPosition = Camera::WorldToScreenPoint(GetCenter());

    Vector cannonToMouseVector;
    cannonToMouseVector.Set(mousePosition);
    cannonToMouseVector.Subtract(cannonPosition);

    cannonRotation = cannonToMouseVector.GetDirection();
}

void Penguins::Update(float dt)
{
    UpdatePosition(dt);
    UpdateCannonRotation();
}

void Penguins::RenderCannonSprite()
{
    Point renderPoint = Camera::WorldToScreenPoint(GetPoint());
    cannonSprite->Render(renderPoint, cannonRotation);
}

void Penguins::Render()
{
    RenderSprite();
    RenderCannonSprite();
}

bool Penguins::IsDead()
{
    return (hp <= 0);
}

void Penguins::NotifyCollision(std::shared_ptr<GameObject> other)
{
    LOG_D("Penguins collided");
}

Point Penguins::CalculateBulletPosition()
{
    Point cannonPosition = GetCenter();
    Point bulletPosition(cannonPosition);

    Vector offset;
    offset.Set(cannonSprite->GetWidth()/2, 0);
    offset.Rotate(cannonRotation);

    bulletPosition.Add(offset);
    return bulletPosition;
}

void Penguins::Shoot()
{
    Point bulletPosition = CalculateBulletPosition();
    GameObjectManager::GetInstance().Add(
        std::make_shared<Bullet>(bulletPosition, cannonRotation));
}

void Penguins::ChangeSpeed(float acceleration)
{
    Vector newSpeed = GetSpeed();
    Vector deltaSpeed;

    deltaSpeed.Set(1, 0);
    deltaSpeed.Normalize();
    deltaSpeed.Multiply(acceleration);

    newSpeed.Add(deltaSpeed);
    newSpeed.Saturate(maxLinearSpeed);
    SetSpeed(newSpeed);
}

void Penguins::SpeedUpCallback()
{
    ChangeSpeed(linearAcceleration);
}

void Penguins::SlowDownCallback()
{
    ChangeSpeed(-linearAcceleration);
}

void Penguins::LeftRotationCallback()
{
    float newRotation = GetRotation();
    newRotation -= angularSpeed;
    SetRotation(newRotation);
}

void Penguins::RightRotationCallback()
{
    float newRotation = GetRotation();
    newRotation += angularSpeed;
    SetRotation(newRotation);
}