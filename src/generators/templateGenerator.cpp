#include "templateGenerator.hpp"
#include <fstream>
#include <iostream>
#include <regex>

enumerator::generators::TemplateGenerator::TemplateGenerator(std::string templateString, std::string extension, std::filesystem::path outputDirectoryIn)
    : templateString(std::move(templateString)), outputDirectory(std::filesystem::absolute(outputDirectoryIn)), extension(extension) {
    // create the output directory if needed
    create_directories(outputDirectory);
}
std::string enumerator::generators::TemplateGenerator::GetVariableReplacementString(const std::string varName) {
    auto rawName = "{{" + varName + "}}";

    std::regex specialChars{R"([-[\]{}()*+?.,\^$|#\s])"};
    return std::regex_replace(rawName, specialChars, R"(\$&)");
}

void enumerator::generators::TemplateGenerator::Generate(const std::string& caseName, const std::map<std::string, std::string>& values) {
    // Get a copy of the string
    auto templateCopy = templateString;

    // replace each variable
    for (const auto& value : values) {
        templateCopy = std::regex_replace(templateCopy, std::regex(GetVariableReplacementString(value.first)), value.second);
    }

    // Create a new file path
    auto outputPath = outputDirectory / (caseName + extension);
    std::ofstream out(outputPath);
    out << templateCopy;
    out.close();
}

#include "registrar.hpp"
REGISTER(enumerator::Generator, enumerator::generators::TemplateGenerator, "Generates template from a specified string", ARG(std::string, "template", "the template string"),
         OPT(std::string, "extension", "optional file extension"), ARG(std::filesystem::path, "outputDirectory", "the path to the output directory"));