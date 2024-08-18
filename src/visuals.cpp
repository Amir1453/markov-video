#include "visuals.hpp"
#include "helpers.hpp"
#include "markov.hpp"
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

void generate_markov_graph(const MarkovChain &mc, const fs::path &output_path, std::size_t highlight_index,
                           const std::string &highlight_color) {
  const std::vector<std::vector<double>> &transitionMatrix = mc.get_transition_matrix();
  const std::vector<std::string> &state_names = mc.get_state_names();
  const std::size_t &n = transitionMatrix.size();
  const std::size_t &nodesPerRow = 3;  // Number of nodes per row
  const double &verticalOffset = -1.5; // Vertical offset for the second row

  std::ofstream markov_graph_latex(output_path);
  if (!markov_graph_latex.is_open()) {
    throw std::runtime_error("Cannot open file.");
  }

  markov_graph_latex << constants::DEFAULT_LATEX_PREAMBLE;

  // Define nodes in two rows
  for (std::size_t i = 0; i < n; ++i) {
    const double &yPosition = (i / nodesPerRow) * verticalOffset; // Calculate y position based on row
    if (i == highlight_index) {
      markov_graph_latex << "    \\node[state, fill=" << highlight_color << "] (S" << i << ") at (" << (i % nodesPerRow)
                         << ", " << yPosition << ") {" << state_names[i] << "};\n";
    } else {
      markov_graph_latex << "    \\node[state] (S" << i << ") at (" << (i % nodesPerRow) << ", " << yPosition << ") {"
                         << state_names[i] << "};\n";
    }
  }

  // Define edges
  for (std::size_t i = 0; i < n; ++i) {
    for (std::size_t j = 0; j < n; ++j) {
      if (transitionMatrix[i][j] > 0) {
        if (i == j) {
          // Self-loop
          markov_graph_latex << "    \\path[->] (S" << i << ") edge[loop above] node {" << std::fixed
                             << std::setprecision(2) << transitionMatrix[i][j] << "} (S" << i << ");\n";
        } else {
          // Determine the position of the nodes
          bool isLeftNode = (i % nodesPerRow) < (j % nodesPerRow);
          bool isSameRow = (i / nodesPerRow) == (j / nodesPerRow);

          if ((j - i) % 2 == 0) {
            // Right bending edge
            markov_graph_latex << "    \\path[->] (S" << i << ") edge[bend left] node["
                               << (isSameRow ? (isLeftNode ? "above" : "below") : "above") << "] {" << std::fixed
                               << std::setprecision(2) << transitionMatrix[i][j] << "} (S" << j << ");\n";
          } else {
            // Left bending edge
            markov_graph_latex << "    \\path[->] (S" << i << ") edge[bend right] node["
                               << (isSameRow ? (isLeftNode ? "below" : "above") : "below") << "] {" << std::fixed
                               << std::setprecision(2) << transitionMatrix[i][j] << "} (S" << j << ");\n";
          }
        }
      }
    }
  }

  markov_graph_latex << "\\end{tikzpicture}\n";
  markov_graph_latex << "\\end{document}\n";
}

void generate_all_markov_graphs(const MarkovChain &mc, const fs::path &output_path) {
  const std::size_t &chain_length = mc.get_transition_matrix_size();
  for (std::size_t i = 0; i < chain_length; i++) {
    const fs::path &output_file_path = std::to_string(i) + ".tex";
    std::cout << "Generating markov graph " << output_file_path << "." << std::endl;
    generate_markov_graph(mc, output_path / output_file_path, i);
  }
}

void compile_markov_graph(const fs::path &folder_path, const fs::path &file_name,
                          const fs::path &latex_output_directory, const std::string &latex_compiler,
                          const std::string &latex_compiler_options, bool verbose) {
  std::ostringstream command;
  command << latex_compiler << " " << latex_compiler_options
          << " -interaction=nonstopmode -output-directory=" << folder_path / latex_output_directory
          << folder_path / file_name;

  execute_command(command, verbose);
}

void compile_all_markov_graphs(const fs::path &latex_folder_path, std::size_t file_count,
                               const fs::path &latex_output_directory, const std::string &latex_compiler,
                               const std::string &latex_compiler_options, bool verbose) {
  const fs::path &build_file_path = latex_folder_path / latex_output_directory;

  create_dir(build_file_path);

  for (std::size_t i = 0; i < file_count; i++) {
    std::cout << "Compiling " << fs::path(std::to_string(i) + ".tex") << std::endl;
    compile_markov_graph(latex_folder_path, std::to_string(i) + ".tex", latex_output_directory, latex_compiler,
                         latex_compiler_options, verbose);
  }
}

void convert_pdf_to_png(const fs::path &file_path, const fs::path &output_path, bool verbose) {
  // Ensure the input file exists
  if (!fs::exists(file_path)) {
    throw std::runtime_error("Input file does not exist: " + file_path.string());
  }

  // Construct the command using ostringstream
  std::ostringstream command;
  command << "magick -density 300 " << file_path << " -quality 100 -resize 200% " << output_path;

  execute_command(command, verbose);
}

void convert_all_pdfs_to_pngs(const fs::path &folder_path, std::size_t file_count, const fs::path &output_path,
                              bool verbose) {
  // Ensure the input file exists
  if (!fs::exists(folder_path)) {
    throw std::runtime_error("Input folder does not exist: " + folder_path.string());
  }

  for (std::size_t i = 0; i < file_count; i++) {
    const fs::path &input_file_path = std::to_string(i) + ".pdf";
    const fs::path &output_file_path = std::to_string(i) + ".png";
    std::cout << "Converting " << input_file_path << " to " << output_file_path << "." << std::endl;
    convert_pdf_to_png(folder_path / input_file_path, output_path / output_file_path, verbose);
  }
}
