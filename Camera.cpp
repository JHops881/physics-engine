#include "Camera.hpp"

core::Camera::Camera(glm::vec3 pos, glm::vec3 facing, glm::vec3 up_dir)
    : position(pos),
      front(glm::normalize(facing)),
      up(glm::normalize(up_dir)),
      world_up(glm::vec3(0.0f, 1.0f, 0.0f)),
      right(glm::normalize(glm::cross(up, front))),
      pitch(0.0f),
      yaw(-90.0f),
      roll(0.0f),
      near_clip_plane(0.1f),
      far_clip_plane(100.0f),
      aspect_ratio(16.0f / 9.0f),
      fov(glm::radians(72.0f))
{
#ifdef _DEBUG
    util::time_and_name_log(__FUNCTION__);
    util::print_vec3(position, "position");
    util::print_vec3(front, "front");
    util::print_vec3(up, "up");
    util::print_vec3(right, "right");

    std::cout << "near_clip_plane=" << near_clip_plane << std::endl;
    std::cout << "far_clip_plane=" << far_clip_plane << std::endl;
    std::cout << "aspect_ratio=" << aspect_ratio << std::endl;
    std::cout << "fov=" << fov << std::endl;
#endif
}

const glm::mat4 core::Camera::get_perspective() const {
    return glm::perspective(fov, aspect_ratio, near_clip_plane, far_clip_plane);
}

const glm::mat4 core::Camera::get_look_at() const {
    glm::mat4 view_matrix = glm::mat4(1.0f);
    view_matrix = glm::lookAt(position, position + front, up);
    return view_matrix;
}

const glm::vec3& core::Camera::get_position() const noexcept {
    return position;
}

void core::Camera::set_postition(const glm::vec3& pos) {
    position = pos;
}

const glm::vec3 core::Camera::get_right() const {
    return right;
}

const glm::vec3 core::Camera::get_up() const {
    return up;
}

const glm::vec3 core::Camera::get_front() const {
    return front;
}

float core::Camera::get_pitch() const {
    return pitch;
}

float core::Camera::get_yaw() const {
    return yaw;
}

float core::Camera::get_roll() const {
    return roll;
}

void core::Camera::set_rotation(float p, float y) {
    // updating the internal state of the camera.
    pitch = p;
    yaw = y;
    // re-calculating the new directional unit vectors based on new angles.
    // Start with the front, which we use to derive the rest.
    glm::vec3 facing = glm::vec3(0.0f);
    facing.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    facing.y = sin(glm::radians(pitch));
    facing.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(facing);
    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}