target_sources(enumeratorLibrary
        PUBLIC
        enumerator.hpp
        enumerator.cpp
        generator.hpp
        reporter.hpp
        )

add_subdirectory(enumerations)
add_subdirectory(reporters)
add_subdirectory(generators)
target_include_directories(enumeratorLibrary PUBLIC ${CMAKE_CURRENT_LIST_DIR})
