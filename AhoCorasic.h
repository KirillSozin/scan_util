//
// Created by kirill on 13.10.22.
//
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <queue>

#define SIGMA 256

class AhoCorasic {
 private:
  struct node {
    int to[SIGMA];
    bool term;
    node() : to() {
      memset(to, -1, SIGMA * sizeof(int));
      term = false;
    }
  };
  void FillLinkAndGo();

 public:
  AhoCorasic() : trie(), link(), go(), cnt(), trie_was_changed(true) {}
  AhoCorasic(const AhoCorasic &other) = delete;
  ~AhoCorasic() = default;

  void add(const std::string &word);
  int run(const std::string &text);
 private:
  std::vector<node> trie;
  std::vector<int> link;
  std::vector<std::vector<int>> go;
  std::vector<int> cnt;
  bool trie_was_changed;
};
