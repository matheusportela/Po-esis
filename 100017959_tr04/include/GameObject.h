// @file   GameObject.h
// @author Matheus Vieira Portela
// @date   31/03/2015
//
// @brief Abstract class for every object in the game

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <memory>

#include "Point.h"
#include "Rect.h"
#include "Sprite.h"
#include "Vector.h"

class GameObject
{
  public:
    // Constructor for game object.
    GameObject() : rotation(0) {};

    // Deletes the game object.
    virtual ~GameObject() {};

    // Gets bounding rectangle.
    Rect GetBox() const;

    // Gets renderization point.
    Point GetPoint() const;

    // Gets object center.
    Point GetCenter() const;

    // Gets object rotation, in radians.
    float GetRotation() const;

    // Gets object speed.
    Vector GetSpeed() const;

    // Sets object linear speed.
    void SetSpeed(const Vector& speed);

    // Sets object sprite. This is usually done first, since other methods may
    // use sprite properties.
    void SetSprite(std::string file);

    // Sets object center, considering the it's sprite size.
    void SetCenter(const Point& center);

    // Sets object rotation, in radians.
    void SetRotation(float rotation);

    // Scales object dimensions.
    void SetScale(float scale);

    // Renders object's sprite.
    void RenderSprite();

    // Updates the game state after dt seconds.
    virtual void Update(float dt) = 0;

    // Renders everything necessary in the current frame.
    virtual void Render() = 0;

    // Implements behavior for the moment when the object is destroyed.
    virtual void OnDeath() {};

    // Communicates whether the game object must be deleted.
    virtual bool IsDead() = 0;

    // Implements behavior for collisions.
    virtual void NotifyCollision(std::shared_ptr<GameObject> other) = 0;

    // Checks whether the game object is of the given string type.
    virtual bool Is(std::string type) = 0;

  protected:
    // GameObject sprite that is rendered on screen.
    std::unique_ptr<Sprite> sprite;

    // Bounding box.
    Rect box;

    // Game object body rotation.
    float rotation;

    // Game object speed.
    Vector speed;
};

#endif // GAME_OBJECT_H_