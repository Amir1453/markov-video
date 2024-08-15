#include "ffmpeg.hpp"
#include "helpers.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

void overlay_image_to_video(const fs::path &video_path, const fs::path &image_path, const fs::path &output_path,
                            const bool &verbose) {
  std::ostringstream command;
  command << "ffmpeg -y -i \"" << video_path << "\" -i \"" << image_path << "\" -filter_complex \"overlay=10:10\" \""
          << output_path << "\"";

  check_verbosity(command, verbose);

  // Execute the command
  std ::system(command.str().c_str());
}

void overlay_images_to_videos(const fs::path &videos_path, const fs::path &images_path, const std::size_t &file_count,
                              const fs::path &outputs_path, const bool &verbose) {
  // Ensure the input file exists
  if (!fs::exists(videos_path)) {
    throw std::runtime_error("Input videos folder does not exist: " + videos_path.string());
  }
  // Ensure the input file exists
  if (!fs::exists(images_path)) {
    throw std::runtime_error("Input images folder does not exist: " + images_path.string());
  }

  create_dir(outputs_path);

  for (std::size_t i = 0; i < file_count; i++) {
    const fs::path &video_file_path = std::to_string(i) + ".mp4";
    const fs::path &image_file_path = std::to_string(i) + ".png";
    const fs::path &output_file_path = std::to_string(i) + std::string(constants::DEFAULT_VIDEO_OVERLAY_NAME) + ".mp4";
    std::cout << "Overlaying " << images_path / image_file_path << " to " << videos_path / video_file_path << "."
              << std::endl;
    overlay_image_to_video(videos_path / video_file_path, images_path / image_file_path,
                           outputs_path / output_file_path, verbose);
  }
}

void create_filelist(const std::vector<std::size_t> &markov_states, const fs::path &filelist_path) {
  std::ofstream filelist(filelist_path);

  if (filelist.is_open()) {
    std::cout << "Creating filelist " << filelist_path << "." << std::endl;
    for (std::size_t state : markov_states) {
      filelist << "file '" << state << constants::DEFAULT_VIDEO_OVERLAY_NAME << ".mp4'" << std::endl;
    }
    filelist.close();
  } else {
    throw std::runtime_error("Error opening filelist.");
  }
}

void combine_segments(const fs::path &filelist_path, const fs::path &output, const bool &verbose) {
  // Construct the ffmpeg command to combine videos side by side
  std::ostringstream command;
  command << "ffmpeg -y -f concat -safe 0 -i \"" << filelist_path << "\" -c copy \"" << output << "\"";

  check_verbosity(command, verbose);

  std::cout << "Combining segments." << std::endl;
  // Execute the command
  int result = std::system(command.str().c_str());
  if (result != 0) {
    std::cerr << "Error combining videos." << std::endl;
  }
}
