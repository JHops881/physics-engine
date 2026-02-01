#include "ConfigurationManager.hpp"

core::ConfigurationManager::ConfigurationManager(std::shared_ptr<ServiceLocator> locator) 
    : locator(std::move(locator))
{
}

void core::ConfigurationManager::read_in_configuration(const char* filename, int context) {
    // JSON deserialization here. TODO
}

void core::ConfigurationManager::write_out_configuration(int context, const char* filename) const {
    // JSON serialization here. TODO
}

std::shared_ptr<core::ConfigurationContext> core::ConfigurationManager::get_context(int context) const {
    return contexts.at(context);
}

void core::ConfigurationManager::unload_context(int context) {

    // We don't want to unload it from memory if it is in use currently.
    assert(contexts.at(context).use_count() == 1);

    contexts.erase(context);
}
