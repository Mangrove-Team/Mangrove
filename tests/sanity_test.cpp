// Placeholder test: checks that the CI pipeline builds and runs tests end to
// end. ctest only looks at the exit code: 0 = pass, anything else = fail.

#include <cstdlib>
#include <iostream>

int main () {
    constexpr int expected = 4;

    if (2 + 2 != expected) {
        std::cerr << "sanity_test: 2 + 2 != 4\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
