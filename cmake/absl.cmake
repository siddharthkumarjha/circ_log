# Minimum Abseil version check and fetch logic

# Prevent multiple inclusions
if(ABSEIL_DEPENDENCY_LOADED)
    return()
endif()
set(ABSEIL_DEPENDENCY_LOADED TRUE)

# Configurable version
if(NOT DEFINED REQUIRED_ABSL_VERSION)
    set(REQUIRED_ABSL_VERSION 20250814)
endif()

# Fetch Abseil function
function(fetch_and_prepare_abseil)
    message(STATUS "Fetching Abseil")
    include(FetchContent)
    FetchContent_Declare(
        absl
        GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
        GIT_TAG lts_2025_08_14  # or specify a specific version/tag
    )
    
    # Set options for Abseil compilation
    set(ABSL_BUILD_TESTING OFF CACHE BOOL "Disable Abseil tests")
    set(ABSL_USE_GOOGLETEST_HEAD OFF CACHE BOOL "Disable Googletest")
    
    # Fetch and make available
    FetchContent_MakeAvailable(absl)
    
    # Ensure static library compilation
    set_target_properties(absl_base PROPERTIES POSITION_INDEPENDENT_CODE ON)
endfunction()

# Main version check logic
function(ensure_abseil_dependency)
    # Try to find Abseil first
    find_package(absl CONFIG QUIET)

    # Check if Abseil was found and meets version requirement
    if(absl_FOUND)
        message(WARNING "Abseil Version: ${absl_VERSION}")
        
        if(absl_VERSION VERSION_LESS ${REQUIRED_ABSL_VERSION})
            message(WARNING "Abseil version is too old. Fetching newer version.")
            set(absl_FOUND FALSE PARENT_SCOPE)
        endif()
    endif()

    # If Abseil is not found or version is too old, fetch and compile
    if(NOT absl_FOUND)
        fetch_and_prepare_abseil()
    endif()
endfunction()

# Call the function to ensure Abseil dependency
ensure_abseil_dependency()
