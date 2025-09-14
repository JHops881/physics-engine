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
  class Object {
    private:
      glm::vec3 position_;
      glm::vec3 velocity_;
      glm::vec3 force_;
      float mass_;
      
    public:
      Object(const glm::vec3& position = glm::vec3(0.0f),
             const glm::vec3& velocity = glm::vec3(0.0f),
             float mass = 1.0f)
        : position_(position), velocity_(velocity), force_(glm::vec3(0.0f)),
          mass_(mass) {}

      const glm::vec3& GetPosition() const {
        return position_;
      }

      const glm::vec3& GetVelocity() const {
        return velocity_;
      }

      const glm::vec3& GetForce() const {
        return force_;
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

  class PhysicsWorldSingleton {
    private:

      std::unordered_map<UUIDv4::UUID, Object> objects_;
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
      Create a new physical object. Returns the UUID of the object--which is
      what the physics environment uses to refer to and retrieve it.
      */
      const UUIDv4::UUID AddObject(Object object) noexcept {
        UUIDv4::UUID uuid = uuid_generator_.getUUID();
        objects_.emplace(uuid, std::move(object));
        return uuid;
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
      Object& GetObject(const UUIDv4::UUID& uuid) {
        auto pair = objects_.find(uuid);
        Object& object = pair->second;
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