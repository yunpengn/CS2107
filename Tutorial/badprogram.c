#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    char text[16];
    
    // The first command-line argument is the file name, so argv[1] is actual "first" argument.
    strcpy(text, argv[1]);
    printf("This is how you print correctly:\n");
    printf("%s", text);
    printf("\n");


    printf("This is how not to print:\n");
    printf(text);
    printf("\n");

    return 0;
}
