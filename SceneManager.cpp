#include "SceneManager.hpp"

core::SceneManager::SceneManager(std::shared_ptr<ServiceLocator> locator) : locator(locator) {}

core::ObjectID core::SceneManager::add_object(Object object) {
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
#endif
    return ObjectID(objects.add(object));
}

void core::SceneManager::set_skybox(GLuint cubemap) {
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    std::cout << "texture=" << cubemap << std::endl;
#endif
    skybox = cubemap;
}

core::Object& core::SceneManager::get_object(ObjectID object_id) {
#ifdef _DEBUG
    if (!objects.has(object_id)) {
        throw std::runtime_error("core::SceneManager::get_object() failed. No object with this ID exists.");
    }
#endif
    return objects.get(object_id);
}

GLuint core::SceneManager::get_skybox() const {
    return skybox;
}

void core::SceneManager::remove_object(ObjectID object_id) {
    
}

std::vector<core::Object>& core::SceneManager::get_all_objects() {
    return objects.get_dense();
}
