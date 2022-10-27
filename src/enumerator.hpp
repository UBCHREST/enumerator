#ifndef ENUMERATOR_ENUMERATOR_HPP
#define ENUMERATOR_ENUMERATOR_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "enumerations/enumeration.hpp"
#include "generator.hpp"
#include "reporter.hpp"

namespace enumerator {

class Enumerator {
   public:
    //! allow access to the case name
    inline static std::string caseNameKey = "id";

   private:
    const std::string name;
    const std::vector<std::shared_ptr<enumerations::Enumeration>> enumerations;
    const std::shared_ptr<Generator> generator;
    const std::vector<std::shared_ptr<Reporter>> reporters;

    /**
     *     Recursive call to
     */
    void Enumerate(const std::vector<std::shared_ptr<enumerations::Enumeration>>& remainingEnumerations, const std::map<std::string, std::string>& values, int& caseCount);

    /**
     * Call that passes options to generator/reporters
     * @param nowRemainingEnumerations
     * @param values
     * @param caseCount
     */
    void ProcessOptionSet(const std::vector<std::shared_ptr<enumerations::Enumeration>>& nowRemainingEnumerations, const std::map<std::string, std::string>& optionSet, int& caseCount);

    std::string BuildCaseName(const int& caseCount);

   public:
    Enumerator(std::string name, std::vector<std::shared_ptr<enumerations::Enumeration>> enumerations, std::shared_ptr<Generator> generator, std::vector<std::shared_ptr<Reporter>> reporters);

    void Enumerate();
};
}  // namespace enumerator

#endif  // ENUMERATOR_ENUMERATOR_HPP
