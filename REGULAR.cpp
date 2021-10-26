#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <map>
#include <set>
#include <cstring>
#include <cassert>
#include <stack>

using vectorll = std::vector<long long>;
using vectorint = std::vector<int>;
using ll = long long;


struct DKA {
    std::set<char>sigma;
    struct Node {
        bool term;
        std::map<char, ll>to;
        Node() {
            term = false;
        }
    };
    std::vector<Node>t;
    DKA(std::set<char>st = { 'a', 'b', 'c' }) {
        t.push_back(Node());
        t[0].term = true;
        sigma = st;
    }
    DKA(char a, std::set<char> st = { 'a', 'b', 'c' }) {
        if (a != '1') {
            t.push_back(Node());
            t.push_back(Node());
            t[0].to[a] = 1;
            t[1].term = true;
        }
        else {
            t.push_back(Node());
            t[0].term = true;
        }
        sigma = st;
    }
    DKA& operator +=(DKA& avt) {
        if (avt.t.size() == 1) {
            t[0].term = true;
            return *this;
        }
        if (t.size() == 1) {
            *this = avt;
            t[0].term = true;
            return *this;
        }


        ll shift1 = 1, shift2 = ll(1) + t.size();

        std::map<ll, Node>newQ;
        newQ[0];
        if (t[0].term || avt.t[0].term) {
            newQ[0].term = true;
        }


        bool first_iteration = true;

        std::queue<std::set<ll>>stack;
        stack.push(std::set<ll>{shift1, shift2});
        std::map<std::set<ll>, ll>versh;
        versh[std::set<ll>{0}] = 0;

        int co = 1;


        while (!stack.empty()) {
            auto top = stack.front();
            stack.pop();
            for (auto c : sigma) {
                int z1 = 0;
                std::set<ll>temp1;
                for (auto i : top) {
                    if (i < shift2) {
                        if (t[i - shift1].to.find(c) != t[i - shift1].to.end()) {
                            temp1.insert(t[i - shift1].to[c] + shift1);
                            if (t[t[i - shift1].to[c]].term)z1 = 1;
                        }
                    }
                    else {
                        if (avt.t[i - shift2].to.find(c) != avt.t[i - shift2].to.end()) {
                            temp1.insert(avt.t[i - shift2].to[c] + shift2);
                            if (avt.t[avt.t[i - shift2].to[c]].term)z1 = 1;
                        }
                    }
                }
                if (temp1.size() > 0) {
                    if (versh.find(temp1) == versh.end()) {
                        stack.push(temp1);
                        versh[temp1] = co;
                        newQ[co];
                        if (z1) {
                            newQ[co].term = true;
                        }
                        ++co;
                    }
                    if (first_iteration) {
                        newQ[0].to[c] = versh[temp1];
                        first_iteration = false;
                    }
                    else newQ[versh[top]].to[c] = versh[temp1];
                }
            }

        }

        t.clear();
        for (auto i : newQ) {
            t.push_back(i.second);
            //t[i.first] = i.second;
        }



        /*
        std::cout << '\n' << '\n' << '\n';
        for (int i = 0; i < t.size(); ++i) {
            std::cout << i << "  " << t[i].term << ": ";
            for (auto j : t[i].to) {
                std::cout << j.first << " " << j.second << ", ";
            }
            std::cout << '\n';
        }
        std::cout << '\n' << '\n' << '\n';
        */


        return *this;
    }


    DKA& operator *=(DKA& avt) {
        if (avt.t.size() == 1) {
            return *this;
        }
        if (t.size() == 1) {
            *this = avt;
            return *this;
        }
        ll shift1 = 0, shift2 = t.size();


        std::map<ll, Node>newQ;
        newQ[0];
        if (t[0].term) {
            newQ[0].term = true;
        }

        std::queue<std::set<ll>>stack;
        stack.push(std::set<ll>{0});
        std::map<std::set<ll>, ll>versh;
        versh[std::set<ll>{0}] = 0;

        int co = 1;


        while (!stack.empty()) {
            auto top = stack.front();
            stack.pop();
            for (auto c : sigma) {
                int z1 = 0;
                std::set<ll>temp1;
                for (auto i : top) {
                    if (i < shift2) {
                        if (t[i - shift1].to.find(c) != t[i - shift1].to.end()) {
                            temp1.insert(t[i - shift1].to[c] + shift1);
                        }
                        if (t[i - shift1].term && avt.t[0].to.find(c) != avt.t[0].to.end()) {
                            temp1.insert(avt.t[0].to[c] + shift2);
                            if (avt.t[avt.t[0].to[c]].term)z1 = 1;
                        }
                    }
                    else {
                        if (avt.t[i - shift2].to.find(c) != avt.t[i - shift2].to.end()) {
                            temp1.insert(avt.t[i - shift2].to[c] + shift2);
                            if (avt.t[avt.t[i - shift2].to[c]].term)z1 = 1;
                        }
                    }
                }
                if (temp1.size() > 0) {
                    if (versh.find(temp1) == versh.end()) {
                        stack.push(temp1);
                        versh[temp1] = co;
                        newQ[co];
                        if (z1) {
                            newQ[co].term = true;
                        }
                        ++co;
                    }
                    newQ[versh[top]].to[c] = versh[temp1];
                }
            }

        }

        t.clear();
        for (auto i : newQ) {
            t.push_back(i.second);
            //t[i.first] = i.second;
        }


        /*
        std::cout << '\n' << '\n' << '\n';
        for (int i = 0; i < t.size(); ++i) {
            std::cout << i << "  " << t[i].term << ": ";
            for (auto j : t[i].to) {
                std::cout << j.first << " " << j.second << ", ";
            }
            std::cout << '\n';
        }
        std::cout << '\n' << '\n' << '\n';
        */


        return *this;



    }

    DKA& Klini() {

        if (t.size() == 1)return *this;

        std::vector<Node>tt;
        tt.push_back(Node());
        tt[0].term = true;
        for (int i = 0; i < t.size(); ++i) {
            for (auto j = t[i].to.begin(); j != t[i].to.end(); ++j) {
                ++((*j).second);
            }
            tt.push_back(t[i]);
        }
        tt[0].to = tt[1].to;
        t = tt;


        std::map<ll, Node>newQ;
        newQ[0];
        newQ[0].term = true;

        bool first_iteration = true;

        std::queue<std::set<ll>>stack;
        stack.push(std::set<ll>{0});
        std::map<std::set<ll>, ll>versh;
        versh[std::set<ll>{0}] = 0;

        int co = 1;

        while (!stack.empty()) {
            auto top = stack.front();
            stack.pop();
            for (auto c : sigma) {
                int z1 = 0;
                std::set<ll>temp1;
                for (auto i : top) {
                    if (t[i].to.find(c) != t[i].to.end()) {
                        temp1.insert(t[i].to[c]);
                        if (t[t[i].to[c]].term)z1 = 1;
                    }
                    if (t[i].term && t[0].to.find(c) != t[0].to.end()) {
                        temp1.insert(t[0].to[c]);
                        if (t[t[0].to[c]].term)z1 = 1;
                    }

                }
                if (temp1.size() > 0) {
                    if (versh.find(temp1) == versh.end()) {
                        stack.push(temp1);
                        versh[temp1] = co;
                        newQ[co];
                        if (z1) {
                            newQ[co].term = true;
                        }
                        ++co;
                    }
                    newQ[versh[top]].to[c] = versh[temp1];
                }
            }

        }
        t.clear();
        for (auto i : newQ) {
            t.push_back(i.second);
            //t[i.first] = i.second;
        }


        /*
        std::cout << '\n' << '\n' << '\n';
        for (int i = 0; i < t.size(); ++i) {
            std::cout << i << "  " << t[i].term << ": ";
            for (auto j : t[i].to) {
                std::cout << j.first << " " << j.second << ", ";
            }
            std::cout << '\n';
        }
        std::cout << '\n' << '\n' << '\n';
        */


        return *this;



    }

};

DKA convert(std::string s, std::set<char> sigma = { 'a','b','c' }) {
    try {
        if (s.length() == 0)throw 1;
    }
    catch (int x) {
        if (x == 1)std::wcout << "Empty string!" << '/n';
    }
    std::stack<DKA>q;
    for (auto i : s) {

        if (i == '.') {
            try {
                if (q.size() < 2)throw - 1;
                DKA x = q.top();
                q.pop();
                DKA y = q.top();
                q.pop();
                y *= x;
                q.push(y);
            }
            catch (int x) {
                if (x == -1)std::wcout << "Wrong format!" << '/n';
            }
        }
        else if (i == '+') {
            try {
                if (q.size() < 2)throw - 1;
                DKA x = q.top();
                q.pop();
                DKA y = q.top();
                q.pop();
                y += x;
                q.push(y);
            }
            catch (int x) {
                if (x == -1)std::wcout << "Wrong format!" << '/n';
            }
        }
        else if (i == '*') {
            try {
                if (q.size() == 0)throw - 1;
                DKA y = q.top();
                q.pop();
                y.Klini();
                q.push(y);
            }
            catch (int x) {
                if (x == -1)std::cout << "Wrong format!" << '/n';
            }
        }
        else {
            try {
                if (sigma.find(i) == sigma.end()) {
                    throw 0;
                }
                q.push(DKA(i));
            }
            catch (int x) {
                if (x == 0)std::cout << "Wrong symbol!" << '/n';
            }
        }
    }
    return q.top();
}

bool is_in(const std::string& s, char c, ll x) {
    DKA kek = convert(s);
    /*
    for (int i = 0; i < kek.t.size();++i) {
        std::cout << i << ": ";
        for (auto j : kek.t[i].to) {
            std::cout << j.first << " " << j.second << ", ";
        }
        std::cout << '\n';
    }
    */

    std::cout << '\n' << "This code for '" << s << "' .dot:";
    std::cout << '\n' << '\n' << "digraph G{" << '\n';
    for (int i = 0; i < kek.t.size(); ++i) {
        if (kek.t[i].term) {
            std::cout << i << "[style = filled];" << '\n';
        }
        for (auto j : kek.t[i].to) {
            std::cout << i << " -> ";
            std::cout << j.second << " [label=" << j.first << "];" << '\n';
        }

    }
    std::cout << "}" << '\n' << '\n';

    for (auto i : kek.t) {
        ll co = x;
        auto j = i;
        while ((j.to.find(c) != j.to.end()) && (co > 0)) {
            --co;
            j = kek.t[j.to[c]];
        }
        if (co == 0)return true;
    }
    return false;
}


void testing() {
    assert(is_in("acb..bab.c.*.ab.ba.+.+*a.", 'b', 2));
    assert(!is_in("ab+c.aba.*.bac.+.+*", 'a', 4));
    assert(!is_in("aa.a.", 'a', 4));
    assert(is_in("aa.a.", 'a', 3));
    assert(is_in("aa.a.bb.b..", 'b', 3));
    assert(is_in("a*", 'a', 100));
}




int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string s;
    char c;
    ll x;

    testing();

    std::cin >> s >> c >> x;
    std::cout << '\n' << '\n' << ((is_in(s, c, x)) ? "YES" : "NO");

}



/*
README
Был реализован ДКА по регулярке. С помощью прохода по польской записи был построен автомат, реализованы функции конкатенации (*=),
объединения (+=) и итерации Клини (Klini) автомата. Функция convert конвертирует регулярку в автомат. Функция is_in дает ответ на 24 
задачу. Функция testing содержит тесты к задаче, включая тесты ил условия. Чтобы получить ответ тербуется просто ввести с клавиатуры 
строку-регулярку, символ и число. Также к каждой регулярке будет выведен рабочий код на .dot, терминальные вершины окрашены в серый.
Kek.
*/
