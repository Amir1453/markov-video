#include "markov_processor.hpp"
#include "ffmpeg.hpp"
#include "helpers.hpp"
#include "markov.hpp"
#include "visuals.hpp"
#include <cstddef>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

MarkovProcessor::MarkovProcessor(MarkovChain &mc, const fs::path &build_folder, const fs::path &output_path,
                                 const fs::path &latex_output_directory, const fs::path &filelist_path,
                                 const std::string &latex_compiler, const std::string &latex_compiler_options,
                                 bool edit_latex, bool verbose, bool no_cleanup)
    : mc(mc), build_folder(build_folder), output_path(output_path), latex_output_directory(latex_output_directory),
      filelist_path(filelist_path), latex_compiler(latex_compiler), latex_compiler_options(latex_compiler_options),
      edit_latex(edit_latex), verbose(verbose), no_cleanup(no_cleanup) {}

void MarkovProcessor::video_and_build(const fs::path &video_folder, std::size_t iterations) {
  const auto &markov_states = iterate_markov_states(mc, iterations);
  const std::size_t &transition_matrix_size = mc.get_transition_matrix_size();

  create_dir(build_folder);

  generate_all_markov_graphs(mc, build_folder);

  if (edit_latex)
    wait_on_enter();

  compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler,
                            latex_compiler_options, verbose);
  convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, build_folder, verbose);
  overlay_images_to_videos(video_folder, build_folder, transition_matrix_size, build_folder, verbose);
  create_filelist(markov_states, build_folder / filelist_path);
  combine_segments(build_folder / filelist_path, output_path, verbose);

  if (!no_cleanup)
    delete_dir_or_file(build_folder);
}

void MarkovProcessor::video_only(const fs::path &video_folder, std::size_t iterations) {
  const std::size_t &transition_matrix_size = mc.get_transition_matrix_size();
  const auto &markov_states = iterate_markov_states(mc, iterations);

  create_dir(build_folder);
  generate_all_markov_graphs(mc, build_folder);

  if (edit_latex)
    wait_on_enter();

  compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler,
                            latex_compiler_options, verbose);
  convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, build_folder, verbose);
  overlay_images_to_videos(video_folder, build_folder, transition_matrix_size, build_folder, verbose);
  create_filelist(markov_states, build_folder / filelist_path);
  combine_segments(build_folder / filelist_path, output_path, verbose);

  if (!no_cleanup)
    delete_dir_or_file(build_folder);
}

void MarkovProcessor::build_only() {
  const std::size_t &transition_matrix_size = mc.get_transition_matrix_size();

  create_dir(build_folder);
  create_dir(output_path);
  generate_all_markov_graphs(mc, build_folder);

  if (edit_latex)
    wait_on_enter();

  compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler,
                            latex_compiler_options, verbose);
  convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, output_path, verbose);

  if (!no_cleanup)
    delete_dir_or_file(build_folder);
}

void MarkovProcessor::no_options() {
  const std::size_t &transition_matrix_size = mc.get_transition_matrix_size();

  create_dir(build_folder);
  create_dir(output_path);
  generate_all_markov_graphs(mc, build_folder);

  if (edit_latex)
    wait_on_enter();

  compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler,
                            latex_compiler_options, verbose);
  convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, output_path, verbose);

  if (!no_cleanup)
    delete_dir_or_file(build_folder);
}

ProcessingMode determine_processing_mode(const bool &build_used, const bool &video_used) {
  if (build_used && video_used)
    return ProcessingMode::VideoAndBuild;
  else if (video_used)
    return ProcessingMode::VideoOnly;
  else if (build_used)
    return ProcessingMode::BuildOnly;
  else
    return ProcessingMode::NoOptions;
}
