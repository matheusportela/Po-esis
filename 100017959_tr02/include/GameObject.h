// @file   GameObject.h
// @author Matheus Vieira Portela
// @date   31/03/2015
//
// @brief Abstract class for every object in the game

#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include "Point.h"
#include "Rect.h"

class GameObject
{
  public:
    // Deletes the game object.
    virtual ~GameObject() {};

    // Getters and setters
    Rect& GetBox() { return box; }
    Point GetCenter()
    {
        Point point;
        point = box.GetCenter();
        return point;
    }

    // Updates the game state after dt seconds.
    virtual void Update(float dt) = 0;

    // Renderizes all necessary sprites.
    virtual void Render() = 0;

    // Communicates whether the game object must be deleted.
    virtual bool IsDead() = 0;

  protected:
    // Bounding box.
    Rect box;
};

#endif // GAME_OBJECT_H_