#include <exception>
#include <iostream>
#include "app/App.h"

int main() {
    try {
        App app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
