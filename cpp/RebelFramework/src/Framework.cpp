

// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

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