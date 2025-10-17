#include "utils.hpp"

void utils::time_and_name_log(const char* function_name)
{
	double now_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
	std::cout << std::fixed << std::endl << "[" << now_time << "sec] Calling " << function_name << "()" << std::endl;
}
