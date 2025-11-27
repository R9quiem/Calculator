#include "Parser.h"
#include <cctype> // для std::isdigit, std::isspace

std::string Parser::parse_token() {
    while (it != end && std::isspace(static_cast<unsigned char>(*it)))
        ++it;

    if (it == end) // если строка закончилась
        return {};

    // число
    if (std::isdigit(static_cast<unsigned char>(*it))) {
        std::string number;
        while (it != end && (std::isdigit(static_cast<unsigned char>(*it)) || *it == '.'))
            number.push_back(*it++);
        return number;
    }

    // скобки
    if (*it == '(' || *it == ')') {
        char c = *it++;
        return std::string(1, c);
    }

    // операторы и имена функций (sym из PluginRegistry)
    const auto tokens = r.get_all_sym();
    for (const auto& t : tokens) {
        unsigned int remain = static_cast<unsigned int>(std::distance(it, end));
        if (remain >= t.size() && std::string_view(&*it, t.size()) == std::string_view(t)) {
            it += t.size();
            return t;
        }
    }

    // ничего не узнали – возвращаем пустую строку
    return "";
}

Expression Parser::parse_simple_expression() {
    auto token = parse_token();
    if (token.empty())
        throw std::runtime_error("Invalid input");

    // скобочное подвыражение
    if (token == "(") {
        auto result = parse();
        if (parse_token() != ")")
            throw std::runtime_error("Expected ')'");
        return result;
    }

    // число
    if (std::isdigit(static_cast<unsigned char>(token[0]))) {
        return Expression(std::move(token));
    }

    // унарный минус
    if (token == "-") {
        Expression arg = parse_simple_expression();
        return Expression("u-", std::move(arg)); // отдельный символ для унарного минуса
    }

    // остальные унарные операторы/функции: sin, cos, sqrt и пр.
    // ВАЖНО: здесь в качестве аргумента берётся ТОЛЬКО одно "простое" выражение:
    //   sin 3.14 / 2  => (sin 3.14) / 2
    //   sin(3.14/2)   => sin(3.14 / 2)
    return Expression(std::move(token), parse_simple_expression());
}

Expression Parser::parse_expression(int min_priority) {
    auto left_expr = parse_simple_expression();

    for (;;) {
        auto op = parse_token();  // <-- убрал дубликат

        if (op.empty()) {
            return left_expr;
        }

        if (op == ")") {
            // вернуть ')' назад, пусть разберётся внешний уровень (parse_simple_expression)
            it -= op.size();
            return left_expr;
        }

        int p = r.priority_of(op);

        if (p <= min_priority) {
            // оператор слабее текущего порога — откатываем и выходим
            it -= op.size();
            return left_expr;
        }

        int next_min = r.is_right_assoc(op) ? (p - 1) : p;
        auto right_expr = parse_expression(next_min);
        left_expr = Expression(std::move(op), std::move(left_expr), std::move(right_expr));
    }
}

Expression Parser::parse() {
    return parse_expression(0);
}

double eval_with_plugins(const Expression& e, const PluginRegistry& R) {
    if (e.args.size() == 2) {
        double a = eval_with_plugins(e.args[0], R);
        double b = eval_with_plugins(e.args[1], R);

        const Plugin& P = R.at(e.token);
        const std::vector<double> args = { a, b };
        return P.call(args);
    }

    if (e.args.size() == 1) {
        double x = eval_with_plugins(e.args[0], R);

        const Plugin& P = R.at(e.token);
        const std::vector<double> args = { x };
        return P.call(args);
    }

    // число
    return std::stod(e.token.c_str(), nullptr);
}
