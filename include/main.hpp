#pragma once

#include "markov.hpp"
#include <cstddef>
#include <filesystem>
#include <string>

void build_and_video(MarkovChain &mc, const std::filesystem::path &build_folder,
                     const std::filesystem::path &video_folder, const std::filesystem::path &output_path,
                     const std::filesystem::path &latex_output_directory, const std::filesystem::path &filelist_path,
                     const std::size_t &iterations, const std::string &latex_compiler,
                     const std::string &latex_compiler_options, const bool &edit_latex, const bool &verbose,
                     const bool &no_cleanup);

void build_only(const MarkovChain &mc, const std::filesystem::path &build_folder,
                const std::filesystem::path &output_path, const std::filesystem::path &latex_output_directory,
                const std::string &latex_compiler, const std::string &latex_compiler_options, const bool &edit_latex,
                const bool &verbose, const bool &no_cleanup);

void video_only(MarkovChain &mc, const std::filesystem::path &build_folder, const std::filesystem::path &video_folder,
                const std::filesystem::path &output_path, const std::filesystem::path &latex_output_directory,
                const std::filesystem::path &filelist_path, const std::size_t &iterations,
                const std::string &latex_compiler, const std::string &latex_compiler_options, const bool &edit_latex,
                const bool &verbose, const bool &no_cleanup);

void no_options(MarkovChain &mc, const std::filesystem::path &build_folder, const std::filesystem::path &output_path,
                const std::filesystem::path &latex_output_directory, const std::string &latex_compiler,
                const std::string &latex_compiler_options, const bool &edit_latex, const bool &verbose,
                const bool &no_cleanup); 
