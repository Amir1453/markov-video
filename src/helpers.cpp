#include "helpers.hpp"
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <stdexcept>

namespace fs = std::filesystem;

void create_dir(const fs::path &folder_path) {
  try {
    if (fs::create_directories(folder_path)) {
      std::cout << "Directory created: " << folder_path << std::endl;
    } else {
      std::cout << "Directory already exists: " << folder_path << ", skipped creating." << std::endl;
    }
  } catch (const fs::filesystem_error &e) {
    throw std::runtime_error("Error creating directory: " + std::string(e.what()));
  }
}

void delete_dir_or_file(const fs::path &folder_path) {
  try {
    if (fs::is_directory(folder_path)) {
      // Attempt to remove a directory and all its contents
      std::size_t removed_file_count = fs::remove_all(folder_path);
      std::cout << "Removed " << removed_file_count << " items from directory: " << folder_path << std::endl;
    } else if (fs::is_regular_file(folder_path)) {
      // Attempt to remove a single file
      if (fs::remove(folder_path)) {
        std::cout << "File removed: " << folder_path << std::endl;
      } else {
        throw std::runtime_error("Error: File could not be removed (it may not exist).");
      }
    } else {
      throw std::runtime_error("Error: Path is neither a file nor a directory.");
    }
  } catch (const fs::filesystem_error &e) {
    throw std::runtime_error("Filesystem error: " + std::string(e.what()));
  }
}

#ifdef _WIN32
void check_verbosity(std::ostringstream &command, bool verbose) {
  if (!verbose) {
    command << "> NUL 2>&1";
  }
}
#else
void check_verbosity(std::ostringstream &command, bool verbose) {
  if (!verbose) {
    command << ">& /dev/null";
  }
}
#endif

void execute_command(std::ostringstream &command, bool verbose) {
  check_verbosity(command, verbose);
  int return_code = std::system(command.str().c_str());
  if (return_code != 0) {
    throw std::runtime_error("Command execution failed: " + command.str());
  }
}

void wait_on_enter() {
  std::cout << "Please press enter after you are done." << std::endl;
  std::cin.get();
  std::cout << "Continuing.." << std::endl;
}
