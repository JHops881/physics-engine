#pragma once
#include <glm/glm.hpp>
#include <typeindex>
#include <map>

namespace core {

class GameObject {
private:
    glm::vec3 position;
    // glm::vec3 orientation; // <- add eventually
public:
    GameObject(glm::vec3 position);

    glm::vec3 get_position() const noexcept;
};
}
