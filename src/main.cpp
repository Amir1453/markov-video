#include "argparse.hpp"
#include "ffmpeg.hpp"
#include "helpers.hpp"
#include "markov.hpp"
#include "visuals.hpp"
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

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

  const bool &verbose = program.get<bool>("--verbose");
  const bool &no_cleanup = program.get<bool>("-nc");
  const bool &edit_latex = program.get<bool>("-el");

  MarkovChain mc(markov_file);
  const std::size_t &transition_matrix_size = mc.getTransitionMatrixSize();

  if (program.is_used("-b")) {
    const fs::path &build_folder = program.get("-b");
    create_dir(build_folder);

    if (program.is_used("-V")) {
      const fs::path &video_folder = program.get("-V");
      const std::size_t &iterations = program.get<std::size_t>("-i");
      const auto &markov_states = iterate_markov_states(mc, iterations);
      generate_all_markov_graphs(mc, build_folder);
      if (edit_latex) {
        wait_on_enter();
      }
      compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler, verbose);
      convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, build_folder, verbose);
      overlay_images_to_videos(video_folder, build_folder, transition_matrix_size, build_folder, verbose);
      create_filelist(markov_states, build_folder / filelist_path);
      combine_segments(build_folder / filelist_path, output_path, verbose);
      if (!no_cleanup) {
        delete_dir_or_file(build_folder);
      }
      return 0;

    } else {
      create_dir(output_path);
      generate_all_markov_graphs(mc, build_folder);
      if (edit_latex) {
        wait_on_enter();
      }
      compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler, verbose);
      convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, output_path, verbose);
      if (!no_cleanup) {
        delete_dir_or_file(build_folder);
      }
      return 0;
    }
  } else {
    if (program.is_used("-V")) {
      const fs::path &video_folder = program.get("-V");
      const fs::path &build_folder = video_folder / constants::DEFAULT_BUILD_DIRECTORY;
      const std::size_t &iterations = program.get<std::size_t>("-i");
      const auto &markov_states = iterate_markov_states(mc, iterations);

      create_dir(build_folder);
      generate_all_markov_graphs(mc, build_folder);
      if (edit_latex) {
        wait_on_enter();
      }
      compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler, verbose);
      convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, build_folder, verbose);
      overlay_images_to_videos(video_folder, build_folder, transition_matrix_size, build_folder, verbose);
      create_filelist(markov_states, build_folder / filelist_path);
      combine_segments(build_folder / filelist_path, output_path, verbose);
      if (!no_cleanup) {
        delete_dir_or_file(build_folder);
      }
      return 0;

    } else {
      const fs::path &build_folder = output_path / constants::DEFAULT_BUILD_DIRECTORY;
      create_dir(build_folder);
      generate_all_markov_graphs(mc, build_folder);
      if (edit_latex) {
        wait_on_enter();
      }
      compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler, verbose);
      convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, output_path, verbose);
      if (!no_cleanup) {
        delete_dir_or_file(build_folder);
      }
      return 0;
    }
  }

  return 0;
}
