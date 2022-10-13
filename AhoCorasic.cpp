//
// Created by kirill on 13.10.22.
//

#include "AhoCorasic.h"

void AhoCorasic::add(const std::string &word) {
  trie_was_changed = true;
  if (trie.empty())
    trie.emplace_back();

  int v = 0;
  for (char i : word) {
    if (trie[v].to[i] == -1) {
      trie[v].to[i] = static_cast<int>(trie.size());
      trie.emplace_back();
    }
    v = trie[v].to[i];
  }
  trie[v].term = true;
}

void AhoCorasic::FillLinkAndGo() {
  link.resize(trie.size());
  go.resize(trie.size());
  cnt.resize(trie.size());
  for (auto &i : go)
    i.resize(SIGMA);

  {
    if (!trie.empty())
      link[0] = 0;
    for (int c = 0; c < SIGMA; ++c) {
      go[0][c] = trie[0].to[c] != -1 ? trie[0].to[c] : 0;
    }
  }

  {
    std::queue<int> q;
    q.push(0);
    while (!q.empty()) {
      int v = q.front();
      q.pop();

      for (int c = 0; c < SIGMA; ++c) {
        int u = trie[v].to[c];
        if (-1 == u)
          continue;

        link[u] = v == 0 ? 0 : go[link[v]][c];

        for (int d = 0; d < SIGMA; ++d)
          go[u][d] = trie[u].to[d] != -1 ? trie[u].to[d] : go[link[u]][d];

        q.push(u);
      }

      cnt[v] = (trie[v].term ? 1 : 0) + cnt[link[v]];
    }
  }
}

int AhoCorasic::run(const std::string &text) {
  int count_occurrences = 0;
  if (trie_was_changed) {
    FillLinkAndGo();
    trie_was_changed = false;
  }

  std::vector<int> suf(text.size() + 1, 0);
  for (int i = 1; i < suf.size(); ++i) {
    if (text[i - 1] < 0) {
      continue;
    }
    suf[i] = go[suf[i - 1]][text[i - 1]];
    count_occurrences += cnt[suf[i]];
  }

  return count_occurrences;
}
