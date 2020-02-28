#include <string.h>
#include <stdio.h>

void test(char *t) {
     ++t;
     ++t;
     printf("%s\n", t);
}

int main() {

    char ex[7] = "OkOKkk";

    test(ex);

    printf("%s\n", ex);

    return 0;
}