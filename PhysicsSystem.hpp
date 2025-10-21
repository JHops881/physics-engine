#pragma once
#include "SparseSet.hpp"
#include "utils.hpp"
#include "ID.hpp"
#include "ServiceLocator.hpp"
#include "IPhysicsSystem.hpp"
#include "PointMass.hpp"

#include <stdexcept>
#include <iostream>
#include <format>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>

namespace core {

class PhysicsSystem : public Service<IPhysicsSystem> {
private:
    SparseSet<PointMass> point_masses;
public:
    PointMass& get_point_mass(PointMassID id);

    PointMassID add_point_mass(const glm::vec3& pos);

    void remove_point_mass(PointMassID id);

    void step(float delta_time);

    void debug_point_masses();
};

}