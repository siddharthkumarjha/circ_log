# Prevent multiple inclusions
if(FMT_DEPENDENCY_LOADED)
    return()
endif()
set(FMT_DEPENDENCY_LOADED TRUE)

# Configurable version
if(NOT DEFINED REQUIRED_FMT_VERSION)
    set(REQUIRED_FMT_VERSION 12.1.0)
endif()

# Fetch fmt function
function(fetch_and_prepare_fmt)
    message(STATUS "Fetching fmt")
    include(FetchContent)
    FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG ${REQUIRED_FMT_VERSION}  # or specify a specific version/tag
    )
    
    # Set options for fmt compilation
    set(FMT_TEST OFF CACHE BOOL "Disable fmt tests")
    set(FMT_DOC OFF CACHE BOOL "Disable fmt documentation")
    set(FMT_INSTALL ON CACHE BOOL "Enable fmt installation")
    
    # Fetch and make available
    FetchContent_MakeAvailable(fmt)
    
    # Ensure static library compilation
    set_target_properties(fmt PROPERTIES POSITION_INDEPENDENT_CODE ON)
endfunction()

# Main version check logic
function(ensure_fmt_dependency)
    # Try to find fmt first
    find_package(fmt CONFIG QUIET)

    # Check if fmt was found and meets version requirement
    if(fmt_FOUND)
        message(WARNING "fmt Version: ${fmt_VERSION}")
        
        if(fmt_VERSION VERSION_LESS ${REQUIRED_FMT_VERSION})
            message(WARNING "fmt version is too old. Fetching newer version.")
            set(fmt_FOUND FALSE PARENT_SCOPE)
        endif()
    endif()

    # If fmt is not found or version is too old, fetch and compile
    if(NOT fmt_FOUND)
        fetch_and_prepare_fmt()
    endif()
endfunction()

# Call the function to ensure fmt dependency
ensure_fmt_dependency()
