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
[Scene Manager Service] -> [3D Rendering Service] -> [OpenGL] -> [Compute Shaders (LOD, Culling)] -> [GPU]

So you'll upload all the geometry, all the instances, etc. to the GPU
Then, you dispatch compute commands that can determine what is visible
From the visible set, you can populate a buffer containing draw commands
Then, you would issue an indirect draw call, using the buffer containing the draw commands

A better metric to use for LOD selection is determining the % of the screen size the object takes (vertical, horizontal,
area, one of these metrics), then using that to select the best suited LOD

Then figure out how to fill the draw call buffer from the CPU and issue indirect draw calls from the CPU
Then figure out how to fill the draw call buffer from a compute shader

*/