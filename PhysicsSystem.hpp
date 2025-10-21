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
#include <memory>

#include <glm/glm.hpp>

namespace core {

class PhysicsSystem : public Service<IPhysicsSystem> {
private:
    std::shared_ptr<ServiceLocator> locator;
    SparseSet<PointMass> point_masses;
public:
    PhysicsSystem(std::shared_ptr<ServiceLocator> locator);

    PointMass& get_point_mass(PointMassID id) override;

    PointMassID add_point_mass(const glm::vec3& pos) override;

    void remove_point_mass(PointMassID id) override;

    void step(float delta_time) override;

    void debug_point_masses() override;
};

}