//DB
#pragma once
#include <glm/glm.hpp>

struct MovementComponent
{
	glm::vec3 velocity = glm::vec3(0.0f);
	glm::vec3 acceleration = glm::vec3(0.0f);
	float damping = 0.98f;

    void RegisterExports(const std::string& ownerName)
    {
        ExportRegistry::Instance().Register({
            "MovementComponent",
            ownerName,
            "velocity",
            typeid(decltype(velocity)).name(),
            this,
            &velocity,
            sizeof(velocity)
            });

        ExportRegistry::Instance().Register({
            "MovementComponent",
            ownerName,
            "acceleration",
            typeid(decltype(acceleration)).name(),
            this,
            &acceleration,
            sizeof(acceleration)
            });

        ExportRegistry::Instance().Register({
            "MovementComponent",
            ownerName,
            "damping",
            typeid(decltype(damping)).name(),
            this,
            &damping,
            sizeof(damping)
            });
    }
};