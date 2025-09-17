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
  class NewtonianComponent {
    private:
      glm::vec3 position_;
      glm::vec3 velocity_;
      glm::vec3 force_;
      float mass_;
      UUIDv4::UUID associated_collider_uuid_;
      
    public:
        NewtonianComponent(
            const glm::vec3& position = glm::vec3(0.0f),
            const glm::vec3& velocity = glm::vec3(0.0f),
            float mass = 1.0f,
            const UUIDv4::UUID& associated_collider_uuid
      )
        : position_(position),
          velocity_(velocity),
          force_(glm::vec3(0.0f)),
          mass_(mass),
          associated_collider_uuid_(associated_collider_uuid)
      {}

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

  class ColliderComponent {
  private:
      ColliderType collider_type_;
      UUIDv4::UUID associated_object_;
      float scale_factor_;
      glm::vec3 relative_position_;

  public:
      ColliderComponent(
          ColliderType collider_type,
          const UUIDv4::UUID& associated_object,
          float scale_factor = 1.0f,
          const glm::vec3& relative_position = glm::vec3(0.0f)
      )
        : collider_type_(collider_type),
          associated_object_(associated_object),
          scale_factor_(scale_factor),
          relative_position_(relative_position)
      {}
  };

  enum PhysicalObjectType {
      STATIC,
      DYNAMIC
  };

  /*
  Hides the implementation of Static vs Dynamic Objects to the Developer.
  */
  class PhysicalObjectReference {
  private:
      PhysicalObjectType type_;
      ColliderComponent* collider_;
      NewtonianComponent* body_;

  public:
      PhysicalObjectReference(
          PhysicalObjectType type,
          ColliderComponent* collider,
          NewtonianComponent* body
      )
        : type_(type),
          collider_(collider),
          body_(body)
      {}



  };

  class PhysicsWorldSingleton {
    private:

      std::unordered_map<UUIDv4::UUID, NewtonianComponent> objects_;
      std::unordered_map<UUIDv4::UUID, ColliderComponent> colliders_;
      glm::vec3 gravity_ = glm::vec3(0, -9.81f, 0);

      UUIDv4::UUIDGenerator<std::mt19937_64> uuid_generator_;

      // Private constructor to prevent direct instantiation
      PhysicsWorldSingleton() = default;

      // Delete copy constructor and assignment operator to prevent copying
      PhysicsWorldSingleton(const PhysicsWorldSingleton&) = delete;
      PhysicsWorldSingleton& operator=(const PhysicsWorldSingleton&) = delete;
    
    public:

      // Static method to get the single instance
      static PhysicsWorldSingleton& getInstance() {
        // Local static variable ensures single instance and thread-safe
        // initialization (C++11 onwards)
        static PhysicsWorldSingleton instance;
        return instance;
      }
      
      /*
      Create a new static object. Returns the UUID of the object--which is
      what the physics environment uses to refer to and retrieve it.
      */
      const UUIDv4::UUID AddStaticObject(
          const glm::vec3& position = glm::vec3(0.0f),
          ColliderType collider_type,
          float scale_factor = 1.0f
      ) noexcept {
        UUIDv4::UUID uuid = uuid_generator_.getUUID();
        colliders_.emplace(uuid, ColliderComponent(
            collider_type, NULL, scale_factor, position
        ));
        return uuid;
      }

      /*
      Create a new dynamic object. Returns the UUID of the object--which is
      what the physics environment uses to refer to and retrieve it.
      */
      const UUIDv4::UUID AddDynamicObject(
          const glm::vec3& position = glm::vec3(0.0f),
          const glm::vec3& velocity = glm::vec3(0.0f),
          float mass = 1.0f,
          ColliderType collider_type,
          float scale_factor = 1.0f,
          const glm::vec3& relative_position = glm::vec3(0.0f)
      ) noexcept {
        UUIDv4::UUID obj_uuid = uuid_generator_.getUUID();
        UUIDv4::UUID collider_uuid = uuid_generator_.getUUID();
        objects_.emplace(obj_uuid, NewtonianComponent(
            position, velocity, mass, collider_uuid
        ));
        colliders_.emplace(collider_uuid, ColliderComponent(
            collider_type, obj_uuid, scale_factor, relative_position
        ));
        return obj_uuid;
      }

      /*
      Remove a physical object from the physics environment by passing in its
      UUID.
      */
      void RemoveObject(const UUIDv4::UUID& uuid) {
        objects_.erase(uuid);
      }

      /*
      Please use your returned Objects responsibly! Never request a UUID that
      doesn't exist--I haven't added error handling..
      */
      PhysicalObjectReference GetObject(const UUIDv4::UUID& uuid) {
        // Check if Dynamic
        if (objects_.find(uuid) != objects_.end()) {
          auto pair = objects_.find(uuid);
          NewtonianComponent* object = &pair->second;
          ColliderComponent* collider = &colliders_.find(object->GetColliderUUID())->second;
        }
        // Must be static
        else {
            // TODO

        }



        return object;
      }

      /*
      Advance the physics simulation by one simulation step.
      */
      void Step(float delta_time) {
        for (auto& [uuid, object] : objects_) {
  
          // Accumulate gravity as a force. other forces (like thrust,
          // collisions, etc.) are added elsewhere
          glm::vec3 force_of_gravity = object.GetMass() * gravity_;
          object.ApplyForce(force_of_gravity);

          // Newton's second law: a = f/m
          glm::vec3 acceleration = object.GetForce() / object.GetMass();

          // Integrates acceleration into velocity. This is the semi - implicit
          // Euler method, which is more stable than simple(explicit) Euler.
          object.Accelerate(acceleration * delta_time);

          // Integrates velocity into position.
          glm::vec3 displacement = object.GetVelocity() * delta_time;
          object.Translate(displacement);

          // Clears the force accumulator so the next step can 
          // apply new forces cleanly.
          object.ResetForce();
        }
      }
  };
}