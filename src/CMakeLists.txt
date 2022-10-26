target_sources(enumeratorLibrary
        PRIVATE
        enumerator.cpp

        PUBLIC
        enumerator.hpp
        generator.hpp
        reporter.hpp
        )

add_subdirectory(enumerations)
add_subdirectory(reporters)
add_subdirectory(generators)
