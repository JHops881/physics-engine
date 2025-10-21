#pragma once
#include "Object.hpp"

namespace core {

class ISceneManager {
public:
    virtual ~ISceneManager() = default;

    /// <summary>
    /// Add an object to the scene.
    /// </summary>
    /// <param name="object">: The object that you want to add to the scene</param>
    /// <returns>An ID that references the object that you just added to the scene</returns>
    virtual ObjectID add_object(Object object) = 0;

    /// <summary>
    /// Retrieve an object from within the scene.
    /// </summary>
    /// <param name="object_id">: The ID referencing the object that you want to get</param>
    /// <returns>A reference to the object you are getting</returns>
    virtual Object& get_object(ObjectID object_id) const = 0;

    /// <summary>
    /// Remove and object from the scene.
    /// </summary>
    /// <param name="object_id">: The ID that references the object that you want to remove from the scene</param>
    virtual void remove_object(ObjectID object_id) = 0;

    /// <summary>
    /// Get a collection of all the objects that exist within the scene.
    /// </summary>
    /// <returns>A collection of all the objects in the scene</returns>
    virtual std::vector<Object>& get_all_objects() const = 0;
};

}