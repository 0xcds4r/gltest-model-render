#include "Application.h"
#include <iostream>

Application* app = nullptr;

int main() {
    app = new Application();
    app->run();
    return 0;
}
