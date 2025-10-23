#pragma once
#include "utils.hpp"
#include "ID.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace core {

struct CameraID : public core::ID<CameraID> {
    using core::ID<CameraID>::ID;
};

/// <summary>
/// A camera that is a point of view within the rendered world.
/// </summary>
class Camera {
private:
    glm::vec3 position;
    glm::vec3 target; // the point in the world that the camera is looking at. Not a direction.
    glm::vec3 reverse_direction;
    glm::vec3 up;
    glm::vec3 right;

    float near_clip_plane;
    float far_clip_plane;
    float aspect_ratio;
    float fov;

public:
    /// <summary>
    /// Add a camera.
    /// </summary>
    /// <param name="pos">: initial position of the camera in 3D world space</param>
    /// <param name="up_dir">: The direction that the top of the camera is facing</param>
    Camera(glm::vec3 pos, glm::vec3 up_dir = glm::vec3(0.0f, 1.0f, 0.0f));

    /// <summary>
    /// Get a projection matrix from the perspective configuration of the camera.
    /// </summary>
    /// <returns>: A projection matrix</returns>
    const glm::mat4 get_perspective() const;

    /// <summary>
    /// Get a view matrix that transforms the world to the point of view of the camera.
    /// </summary>
    /// <returns>: a view matrix ... from the camera's pov</returns>
    const glm::mat4 get_look_at() const;

    /// <summary>
    /// Get the current position of the camera in the scene.
    /// </summary>
    /// <returns>The current position of the camera</returns>
    const glm::vec3& get_position() const noexcept;
};

}

DEFINE_ID_HASH(core::CameraID);