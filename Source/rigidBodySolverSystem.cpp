//DB
#include "rigidBodySolverSystem.h"
#include "World.h"
#include "componentManager.h"
#include "rigidBodyComponent.h"
#include "Transform.h"

void RigidBodySolverSystem::Update(World& world, float dt)
{
    auto& cm = world.GetComponentManager();
    auto& contacts = world.GetNarrowphaseSystem()->GetContacts();

    for (auto& c : contacts)
    {
        RigidBodyComponent* rbA = cm.GetRigidBody(c.a);
        RigidBodyComponent* rbB = cm.GetRigidBody(c.b);

        // If either object has no rigidbody, skip
        if (!rbA && !rbB)
            continue;

        Transform* tA = cm.GetTransform(c.a);
        Transform* tB = cm.GetTransform(c.b);

        // Positional correction (push objects apart)
        float totalMass = (rbA ? rbA->mass : 0.0f) + (rbB ? rbB->mass : 0.0f);
        if (totalMass <= 0.0f)
            continue;

        float ratioA = rbA ? rbA->mass / totalMass : 0.0f;
        float ratioB = rbB ? rbB->mass / totalMass : 0.0f;

        glm::vec3 correction = c.normal * c.penetration;

        if (rbA && !rbA->isStatic)
            tA->position -= correction * ratioA;

        if (rbB && !rbB->isStatic)
            tB->position += correction * ratioB;

        // Velocity correction (bounce + friction)
        if (rbA && !rbA->isStatic)
        {
            float vn = glm::dot(rbA->velocity, c.normal);
            if (vn > 0.0f) vn = 0.0f;

            rbA->velocity -= c.normal * vn * (1.0f + rbA->restitution);
            rbA->velocity *= rbA->friction;
        }

        if (rbB && !rbB->isStatic)
        {
            float vn = glm::dot(rbB->velocity, -c.normal);
            if (vn > 0.0f) vn = 0.0f;

            rbB->velocity -= (-c.normal) * vn * (1.0f + rbB->restitution);
            rbB->velocity *= rbB->friction;
        }
    }

    // Integrate velocities -> positions
    auto& entities = cm.GetRigidBodyEntities();
    for (auto& e : entities)
    {
        RigidBodyComponent* rb = cm.GetRigidBody(e);
        Transform* t = cm.GetTransform(e);

        if (!rb || !t || rb->isStatic)
            continue;

        // Apply acceleration
        rb->velocity += rb->acceleration * dt;

        // Integrate velocity
        t->position += rb->velocity * dt;

        // Reset acceleration each frame
        rb->acceleration = glm::vec3(0.0f);
    }
}