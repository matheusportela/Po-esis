// @file   StageState.cpp
// @author Matheus Vieira Portela
// @date   25/03/2015
//
// @brief Game level

#include "StageState.h"

StageState::StageState()
{
    quitRequested = false;
    bg = std::unique_ptr<Sprite>(new Sprite(CFG_GETP("STATE_BACKGROUND")));
    std::unique_ptr<TileSet> tileSet(new TileSet(CFG_GETI("TILE_WIDTH"),
        CFG_GETI("TILE_HEIGHT"), CFG_GETP("TILE_SET")));
    tileMap = std::unique_ptr<TileMap>(new TileMap(CFG_GETP("TILE_MAP"),
        std::move(tileSet)));

    InitializePenguins();
    InitializeAlien();

    ConfigureInputCommands();
}

void StageState::InitializePenguins()
{
    Point position(CFG_GETI("PENGUINS_INITIAL_X"),
        CFG_GETI("PENGUINS_INITIAL_Y"));
    GameObjectFactory::CreatePenguins(position, "player");
    Camera::Follow(GameObjectManager::GetInstance().GetObject("player"));
}

void StageState::InitializeAlien()
{
    Point position(CFG_GETI("ALIEN_INITIAL_X"),
        CFG_GETI("ALIEN_INITIAL_Y"));
    int numMinions = CFG_GETI("ALIEN_NUM_MINIONS");
    GameObjectFactory::CreateAlien(position, numMinions, "alien");
}

void StageState::ConfigureInputCommands()
{
    quitButtonCommand = CommandFactory::CreateQuitCommand(this,
        InputType::QuitButtonPress);

    quitEscCommand = CommandFactory::CreateQuitCommand(this,
        InputType::KeyPress, KeyboardButton::Esc);

    cameraUpCommand = CommandFactory::CreateCameraUpCommand(
        InputType::KeyDown, KeyboardButton::ArrowUp);

    cameraDownCommand = CommandFactory::CreateCameraDownCommand(
        InputType::KeyDown, KeyboardButton::ArrowDown);
    
    cameraLeftCommand = CommandFactory::CreateCameraLeftCommand(
        InputType::KeyDown, KeyboardButton::ArrowLeft);

    cameraRightCommand = CommandFactory::CreateCameraRightCommand(
        InputType::KeyDown, KeyboardButton::ArrowRight);
}

void StageState::Update(float dt)
{
    Camera::Update(dt);
    InputManager::GetInstance().ProcessInputs();
    GameObjectManager::GetInstance().Update(dt);
    CollisionSimulator::Collide();
}

void StageState::Render()
{
    RenderBottomTiles();
    GameObjectManager::GetInstance().Render();
    RenderTopTiles();
}

void StageState::RenderBottomTiles()
{
    Point bg_point(0, 0);

    // Movement illusion requires background to move in the direction opposite
    // to the camera. Hence, we negate it's position.
    Point tile_map_point = Camera::position;
    tile_map_point.Negate();

    bg->Render(bg_point);
    tileMap->RenderBaseLayer(tile_map_point);
}

void StageState::RenderTopTiles()
{
    // Movement illusion requires background to move in the direction opposite
    // to the camera. Hence, we negate it's position.
    Point tile_map_point = Camera::position;
    tile_map_point.Negate();
    
    tileMap->RenderUpperLayers(tile_map_point);
}