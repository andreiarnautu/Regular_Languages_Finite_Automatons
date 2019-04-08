/**
  *  Worg
  */
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

std::ifstream fin("input.txt");
std::ofstream fout("output.txt");

class Automaton {
public:

  std::vector<int > Q;
  std::vector<char > sigma;
  std::vector<int > final_states;
  std::map<std::pair<int, char >, int > delta;
  int initial_state;

  void SetStates(const int n) {
    Q = std::vector<int >(n);

    for (int i = 0; i < n; i++) {
      fin >> Q[i];
    }
  }

  void SetSigma(const int n) {
    sigma = std::vector<char >(n);

    for (int i = 0; i < n; i++) {
      fin >> sigma[i];
    }
  }

  void SetCurrentState(const int n) {
    initial_state = n;
  }

  void SetFinalStates(const int n) {
    final_states = std::vector<int >(n);

    for (int i = 0; i < n; i++) {
      fin >> final_states[i];
    }
  }

  void SetDelta(const int n) {
    for (int i = 0; i < n; i++) {
      int state; char character; int next_state; fin >> state >> character >> next_state;

      delta[std::make_pair(state, character)] = next_state;
    }
  }

  bool CheckCharacter(char c) {
    for (auto ch : sigma) {
      if (c == ch) {
        return true;
      }
    }

    return false;
  }

  void CheckWord(const std::string word) {
    int current_state = initial_state;

    for (int i = 0; i < word.size(); i++) {
      if (CheckCharacter(word[i]) == false) {
        fout << "NU\n"; return;
      }

      if (delta.find(std::make_pair(current_state, word[i])) == delta.end()) {
        fout << "NU\n"; return;
      }

      current_state = delta[std::make_pair(current_state, word[i])];
    }

    for (auto state : final_states) {
      if (current_state == state) {
        fout << "DA\n"; return;
      }
    }

    fout << "NU\n"; return;
  }
};

int main() {
  Automaton automaton;
  int states; fin >> states;
  automaton.SetStates(states);

  int alphabet_size; fin >> alphabet_size;
  automaton.SetSigma(alphabet_size);

  int initial_state; fin >> initial_state;
  automaton.SetCurrentState(initial_state);

  int final_states; fin >> final_states;
  automaton.SetFinalStates(final_states);

  int transitions; fin >> transitions;
  automaton.SetDelta(transitions);

  int to_check; fin >> to_check;
  for (int i = 0; i < to_check; i++) {
    std::string s; fin >> s;
    automaton.CheckWord(s);
  }

  return 0;
}