#include <iostream>
#include <limits>
#include <vector>
#include <set>
#include <map>

#define EPSILON ' '

typedef std::vector<char> Rule;
typedef std::set<char> Set;

void error(std::string msg, std::string prod_rule) {
    std::cerr << msg << ": " << prod_rule << std::endl;
    std::exit(1);
}

void print_rules(std::vector<Rule> rules) {
    for (auto &vec : rules) {
        std::cout << std::endl;
        for (auto &ch : vec) {
            if (std::isupper(ch)) {
                std::cout << ch << ' ';
            }
        }
    }
    std::cout << std::endl;
}

void print_first(std::map<char, Set> first) {
    std::cout << std::endl;
    for (auto it : first) {
        if (std::isupper(it.first)) {
            std::cout << "FIRST(" << it.first << ") : [";
            for (auto i: it.second) {
                std::cout << '\'' << (i) << '\'' << ',';
            }
            std::cout << ']' << std::endl;
        }
    }
}

std::vector<Rule> get_rules(size_t num_prods) {
    std::vector<Rule> rules;
    std::string prod_rule;
    while (true) {
        std::getline(std::cin, prod_rule);
        if (!std::isupper(prod_rule.front())) {
            error("Start of production rule must be Uppercase character", prod_rule);
        }
        char non_terminal = prod_rule.front();
        size_t body_idx = prod_rule.find("->", 1);
        if (body_idx == std::string::npos) {
            error("Not valid production rule", prod_rule);
        }

        std::string lhs = prod_rule.substr(1, body_idx - 1);
        for (auto &ch : lhs) {
            if (!std::isspace(ch)) {
                error("Junk after Non Terminal in LHS", prod_rule);
            }
        }

        std::string rhs = prod_rule.substr(body_idx + 2, std::string::npos);
        Rule rule;
        rule.push_back(non_terminal);
        for (auto &ch : rhs) {
            if (!std::isspace(ch)) {
                if (ch == '|') {
                    if (--num_prods == 0) {
                        break;
                    }
                    rules.push_back(rule);
                    rule.clear();
                    rule.push_back(non_terminal);
                }
                else {
                    rule.push_back(ch);
                }
            }
        }

        rules.push_back(rule);
        if (--num_prods == 0) {
            break;
        }
    }

    return rules;
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
                for (auto ch : first.at(*it)) {
                    if (ch != EPSILON) {
                        // std::cout << (non_terminal) << ch << std::endl;
                        did_change |= first.at(non_terminal).insert(ch).second;
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

int main(void) {
    std::cout << "Enter number of production rules: ";
    size_t num_prods = 0;
    std::cin >> num_prods;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::vector<Rule> rules = get_rules(num_prods);
    // print_rules(rules);

    std::map<char, Set> first = get_first(rules);
    print_first(first);
    return 0;
}
