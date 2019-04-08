/**
  *  Worg
  */
#include <set>
#include <map>
#include <vector>
#include <fstream>

std::ifstream fin("input.txt"); std::ofstream fout("output.txt");

class Automaton {
private:
  int n;  //  number of states
  std::vector<int > states;
  int s;  //  language size
  std::vector<char > letters;
  int starting_state;
  std::vector<int > final_states;
  std::map<std::pair<int, int >, std::vector<int > > transitions;
  std::map<int, std::vector<int > > lambda_transitions;
  std::map<int, std::vector<int > > lambda_components;

  void DFS(int node, std::set<int > &visited, int starting_node) {
    lambda_components[starting_node].push_back(node);
    visited.insert(node);

    for (auto next : lambda_transitions[node]) {
      if (visited.find(next) == visited.end()) {
        DFS(next, visited, starting_node);
      }
    }
  }

public:
  Automaton() : n(0), s(0), starting_state(0) {}


  void ReadData() {
    fin >> n; states.resize(n);
    for (auto& state : states) {
      fin >> state;
    }

    fin >> s; letters.resize(s);
    for (auto &letter : letters) {
      fin >> letter;
    }

    fin >> starting_state;

    int f; fin >> f; final_states.resize(static_cast<unsigned long>(f));  //  f = number of final states
    for (auto &final_state : final_states) {
      fin >> final_state;
    }

    int t; fin >> t;  //  t = number of transitions
    for (int i = 0; i < t; i++) {
      int state; char character; int next_state;
      fin >> state >> character >> next_state;

      if (character == '.') {
        lambda_transitions[state].push_back(next_state);
      } else {
        transitions[{state, character}].push_back(next_state);
      }
    }
  }


  void ProcessLambdaComponents() {
    //  Compute each node's lambda component @ all nodes that can be reached starting from a certain node using only
    //  lambda transitions.
    for (auto state : states) {
      lambda_components[state].push_back(state);
      std::set<int > visited;
      DFS(state, visited, state);
    }
  }


  std::string CheckWord(const std::string &word) {
    std::set<int > current_states;
    for (auto state : lambda_components[starting_state]) {
      current_states.insert(state);
    }

    for (int i = 0; i < word.size(); i++) {
      char c = word[i];

      std::set<int > next_states;
      for (auto state : current_states) {
        for (auto x : transitions[{state, c}]) {  //  x is the ending state. Check its lambda component.
          for (auto y : lambda_components[x]) {
            next_states.insert(y);
          }
        }
      }

      current_states = next_states;
    }

    std::string verdict = "NU\n";
    for (auto state : final_states) {
      if (current_states.find(state) != current_states.end()) {
        verdict = "DA\n";
      }
    }

    return verdict;
  }
};


int main() {
  Automaton a;
  a.ReadData();
  a.ProcessLambdaComponents();

  int queries; fin >> queries;
  for (int i = 0; i < queries; i++) {
    std::string word; fin >> word;
    fout << a.CheckWord(word);
  }

  return 0;
}