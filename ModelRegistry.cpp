#include "ModelRegistry.hpp"

core::ModelID core::ModelRegistry::add_model(Model model)
{
    // TODO: verify that all model values are valid. 
    ModelID id = ModelID(models.add(model));
    return id;
}

void core::ModelRegistry::remove_model(ModelID id)
{
#ifdef _DEBUG
    if (!models.has(id))
    {
        throw std::runtime_error("gfx::ModelhRegistry::remove_model() failed. No model with this ModelID exists to remove!");
    }
#endif

    models.remove(id);
}

core::Model& core::ModelRegistry::get_model(ModelID id)
{
#ifdef _DEBUG
    if (!models.has(id))
    {
        throw std::runtime_error("gfx::ModelRegistry::get_model() failed. No model with this ModelID exists to get!");
    }
#endif

    return models.get(id);
}
