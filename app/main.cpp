#include "Plugin.h"
#include "PluginRegistry.h"
#include "Parser.h"
#include "Loader.h"

#include <iostream>

int main() {

    SetConsoleOutputCP(CP_UTF8);
    PluginRegistry reg;
    load_plugins(reg);
    
    std::cout << "Калькулятор запущен. Введите выражение или 'exit' для выхода.\n";

    for (;;) {
        std::cout << "\n> ";
        std::string input;
        std::getline(std::cin, input);
        if (input == "exit" || input == "quit")
            break;
        if (input.empty())
            continue;

        try {
            Parser parser(input.begin(), input.end(), reg);
            Expression expr = parser.parse();
            double result = eval_with_plugins(expr, reg);
            std::cout << "= " << result << "\n";
        } catch (const std::exception& ex) {
            std::cerr << "Ошибка: " << ex.what() << "\n";
        }
    }

    std::cout << "Выход из программы.\n";
    return 0;
}
