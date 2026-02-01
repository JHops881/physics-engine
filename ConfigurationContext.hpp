#pragma once

#include <string>
#include <mutex>

namespace core {

/// <summary>
/// A grouping of configurations. i.e. a model representing a configration file.
/// </summary>
class ConfigurationContext {
public:
    /// <summary>
    /// Get a configuration value.
    /// </summary>
    /// <param name="key">: The key in the configuration that identifies the value.</param>
    /// <returns>The configuration value associated with that key.</returns>
    const std::string get_value(const char* key) const;

    /// <summary>
    /// Set a configuration value.
    /// </summary>
    /// <param name="key">: The key in the configuration that identifies the value.</param>
    /// <param name="value">: The configuration value associated with that key.</param>
    void set_value(const char* key, const char* value);
};

}