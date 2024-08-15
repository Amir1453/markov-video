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
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

MarkovChain::MarkovChain(const std::vector<std::vector<double>> &transitionMatrix)
    : transitionMatrix(transitionMatrix), currentState(0), generator(std::random_device{}()) {
  validateTransitionMatrix();
  linearStateNames();
}

MarkovChain::MarkovChain(const std::vector<std::vector<double>> &transitionMatrix,
                         const std::vector<std::string> &stateNames)
    : transitionMatrix(transitionMatrix), stateNames(stateNames), currentState(0), generator(std::random_device{}()) {
  validateTransitionMatrix();
  validateStateNames();
}

MarkovChain::MarkovChain(const fs::path &markov_file) : currentState(0), generator(std::random_device{}()) {
  transitionMatrixFromFile(markov_file);
  validateTransitionMatrix();
  linearStateNames();
}

MarkovChain::MarkovChain(const fs::path &markov_file, const std::vector<std::string> &stateNames)
    : stateNames(stateNames), currentState(0), generator(std::random_device{}()) {
  transitionMatrixFromFile(markov_file);
  validateTransitionMatrix();
  validateStateNames();
}

void MarkovChain::validateTransitionMatrix() {
  std::size_t matrix_length = transitionMatrix.size();
  for (const auto &row : transitionMatrix) {
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
    if (fabs(sum - constants::ONE) > constants::EPSILON) {
      throw std::invalid_argument("Transition probabilities must sum to 1.");
    }
  }
}

void MarkovChain::transitionMatrixFromFile(const fs::path &markov_file) {
  std::ifstream mc_file(markov_file);
  if (!mc_file.is_open()) {
    throw std::invalid_argument("Could not read markov chain file.");
  }

  std::size_t lines = std::count(std::istreambuf_iterator<char>(mc_file), std::istreambuf_iterator<char>(), '\n');
  transitionMatrix.resize(lines);

  mc_file.clear();  // resets the pointer location and any errors.
  mc_file.seekg(0); // average cpp moment

  std::string line;
  std::size_t transition_matrix_index = 0;

  while (std::getline(mc_file, line)) {
    std::stringstream ss(line);
    std::string probability_value;
    while (std::getline(ss, probability_value, ',')) {
      transitionMatrix[transition_matrix_index].push_back(std::stod(probability_value));
    }
    transition_matrix_index++;
  }
  mc_file.close();
}

void MarkovChain::linearStateNames() {
  std::size_t mc_size = transitionMatrix.size();
  stateNames.resize(mc_size);

  for (std::size_t i = 0; i < mc_size; i++) {
    stateNames[i] = std::to_string(i);
  }
}

void MarkovChain::validateStateNames() {
  if (stateNames.size() != transitionMatrix.size()) {
    throw std::invalid_argument("State names vector length does not match with transition matrix.");
  }
}

void MarkovChain::setCurrentState(std::size_t state) {
  if (state >= transitionMatrix.size()) {
    throw std::out_of_range("State index out of range.");
  }
  currentState = state;
}

std::size_t MarkovChain::getCurrentState() { return currentState; }

std::size_t MarkovChain::nextState() {
  std::discrete_distribution<int> distribution(transitionMatrix[currentState].begin(),
                                               transitionMatrix[currentState].end());
  currentState = distribution(generator);
  return currentState;
}

const std::vector<std::vector<double>> &MarkovChain::getTransitionMatrix() const { return transitionMatrix; }

std::size_t MarkovChain::getTransitionMatrixSize() const { return transitionMatrix.size(); }

void MarkovChain::viewTransitionMatrix() const {
  for (const auto &row : transitionMatrix) {
    for (double prob : row) {
      std::cout << prob << " ";
    }
    std::cout << std::endl;
  }
}

const std::vector<std::string> &MarkovChain::getStateNames() const { return stateNames; }

void MarkovChain::viewStateNames() const {
  for (const std::string &state : stateNames) {
    std::cout << state << std::endl;
  }
}

std::vector<std::size_t> iterate_markov_states(MarkovChain &mc, std::size_t iterations) {
  std::vector<std::size_t> markov_iterations;
  markov_iterations.push_back(mc.getCurrentState());
  for (std::size_t i = 0; i < iterations; i++) {
    markov_iterations.push_back(mc.nextState());
  }
  return markov_iterations;
}
