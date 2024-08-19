// Source code of the implementation of a Markov Chain.
//
// EVA License

#include "markov.hpp"
#include "helpers.hpp"
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

MarkovChain::MarkovChain(const std::vector<std::vector<double>> &transition_matrix)
    : transition_matrix(transition_matrix), current_state(0), generator(std::random_device{}()) {
  validate_transition_matrix();
  linear_state_names();
}

MarkovChain::MarkovChain(const std::vector<std::vector<double>> &transition_matrix,
                         const std::vector<std::string> &state_names)
    : transition_matrix(transition_matrix), state_names(state_names), current_state(0),
      generator(std::random_device{}()) {
  validate_transition_matrix();
  validate_state_names();
}

MarkovChain::MarkovChain(const fs::path &markov_file) : current_state(0), generator(std::random_device{}()) {
  transition_matrix_from_file(markov_file);
  validate_transition_matrix();
  linear_state_names();
}

MarkovChain::MarkovChain(const fs::path &markov_file, const std::vector<std::string> &state_names)
    : state_names(state_names), current_state(0), generator(std::random_device{}()) {
  transition_matrix_from_file(markov_file);
  validate_transition_matrix();
  validate_state_names();
}

void MarkovChain::validate_transition_matrix() const {
  std::size_t matrix_length = transition_matrix.size();
  for (const auto &row : transition_matrix) {
    if (row.size() != matrix_length) {
      throw std::invalid_argument("Transition matrix is not a square matrix.");
    }
    double sum = constants::ZERO;
    for (double prob : row) {
      if (prob < constants::ZERO) {
        throw std::invalid_argument("Transition matrix probabilities must be non-negative.");
      }
      sum += prob;
    }
    // Have to use this due to floating point arithmetic.
    if (std::fabs(sum - constants::ONE) > constants::EPSILON) {
      throw std::invalid_argument("Transition probabilities must sum to 1.");
    }
  }
}

void MarkovChain::transition_matrix_from_file(const fs::path &markov_file) {
  std::ifstream mc_file(markov_file);
  if (!mc_file.is_open()) {
    throw std::invalid_argument("Could not read markov chain file.");
  }

  std::size_t lines = std::count(std::istreambuf_iterator<char>(mc_file), std::istreambuf_iterator<char>(), '\n');
  transition_matrix.resize(lines);

  mc_file.clear();  // resets the pointer location and any errors.
  mc_file.seekg(0); // average cpp moment

  std::string line;
  std::size_t transition_matrix_index = 0;

  while (std::getline(mc_file, line)) {
    std::stringstream ss(line);
    std::string probability_value;
    while (std::getline(ss, probability_value, ',')) {
      transition_matrix[transition_matrix_index].push_back(std::stod(probability_value));
    }
    transition_matrix_index++;
  }
  mc_file.close();
}

void MarkovChain::linear_state_names() {
  std::size_t mc_size = transition_matrix.size();
  state_names.resize(mc_size);

  for (std::size_t i = 0; i < mc_size; i++) {
    state_names[i] = std::to_string(i);
  }
}

void MarkovChain::validate_state_names() const {
  if (state_names.size() != transition_matrix.size()) {
    throw std::invalid_argument("State names vector length does not match with transition matrix.");
  }
}

void MarkovChain::set_current_state(std::size_t state) {
  if (state >= transition_matrix.size()) {
    throw std::out_of_range("State index out of range.");
  }
  current_state = state;
}

std::size_t MarkovChain::get_current_state() const { return current_state; }

std::size_t MarkovChain::next_state() {
  std::discrete_distribution<int> distribution(transition_matrix[current_state].begin(),
                                               transition_matrix[current_state].end());
  current_state = distribution(generator);
  return current_state;
}

const std::vector<std::vector<double>> &MarkovChain::get_transition_matrix() const { return transition_matrix; }

std::size_t MarkovChain::get_transition_matrix_size() const { return transition_matrix.size(); }

void MarkovChain::view_transition_matrix() const {
  for (const auto &row : transition_matrix) {
    for (double prob : row) {
      std::cout << prob << " ";
    }
    std::cout << std::endl;
  }
}

const std::vector<std::string> &MarkovChain::get_state_names() const { return state_names; }

void MarkovChain::view_state_names() const {
  for (const std::string &state : state_names) {
    std::cout << state << std::endl;
  }
}

std::vector<std::size_t> iterate_markov_states(MarkovChain &mc, std::size_t iterations) {
  std::vector<std::size_t> markov_iterations;
  markov_iterations.push_back(mc.get_current_state());
  for (std::size_t i = 0; i < iterations; i++) {
    markov_iterations.push_back(mc.next_state());
  }
  return markov_iterations;
}
