#include "RigidBody.hpp"

namespace phys
{

RigidBody::RigidBody
(
    const UUIDv4::UUID& associated_collider_uuid,
    const glm::vec3&    position,
    float               mass,
    const glm::vec3&    velocity
)
    :
    associated_collider_uuid_(associated_collider_uuid),
    position_(position),
    mass_(mass),
    velocity_(velocity),
    force_(glm::vec3(0.0f))
{
}

const glm::vec3& RigidBody::GetPosition() const
{
    return position_;
}

const glm::vec3& RigidBody::GetVelocity() const
{
    return velocity_;
}

const glm::vec3& RigidBody::GetForce() const
{
    return force_;
}

const UUIDv4::UUID& RigidBody::GetAssociatedColliderUUID() const
{
    return associated_collider_uuid_;
}

float RigidBody::GetMass() const
{
    return mass_;
}

void RigidBody::ApplyForce(const glm::vec3& force)
{
    force_ += force;
}

void RigidBody::Translate(const glm::vec3& displacement)
{
    position_ += displacement;
}

void RigidBody::Accelerate(const glm::vec3& acceleration)
{
    velocity_ += acceleration;
}

void RigidBody::ResetForce()
{
    static const glm::vec3 zero_vec(0.0f);
    force_ = zero_vec;
}

} // namespace phys