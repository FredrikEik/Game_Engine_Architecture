#pragma once
#include "BaseSystem.h"
#include <iostream>
#include "../Components/Components.h"

static constexpr float Gravity = 9.81f;

class PhysicsSystem final : public BaseSystem
{

public:
    static void UpdatePhysics(class ECSManager* ECS, uint32 terrainEntity, float DeltaTime);

private:

};

