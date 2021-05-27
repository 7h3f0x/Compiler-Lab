#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <map>

#define EPSILON ' '

typedef std::vector<char> Rule;
typedef std::set<char> Set;
typedef std::pair<char, char> Index;
typedef std::map<Index, int> ParsingTable;

std::vector<Rule> grammar = {
    {'S', 'T', 'B'},
    {'B', '+', 'T', 'B'},
    {'B'},
    {'T', 'F', 'C'},
    {'C', '*', 'F', 'C'},
    {'C'},
    {'F', '(', 'S', ')'},
    {'F', 'i'}
};

void print_set(std::map<char, Set> some_set) {
    std::cout << std::endl;
    for (auto &it : some_set) {
        if (std::isupper(it.first)) {
            std::cout << "SET(" << it.first << ") : [";
            for (auto &i: it.second) {
                std::cout << '\'' << (i) << '\'' << ',';
            }
            std::cout << ']' << std::endl;
        }
    }
}

void print_tbl(ParsingTable tbl) {
    for (auto &it : tbl) {
        std::cout << it.first.first << "," << it.first.second << ":"
            << it.second << std::endl;
    }
}


std::map<char, Set> get_first(std::vector<Rule> rules) {
    std::map<char, Set> first;

    for (auto &vec : rules) {
        for (auto &ch : vec) {
            if (std::isupper(ch)) {
                first.insert({ch, {}});
            }
            else {
                first.insert({ch, {ch}});
            }
        }
    }

    bool did_change = false;
    do {
        did_change = false;
        // print_first(first);
        for (auto &rule : rules) {
            char non_terminal = rule.front();
            bool is_nullable = true;
            auto it = rule.begin() + 1;
            for (; it < rule.end() && is_nullable; ++it) {
                is_nullable = false;
                for (auto &ch : first.at(*it)) {
                    if (ch != EPSILON) {
                        // std::cout << (non_terminal) << ch << std::endl;
                        if (!did_change) {
                            did_change = first.at(non_terminal).insert(ch).second;
                        } else {
                            first.at(non_terminal).insert(ch);
                        }
                    }
                    else {
                        is_nullable = true;
                    }
                }
            }
            if (is_nullable && it == rule.end()) {
                first.at(non_terminal).insert(EPSILON);
            }
        }
    } while(did_change);

    return first;
}

std::map<char, Set> get_follow(std::vector<Rule> rules, std::map<char, Set> first) {
    std::map<char, Set> follow;
    for (auto &it : rules) {
        for (auto &ch : it) {
            follow.insert({ch, {}});
        }
    }
    follow.at(rules.begin()->front()) = {'$'};
    // print_set(follow);

    bool did_change = false;
    do {
        did_change = false;
        for (auto &rule : rules) {
            for (auto &symbol : follow.at(rule.front())) {
                did_change |= follow.at(rule.back()).insert(symbol).second;
            }
            auto rest = follow.at(rule.front());
            for (size_t idx = rule.size() - 1; idx >= 2; --idx) {
                for (auto &it : first.at(rule[idx])) {
                    if (it != EPSILON) {
                        did_change |= follow.at(rule[idx - 1]).insert(it).second;
                    } else {
                        for (auto &v : rest) {
                            did_change |= follow.at(rule[idx - 1]).insert(v).second;
                        }
                    }
                }
                rest = follow.at(rule[idx - 1]);
            }
        }
    } while (did_change);

    return follow;
}

ParsingTable get_table(std::vector<Rule> rules,
        std::map<char, Set> first, std::map<char, Set> follow) {
    ParsingTable tbl;
    for (size_t i = 0; i < rules.size(); ++i) {
        bool is_nullable = true;
        size_t idx = 1;
        for (; idx < rules[i].size() && is_nullable; ++idx) {
            is_nullable = false;
            for (auto &ch : first.at(rules[i][idx])) {
                if (ch != EPSILON) {
                    tbl.insert({{rules[i].front(), ch}, i});
                } else {
                    is_nullable = true;
                }

            }
        }
        if (is_nullable && idx == rules[i].size()) {
            for (auto &it : follow.at(rules[i].front())) {
                tbl.insert({{rules[i].front(), it}, i});
            }
        }
    }
    return tbl;
}


void parse_input(std::string input, ParsingTable tbl, std::vector<Rule> rules) {
    std::vector<char> stack = {'$', rules[0].front()};
    size_t inp_length = input.length();
    auto inp_it = input.begin();

    const size_t STACK_MAX_DISPLAY = 30;
    std::cout << std::left << std::setw(STACK_MAX_DISPLAY) 
        << "Stack" << "INPUT" << std::endl << std::endl;

    while (!stack.empty() || inp_it < input.end()) {
        if (std::isspace(*inp_it)) {
            ++inp_it;
        }

        // Display
        size_t i = 0;
        for (auto &it : stack) {
            std::cout << it;
            ++i;
        }
        for (; i < STACK_MAX_DISPLAY; ++i) {
            std::cout << ' ';
        }

        std::cout << std::setw(inp_length + 5) << input << std::endl; 

        // Logic
        char top = stack.back();
        char inp = input.front();
        if (std::isupper(top)) {
            if (tbl.find({top, inp}) != tbl.end()) {
                stack.pop_back();
                int rule_idx = tbl.at({top, inp});
                for (auto it = rules[rule_idx].end() - 1; it > rules[rule_idx].begin(); --it) {
                    stack.push_back(*it);
                }
            } else {
                std::cerr << "Can't parse further" << std::endl;
                std::exit(1);
            }
        } else {
            if (top == inp) {
                stack.pop_back();
                input = input.substr(1, std::string::npos);
            } else {
                std::cerr << "Can't parse further" << std::endl;
                std::exit(1);
            }
        }
    }
    if (stack.empty() && inp_it == input.end()) {
        std::cout << std::endl << "Successfully parsed" << std::endl;
    }
}

int main(void) {
    std::map<char, Set> first = get_first(grammar);
    // print_set(first);

    std::map<char, Set> follow = get_follow(grammar, first);
    // print_set(follow);

    ParsingTable tbl = get_table(grammar, first, follow);
    // print_tbl(tbl);

    std::string input;
    std::getline(std::cin, input);

    input.push_back('$');
    parse_input(input, tbl, grammar);

    return 0;
}

