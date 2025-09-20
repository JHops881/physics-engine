#include "ObjectInteractor.hpp"
#include <iostream>

namespace phys
{

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

} // namespace phys