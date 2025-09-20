#include "physics.hpp"
#include <iostream>

namespace phys
{

RigidBody::RigidBody
(
    const UUIDv4::UUID& associated_collider_uuid,
    const glm::vec3&    position,
          float         mass,
    const glm::vec3&    velocity
)
    :
    associated_collider_uuid_(associated_collider_uuid),
    position_                (position),
    mass_                    (mass),
    velocity_                (velocity),
    force_                   (glm::vec3(0.0f))
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



ObjectInteractor::ObjectInteractor
(
    ObjectType type,
    Collider*  collider,
    RigidBody* rigid_body
)
    :
    type_      (type),
    collider_  (collider),
    rigid_body_(rigid_body)
{
}

const glm::vec3& ObjectInteractor::GetPosition() const
{
    if (type_ == ObjectType::DYNAMIC)
    {
        return rigid_body_->GetPosition();
    }
    else if (type_ == ObjectType::STATIC)
    {
        return collider_->GetRelativePosition();
    }
}

const glm::vec3 ObjectInteractor::GetVelocity() const
{
    if (type_ == ObjectType::DYNAMIC)
    {
        return rigid_body_->GetVelocity();
    }
    else if (type_ == ObjectType::STATIC)
    {
        return glm::vec3(0.0f);
    }
}

const glm::vec3 ObjectInteractor::GetForce() const
{
    if (type_ == ObjectType::DYNAMIC)
    {
        return rigid_body_->GetForce();
    }
    else if (type_ == ObjectType::STATIC)
    {
        return glm::vec3(0.0f);
    }
}

float ObjectInteractor::GetMass() const
{
    if (type_ == ObjectType::DYNAMIC)
    {
        return rigid_body_->GetMass();
    }
    else if (type_ == ObjectType::STATIC)
    {
        return 0.0f;
    }

}

void ObjectInteractor::ApplyForce(const glm::vec3& force)
{
    if (type_ == ObjectType::DYNAMIC)
    {
        rigid_body_->ApplyForce(force);
    }
    else if (type_ == ObjectType::STATIC)
    {
        std::cout << "Nothing happens.. You've applied force to a static object.";
    }
}

void ObjectInteractor::Translate(const glm::vec3& displacement)
{
    if (type_ == ObjectType::DYNAMIC)
    {
        rigid_body_->Translate(displacement);
    }
    else if (type_ == ObjectType::STATIC)
    {
        collider_->Translate(displacement);
    }
}

// Static method to get the single instance
PhysicsEnvSingleton& PhysicsEnvSingleton::getInstance()
{
    // Local static variable ensures single instance and thread-safe
    // initialization (C++11 onwards)
    static PhysicsEnvSingleton instance;
    return instance;
}

const UUIDv4::UUID PhysicsEnvSingleton::AddStaticObject
(
    const glm::vec3&   position,
          ColliderType collider_type
)
    noexcept {
    UUIDv4::UUID uuid = uuid_generator_.getUUID();
    colliders_.emplace(uuid, Collider(collider_type, std::nullopt, 1.0f, position));
    return uuid;
}

const UUIDv4::UUID PhysicsEnvSingleton::AddDynamicObject
(
          ColliderType collider_type,
    const glm::vec3&   position,
    const glm::vec3&   velocity,
          float        mass,
          float        scale_factor,
    const glm::vec3&   relative_position
)
    noexcept {
    // Under the hood, Dynamic Objects are a coupling of a Collider and a RigidBody.
    // Create 2 new UUIDs. One for RigidBody and on for Collider
    UUIDv4::UUID rigid_body_uuid = uuid_generator_.getUUID();
    UUIDv4::UUID collider_uuid = uuid_generator_.getUUID();
    // Add the new RigidBody in the physics environment
    rigid_bodies_.emplace(rigid_body_uuid, RigidBody(collider_uuid, position, mass, velocity));
    // Add the new collider in the physics environment
    colliders_.emplace(collider_uuid, Collider(collider_type, rigid_body_uuid, scale_factor, relative_position));
    // Give back the UUID that the developer can use to refer to this coupling. It's always just
    // the Colider UUID, but that implementation is hidden by using ObjectInteractor.
    return collider_uuid;
}

void PhysicsEnvSingleton::RemoveObject(const UUIDv4::UUID& uuid)
{
    // For colliders with associated RigidBodies, clean up the RigidBodies too.
    if (colliders_.find(uuid)->second.GetAssociatedRigidBodyUUID().has_value())
    {
        UUIDv4::UUID rigid_body_uuid = colliders_.find(uuid)->second.GetAssociatedRigidBodyUUID().value();
        colliders_.erase(uuid);
        rigid_bodies_.erase(rigid_body_uuid);
    }
    else
    {
        colliders_.erase(uuid); // Otherwise, it was just a collider. Let's clean it up.
    }
}

ObjectInteractor PhysicsEnvSingleton::GetObject(const UUIDv4::UUID& uuid)
{
    // These UUIDs of the "objects" are actually just the Collider UUID
    Collider& collider = colliders_.find(uuid)->second;
    // Check if the collider has an associated RidigBody (dynamic objects)
    if (collider.GetAssociatedRigidBodyUUID().has_value())
    {
        RigidBody& rigid_body = rigid_bodies_.find(collider.GetAssociatedRigidBodyUUID().value())->second;
        ObjectInteractor object_interactor = ObjectInteractor(ObjectType::DYNAMIC, &collider, &rigid_body);
        return object_interactor;
    }
    else
    {
        ObjectInteractor object_interactor = ObjectInteractor(ObjectType::STATIC, &collider, nullptr);
        return object_interactor;
    }
}

void PhysicsEnvSingleton::Step(float delta_time)
{
    for (auto& [uuid, rb] : rigid_bodies_)
    {
        // Accumulate gravity as a force. other forces (like thrust,
        // collisions, etc.) are added elsewhere
        glm::vec3 force_of_gravity = rb.GetMass() * gravity_;
        rb.ApplyForce(force_of_gravity);

        // Newton's second law: a = f/m
        glm::vec3 acceleration;
        if (rb.GetMass() != 0.0f) // avoid diving my zero if the mass is 0
        {
            acceleration = rb.GetForce() / rb.GetMass();
        }
        else
        {
            acceleration = glm::vec3(0.0f);
        }

        // Integrates acceleration into velocity. This is the semi - implicit
        // Euler method, which is more stable than simple(explicit) Euler.
        rb.Accelerate(acceleration * delta_time);

        // Integrates velocity into position.
        glm::vec3 displacement = rb.GetVelocity() * delta_time;
        rb.Translate(displacement);

        // Clears the force accumulator so the next step can
        // apply new forces cleanly.
        rb.ResetForce();
    }
}

} // namespace phys