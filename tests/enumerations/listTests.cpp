#include "enumerations/list.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mockFactory.hpp"
namespace enumeratorTests {

TEST(ListTests, ShouldCreateFromConstructor) {
    // arrange
    // act
    enumerator::enumerations::List list("listName", "blue", "green", 3.3, 5);
    // assert
    std::set<std::string> expectedList = {"blue", "green", "3.3", "5"};
    ASSERT_EQ(expectedList, list.Values());
    ASSERT_EQ("listName", list.Name());
}

TEST(ListTests, ShouldCreateFromList) {
    // arrange
    // act
    enumerator::enumerations::List list("listName", {"blue", "green", "3.3", "5"});
    // assert
    std::set<std::string> expectedList = {"blue", "green", "3.3", "5"};
    ASSERT_EQ(expectedList, list.Values());
    ASSERT_EQ("listName", list.Name());
}

TEST(ListTests, ShouldCreateFromRegistar) {
    // arrange
    std::shared_ptr<cppParserTesting::MockFactory> mockFactory = std::make_shared<cppParserTesting::MockFactory>();
    const std::string expectedClassType;  // should be default
    EXPECT_CALL(*mockFactory, GetClassType()).Times(::testing::Exactly(1)).WillOnce(::testing::ReturnRef(expectedClassType));
    EXPECT_CALL(*mockFactory, Get(cppParser::ArgumentIdentifier<std::string>{.inputName = "name"})).Times(::testing::Exactly(1)).WillOnce(::testing::Return("listName1"));
    EXPECT_CALL(*mockFactory, Get(cppParser::ArgumentIdentifier<std::vector<std::string>>{.inputName = "values"}))
        .Times(::testing::Exactly(1))
        .WillOnce(::testing::Return(std::vector<std::string>{"2.0", "3.0", "4.0"}));

    // act
    auto createMethod = Creator<enumerator::enumerations::Enumeration>::GetCreateMethod(mockFactory->GetClassType());
    auto instance = createMethod(mockFactory);

    // assert
    ASSERT_TRUE(instance != nullptr) << " should create an instance of the ParsedSeries";
    ASSERT_TRUE(std::dynamic_pointer_cast<enumerator::enumerations::List>(instance) != nullptr) << " should be an instance of enumerator::List";
    ASSERT_EQ(instance->Name(), "listName1");
    std::set<std::string> expectedValues = {"2.0", "3.0", "4.0"};
    ASSERT_EQ(instance->Values(), expectedValues);
}

}  // namespace enumeratorTests