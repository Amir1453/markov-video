#pragma once

#include "markov.hpp"
#include <filesystem>
#include <string>

// Generates a latex file in the latex_file_output_path based on the Markov Chain provided.
void generate_markov_graph(const MarkovChain &mc, const std::filesystem::path &latex_file_output_path,
                           const std::size_t &highlight_index, const std::string &highlight_color = "orange");
// Generates multiple latex files in the specified path with each of the files highlighting a single node.
void generate_all_markov_graphs(const MarkovChain &mc, const std::filesystem::path &latex_files_output_folder);

// Compiles the Markov Graph using the specified latex compiler.
void compile_markov_graph(const std::filesystem::path &folder_path, const std::filesystem::path &file_name,
                          const std::filesystem::path &latex_output_directory, const std::string &latex_compiler,
                          const std::string &latex_compiler_options, const bool &verbose);
// Compiles the Markov Graphs in a specified folder.
void compile_all_markov_graphs(const std::filesystem::path &latex_folder_path, const std::size_t &file_count,
                               const std::filesystem::path &latex_output_directory, const std::string &latex_compiler,
                               const std::string &latex_compiler_options, const bool &verbose = false);
// Converts the specified PDF into a PNG using ImageMagick.
void convert_pdf_to_png(const std::filesystem::path &pdf_file_path, const std::filesystem::path &output_png_path,
                        const bool &verbose);
// Converts the PDFs in the specified folder using an index up to file_count to PNGs in the output_path.
void convert_all_pdfs_to_pngs(const std::filesystem::path &folder_path, const std::size_t &file_count,
                              const std::filesystem::path &output_path, const bool &verbose = false);
