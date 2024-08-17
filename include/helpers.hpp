#pragma once

#include <filesystem>
#include <sstream>

// Namespace that contains constants.
namespace constants {
constexpr double EPSILON = 1e-9;
constexpr double ZERO = 0.0;
constexpr double ONE = 1.0;

constexpr std::string_view DEFAULT_VIDEO_OVERLAY_NAME = "_overlayed";
constexpr std::string_view DEFAULT_LATEX_COMPILER = "xelatex";
constexpr std::string_view DEFAULT_LATEX_OUTPUT_DIRECTORY = "latex_build";
constexpr std::string_view DEFAULT_FFMPEG_FILELIST = "filelist.txt";
constexpr std::string_view DEFAULT_BUILD_DIRECTORY = "temp";

constexpr std::string_view DEFAULT_LATEX_PREAMBLE =
    "\\documentclass[tikz, border=10pt]{standalone}\n"
    "\\usepackage{tikz}\n\\usetikzlibrary{automata, positioning}\n"
    "\\begin{document}\n\\begin{tikzpicture}[scale=2]\n"
    "    \\tikzset{state/.style={draw, fill=white, circle, minimum size=1cm}}\n";
} // namespace constants

// Creates directory at specified path. Throws an error if it cant.
void create_dir(const std::filesystem::path &folder_path);
// Deletes the directory or file at the specified path.
void delete_dir_or_file(const std::filesystem::path &folder_path);
// If verbose is true, appends to the command output so that the output is silenced. Implemented seperately for Windows.
void check_verbosity(std::ostringstream &command, const bool &verbose);
// Executes a command and throws if a problem is encountered. Modifies the verbosity.
void execute_command(std::ostringstream &command, const bool &verbose);
// Waits until the user presses enter.
void wait_on_enter();
