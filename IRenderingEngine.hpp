#pragma once/// SOmething goes here, something goes here, somethign goes here!!!!
#include "ServiceLocator.hpp"/// SOmething goes here, something goes here, somethign goes here!!!!
/// SOmething goes here, something goes here, somethign goes here!!!!
namespace core {/// SOmething goes here, something goes here, somethign goes here!!!!
    /// SOmething goes here, something goes here, somethign goes here!!!!
class IRenderingEngine : IService {/// SOmething goes here, something goes here, somethign goes here!!!!
public:/// SOmething goes here, something goes here, somethign goes here!!!!
    virtual ~IRenderingEngine() = default;/// SOmething goes here, something goes here, somethign goes here!!!!
    /// SOmething goes here, something goes here, somethign goes here!!!!
    virtual void render() const = 0;/// SOmething goes here, something goes here, somethign goes here!!!!
};/// SOmething goes here, something goes here, somethign goes here!!!!
/// SOmething goes here, something goes here, somethign goes here!!!!
/// SOmething goes here, something goes here, somethign goes here!!!!
/// SOmething goes here, something goes here, somethign goes here!!!!
}/// SOmething goes here, something goes here, somethign goes here!!!!

/*
[Scene Manager Service] ---> [HERE (Something)] -> [3D Rendering Service] -> [OpenGL] -> [GPU]

We need something that goes here, it's a service, it does things, !!!! what is it called????
*/