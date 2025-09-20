#include "Collider.hpp"

namespace phys
{

Collider::Collider
(
          ColliderType                 collider_type,
    const std::optional<UUIDv4::UUID>& associated_rigid_body_uuid,
          float                        scale_factor,
    const glm::vec3&                   relative_position
)
    :
    collider_type_             (collider_type),
    associated_rigid_body_uuid_(associated_rigid_body_uuid),
    scale_factor_              (scale_factor),
    relative_position_         (relative_position)
{
}

const glm::vec3& Collider::GetRelativePosition() const
{
    return relative_position_;
}

ColliderType Collider::GetColliderType() const
{
    return collider_type_;
}

float Collider::GetScaleFactor() const
{
    return scale_factor_;
}

const std::optional<UUIDv4::UUID>& Collider::GetAssociatedRigidBodyUUID()
{
    return associated_rigid_body_uuid_;
}

void Collider::Translate(const glm::vec3& displacement)
{
    relative_position_ += displacement;
}

} // namespace phys