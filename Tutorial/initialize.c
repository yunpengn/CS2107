#include <stdio.h>

int main() {
    unsigned char a[10000];
    
    for (int i = 0; i < 10000; i++) {
        printf("%c", a[i]);
    }

    return 0;
}
