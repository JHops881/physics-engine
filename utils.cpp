#include "utils.hpp"

void utils::time_and_name_log(const char* function_name) {
	double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	std::cout << std::fixed << std::endl << "[" << now_time << "sec] Calling " << function_name << "()" << std::endl;
}

void utils::print_vec3(glm::vec3 vec3, const std::string& vec_name) {
    std::cout << std::fixed << std::format("{}=glm::vec3({}, {}, {})", vec_name, vec3.x, vec3.y, vec3.z) << std::endl;
}
