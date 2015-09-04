#include "Shell.hpp"

using namespace std;

int main() {
    Shell shell("user@DV1942");
    
    bool run = true;

    while (run)
        run = shell.getCommand();

    return 0;
}
