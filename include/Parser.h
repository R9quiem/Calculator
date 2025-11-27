#pragma once

#include <string>
#include <vector>

#include "Plugin.h" 
#include "PluginRegistry.h"

struct Expression {
    std::string token;
    std::vector<Expression> args;

    Expression(std::string t) : token(std::move(t)) {}
    Expression(std::string t, Expression a) : token(std::move(t)), args{ std::move(a) } {}
    Expression(std::string t, Expression a, Expression b) : token(std::move(t)), args{ std::move(a), std::move(b) } {}
};

class Parser {
    std::string parse_token();
    Expression parse_simple_expression();
    Expression parse_expression(int min_priority);

    std::string::const_iterator it, end;

    const PluginRegistry& r;
public:
    explicit Parser(std::string::const_iterator begin,
                    std::string::const_iterator end,
                    const PluginRegistry &reg    
                )
        : it(begin), end(end), r(reg) {}
    Expression parse();

};

double eval_with_plugins(const Expression& e, const PluginRegistry& R);
