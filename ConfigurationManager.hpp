#pragma once
#include "ServiceLocator.hpp"
#include "IConfigurationManager.hpp"
#include "ConfigurationContext.hpp"

#include <stdexcept>
#include <format>
#include <map>
#include <memory>
#include <cassert>

namespace core {

class ConfigurationManager : public Service<IConfigurationManager> {
private:
    std::map<int, std::shared_ptr<ConfigurationContext>> contexts;
    std::shared_ptr<ServiceLocator> locator;
public:
    /// <summary>
    /// Create a new instance of the Configuration Management Service.
    /// </summary>
    /// <param name="locator">: A reference to the Service Location Service.</param>
    ConfigurationManager(std::shared_ptr<ServiceLocator> locator);

    void read_in_configuration(const char* filename, int context);

    void write_out_configuration(int context, const char* filename) const;

    std::shared_ptr<ConfigurationContext> get_context(int context) const;

    void unload_context(int context);
};

}