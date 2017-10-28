#include <stdio.h>
#include <stdlib.h>

int check(char * value) {
    if (value[0] != 99) { return 0; }
    if (value[1] != 115) { return 0; }
    if (value[2] != 50) { return 0; }
    if (value[3] != 49) { return 0; }
    if (value[4] != 48) { return 0; }
    if (value[5] != 55) { return 0; }
    if (value[6] != 123) { return 0; }
    if (value[7] != 119) { return 0; }
    if (value[8] != 105) { return 0; }
    if (value[9] != 110) { return 0; }
    if (value[10] != 110) { return 0; }
    if (value[11] != 105) { return 0; }
    if (value[12] != 110) { return 0; }
    if (value[13] != 103) { return 0; }
    if (value[14] != 95) { return 0; }
    if (value[15] != 116) { return 0; }
    if (value[16] != 104) { return 0; }
    if (value[17] != 105) { return 0; }
    if (value[18] != 115) { return 0; }
    if (value[19] != 95) { return 0; }
    if (value[20] != 105) { return 0; }
    if (value[21] != 115) { return 0; }
    if (value[22] != 95) { return 0; }
    if (value[23] != 103) { return 0; }
    if (value[24] != 48) { return 0; }
    if (value[25] != 48) { return 0; }
    if (value[26] != 100) { return 0; }
    if (value[27] != 125) { return 0; }
    return 1;
}

int main(int argc, char * argv[]) {

    if (argc != 2) {
        printf("Sorry, %s accepts an argument.\n", argv[0]);
        exit(1);
    }

    if (check(argv[1])) {
        printf("Flag: %s\n", argv[1]);
    }
    else {
        puts("Incorrect flag");
    }
}
