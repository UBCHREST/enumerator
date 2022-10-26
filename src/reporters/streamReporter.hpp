#ifndef ENUMERATOR_STREAMREPORTER_HPP
#define ENUMERATOR_STREAMREPORTER_HPP

#include <iostream>
#include "reporter.hpp"

namespace enumerator::reporters {

class StreamReporter : public Reporter {
   private:
    std::ostream& outputStream;

   public:
    explicit StreamReporter(std::ostream& outputStream = std::cout);
    void Report(const std::string& caseName, const std::map<std::string, std::string>& values) override;
};

}  // namespace enumerator::reporters
#endif  // ENUMERATOR_STREAMREPORTER_HPP
