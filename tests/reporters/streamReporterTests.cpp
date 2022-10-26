#include <enumerator.hpp>
#include <memory>
#include "enumerations/enumeration.hpp"
#include "gtest/gtest.h"
#include "reporters/streamReporter.hpp"

using namespace enumerator;

namespace ablateTesting {

struct StreamReporterTestParameters {
    std::vector<std::pair<std::string, std::map<std::string, std::string>>> cases;
    std::string expectedString;
};

class StreamReporterTestFixture : public ::testing::TestWithParam<StreamReporterTestParameters> {};

TEST_P(StreamReporterTestFixture, ShouldReport) {
    // arrange
    std::stringstream actualStream;
    reporters::StreamReporter reporter(actualStream);

    // act
    for (const auto& cas : GetParam().cases) {
        reporter.Report(cas.first, cas.second);
    }

    // assert
    ASSERT_EQ(GetParam().expectedString, actualStream.str());
}

INSTANTIATE_TEST_SUITE_P(StreamReporterTests, StreamReporterTestFixture,
                         testing::Values((StreamReporterTestParameters){.cases = {std::make_pair("case1.", std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}})},
                                                                        .expectedString = "case1.\n\ta: 1\n\tb: 2\n"},
                                         (StreamReporterTestParameters){.cases = {std::make_pair("case1.", std::map<std::string, std::string>{{"a", "1"}, {"b", "2"}}),
                                                                                  std::make_pair("case2.", std::map<std::string, std::string>{}),
                                                                                  std::make_pair("case3.", std::map<std::string, std::string>{{"c", "3"}})},
                                                                        .expectedString = "case1.\n\ta: 1\n\tb: 2\ncase2.\ncase3.\n\tc: 3\n"}));
}  // namespace ablateTesting