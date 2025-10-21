#pragma once

#include "ID.hpp"
#include "PhysicsSystem.hpp"
#include "Model.hpp"

namespace core {

/// <summary>
/// An ID that references an object in the scene
/// </summary>
struct ObjectID : public core::ID<ObjectID> {
    using core::ID<ObjectID>::ID;
};

/// <summary>
/// An object that exists in the scene.
/// </summary>
struct Object {
    ModelID model_id;
    PointMassID pm_id; // this will change
};

}