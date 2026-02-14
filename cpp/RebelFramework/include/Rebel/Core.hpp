// Copyright (c) 2026, and future.
// Alejandro Morcillo Montejo - All Rights Reserved

//
//

#pragma once

#ifdef _WIN32
    #ifdef REBEL_FRAMEWORK_EXPORTS
        #define REBEL_FRAMEWORK __declspec(dllexport)
    #else
        #define REBEL_FRAMEWORK __declspec(dllimport)
    #endif
#else
    #define REBEL_FRAMEWORK __attribute__((visibility("default")))
#endif

namespace Rebel {
    // Framework version information
    REBEL_FRAMEWORK const char* GetFrameworkVersion();

    REBEL_FRAMEWORK const char* GetFrameworkName();

    REBEL_FRAMEWORK const char* GetFrameworkDescription();
}