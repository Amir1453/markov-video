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
                                 const std::string &file_extension, const std::string &overlay_extension,
                                 const std::string &latex_compiler, const std::string &latex_compiler_options,
                                 bool edit_latex, bool verbose, bool no_cleanup)
    : mc(mc), build_folder(build_folder), output_path(output_path), latex_output_directory(latex_output_directory),
      filelist_path(filelist_path), file_extension(file_extension), overlay_extension(overlay_extension),
      latex_compiler(latex_compiler), latex_compiler_options(latex_compiler_options), edit_latex(edit_latex),
      verbose(verbose), no_cleanup(no_cleanup) {}

void MarkovProcessor::video(const fs::path &video_folder, std::size_t iterations) const {
  const std::size_t &transition_matrix_size = mc.get_transition_matrix_size();
  const auto &markov_states = iterate_markov_states(mc, iterations);

  create_dir(build_folder);
  generate_all_markov_graphs(mc, build_folder);

  if (edit_latex)
    wait_on_enter();

  compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler,
                            latex_compiler_options, verbose);
  convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, build_folder, verbose);
  overlay_images_to_videos(video_folder, file_extension, build_folder, transition_matrix_size, build_folder, verbose);
  create_filelist(markov_states, build_folder / filelist_path, overlay_extension, file_extension);
  combine_segments(build_folder / filelist_path, output_path, verbose);

  if (!no_cleanup)
    delete_dir_or_file(build_folder);
}

void MarkovProcessor::gif(std::size_t iterations) const {
  const std::size_t &transition_matrix_size = mc.get_transition_matrix_size();
  const auto &markov_states = iterate_markov_states(mc, iterations);

  create_dir(build_folder);
  generate_all_markov_graphs(mc, build_folder);

  if (edit_latex)
    wait_on_enter();

  compile_all_markov_graphs(build_folder, transition_matrix_size, latex_output_directory, latex_compiler,
                            latex_compiler_options, verbose);
  convert_all_pdfs_to_pngs(build_folder / latex_output_directory, transition_matrix_size, build_folder, verbose);
  create_filelist(markov_states, build_folder / filelist_path, "", "png");
  create_gif(build_folder / filelist_path, output_path, verbose);

  if (!no_cleanup)
    delete_dir_or_file(build_folder);
}

void MarkovProcessor::build_only() const {
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

void MarkovProcessor::no_options() const {
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

ProcessingMode determine_processing_mode(bool video_used, bool gif_used) {
  if (video_used)
    return ProcessingMode::Video;
  else if (gif_used)
    return ProcessingMode::GIF;
  else
    return ProcessingMode::BuildOnly;
}
