#include "OpenGLApplication.hpp"

int main()
{
    // std::cout << std::is_trivially_copyable<>::value << std::endl;

    OpenGLApplication app;
    try
    {
        app.Run();
    }
    catch (const std::exception& exception)
    {
        std::cerr << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}