#pragma once
#include <glm/glm.hpp>
#include <uuid_v4.h>

namespace phys
{
/// <summary>
/// A Newtonian physical object with an unchanging form, but is affected by forces.
/// </summary>
class RigidBody
{
  private:
    glm::vec3    position_;
    glm::vec3    velocity_;
    glm::vec3    force_;
    float        mass_;
    UUIDv4::UUID associated_collider_uuid_;

  public:
    /// <summary>
    /// Create a new RigidBody
    /// </summary>
    /// <param name="associated_collider_uuid">: UUID of Collider coupled to the RigidBody to enable collision</param>
    /// <param name="position">: 3D spatial location in the physics environment</param>
    /// <param name="mass">: mass of object</param>
    /// <param name="velocity">: initial speed of object</param>
    RigidBody
    (
        const UUIDv4::UUID& associated_collider_uuid,
        const glm::vec3&    position = glm::vec3(0.0f),
              float         mass     = 1.0f,
        const glm::vec3&    velocity = glm::vec3(0.0f)
    );

    const glm::vec3& GetPosition() const;

    const glm::vec3& GetVelocity() const;

    const glm::vec3& GetForce() const;

    const UUIDv4::UUID& GetAssociatedColliderUUID() const;

    float GetMass() const;

    void ApplyForce(const glm::vec3& force);

    /// <summary>
    /// Move the RigidBody in the physics environment
    /// </summary>
    /// <param name="displacement">: change in distance amount</param>
    void Translate(const glm::vec3& displacement);

    /// <summary>
    /// Change the velocity of the RigidBody
    /// </summary>
    /// <param name="acceleration">: change in velocity amount</param>
    void Accelerate(const glm::vec3& acceleration);

    /// <summary>
    /// Reset the net force of the Object to zero
    /// </summary>
    void ResetForce();
};

} // namespace phys