#pragma once
#include <iostream>
#include <chrono>

namespace utils
{ 
	/// <summary>
	/// Print out the current time in seconds as well as the function name. Call this in debug logging.
	/// </summary>
	/// <param name="function_name">: Name of the function being called</param>
	void time_and_name_log(const char* function_name);
}