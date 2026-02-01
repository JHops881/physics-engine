#pragma once
#include "ConfigurationContext.hpp"
#include "ServiceLocator.hpp"
#include <memory>

namespace core {

/// <summary>
/// Service that manages access to program configuration values.
/// </summary>
class IConfigurationManager : public IService {
public:
    virtual ~IConfigurationManager() = default;

    /// <summary>
    /// Read a configuration file from disk, load it into memory, and associate it with a context.
    /// </summary>
    /// <param name="filename"></param>
    /// <param name="context"></param>
    virtual void read_in_configuration(const char* filename, int context) = 0;

    /// <summary>
    /// Write out a selected configuration context to disk as a file.
    /// </summary>
    /// <param name="context"></param>
    /// <param name="filename"></param>
    virtual void write_out_configuration(int context, const char* filename) const = 0;

    /// <summary>
    /// Get a configuration context.
    /// </summary>
    /// <param name="context">: The context handle.</param>
    /// <returns></returns>
    virtual std::shared_ptr<ConfigurationContext> get_context(int context) const = 0;

    /// <summary>
    /// Unloads a configuration context from memory. 
    /// </summary>
    /// <param name="context"></param>
    virtual void unload_context(int context) = 0;
};

}