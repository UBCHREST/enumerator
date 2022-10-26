#include "enumerator.hpp"

enumerator::Enumerator::Enumerator(std::string name, std::vector<std::shared_ptr<enumerations::Enumeration>> enumerations, std::shared_ptr<Generator> generator,
                                   std::vector<std::shared_ptr<Reporter>> reporters)
    : name(name), enumerations(std::move(enumerations)), generator(std::move(generator)), reporters(std::move(reporters)) {}

void enumerator::Enumerator::Enumerate() {
    // March over each enumerate
    int caseCount = 0;
    Enumerate(enumerations, {}, caseCount);
}
std::string enumerator::Enumerator::BuildCaseName(const int& caseCount) { return name + std::to_string(caseCount); }

void enumerator::Enumerator::ProcessOptionSet(const std::vector<std::shared_ptr<enumerations::Enumeration>>& nowRemainingEnumerations, const std::map<std::string, std::string>& optionSet,
                                              int& caseCount) {
    // If this is the end of the line
    if (nowRemainingEnumerations.empty()) {
        // build the name
        auto caseName = BuildCaseName(caseCount++);

        // add the option to get the case name
        auto optionSetCopy = optionSet;
        optionSetCopy[caseNameKey] = caseName;

        // Generate the value
        generator->Generate(caseName, optionSetCopy);

        // report the value to the reporters
        for (const auto& reporter : reporters) {
            reporter->Report(caseName, optionSet);
        }
    } else {
        // Enumerate any remaining options
        Enumerate(nowRemainingEnumerations, optionSet, caseCount);
    }
}

void enumerator::Enumerator::Enumerate(const std::vector<std::shared_ptr<enumerations::Enumeration>>& remainingEnumerations, const std::map<std::string, std::string>& values, int& caseCount) {
    // Build a list of remainingEnumerations
    std::vector<std::shared_ptr<enumerations::Enumeration>> nowRemainingEnumerations;
    nowRemainingEnumerations = std::vector<std::shared_ptr<enumerations::Enumeration>>(remainingEnumerations.begin() + 1, remainingEnumerations.end());

    // March over each option
    if (remainingEnumerations.front()->Values().empty()) {
        // This is for a special case when an empty Enumeration is given
        ProcessOptionSet(nowRemainingEnumerations, values, caseCount);

    } else {
        for (auto optionValue : remainingEnumerations.front()->Values()) {
            auto valuesCopy = values;
            valuesCopy[remainingEnumerations.front()->Name()] = optionValue;
            ProcessOptionSet(nowRemainingEnumerations, valuesCopy, caseCount);
        }
    }
}
#include "registrar.hpp"
REGISTER_DEFAULT(enumerator::Enumerator, enumerator::Enumerator, "The default enumerator", ARG(std::string, "name", "base name for the resulting cases"),
                 ARG(std::vector<enumerator::enumerations::Enumeration>, "enumerations", "The list of enumerations"), ARG(enumerator::Generator, "generator", "where to generate the files"),
                 OPT(std::vector<enumerator::Reporter>, "reporters", "optional list of reporters/or where to write the history"));
