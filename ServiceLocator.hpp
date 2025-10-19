#pragma once
#include <memory>
#include <typeindex>
#include <map>
#include <mutex>
#include <stdexcept>

namespace core {

/// <summary>
/// Foundational class for all services that provides the service with the functionality to be identified with RTTI.
/// </summary>
class IService {
public:
    virtual ~IService() = default;
    virtual std::type_index get_type_index() const = 0;
};

/// <summary>
/// Base class for all services that provides the standard implementation for get_type_index()
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T>
class Service : public T {
public:
    /// <summary>
    /// Returns the type index of the interface that derives this base class.
    /// </summary>
    /// <returns>: the type index of the specific service interface</returns>
    std::type_index get_type_index() const override {
        return std::type_index(typeid(T));
    }
};

/// <summary>
/// A thread safe service locator.
/// </summary>
class ServiceLocator {
private:
    std::map<std::type_index, std::shared_ptr<IService>> services;
    mutable std::mutex mutex;
public:
    /// <summary>
    /// Register a service with the service locator.
    /// </summary>
    /// <typeparam name="T">: The specific service interface type</typeparam>
    /// <param name="service">: A shared pointer to an instance of the implemented service</param>
    template<typename T>
    void register_service(std::shared_ptr<T> service) {
        static_assert(
            std::is_base_of<IService, T>::value, "core::ServiceLocator::register_service() failed. T must inherit IService!");
        std::lock_guard<std::mutex> lock(mutex);
        std::type_index type_index = service->get_type_index();
        if (services.find(type_index) != services.end()) {
            throw std::runtime_error("core::ServiceLocator::register_service() failed. This service is already registered!");
        }
        services[type_index] = service;
    }

    /// <summary>
    /// Get a service from the service locator.
    /// </summary>
    /// <typeparam name="T">: The specific service interface type</typeparam>
    /// <returns>A shared pointer to an instance of the implemented service</returns>
    template<typename T>
    std::shared_ptr<T> get_service() const {
        std::lock_guard<std::mutex> lock(mutex);
        std::type_index type_index = std::type_index(typeid(T));
        auto it = services.find(type_index);
    #ifdef _DEBUG
        if (it == services.end()) {
            throw std::runtime_error("core::ServiceLocator::get_service() failed. Service doesn't exist.");
        }
    #endif
        return std::static_pointer_cast<T>(it->second);
    }

    /// <summary>
    /// Unregister a service with the service locator.
    /// </summary>
    /// <typeparam name="T">: The specific service interface type</typeparam>
    template<typename T>
    void unregister_service() {
        std::lock_guard<std::mutex> lock(mutex);
        std::type_index type_index = std::type_index(typeid(T));
        if (services.find(type_index) == services.end()) {
            throw std::runtime_error(
                "core::ServiceLocator::unregister_service() failed. A service of this type is not registered.");
        }
        services.erase(type_index);
    }
};

}