#include "physics.hpp"
#include <chrono>
#include <format>
#include <iostream>
#include <type_traits>


int main()
{
    // std::cout << std::is_trivially_copyable<>::value << std::endl;

    UUIDv4::UUID my_uuid = phys::PhysicsEnvSingleton::getInstance().AddDynamicObject(phys::ColliderType::CUBE);

    auto last_update_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
    const double tick_rate = 1.0; // in updates/sec
    double accumulator = 0;

    // Application loop
    while (true)
    {
        double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        double delta_time = now_time - last_update_time;
        last_update_time += delta_time;
        accumulator += delta_time;

        while (accumulator > tick_rate)
        {
            phys::PhysicsEnvSingleton::getInstance().Step(tick_rate);

            phys::PhysicsEnvSingleton::getInstance().DebugObject(my_uuid);

            accumulator -= tick_rate;
        }

        // Draw calls here.
    }

    return 0;
}