#include <boost/program_options.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/parsers.hpp>
#include <ccl/lex/analyzer_generator/analyzer_generator.hpp>
#include <fstream>
#include <iostream>

namespace po = boost::program_options;

auto main(int argc, char *argv[]) -> int
{
    std::string source_file;
    std::string header_name;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")(
        "lexical-analyzer-rules,L", po::value<std::string>(&source_file),
        "file with rules for lexical analyzer")(
        "header-file,H", po::value<std::string>(&header_name), "header file name");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.contains("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    if (!vm.contains("header-file")) {
        std::cout << "File with rules for lexical analyzer was not specified." << std::endl;
        return 1;
    }

    auto [generated_header, generated_source] =
        ccl::lex::AnalyzerGenerator::generateStaticVersion(source_file);

    auto file_stream = std::fstream{};
    file_stream.open(header_name, std::ios::out);

    if (!file_stream.is_open()) {
        fmt::print("Error: cannot open file {}\n", header_name);
        return 1;
    }

    file_stream << generated_header;
    file_stream.close();

    return 0;
}