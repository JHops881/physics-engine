#pragma once
#include <glm/glm.hpp>
#include "Collider.hpp"
#include "RigidBody.hpp"

namespace phys
{

/// <summary>
/// Aspect that determines whether or not an object is Newtonian/affected by forces.
/// </summary>
enum ObjectType
{
    DYNAMIC,
    STATIC
};

/// <summary>
/// An interface for interacting with a physical object. Abstracts away from whether or not the object is .
/// </summary>
class ObjectInteractor
{
  private:
    ObjectType type_;
    Collider*  collider_;
    RigidBody* rigid_body_;

  public:
    ObjectInteractor
    (
        ObjectType type,
        Collider*  collider,
        RigidBody* rigid_body
    );

    const glm::vec3& GetPosition() const;

    const glm::vec3 GetVelocity() const;

    const glm::vec3 GetForce() const;

    float GetMass() const;

    void ApplyForce(const glm::vec3& force);

    // Move the Object in the physical environment by the `displacement` amount.
    void Translate(const glm::vec3& displacement);
};

} // namespace phys