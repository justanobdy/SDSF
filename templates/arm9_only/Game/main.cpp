#include <SDSF/Exception.hpp>
#include <iostream>
#include <nds.h>

int run() {
    // Put your code here

    while(true) {
        swiWaitForVBlank();
    }

    return 0;
}

int main() {
    // Try to run the program, but catch any exceptions that may pop up
    try {
        return run();
    }catch(const Exception& e) {
        // Print the exception if something went wrong
        consoleDemoInit();

        std::cout << e.what() << std::endl;

        while(1);
    }
}