#include "poiesis/systems/InfectionSystem.h"

std::string InfectionSystem::GetName()
{
    return "InfectionSystem";
}

void InfectionSystem::Update(float dt)
{
    auto entities = Engine::GetInstance().GetAllEntitiesWithComponentOfClass("InfectionComponent");

    for (auto entity : entities)
    {
        auto infectionComponent = std::static_pointer_cast<InfectionComponent>(Engine::GetInstance().GetSingleComponentOfClass(entity, "InfectionComponent"));
        
        if (infectionComponent->GetTemporary())
        {
            auto remainingTime = infectionComponent->GetRemainingTime();

            remainingTime -= dt;

            if (remainingTime <= 0)
            {
                remainingTime = 0;
                infectionComponent->SetInfectionType(NoInfection);
                infectionComponent->SetTemporary(false);

                auto spriteComponents = Engine::GetInstance().GetComponentsOfClass(entity, "SpriteComponent");
                Engine::GetInstance().GetEntityManager()->DeleteComponentsOfClass(entity, "SpriteComponent");
                Engine::GetInstance().AddComponent(
                    std::make_shared<SpriteComponent>(CFG_GETP("CELL_ANIMATION"),
                        Vector(0, 0), 0, 0, true,
                        CFG_GETF("CELL_ANIMATION_SCALE"),
                        CFG_GETI("CELL_ANIMATION_NUM_FRAMES"),
                        CFG_GETF("CELL_ANIMATION_FRAME_DURATION"), true, true),
                    entity);
                for (unsigned int i = 1; i < spriteComponents.size(); ++i)
                    Engine::GetInstance().AddComponent(spriteComponents[i], entity);
            }

            infectionComponent->SetRemainingTime(remainingTime);
        }

        if (infectionComponent->GetInfectionType() == StrongImpulses)
        {
            Random r;
            float strongImpulseChance = 0.1;

            if (r.GenerateFloat() < strongImpulseChance)
            {
                auto particleComponent = std::static_pointer_cast<ParticleComponent>(Engine::GetInstance().GetSingleComponentOfClass(entity, "ParticleComponent"));

                Vector randomForce(r.GenerateFloat(-1, 1), r.GenerateFloat(-1, 1));
                randomForce *= CFG_GETF("INFECTION_IMPULSES_FORCE");

                particleComponent->SetForce(randomForce);
            }
        }
    }
}