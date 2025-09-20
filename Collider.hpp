#pragma once
#include <glm/glm.hpp>
#include <uuid_v4.h>
#include <optional>

namespace phys
{
/// <summary>
/// 3D shape of a Collider's bounding box
/// </summary>
enum ColliderType
{
    CUBE,
    SPHERE
};

/// <summary>
/// 3D volume that exists in space and is able to collide with other instances of itself
/// </summary>
class Collider
{
  private:
    ColliderType                collider_type_;
    std::optional<UUIDv4::UUID> associated_rigid_body_uuid_;
    float                       scale_factor_;
    glm::vec3                   relative_position_;

  public:
    /// <summary>
    /// Create a new Collider
    /// </summary>
    /// <param name="collider_type">: 3D shape of a Collider's bounding box</param>
    /// <param name="associated_rigid_body_uuid">: optional reference to the UUID of a RigidBody to be coupled with.
    /// (for dynamic objects)</param>
    /// <param name="scale_factor">: size multiplier of Collider bounding box relative to RigidBody if it has one</param>
    /// <param name="relative_position">: positional offset of Collider relative to the RigidBody if it has one, otherwise
    /// it is relative to the world origin.</param>
    Collider
    (
              ColliderType                 collider_type,
        const std::optional<UUIDv4::UUID>& associated_rigid_body_uuid = std::nullopt,
              float                        scale_factor               = 1.0f,
        const glm::vec3&                   relative_position          = glm::vec3(0.0f)
    );

    const glm::vec3& GetRelativePosition() const;

    ColliderType GetColliderType() const;

    float GetScaleFactor() const;

    const std::optional<UUIDv4::UUID>& GetAssociatedRigidBodyUUID();

    /// <summary>
    /// Moves the Collider relative to the RigidBody if it has one, othewise relative to the world origin.
    /// </summary>
    /// <param name="displacement">: change in distance amount</param>
    void Translate(const glm::vec3& displacement);
};

} // namespace phys