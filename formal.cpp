#include <gtest/gtest.h>
#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using vectorll = std::vector<long long>;
using vectorint = std::vector<int>;
using ll = long long;

bool is_high_letter(char c) { return !(c > 'Z' || 'A' > c); }

struct Situation {
  std::string rule;
  ll pos;
  ll ii;
  ll jj;
  char name;
  Situation(const std::string &s = "", ll ps = 0, ll i = 0, ll j = 0,
            char st = '\0')
      : rule(s), pos(ps), ii(i), jj(j), name(st) {}
  Situation(const Situation &s)
      : rule(s.rule), pos(s.pos), ii(s.ii), jj(s.jj), name(s.name) {}
  Situation &operator=(const Situation &s) {
    rule = s.rule;
    pos = s.pos;
    ii = s.ii;
    jj = s.jj;
    name = s.name;
    return *this;
  }
};

bool operator==(const Situation &v, const Situation &s) {
  return (v.rule == s.rule && v.pos == s.pos && v.ii == s.ii && v.jj == s.jj &&
          v.name == s.name);
}
bool operator!=(const Situation &v, const Situation &s) { return !(v == s); }
bool operator<=(const Situation &v, const Situation &s) {
  if (v.rule < s.rule)
    return true;
  if (v.rule > s.rule)
    return false;
  if (v.pos < s.pos)
    return true;
  if (v.pos > s.pos)
    return false;
  if (v.ii < s.ii)
    return true;
  if (v.ii > s.ii)
    return false;
  if (v.jj < s.jj)
    return true;
  if (v.jj > s.jj)
    return false;
  if (v.name < s.name)
    return true;
  if (v.name > s.name)
    return false;
  return true;
}
bool operator<(const Situation &v, const Situation &s) {
  return (v <= s) && (v != s);
}
bool operator>=(const Situation &v, const Situation &s) { return (s <= v); }
bool operator>(const Situation &v, const Situation &s) {
  return (v >= s) && (v != s);
}

struct KS {
  std::set<char> sigma;
  std::set<char> SIGMA;
  std::map<char, std::unordered_set<std::string>> t;

  void add_rule(char c, const std::string &s = "1") {

    try {
      if (s.length() == 0)
        throw 1;
      if (c > 'Z' || 'A' > c)
        throw 2;
      for (auto i : s) {
        if ((i > 'Z' || 'A' > i) && (sigma.find(i) == sigma.end()) &&
            (i != '1')) {
          throw 1;
        }
      }
    } catch (int x) {
      if (x == 1) {
        std::wcout << "Wrong right part!" << '\n';
      }
      if (x == 2) {
        std::wcout << "Wrong left part!" << '\n';
      }
      return;
    }
    SIGMA.insert(c);
    t[c].insert(s);
  }
  ll get_size() { return t.size(); }
  KS(const std::vector<std::pair<char, std::string>> rl = {},
     const std::set<char> &st = {'a', 'b', 'c'}) {
    t['S'];
    for (auto i : rl) {
      add_rule(i.first, i.second);
    }
    sigma = st;
  }

  bool Homsk() {
    for (auto i : t) {
      for (auto j : i.second) {
        if (j.length() == 2) {
          if (!is_high_letter(j[0]) || !is_high_letter(j[1]) || (j[0] == 'S') ||
              (j[1] == 'S'))
            return false;
        } else if (j.length() == 1) {
          if ((j[0] == '1') && (i.first == 'S')) {
            continue;
          } else if (sigma.find(j[0]) == sigma.end()) {
            return false;
          }
        } else
          return false;
      }
    }
    return true;
  }

  bool CYK(const std::string &str) {
    try {
      if (!Homsk())
        throw 1;
    } catch (int x) {
      if (x == 1)
        std::wcout << "KS isn`t in normal form" << '\n';
    }
    ll len = str.length();
    std::unordered_map<char, std::vector<std::vector<ll>>> MTR;
    std::vector<char> neterm;
    for (auto i : t) {
      std::vector<std::vector<ll>> temp(str.length(),
                                        std::vector<ll>(str.length(), 0));
      for (auto j : i.second) {
        if ((j.length() == 1) && (j[0] != '1')) {
          char c = j[0];
          for (ll h = 0; h < len; ++h)
            if (str[h] == c)
              temp[h][h] = 1;
        }
      }
      MTR[i.first] = temp;
      neterm.push_back(i.first);
    }

    for (ll l = 1; l < len; ++l) {
      for (ll i = 0; i < len - l; ++i) {
        for (auto A : neterm) {
          for (auto BC : t[A]) {
            if (BC.length() == 2) {
              MTR[A][i][i + l] = 0;
              for (ll k = i; k <= i + l - 1; ++k) {
                MTR[A][i][i + l] +=
                    (MTR[BC[0]][i][k] * MTR[BC[1]][k + 1][i + l]);
              }
            }
          }
        }
      }
    }

    return MTR['S'][0][len - 1];
  }

  void predict(ll k, std::map<char, std::set<Situation>> *SET) {
    bool flag = true;
    while (flag) {
      flag = false;
      for (auto C : SIGMA) {
        if (!SET[k][C].empty()) {
          for (auto i : t[C]) {
            Situation sit(i, 0, k, k, C);
            if (SET[k][i[0]].find(sit) == SET[k][i[0]].end()) {
              flag = true;
              SET[k][i[0]].insert(sit);
            }
          }
        }
      }
    }
  }

  void scan(ll k, char C, std::map<char, std::set<Situation>> *SET) {
    for (auto i : SET[k][C]) {
      Situation sit(i);
      ++i.pos;
      ++i.jj;
      if (i.pos == i.rule.length()) {
        SET[i.jj]['1'].insert(sit);
      } else {
        SET[i.jj][i.rule[i.pos]].insert(sit);
      }
    }
  }

  void complete(ll k, std::map<char, std::set<Situation>> *SET) {
    std::set<Situation> used(SET[k]['1']);
    while (!used.empty()) {
      std::set<Situation> used1;
      for (auto i : used) {
        if (i.name == '\0')
          continue;
        for (auto j : SET[i.ii][i.name]) {
          Situation sit(j.rule, 1 + j.pos, j.ii, k, j.name);
          if ((sit.pos != sit.rule.length()) && (sit.rule[sit.pos] != '1')) {
            SET[k][sit.rule[sit.pos]].insert(sit);
          } else if (SET[k]['1'].find(sit) == SET[k]['1'].end() &&
                     used.find(sit) == used.end() &&
                     used1.find(sit) == used1.end()) {
            used1.insert(sit);
          }
        }
      }
      for (auto i : used)
        SET[k]['1'].insert(i);
      used.clear();
      for (auto i : used1)
        used.insert(i);
    }
  }

  bool ERLY(const std::string &str) {
    try {
      if (str.length() > 99)
        throw 1;
      for (ll i = 0; i < str.length(); ++i) {
        if (sigma.find(str[i]) == sigma.end())
          throw 2;
      }
    } catch (int x) {
      if (x == 1)
        std::wcout << "Word limit exceeded!";
      else if (x == 2)
        std::wcout << "Wrong word!";
    }

    std::map<char, std::set<Situation>> *SET =
        new std::map<char, std::set<Situation>>[str.length() + 1];
    SET[0]['S'].insert(Situation("S", 0, 0, 0, '\0'));
    for (ll i = 0; i <= str.length(); ++i) {
      if (i > 0) {
        scan(i - 1, str[i - 1], SET);
      }
      bool flag = true;
      while (flag) {
        flag = false;
        auto sz = SET[i].size();
        predict(i, SET);
        complete(i, SET);
        if (SET[i].size() != sz)
          flag = true;
      }
    }
    return (
        SET[str.length()]['1'].find(Situation("S", 1, 0, str.length(), '\0')) !=
        SET[str.length()]['1'].end());
  }
};

void print_in_console(KS &nka) {

  std::cout << '\n';
  for (auto i : nka.t) {
    for (auto j : i.second) {
      std::cout << i.first << "->" << j << '\n';
    }
  }
  if (nka.Homsk())
    std::cout << "KS is in normal form" << '\n';
  else
    std::cout << "KS isn`t in normal form" << '\n';
}

TEST(TestCaseCYK, CYK) {

  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);
  std::cout.tie(nullptr);

  KS ks0;
  ks0.add_rule('S', "1");
  ks0.add_rule('S', "BB");
  ks0.add_rule('S', "CD");
  ks0.add_rule('B', "BB");
  ks0.add_rule('B', "CD");
  ks0.add_rule('C', "a");
  ks0.add_rule('D', "BE");
  ks0.add_rule('E', "b");
  ks0.add_rule('D', "b");
  print_in_console(ks0);

  EXPECT_TRUE(!ks0.CYK("aabbabaababb"));
  EXPECT_TRUE(ks0.CYK("ab"));
  EXPECT_TRUE(!ks0.CYK("aaaabbb"));
  EXPECT_TRUE(ks0.CYK("aabb"));
  EXPECT_TRUE(!ks0.CYK("abaaaaa"));
}

TEST(TestCaseEARLEY, EARLEY) {

  KS ks;
  ks.add_rule('S', "1");
  ks.add_rule('S', "aSbS");
  print_in_console(ks);

  EXPECT_TRUE(ks.ERLY("aabbabaababb"));
  EXPECT_TRUE(!ks.ERLY("abbb"));
  EXPECT_TRUE(ks.ERLY("aaaabbbb"));
  EXPECT_TRUE(!ks.ERLY("bbb"));
  EXPECT_TRUE(!ks.ERLY("abaaaaa"));

  KS ks0;
  ks0.add_rule('S', "1");
  ks0.add_rule('S', "BB");
  ks0.add_rule('S', "CD");
  ks0.add_rule('B', "BB");
  ks0.add_rule('B', "CD");
  ks0.add_rule('C', "a");
  ks0.add_rule('D', "BE");
  ks0.add_rule('E', "b");
  ks0.add_rule('D', "b");
  print_in_console(ks0);

  EXPECT_TRUE(ks0.ERLY("aabb"));
  EXPECT_TRUE(!ks0.ERLY("abbb"));
  EXPECT_TRUE(!ks0.ERLY("bb"));
  EXPECT_TRUE(!ks0.ERLY("bbaa"));
  EXPECT_TRUE(!ks0.ERLY("abb"));
  EXPECT_TRUE(ks0.ERLY("abab"));
  EXPECT_TRUE(!ks0.ERLY("b"));
}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
README
Реализован алгоритм Эрли, действующий как метод от грамматики и выдающий true,
если последовательность парсится грамматикой и false иначе. Также была
релизована вспомогательная структура Situation, описывающая ситуации в дереве
разбора. Пример - ПСП. Сделал gtests. Kek.
*/
