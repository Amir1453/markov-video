#include "argparse.hpp"
#include "helpers.hpp"
#include "markov.hpp"
#include "markov_processor.hpp"
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

int main(int argc, char *argv[]) {
  argparse::ArgumentParser program("markov-video", "0.1");
  auto &video_or_gif = program.add_mutually_exclusive_group();
  program.add_description("Create Markov Chain videos and GIFs.");
  program.add_epilog("Did it not work out for you ?");

  program.add_argument("--verbose").flag().help("enable output for ffmpeg, latex and ImageMagick.");
  program.add_argument("-m", "--markov-file").required().help("specify the file which contains the markov chain.");
  program.add_argument("-o", "--output-file").required().help("specify the output file path.");
  video_or_gif.add_argument("-V", "--videos-folder").help("specify the folder which contains the video segments.");
  video_or_gif.add_argument("-G", "--is-gif").flag().help("specify if output will be a gif. (NYI)");
  program.add_argument("-i", "--iterations")
      .scan<'i', std::size_t>()
      .help("specify the number of iterations for the markov chain.");
  program.add_argument("-b", "--build-folder").help("specify the folder which will contain the auxillary files.");
  program.add_argument("-nc", "--no-cleanup").flag().help("disables removing auxillary files and folders.");
  program.add_argument("-el", "--edit-latex")
      .flag()
      .help("prompts user to press enter after creating the latex files.");
  program.add_argument("-lc", "--latex-compiler")
      .default_value(std::string(constants::DEFAULT_LATEX_COMPILER))
      .help("specify the latex compiler which will compile the .tex files.");
  program.add_argument("-lod", "--latex-output-directory")
      .default_value(std::string(constants::DEFAULT_LATEX_OUTPUT_DIRECTORY))
      .help("specify the directory where the latex compiler will compile the .tex files to.");
  program.add_argument("-lco", "--latex-compiler-options")
      .default_value("")
      .help("specify the latex compiler options which will be added to compile the .tex files.");
  program.add_argument("-flp", "--filelist-path")
      .default_value(std::string(constants::DEFAULT_FFMPEG_FILELIST))
      .help("specify the of the filelist path.");

  try {
    program.parse_args(argc, argv);
    if ((program.is_used("-V") || program.is_used("-G")) && !program.is_used("-i")) {
      std::cerr << "-i required when using -V or -G" << std::endl;
      std::cerr << program;
      return 1;
    }
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << program;
    return 1;
  }

  const fs::path &markov_file = program.get("-m");
  const fs::path &output_path = program.get("-o");
  const fs::path &latex_output_directory = program.get("-lod");
  const fs::path &filelist_path = program.get("-flp");

  const std::string &latex_compiler = program.get("-lc");
  const std::string &latex_compiler_options = program.get("-lco");

  const bool verbose = program.get<bool>("--verbose");
  const bool no_cleanup = program.get<bool>("-nc");
  const bool edit_latex = program.get<bool>("-el");

  const fs::path &build_folder =
      program.is_used("-b") ? fs::path(program.get("-b")) : output_path / constants::DEFAULT_BUILD_DIRECTORY;

  MarkovChain mc(markov_file);
  MarkovProcessor processor(mc, build_folder, output_path, latex_output_directory, filelist_path, latex_compiler,
                            latex_compiler_options, edit_latex, verbose, no_cleanup);

  ProcessingMode mode = determine_processing_mode(program.is_used("-b"), program.is_used("-V"));

  switch (mode) {
  case ProcessingMode::BuildAndVideo: {
    const fs::path &video_folder = program.get("-V");
    const std::size_t iterations = program.get<std::size_t>("-i");
    processor.build_and_video(video_folder, iterations);
    return 0;
  }
  case ProcessingMode::BuildOnly:
    processor.build_only();
    return 0;
  case ProcessingMode::VideoOnly: {
    const fs::path &video_folder = program.get("-V");
    const std::size_t iterations = program.get<std::size_t>("-i");
    processor.video_only(video_folder, iterations);
    return 0;
  }
  case ProcessingMode::NoOptions:
    processor.no_options();
    return 0;
  }

  return 0;
}
