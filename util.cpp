#include "util.hpp"

std::string util::read_file(const char* filepath) {
    std::ifstream file(filepath);
    std::string file_contents{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
    return file_contents;
}

void util::time_and_name_log(const char* function_name) {
	double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	std::cout << std::fixed << std::endl << "[" << now_time << "sec] Calling " << function_name << "()" << std::endl;
}

void util::print_vec3(glm::vec3 vec3, const std::string& vec_name) {
    std::cout << std::fixed << std::format("{}=glm::vec3({}, {}, {})", vec_name, vec3.x, vec3.y, vec3.z) << std::endl;
}

void util::set_uniform_vec3(GLuint shader, const char* uniform_name, glm::vec3 value) {
    GLint uniform_location = glGetUniformLocation(shader, uniform_name);
    glUniform3fv(uniform_location, 1, glm::value_ptr(value));
}

void util::set_uniform_float(GLuint shader, const char* uniform_name, GLfloat value) {
    GLint uniform_location = glGetUniformLocation(shader, uniform_name);
    glUniform1f(uniform_location, value);
}

void util::set_uniform_mat4(GLuint shader, const char* uniform_name, glm::mat4 value) {
    GLint uniform_location = glGetUniformLocation(shader, uniform_name);
    glUniformMatrix4fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
}

void util::set_uniform_mat3(GLuint shader, const char* uniform_name, glm::mat3 value) {
    GLint uniform_location = glGetUniformLocation(shader, uniform_name);
    glUniformMatrix3fv(uniform_location, 1, GL_FALSE, glm::value_ptr(value));
}
