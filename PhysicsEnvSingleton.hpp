#pragma once
#include <glm/glm.hpp>
#include <unordered_map>
#include <uuid_v4.h>
#include <format>

#include "RigidBody.hpp"
#include "Collider.hpp"
#include "ObjectInterActor.hpp"

namespace phys
{

/// <summary>
/// Physics Environment
/// </summary>
class PhysicsEnvSingleton
{
  private:
    std::unordered_map<UUIDv4::UUID, RigidBody> rigid_bodies_;
    std::unordered_map<UUIDv4::UUID, Collider>  colliders_;
    UUIDv4::UUIDGenerator<std::mt19937_64>      uuid_generator_;

    glm::vec3 gravity_ = glm::vec3(0, -9.81f, 0);

    // Private constructor to prevent direct instantiation
    PhysicsEnvSingleton() = default;

    // Delete copy constructor and assignment operator to prevent copying
    PhysicsEnvSingleton(const PhysicsEnvSingleton&) = delete;
    PhysicsEnvSingleton& operator=(const PhysicsEnvSingleton&) = delete;

  public:
    /// <summary>
    /// Static method to get the single instance
    /// </summary>
    /// <returns>Reference to PhysicsEnvSingleton</returns>
    static PhysicsEnvSingleton& getInstance();
        
    /// <summary>
    /// Create a new static object. 
    /// </summary>
    /// <param name="position">: location in the physics environment where the static object will be placed</param>
    /// <param name="collider_type">: shape of the bounding box of the static object</param>
    /// <returns>UUID of the object--which is what the physics environment uses to refer to and retrieve it</returns>
    const UUIDv4::UUID AddStaticObject(const glm::vec3& position, ColliderType collider_type) noexcept;

    /// <summary>
    /// Create a new physics object that is newtonian/affected by forces.
    /// </summary>
    /// <param name="collider_type">: shape of the bounding box of the dynamic object</param>
    /// <param name="position">: location in the physics environment where the dynamic object will be placed</param>
    /// <param name="velocity">: speed at which the dynamic object is traveling initially.</param>
    /// <param name="mass">: mass of the dynamic object</param>
    /// <param name="scale_factor">: scale factor of the bounding box relative to the dynamic object</param>
    /// <param name="relative_position">: positional offset of the bouding box relative to the dynamic object</param>
    /// <returns>UUID of the object--which is what the physics environment uses to refer to and retrieve it</returns>
    const UUIDv4::UUID AddDynamicObject
    (
              ColliderType collider_type,
        const glm::vec3&   position          = glm::vec3(0.0f),
        const glm::vec3&   velocity          = glm::vec3(0.0f),
              float        mass              = 1.0f,
              float        scale_factor      = 1.0f,
        const glm::vec3&   relative_position = glm::vec3(0.0f)
    )
    noexcept;

    /// <summary>
    /// Remove an object from the physics environment.
    /// </summary>
    /// <param name="uuid">: UUID of the object that you want to remove</param>
    void RemoveObject(const UUIDv4::UUID& uuid);

    /// <summary>
    /// Retrieve and interface for interacting with an object by requesting it from the physics environment by UUID.
    /// </summary>
    /// <param name="uuid">: UUID of the object that you want to interact with</param>
    /// <returns>ObjectInteractor interface to the object--used to interact with it</returns>
    ObjectInteractor GetObject(const UUIDv4::UUID& uuid);

    /// <summary>
    /// Advance the physics simulation by one simulation step.
    /// </summary>
    /// <param name="delta_time">: time elapsed since the last update</param>
    void Step(float delta_time);

    /// <summary>
    /// Prints debug information about a physical object in the console.
    /// </summary>
    /// <param name="uuid">: UUID of the object</param>
    void DebugObject(const UUIDv4::UUID& uuid);
};

} // namespace phys