#include <iostream>
#include "Physics.h"
#include <chrono>
#include <format>

/*
Display the information about the physical object to the stdout. 
Namely, the UUID, posiion, and velocity.
*/
void DebugObject(const UUIDv4::UUID& uuid) {
  phys::Object& object =
    phys::PhysicsWorldSingleton::getInstance().GetObject(uuid);
  const glm::vec3& position = object.GetPosition();
  const glm::vec3& velocity = object.GetVelocity();

  std::cout << "The UUID of the Object is: " << uuid << std::endl;

  std::string message1 = std::format("@ x: {}, y: {}, z: {}",
    position.x, position.y, position.z);

  std::cout << message1 << std::endl;

  std::string message2 = std::format("going x: {}, y: {}, z: {} m/s",
    velocity.x, velocity.y, velocity.z);

  std::cout << message2 << std::endl;
}

int main() {
  
  std::cout << "initializing.." << std::endl;
  // Make an object
  phys::Object my_object;
  UUIDv4::UUID my_uuid =
    phys::PhysicsWorldSingleton::getInstance().AddObject(my_object);


  auto last_update_time = std::chrono::duration<double>(
    std::chrono::high_resolution_clock::now().time_since_epoch()
  ).count();
  const double tick_rate = 1.0; // in updates/sec
  double accumulator = 0;

  // Classic game loop
  while (true) {
    
    double now_time = std::chrono::duration<double>(
      std::chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
    double delta_time = now_time - last_update_time;
    last_update_time += delta_time;
    accumulator += delta_time;

    while (accumulator > tick_rate) {
      phys::PhysicsWorldSingleton::getInstance().Step(tick_rate);

      // Debug
      DebugObject(my_uuid);

      accumulator -= tick_rate;
    }
    //draw();
    
  }

  return 0;
}