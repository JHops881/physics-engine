#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <functional>
#include <unordered_map>
#include <uuid_v4.h>

namespace phys {
  /*
  A physical object.
  */
  class RigidBody {
  private:
    glm::vec3 position_;
    glm::vec3 velocity_;
    glm::vec3 force_;
    float mass_;
    UUIDv4::UUID associated_collider_uuid_;

  public:
    RigidBody(
      const UUIDv4::UUID& associated_collider_uuid,
      const glm::vec3& position = glm::vec3(0.0f),
      float mass = 1.0f,
      const glm::vec3& velocity = glm::vec3(0.0f)
    ) :
      associated_collider_uuid_(associated_collider_uuid),
      position_(position),
      mass_(mass),
      velocity_(velocity),
      force_(glm::vec3(0.0f))
    {
    }

    const glm::vec3& GetPosition() const {
      return position_;
    }

    const glm::vec3& GetVelocity() const {
      return velocity_;
    }

    const glm::vec3& GetForce() const {
      return force_;
    }

    const UUIDv4::UUID& GetColliderUUID() const {
      return associated_collider_uuid_;
    }

    float GetMass() const {
      return mass_;
    }

    void ApplyForce(const glm::vec3& force) {
      force_ += force;
    }

    /*
    Move the Object in the physical environment by the `displacement` amount.
    */
    void Translate(const glm::vec3& displacement) {
      position_ += displacement;
    }

    /*
    Accelerate the Object's speed by the `acceleration` amount.
    */
    void Accelerate(const glm::vec3& acceleration) {
      velocity_ += acceleration;
    }

    /*
    Reset the net force of the Object to zero.
    */
    void ResetForce() {
      static const glm::vec3 zero_vec(0.0f);
      force_ = zero_vec;
    }
  };

  enum ColliderType {
    CUBE,
    SPHERE
  };

  class Collider {
  private:
    ColliderType collider_type_;
    UUIDv4::UUID associated_object_;
    float scale_factor_;
    glm::vec3 relative_position_;

  public:
    Collider(
      ColliderType collider_type,
      const UUIDv4::UUID& associated_object,
      float scale_factor = 1.0f,
      const glm::vec3& relative_position = glm::vec3(0.0f)
    )
      : collider_type_(collider_type),
      associated_object_(associated_object),
      scale_factor_(scale_factor),
      relative_position_(relative_position)
    {
    }

    const glm::vec3& GetRelativePosition() const {
      return relative_position_;
    }

    ColliderType GetColliderType() const {
      return collider_type_;
    }

    float GetScaleFactor() const {
      return scale_factor_;
    }

    void Translate(const glm::vec3& displacement) {
      relative_position_ += displacement;
    }
  };

  enum ObjectType {
    ANIMATE,
    INANIMATE
  };

  /*
  An interface for interacting with a physical object. Abstracts away from
  whether or not the object is animate.
  */
  class ObjectInteractor {
  private:
    ObjectType type_;
    Collider* collider_;
    RigidBody* rigid_body_;

  public:
    ObjectInteractor(
      ObjectType type,
      Collider* collider,
      RigidBody* rigid_body
    )
      : type_(type),
      collider_(collider),
      rigid_body_(rigid_body)
    {
    }

    const glm::vec3& GetPosition() const {
      if (type_ == ObjectType::ANIMATE) {
        return rigid_body_->GetPosition();
      }
      else if (type_ == ObjectType::INANIMATE) {
        return collider_->GetRelativePosition();
      }
    }

    const glm::vec3 GetVelocity() const {
      if (type_ == ObjectType::ANIMATE) {
        return rigid_body_->GetVelocity();
      }
      else if (type_ == ObjectType::INANIMATE) {
        return glm::vec3(0.0f);
      }
    }

    const glm::vec3 GetForce() const {
      if (type_ == ObjectType::ANIMATE) {
        return rigid_body_->GetForce();
      }
      else if (type_ == ObjectType::INANIMATE) {
        return glm::vec3(0.0f);
      }
    }

    float GetMass() const {
      if (type_ == ObjectType::ANIMATE) {
        return rigid_body_->GetMass();
      }
      else if (type_ == ObjectType::INANIMATE) {
        return 0.0f;
      }
    }

    void ApplyForce(const glm::vec3& force) {
      if (type_ == ObjectType::ANIMATE) {
        rigid_body_->ApplyForce(force);
      }
      else if (type_ == ObjectType::INANIMATE) {}
    }

    /*
    Move the Object in the physical environment by the `displacement` amount.
    */
    void Translate(const glm::vec3& displacement) {
      if (type_ == ObjectType::ANIMATE) {
        rigid_body_->Translate(displacement);
      }
      else if (type_ == ObjectType::INANIMATE) {
        collider_->Translate(displacement);
      }
    }
  };

  class PhysicsEnvSingleton {
  private:

    std::unordered_map<UUIDv4::UUID, RigidBody> rigid_bodies_;
    std::unordered_map<UUIDv4::UUID, Collider> colliders_;

    glm::vec3 gravity_ = glm::vec3(0, -9.81f, 0);

    UUIDv4::UUIDGenerator<std::mt19937_64> uuid_generator_;

    // Private constructor to prevent direct instantiation
    PhysicsEnvSingleton() = default;

    // Delete copy constructor and assignment operator to prevent copying
    PhysicsEnvSingleton(const PhysicsEnvSingleton&) = delete;
    PhysicsEnvSingleton& operator=(const PhysicsEnvSingleton&) = delete;

  public:

    // Static method to get the single instance
    static PhysicsEnvSingleton& getInstance() {
      // Local static variable ensures single instance and thread-safe
      // initialization (C++11 onwards)
      static PhysicsEnvSingleton instance;
      return instance;
    }

    /*
    Create a new static object. Returns the UUID of the object--which is
    what the physics environment uses to refer to and retrieve it.
    */
    const UUIDv4::UUID AddInanimateObject(
      const glm::vec3& position,
      ColliderType collider_type,
      float scale_factor = 1.0f
    ) noexcept {
      UUIDv4::UUID uuid = uuid_generator_.getUUID();
      colliders_.emplace(uuid, Collider(
        collider_type, NULL, scale_factor, position
      ));
      return uuid;
    }

    /*
    Create a new dynamic object. Returns the UUID of the object--which is
    what the physics environment uses to refer to and retrieve it.
    */
    const UUIDv4::UUID AddAnimateObject(
      ColliderType collider_type,
      const glm::vec3& position = glm::vec3(0.0f),
      const glm::vec3& velocity = glm::vec3(0.0f),
      float mass = 1.0f,
      float scale_factor = 1.0f,
      const glm::vec3& relative_position = glm::vec3(0.0f)
    ) noexcept {
      UUIDv4::UUID rigid_body_uuid = uuid_generator_.getUUID();
      UUIDv4::UUID collider_uuid = uuid_generator_.getUUID();
      rigid_bodies_.emplace(rigid_body_uuid, RigidBody(
        collider_uuid, position, mass, velocity
      ));
      colliders_.emplace(collider_uuid, Collider(
        collider_type, rigid_body_uuid, scale_factor, relative_position
      ));
      return rigid_body_uuid;
    }

    /*
    Remove an object from the physics environment by passing in its UUID.
    */
    void RemoveObject(const UUIDv4::UUID& uuid) {
      if (rigid_bodies_.contains(uuid)) {
        UUIDv4::UUID collider_uuid =
          rigid_bodies_.find(uuid)->second.GetColliderUUID();
        colliders_.erase(collider_uuid);
        rigid_bodies_.erase(uuid);
      }
      else {
        colliders_.erase(uuid);
      }
    }

    /*
    Please use your returned Objects responsibly! Never request a UUID that
    doesn't exist--I haven't added error handling..
    */
    ObjectInteractor GetObject(const UUIDv4::UUID& uuid) {
      // Check if animate
      if (rigid_bodies_.contains(uuid)) {
        RigidBody* rigid_body = &rigid_bodies_.find(uuid)->second;
        Collider* collider =
          &colliders_.find(rigid_body->GetColliderUUID())->second;
        ObjectInteractor object_interactor = ObjectInteractor(
          ObjectType::ANIMATE, collider, rigid_body
        );
        return object_interactor;
      }
      // Must be inanimate
      else {
        Collider* collider =
          &colliders_.find(uuid)->second;
        ObjectInteractor object_interactor = ObjectInteractor(
          ObjectType::INANIMATE, collider, NULL // No RigidBody associated
        );
        return object_interactor;
      }
    }

    /*
    Advance the physics simulation by one simulation step.
    */
    void Step(float delta_time) {
      for (auto& [uuid, rb] : rigid_bodies_) {
        // Accumulate gravity as a force. other forces (like thrust,
        // collisions, etc.) are added elsewhere
        glm::vec3 force_of_gravity = rb.GetMass() * gravity_;
        rb.ApplyForce(force_of_gravity);

        // Newton's second law: a = f/m
        glm::vec3 acceleration;
        if (rb.GetMass() != 0.0f) acceleration = rb.GetForce() / rb.GetMass();
        else acceleration = glm::vec3(0.0f);

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
  };
}