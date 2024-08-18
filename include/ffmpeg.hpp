#pragma once

#include <cstddef>
#include <filesystem>
#include <vector>

// Uses ffmpeg to overlay a PNG image to the specified video.
void overlay_image_to_video(const std::filesystem::path &video_file_path, const std::filesystem::path &image_file_path,
                            const std::filesystem::path &output_video_path, bool verbose);
// Overlays multiple images and videos using the file_count as an index.
void overlay_images_to_videos(const std::filesystem::path &videos_folder_path, const std::string &video_extension,
                              const std::filesystem::path &images_folder_path, std::size_t file_count,
                              const std::filesystem::path &outputs_folder_path, bool verbose = false);

// Takes in a vector of Markov Chain states, and creates a filelist for ffmpeg to merge the videos together. The output
// path is filelist_path.
void create_filelist(const std::vector<std::size_t> &markov_states, const std::filesystem::path &filelist_path,
                     const std::string &file_extension);
// Takes in a filelist_path, reads from the filelist and uses the ffmpeg CLI to output a merged video to the output
// location.
void combine_segments(const std::filesystem::path &filelist_path, const std::filesystem::path &output,
                      bool verbose = false);
