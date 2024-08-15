#pragma once

#include <cstddef>
#include <filesystem>
#include <random>
#include <string>
#include <vector>

class MarkovChain {
public:
  MarkovChain(const std::vector<std::vector<double>> &transitionMatrix);
  MarkovChain(const std::vector<std::vector<double>> &transitionMatrix, const std::vector<std::string> &stateNames);

  MarkovChain(const std::filesystem::path &markov_file);
  MarkovChain(const std::filesystem::path &markov_file, const std::vector<std::string> &stateNames);

  // Sets the current state of the Markov Chain to the value provided.
  void setCurrentState(std::size_t state);
  // Returns the currrent state of the Markov Chain.
  std::size_t getCurrentState();
  // Causes the Markov Chain to evolve in to the next state. The probabilities are decided based on the
  // transitionMatrix.
  std::size_t nextState();

  // Returns a read-only reference to the transitionMatrix.
  const std::vector<std::vector<double>> &getTransitionMatrix() const;
  // Returns the size of the Markov Chain.
  std::size_t getTransitionMatrixSize() const;
  // Prints out the transitionMatrix with std::cout.
  void viewTransitionMatrix() const;

  // Returns a read-only reference to the stateNames.
  const std::vector<std::string> &getStateNames() const;
  // Prints out the stateNames with std::cout.
  void viewStateNames() const;

private:
  std::vector<std::vector<double>> transitionMatrix;
  std::vector<std::string> stateNames;
  std::size_t currentState;
  std::mt19937 generator;

  // Validates that the transitionMatrix follows the rules of a regular Markov Chain.
  // That is, the matrix is a square matrix, the probabilities in each row sum to 1.0, and the probabilities are
  // non-negative.
  void validateTransitionMatrix();
  // Initializes the transitionMatrix of the Markov Chain from a file. The file must have the values of each row
  // seperated by a comma. Function takes in the path to the file.
  void transitionMatrixFromFile(const std::filesystem::path &markov_file);

  // Fills up the stateNames vector with linear values between 0 and N.
  void linearStateNames();
  // Validates that the state names vector length matches the transition matrix vector length.
  void validateStateNames();
};

// Iterates the Markov Chain iterations times. Each state is written to an std::vector with values from std::size_t.
// The starting value of the Markov Chain is included in the vector. Modifies original object.
std::vector<std::size_t> iterate_markov_states(MarkovChain &mc, std::size_t iterations);
