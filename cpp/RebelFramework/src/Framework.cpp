

#include "Rebel/Core.hpp"


namespace Rebel {
    const char* GetFrameworkVersion() {
        return PROJECT_VERSION;
    }

    const char* GetFrameworkName() {
        return PROJECT_NAME;
    }

    const char* GetFrameworkDescription() {
        return PROJECT_DESCRIPTION;
    }
}