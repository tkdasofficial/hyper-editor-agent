#include "core/editor.hpp"
#include "parsers/json_parser.hpp"
#include "utils/logger.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>

void printUsage(const char* progName) {
    std::cout << "\033[1m\033[35mHyper Editor Agent - Advanced Headless Editor\033[0m\n"
              << "High-performance modular headless C++ video editing tool\n\n"
              << "\033[1mUsage:\033[0m\n"
              << "  " << progName << " --timeline <path_to_json> [options]\n\n"
              << "\033[1mOptions:\033[0m\n"
              << "  --timeline, -t <file>    Path to input timeline JSON configuration (Required)\n"
              << "  --output, -o <file>      Override output master MP4 file path\n"
              << "  --threads, -j <N>        Number of rendering worker threads (Default: auto)\n"
              << "  --verbose, -v            Enable verbose debug logs\n"
              << "  --help, -h               Show this help message and exit\n\n"
              << "\033[1mExample:\033[0m\n"
              << "  " << progName << " --timeline sample_timeline.json --output render_output.mp4\n\n";
}

int main(int argc, char* argv[]) {
    std::string timelinePath;
    std::string outputPathOverride;
    int threadCount = 0;
    bool verbose = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--timeline" || arg == "-t") {
            if (i + 1 < argc) timelinePath = argv[++i];
        } else if (arg == "--output" || arg == "-o") {
            if (i + 1 < argc) outputPathOverride = argv[++i];
        } else if (arg == "--threads" || arg == "-j") {
            if (i + 1 < argc) threadCount = std::stoi(argv[++i]);
        } else if (arg == "--verbose" || arg == "-v") {
            verbose = true;
        } else if (arg == "--help" || arg == "-h") {
            printUsage(argv[0]);
            return 0;
        }
    }

    if (verbose) {
        HyperEditor::Utils::Logger::instance().setLevel(HyperEditor::Utils::LogLevel::DEBUG);
    }

    if (timelinePath.empty()) {
        LOG_ERROR("Main", "Missing required --timeline parameter.");
        printUsage(argv[0]);
        return 1;
    }

    if (!std::filesystem::exists(timelinePath)) {
        LOG_ERROR("Main", "Timeline file does not exist: ", timelinePath);
        return 1;
    }

    LOG_INFO("Main", "Parsing timeline specification: ", timelinePath);
    HyperEditor::Core::Timeline timeline;
    if (!HyperEditor::Parsers::JsonParser::parseFile(timelinePath, timeline)) {
        LOG_ERROR("Main", "Failed to parse timeline JSON file.");
        return 1;
    }

    if (!outputPathOverride.empty()) {
        timeline.outputPath = outputPathOverride;
    }

    HyperEditor::Core::AdvancedHeadlessEditor editor;
    if (threadCount > 0) {
        editor.setThreadCount(threadCount);
    }

    bool success = editor.render(timeline);
    if (!success) {
        LOG_ERROR("Main", "Advanced Headless Editor rendering failed.");
        return 1;
    }

    LOG_INFO("Main", "Execution finished successfully.");
    return 0;
}
