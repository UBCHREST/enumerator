#include <enumerator.hpp>
#include <functional>
#include <memory>
#include "enumerations/enumeration.hpp"
#include "enumerations/list.hpp"
#include "gtest/gtest.h"
#include "mocks.hpp"

using namespace enumerator;
using List = enumerator::enumerations::List;

namespace ablateTesting {

struct EnumeratorTestParameters {
    std::string name;
    std::vector<std::shared_ptr<enumerations::Enumeration>> enumerations;
    std::function<std::shared_ptr<Generator>()> setupMockGenerator;
    std::function<std::vector<std::shared_ptr<Reporter>>()> setupMockReporters;
};

class EnumeratorTestFixture : public ::testing::TestWithParam<EnumeratorTestParameters> {};

TEST_P(EnumeratorTestFixture, ShouldEnumerate) {
    // arrange
    auto mockGenerator = GetParam().setupMockGenerator();
    auto mockReporters = GetParam().setupMockReporters();

    enumerator::Enumerator enumerator(GetParam().name, GetParam().enumerations, mockGenerator, mockReporters);

    // act/assert
    enumerator.Enumerate();
}

INSTANTIATE_TEST_SUITE_P(
    EnumeratorTests, EnumeratorTestFixture,
    testing::Values(
        (EnumeratorTestParameters){
            .name = "t1_",
            .enumerations = {std::make_shared<List>("a", 1, 2, 3)},
            .setupMockGenerator =
                []() {
                    auto mockGenerator = std::make_shared<enumeratorTests::MockGenerator>();

                    EXPECT_CALL(*mockGenerator, Generate("t1_0", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t1_0"}, {"a", "1"}})).Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t1_1", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t1_1"}, {"a", "2"}})).Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t1_2", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t1_2"}, {"a", "3"}})).Times(::testing::Exactly(1));

                    return mockGenerator;
                },
            .setupMockReporters =
                []() {
                    auto mockReporter = std::make_shared<enumeratorTests::MockReporter>();

                    EXPECT_CALL(*mockReporter, Report("t1_0", std::map<std::string, std::string>{{"a", "1"}})).Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockReporter, Report("t1_1", std::map<std::string, std::string>{{"a", "2"}})).Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockReporter, Report("t1_2", std::map<std::string, std::string>{{"a", "3"}})).Times(::testing::Exactly(1));

                    return std::vector<std::shared_ptr<Reporter>>{mockReporter};
                }},
        (EnumeratorTestParameters){.name = "t2.",
                                   .enumerations = {std::make_shared<List>("a", 1, 2, 3), std::make_shared<List>("b", "aa", "bb", "cc")},
                                   .setupMockGenerator =
                                       []() {
                                           auto mockGenerator = std::make_shared<enumeratorTests::MockGenerator>();

                                           EXPECT_CALL(*mockGenerator, Generate("t2.0", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.0"}, {"a", "1"}, {"b", "aa"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t2.1", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.1"}, {"a", "1"}, {"b", "bb"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t2.2", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.2"}, {"a", "1"}, {"b", "cc"}}))
                                               .Times(::testing::Exactly(1));

                                           EXPECT_CALL(*mockGenerator, Generate("t2.3", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.3"}, {"a", "2"}, {"b", "aa"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t2.4", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.4"}, {"a", "2"}, {"b", "bb"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t2.5", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.5"}, {"a", "2"}, {"b", "cc"}}))
                                               .Times(::testing::Exactly(1));

                                           EXPECT_CALL(*mockGenerator, Generate("t2.6", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.6"}, {"a", "3"}, {"b", "aa"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t2.7", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.7"}, {"a", "3"}, {"b", "bb"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t2.8", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t2.8"}, {"a", "3"}, {"b", "cc"}}))
                                               .Times(::testing::Exactly(1));

                                           return mockGenerator;
                                       },
                                   .setupMockReporters =
                                       []() {
                                           auto mockReporter1 = std::make_shared<enumeratorTests::MockReporter>();
                                           auto mockReporter2 = std::make_shared<enumeratorTests::MockReporter>();

                                           EXPECT_CALL(*mockReporter1, Report("t2.0", std::map<std::string, std::string>{{"a", "1"}, {"b", "aa"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.0", std::map<std::string, std::string>{{"a", "1"}, {"b", "aa"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter1, Report("t2.1", std::map<std::string, std::string>{{"a", "1"}, {"b", "bb"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.1", std::map<std::string, std::string>{{"a", "1"}, {"b", "bb"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter1, Report("t2.2", std::map<std::string, std::string>{{"a", "1"}, {"b", "cc"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.2", std::map<std::string, std::string>{{"a", "1"}, {"b", "cc"}})).Times(::testing::Exactly(1));

                                           EXPECT_CALL(*mockReporter1, Report("t2.3", std::map<std::string, std::string>{{"a", "2"}, {"b", "aa"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.3", std::map<std::string, std::string>{{"a", "2"}, {"b", "aa"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter1, Report("t2.4", std::map<std::string, std::string>{{"a", "2"}, {"b", "bb"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.4", std::map<std::string, std::string>{{"a", "2"}, {"b", "bb"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter1, Report("t2.5", std::map<std::string, std::string>{{"a", "2"}, {"b", "cc"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.5", std::map<std::string, std::string>{{"a", "2"}, {"b", "cc"}})).Times(::testing::Exactly(1));

                                           EXPECT_CALL(*mockReporter1, Report("t2.6", std::map<std::string, std::string>{{"a", "3"}, {"b", "aa"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.6", std::map<std::string, std::string>{{"a", "3"}, {"b", "aa"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter1, Report("t2.7", std::map<std::string, std::string>{{"a", "3"}, {"b", "bb"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.7", std::map<std::string, std::string>{{"a", "3"}, {"b", "bb"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter1, Report("t2.8", std::map<std::string, std::string>{{"a", "3"}, {"b", "cc"}})).Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockReporter2, Report("t2.8", std::map<std::string, std::string>{{"a", "3"}, {"b", "cc"}})).Times(::testing::Exactly(1));

                                           return std::vector<std::shared_ptr<Reporter>>{mockReporter1, mockReporter2};
                                       }},
        (EnumeratorTestParameters){
            .name = "t3.",
            .enumerations = {std::make_shared<List>("a", "x", "y"), std::make_shared<List>("b", 1, 2, 3), std::make_shared<List>("c", "aa", "bb", "cc")},
            .setupMockGenerator =
                []() {
                    auto mockGenerator = std::make_shared<enumeratorTests::MockGenerator>();

                    EXPECT_CALL(*mockGenerator, Generate("t3.0", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.0"}, {"a", "x"}, {"b", "1"}, {"c", "aa"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.1", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.1"}, {"a", "x"}, {"b", "1"}, {"c", "bb"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.2", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.2"}, {"a", "x"}, {"b", "1"}, {"c", "cc"}}))
                        .Times(::testing::Exactly(1));

                    EXPECT_CALL(*mockGenerator, Generate("t3.3", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.3"}, {"a", "x"}, {"b", "2"}, {"c", "aa"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.4", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.4"}, {"a", "x"}, {"b", "2"}, {"c", "bb"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.5", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.5"}, {"a", "x"}, {"b", "2"}, {"c", "cc"}}))
                        .Times(::testing::Exactly(1));

                    EXPECT_CALL(*mockGenerator, Generate("t3.6", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.6"}, {"a", "x"}, {"b", "3"}, {"c", "aa"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.7", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.7"}, {"a", "x"}, {"b", "3"}, {"c", "bb"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.8", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.8"}, {"a", "x"}, {"b", "3"}, {"c", "cc"}}))
                        .Times(::testing::Exactly(1));

                    EXPECT_CALL(*mockGenerator, Generate("t3.9", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.9"}, {"a", "y"}, {"b", "1"}, {"c", "aa"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.10", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.10"}, {"a", "y"}, {"b", "1"}, {"c", "bb"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.11", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.11"}, {"a", "y"}, {"b", "1"}, {"c", "cc"}}))
                        .Times(::testing::Exactly(1));

                    EXPECT_CALL(*mockGenerator, Generate("t3.12", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.12"}, {"a", "y"}, {"b", "2"}, {"c", "aa"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.13", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.13"}, {"a", "y"}, {"b", "2"}, {"c", "bb"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.14", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.14"}, {"a", "y"}, {"b", "2"}, {"c", "cc"}}))
                        .Times(::testing::Exactly(1));

                    EXPECT_CALL(*mockGenerator, Generate("t3.15", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.15"}, {"a", "y"}, {"b", "3"}, {"c", "aa"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.16", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.16"}, {"a", "y"}, {"b", "3"}, {"c", "bb"}}))
                        .Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t3.17", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t3.17"}, {"a", "y"}, {"b", "3"}, {"c", "cc"}}))
                        .Times(::testing::Exactly(1));

                    return mockGenerator;
                },
            .setupMockReporters = []() { return std::vector<std::shared_ptr<Reporter>>{}; }},
        (EnumeratorTestParameters){.name = "t4.",
                                   .enumerations = {std::make_shared<List>("a", "x", "y"), std::make_shared<List>("b", 1)},
                                   .setupMockGenerator =
                                       []() {
                                           auto mockGenerator = std::make_shared<enumeratorTests::MockGenerator>();

                                           EXPECT_CALL(*mockGenerator, Generate("t4.0", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t4.0"}, {"a", "x"}, {"b", "1"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t4.1", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t4.1"}, {"a", "y"}, {"b", "1"}}))
                                               .Times(::testing::Exactly(1));

                                           return mockGenerator;
                                       },
                                   .setupMockReporters = []() { return std::vector<std::shared_ptr<Reporter>>{}; }},
        (EnumeratorTestParameters){
            .name = "t5.",
            .enumerations = {std::make_shared<List>("a", "x", "y"), std::make_shared<List>("b")},
            .setupMockGenerator =
                []() {
                    auto mockGenerator = std::make_shared<enumeratorTests::MockGenerator>();

                    EXPECT_CALL(*mockGenerator, Generate("t5.0", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t5.0"}, {"a", "x"}})).Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockGenerator, Generate("t5.1", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t5.1"}, {"a", "y"}})).Times(::testing::Exactly(1));

                    return mockGenerator;
                },
            .setupMockReporters =
                []() {
                    auto mockReporter = std::make_shared<enumeratorTests::MockReporter>();

                    EXPECT_CALL(*mockReporter, Report("t5.0", std::map<std::string, std::string>{{"a", "x"}})).Times(::testing::Exactly(1));
                    EXPECT_CALL(*mockReporter, Report("t5.1", std::map<std::string, std::string>{{"a", "y"}})).Times(::testing::Exactly(1));
                    return std::vector<std::shared_ptr<Reporter>>{mockReporter};
                }},
        (EnumeratorTestParameters){.name = "t6.",
                                   .enumerations = {std::make_shared<List>("a", "x", "y"), std::make_shared<List>("b"), std::make_shared<List>("c", 1, 2)},
                                   .setupMockGenerator =
                                       []() {
                                           auto mockGenerator = std::make_shared<enumeratorTests::MockGenerator>();

                                           EXPECT_CALL(*mockGenerator, Generate("t6.0", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t6.0"}, {"a", "x"}, {"c", "1"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t6.1", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t6.1"}, {"a", "x"}, {"c", "2"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t6.2", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t6.2"}, {"a", "y"}, {"c", "1"}}))
                                               .Times(::testing::Exactly(1));
                                           EXPECT_CALL(*mockGenerator, Generate("t6.3", std::map<std::string, std::string>{{enumerator::Enumerator::caseNameKey, "t6.3"}, {"a", "y"}, {"c", "2"}}))
                                               .Times(::testing::Exactly(1));
                                           return mockGenerator;
                                       },
                                   .setupMockReporters = []() { return std::vector<std::shared_ptr<Reporter>>{}; }}

        ));
}  // namespace ablateTesting