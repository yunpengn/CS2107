#include <stdio.h>
#include <string.h>

int main() {
    unsigned char a, total, secret;
    unsigned char str[256];

    a = 40;
    total = 0;
    secret = 11;

    printf("Enter your name: ");
    scanf("%255s", str);

    total = a + strlen(str);

    if (total < 40) {
        printf("This is what the attacker wants to see: %d\n", secret);
    } else {
        printf("The attack does not want to see this line.\n");
    }

    return 0;
}

