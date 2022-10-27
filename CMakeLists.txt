cmake_minimum_required(VERSION 3.14)

# Create the new project
project(Enumerator VERSION 0.1.1)

# Set the standards
set(CMAKE_CXX_STANDARD 17)

# Create the project
add_library(enumeratorLibrary SHARED)

# FetContent will allow downloading of other libraries
include(FetchContent)

# make cache variables for install destinations
include(GNUInstallDirs)

# Build a stand alone executable
add_executable(enumerator main.cpp)
target_link_libraries(enumerator PRIVATE enumeratorLibrary)

# Load in the source code
add_subdirectory(src)

# Allow public access to the header files in the directory
target_include_directories(enumeratorLibrary PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/src"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

# Get the CPP dependency
FetchContent_Declare(
        cppParser
        GIT_REPOSITORY https://github.com/UBCHREST/CppParser.git
        GIT_TAG  v0.2.3
)
FetchContent_MakeAvailable(cppParser)

# load in the build/compiler standards
FetchContent_Declare(
        chrestCompilerFlags
        GIT_REPOSITORY https://github.com/UBCHREST/chrestCompilerFlags.git
        GIT_TAG main
)
FetchContent_MakeAvailable(chrestCompilerFlags)

# Link to the enumeratorLibrary
target_link_libraries(enumeratorLibrary PUBLIC yaml-cpp cppParserLibrary PRIVATE chrestCompilerFlags)

# Check if we should enable testing options
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)
    include(CTest)

    # Include the format command only if we are the main project
    include(clangFormatter.cmake)

    # Setup and configure testing
    if(BUILD_TESTING)
        enable_testing()
        include(GoogleTest)
        add_subdirectory(tests)
    endif()
endif()

# install the enumerator (and others) target and create export-set
include(cmakeFunctions.cmake)
install(TARGETS enumerator enumeratorLibrary cppParserLibrary yaml-cpp chrestCompilerFlags
        EXPORT enumeratorTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        )
set_target_properties(enumerator PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

# install header file(s)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.hpp")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} FILES_MATCHING PATTERN "*.h")
update_header_paths_for_install(enumeratorLibrary ${PROJECT_SOURCE_DIR}/src ${CMAKE_INSTALL_INCLUDEDIR})

# generate and install export file
install(EXPORT enumeratorTargets
        FILE enumeratorTargets.cmake
        NAMESPACE CHREST::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enumerator
        )

# generate the version file for the config file
write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/enumeratorConfigVersion.cmake"
        VERSION "${CMAKE_PROJECT_VERSION}"
        COMPATIBILITY AnyNewerVersion
)

# create config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/enumeratorConfig.cmake"
        INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enumerator
        )

# install config files
install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/enumeratorConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/enumeratorConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/enumerator
        )

# generate the export targets for the build tree
export(EXPORT enumeratorTargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/cmake/enumeratorTargets.cmake"
        NAMESPACE CHREST::
        )

## update the install rpaths
if (APPLE)
    set(CMAKE_MACOSX_RPATH 1)
    set_target_properties(enumerator PROPERTIES
            INSTALL_RPATH "@loader_path;@loader_path/...;@executable_path;@executable_path/../${CMAKE_INSTALL_LIBDIR};@rpath")
elseif (UNIX)
    set_target_properties(enumerator PROPERTIES
            INSTALL_RPATH "$ORIGIN:$ORIGIN/../${CMAKE_INSTALL_LIBDIR}:$ORIGIN/...")
endif ()