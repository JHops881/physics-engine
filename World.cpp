#include "World.hpp"

core::WorldObjectID core::World::add_object(WorldObject object) {
#ifdef _DEBUG
    // TODO
    utils::time_and_name_log(__FUNCTION__);
#endif
    return WorldObjectID(objects.add(object));
}

core::WorldObject& core::World::get_object(WorldObjectID id) { return objects.get(id); }

void core::World::remove_object(WorldObjectID id) {
#ifdef _DEBUG
    // TODO
    utils::time_and_name_log(__FUNCTION__);
#endif
    objects.remove(id);
}

std::vector<core::WorldObject>& core::World::get_objects() { return objects.get_dense(); }
