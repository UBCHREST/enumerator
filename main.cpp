#include <filesystem>
#include <iostream>
#include <listing.hpp>
#include <map>
#include <optional>
#include <regex>
#include <string>
#include <yamlParser.hpp>
#include "enumerator.hpp"

static std::optional<std::pair<std::string, std::string>> DecomposeArgument(std::string arg) {
    std::smatch match;
    // check for pairs
    if (std::regex_search(arg, match, std::regex("--(.*)=(.*)"))) {
        return std::make_pair(match[1], match[2]);
    }
    // check for standalone
    if (std::regex_search(arg, match, std::regex("--(.*)"))) {
        return std::make_pair(match[1], "");
    }

    return {};
}

void PrintHelp() {
    std::cout << "Enumerator" << std::endl;
    std::cout << "  This executable allows generating an enumerations of inputs." << std::endl;
    std::cout << "  The minimum required input is a path to an input file: " << std::endl;
    std::cout << "      ./enumerator    --input=path/to/input.yaml" << std::endl;
    std::cout << std::endl;
    std::cout << "  Additional examples can be found at https://github.com/UBCHREST/enumerator " << std::endl;
    std::cout << std::endl;
    std::cout << "Available Input Options:" << std::endl;
    std::cout << cppParser::Listing::Get() << std::endl;
}

int main(int argc, char** args) {
    // Parse the inputs
    std::map<std::string, std::string> parsedArguments;
    for (int arg = 0; arg < argc; arg++) {
        auto pair = DecomposeArgument(std::string(args[arg]));
        if (pair.has_value()) {
            parsedArguments.insert(pair.value());
        }
    }
    // check for help
    if (parsedArguments.count("help")) {
        PrintHelp();
        return 0;
    }

    // get the input file
    if (!parsedArguments.count("input")) {
        PrintHelp();
        throw std::invalid_argument("The input file path must be specified.");
    }
    std::filesystem::path inputFilePath = parsedArguments["input"];
    if (!exists(inputFilePath)) {
        throw std::invalid_argument("Unable to locate the input file specified at " + inputFilePath.string());
    }

    // create the yaml parser
    std::shared_ptr<cppParser::YamlParser> parser = std::make_shared<cppParser::YamlParser>(inputFilePath);

    // run the enumeration
    auto enumerator = parser->GetByName<enumerator::Enumerator>("");
    enumerator->Enumerate();

    // check for unused parameters
    auto unusedValues = parser->GetUnusedValues();
    if (!unusedValues.empty()) {
        std::cout << "WARNING: The following input parameters were not used:" << std::endl;
        for (auto unusedValue : unusedValues) {
            std::cout << unusedValue << std::endl;
        }
    }
}