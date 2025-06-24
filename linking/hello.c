#include <stdio.h>

int main() {
    const char *msg = "Hello, PLT/GOT world!";
    printf(">>> %s <<<\n", msg);  // 👈 최적화 못 함!
    return 0;
}