#pragma once
#include <iostream>
#include <chrono>
#include <string>
#include <fstream>

#include <glm/glm.hpp>

namespace utils { 

    /// <summary>
    /// Read a file.
    /// </summary>
    /// <param name="filepath">: Path to the file that you want to read</param>
    /// <returns>A string containing the file text content</returns>
    std::string read_file(const char* filepath);

	/// <summary>
	/// Print out the current time in seconds as well as the function name. Call this in debug logging.
	/// </summary>
	/// <param name="function_name">: Name of the function being called</param>
	void time_and_name_log(const char* function_name);

    /// <summary>
    /// Prints out a named glm::vec3 neatly to the console.
    /// </summary>
    /// <param name="vec3">: The glm::vec3 which you want the values printed out for</param>
    /// <param name="vec_name">: The name of the glm::vec3 or whatever you would like it to be identified as in the print msg</param>
    void print_vec3(glm::vec3 vec3, const std::string& vec_name);
}