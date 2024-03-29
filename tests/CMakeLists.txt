# Download google test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG main
)

FetchContent_MakeAvailable(googletest)

# Define a test exe
add_executable(enumeratorTests )
target_link_libraries(enumeratorTests PUBLIC gtest gmock gtest_main enumeratorLibrary cppParserTestLibrary)

target_sources(enumeratorTests
        PUBLIC
        enumeratorTests.cpp
        mocks.hpp
        )

# Add each subDir
add_subdirectory(enumerations)
add_subdirectory(reporters)
add_subdirectory(generators)

gtest_discover_tests(enumeratorTests
        # set a working directory so your project root so that you can find test data via paths relative to the project root
        WORKING_DIRECTORY ${PROJECT_DIR}
        PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${PROJECT_DIR}"
        )

# Allow public access to the header files in the directory
target_include_directories(enumeratorTests PUBLIC ${CMAKE_CURRENT_LIST_DIR})
