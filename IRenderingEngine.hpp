#pragma once
#include "ServiceLocator.hpp"

namespace core {

class IRenderingEngine : IService {
public:
    virtual ~IRenderingEngine() = default;

    virtual void render() const = 0;
};

}