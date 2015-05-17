#include "bandit/system/RenderingSystem.h"

RenderingSystem::RenderingSystem(std::shared_ptr<EntityManager> entityManager,
    std::shared_ptr<GraphicsAdapter> graphicsAdapter) :
    System(entityManager), graphicsAdapter(graphicsAdapter)
{
}

void RenderingSystem::Update(float dt)
{
    LOG_D("[RenderingSystem] Updating");

    std::string componentClass = "SpriteComponent";
    std::vector<std::shared_ptr<Entity>> entities =
        entityManager->GetAllEntitiesWithComponentOfClass(componentClass);
    std::vector<std::shared_ptr<Component>> components;

    for (std::shared_ptr<Entity> entity : entities)
    {
        components = entityManager->GetComponentsOfClass(entity, componentClass);
        for (std::shared_ptr<Component> component : components)
        {
            std::shared_ptr<SpriteComponent> spriteComponent =
                std::static_pointer_cast<SpriteComponent>(component);

            if (!graphicsAdapter->IsLoaded(spriteComponent->filename))
            {
                graphicsAdapter->LoadImage(spriteComponent->filename);
                LOG_D("[RenderingSystem] Loaded image \"" << spriteComponent->filename
                    << "\" for entity with ID: " << entity->GetId());
            }

            graphicsAdapter->RenderImage(spriteComponent->filename);
            LOG_D("[RenderingSystem] Rendered image \"" << spriteComponent->filename
                    << "\" for entity with ID: " << entity->GetId());
        }
    }
}