#include <cstdlib>

#include <engine.hpp>
#include <logging_utilities.hpp>

int main() {

    Engine engine = Engine::Engine();
    bool enableDebugMode = true;
    if (engine.init(enableDebugMode) == false) {
        logging::ERROR("FAILED TO INITIALIZE ENGINE");
        return EXIT_FAILURE;
    }

    engine.eventLoop();

    return EXIT_SUCCESS;
}