#pragma once
#include "Object.hpp"
#include "ServiceLocator.hpp"
#include "ISceneManager.hpp"
#include "SparseSet.hpp"
#include "utils.hpp"

#include <glad/glad.h>

#include <vector>
#include <memory>

namespace core {

class SceneManager : public Service<ISceneManager> {
private:
    SparseSet<Object> objects;
    GLuint skybox;
    std::shared_ptr<ServiceLocator> locator;
public:
    SceneManager(std::shared_ptr<ServiceLocator> locator);

    ObjectID add_object(Object object) override;

    void set_skybox(GLuint cubemap) override;

    Object& get_object(ObjectID object_id) override;

    GLuint get_skybox() const override;

    void remove_object(ObjectID object_id) override;

    std::vector<Object>& get_all_objects() override;
};

}