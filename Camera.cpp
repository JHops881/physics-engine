#include "Camera.hpp"

core::Camera::Camera(glm::vec3 pos, glm::vec3 up_dir)
    : position(pos),
      target(glm::vec3(0.0f)),
      reverse_direction(glm::normalize(position - target)),
      up(up_dir),
      right(glm::normalize(glm::cross(up, reverse_direction))),
      near_clip_plane(0.1f),
      far_clip_plane(100.0f),
      aspect_ratio(16.0f / 9.0f),
      fov(glm::radians(72.0f))
{
#ifdef _DEBUG
    utils::time_and_name_log(__FUNCTION__);
    utils::print_vec3(position, "position");
    utils::print_vec3(target, "target");
    utils::print_vec3(reverse_direction, "reverse_direction");
    utils::print_vec3(up, "up");
    utils::print_vec3(right, "right");

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
    view_matrix = glm::lookAt(position, target, up);
    return view_matrix;
}

const glm::vec3& core::Camera::get_position() const noexcept {
    return position;
}
