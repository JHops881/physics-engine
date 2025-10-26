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
    /**
     * A note that it is probably best to avoid adding instance variables like 'speed'.
     * I feel that these encourage translation functionality for the camera. fundementally,
     * the camera exists in the scene at a position--it itself is not a moving entity.
     * The illusion of movement is created when setting the position of the camera
     * in accordance with some other entity or functionality.
     */
    glm::vec3 position;
    glm::vec3 front; // the direction in which the camera is facing
    glm::vec3 up;
    glm::vec3 world_up;
    glm::vec3 right;

    float pitch;
    float yaw;
    float roll;

    float near_clip_plane;
    float far_clip_plane;
    float aspect_ratio;
    float fov;
public:
    /// <summary>
    /// Add a camera.
    /// </summary>
    /// <param name="pos">: initial position of the camera in 3D world space</param>
    /// <param name="facing">: A unit vector in the direction that the camera is facing</param>
    /// <param name="up_dir">: The direction that the top of the camera is facing</param>
    Camera(glm::vec3 pos, glm::vec3 facing, glm::vec3 up_dir = glm::vec3(0.0f, 1.0f, 0.0f));

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

    /// <summary>
    /// Set the position of the camera in the scene.
    /// </summary>
    /// <param name="pos">: The new position of the camera</param>
    void set_postition(const glm::vec3& pos);

    /// <summary>
    /// Get a unit vector pointing to the camera's right (from the camera's perspective).
    /// </summary>
    /// <returns>: The direction of "right" relative to the camera</returns>
    const glm::vec3 get_right() const;

    /// <summary>
    /// Get a unit vector pointing up from the camera (from the camera's perspective).
    /// </summary>
    /// <returns>: The direction of "up" relative to the camera</returns>
    const glm::vec3 get_up() const;

    /// <summary>
    /// Get a unit vector pointing forward from the camera (from the camera's perspective).
    /// </summary>
    /// <returns>: The direction of "front" relative to the camera</returns>
    const glm::vec3 get_front() const;

    /// <summary>
    /// Get the pitch of the camera orientation.
    /// </summary>
    /// <returns>: The pitch angle in degrees</returns>
    float get_pitch() const;

    /// <summary>
    /// Get the yaw of the camera orientation.
    /// </summary>
    /// <returns>: the yaw angle in degrees.</returns>
    float get_yaw() const;

    /// <summary>
    /// Get the roll of the camera orientation.
    /// </summary>
    /// <returns>: The roll angle in degrees.</returns>
    float get_roll() const;

    /// <summary>
    /// Set the direction that the front of the camera is facing.
    /// </summary>
    /// <param name="pitch">: The pitch angle of the camera orientation in degrees</param>
    /// <param name="yaw">: The yaw angle of the camera orientation in degrees</param>
    void set_rotation(float p, float y);
};

}

DEFINE_ID_HASH(core::CameraID);