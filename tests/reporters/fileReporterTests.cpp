#include <enumerator.hpp>
#include <memory>
#include "enumerations/enumeration.hpp"
#include "gtest/gtest.h"
#include "reporters/fileReporter.hpp"

using namespace enumerator;

namespace ablateTesting {

struct FileReporterTestParameters {
    std::vector<std::pair<std::string, std::map<std::string, std::string>>> cases;
    std::string expectedString;
};

class FileReporterTestFixture : public ::testing::TestWithParam<FileReporterTestParameters> {};

TEST_P(FileReporterTestFixture, ShouldReport) {
    // arrange
    std::filesystem::path tempFilePath = std::filesystem::temp_directory_path() / "FileReporterTestFixture.tmp";
    if (exists(tempFilePath)) {
        std::filesystem::remove(tempFilePath);
    }

    {  // act
        reporters::FileReporter reporter(tempFilePath);
        for (const auto& cas : GetParam().cases) {
            reporter.Report(cas.first, cas.second);
        }
    }

    // assert
    std::ifstream fileStream(tempFilePath);
    std::stringstream actualBuffer;
    actualBuffer << fileStream.rdbuf();

    ASSERT_EQ(GetParam().expectedString, actualBuffer.str());
}

INSTANTIATE_TEST_SUITE_P(FileReporterTests, FileReporterTestFixture,
                         testing::Values((FileReporterTestParameters){.cases = {std::make_pair("case1.", std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}})},
                                                                      .expectedString = "case1.\n\ta: 1\n\tb: 2\n"},
                                         (FileReporterTestParameters){.cases = {std::make_pair("case1.", std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}}),
                                                                                std::make_pair("case2.", std::map<std::string, std::string>{}),
                                                                                std::make_pair("case3.", std::map<std::string, std::string>{{"c", "3"}})},
                                                                      .expectedString = "case1.\n\ta: 1\n\tb: 2\ncase2.\ncase3.\n\tc: 3\n"}));
}  // namespace ablateTesting