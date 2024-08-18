#pragma once

#include "markov.hpp"
#include <cstddef>
#include <filesystem>
#include <string>

class MarkovProcessor {
public:
  MarkovProcessor(MarkovChain &mc, const std::filesystem::path &build_folder, const std::filesystem::path &output_path,
                  const std::filesystem::path &latex_output_directory, const std::filesystem::path &filelist_path,
                  const std::string &file_extension, const std::string &latex_compiler,
                  const std::string &latex_compiler_options, bool edit_latex, bool verbose, bool no_cleanup);

  void video_and_build(const std::filesystem::path &video_folder, std::size_t iterations);

  void video_only(const std::filesystem::path &video_folder, std::size_t iterations);

  void build_only();

  void no_options();

private:
  MarkovChain &mc;
  const std::filesystem::path &build_folder;
  const std::filesystem::path &output_path;
  const std::filesystem::path &latex_output_directory;
  const std::filesystem::path &filelist_path;
  const std::string &file_extension;
  const std::string &latex_compiler;
  const std::string &latex_compiler_options;
  bool edit_latex;
  bool verbose;
  bool no_cleanup;
};

enum class ProcessingMode { VideoAndBuild, VideoOnly, BuildOnly, NoOptions };

ProcessingMode determine_processing_mode(bool build_used, bool video_used);
