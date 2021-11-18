#include <enumerator.hpp>
#include <fstream>
#include <memory>
#include "enumeration.hpp"
#include "generators/templateGenerator.hpp"
#include "gtest/gtest.h"

using namespace enumerator;

namespace ablateTesting {

struct TemplateGeneratorTestParameters {
    std::string templateString;
    std::vector<std::pair<std::string, std::map<std::string, std::string>>> cases;
    std::map<std::string, std::string> expectedFiles;
};

class TemplateGeneratorTestFixture : public ::testing::TestWithParam<TemplateGeneratorTestParameters> {};

TEST_P(TemplateGeneratorTestFixture, ShouldReport) {
    // arrange
    std::filesystem::path tempFileDirectory = std::filesystem::temp_directory_path() / "TemplateGeneratorTestFixture";
    if (exists(tempFileDirectory)) {
        std::filesystem::remove_all(tempFileDirectory);
    }

    {  // act
        generators::TemplateGenerator generator(GetParam().templateString, ".txt", tempFileDirectory);
        for (const auto& cas : GetParam().cases) {
            generator.Generate(cas.first, cas.second);
        }
    }

    // assert
    for (const auto& expectedFile : GetParam().expectedFiles) {
        std::ifstream fileStream(tempFileDirectory / expectedFile.first);
        std::stringstream actualBuffer;
        actualBuffer << fileStream.rdbuf();

        ASSERT_EQ(expectedFile.second, actualBuffer.str()) << "Expected file " << tempFileDirectory / expectedFile.first;
    }
}

INSTANTIATE_TEST_SUITE_P(SutherlandTests, TemplateGeneratorTestFixture,
                         testing::Values((TemplateGeneratorTestParameters){.templateString = "Start {{a}} and add {{b}} with {{a}}\n\t {{c}} + {{a}} + {{b}}\n",
                                                                           .cases = {std::make_pair("case1", std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}})},
                                                                           .expectedFiles = {{"case1.txt", "Start 1 and add 2 with 1\n\t {{c}} + 1 + 2\n"}}},
                                         (TemplateGeneratorTestParameters){.templateString = "Start {{a}} and add {{b}} with {{a}}{{cc}}{hi}\n\t {{cc}} + \"{{a}}\" + {{b}}\n",
                                                                           .cases = {std::make_pair("case1", std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}}),
                                                                                     std::make_pair("case2", std::map<std::string, std::string>{{"a", "alpha"}, {"b", "beta"}, {"cc", "gamma"}})},
                                                                           .expectedFiles = {{"case1.txt", "Start 1 and add 2 with 1{{cc}}{hi}\n\t {{cc}} + \"1\" + 2\n"},
                                                                                             {"case1.txt", "Start alpha and add beta with alphagamma{hi}\n\t gamma + \"alpha\" + beta\n"}}}));
}  // namespace ablateTesting