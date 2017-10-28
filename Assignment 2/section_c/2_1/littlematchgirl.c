#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct matchbook {
    unsigned char buffer[24];
    void (*release)();
};

void dancing_visions() {
    puts("Her eyes are still open wide.");
    puts("The image fills all with dread.");
    puts("The last of her matches did die.");
    system("/bin/sh");
}

void fade_away() {
    puts("People gather around the alley dark.");
    puts("As the ancient carolers sing.");
    puts("There are cries heard around the park.");
}

void light(struct matchbook * mb, unsigned int number) {
    for (int i = 0; i < number; i++) {
        printf("How many dancing lights will she see? ");
        unsigned int how_many;
        scanf("%d", &how_many);
        if (how_many == 0) {
            break;
        }
        mb->buffer[i] = how_many & 0x000000ff;
    }
}

int main() {
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);

    struct matchbook * little_matchbook = malloc(sizeof(struct matchbook));
    memset(little_matchbook->buffer, 0, 24);
    little_matchbook->release = &fade_away;

    puts("Swaying on thin swaying feet.");
    puts("Cold winter frost choking her.");
    puts("A little girl makes her way down the street.\n");

    int number;
    puts("Precious matches number twenty-four.");
    printf("How many will you answer for: ");
    scanf("%d", &number);

    if (number > 24) {
        puts("You ask for the impossible.\n");
        number = 0;
    }
    else {
        light(little_matchbook, number);
    }

    for (int i = 0; i < number; i++) {
        printf("%d. \t", i + 1);
        for (int j = 0; j < little_matchbook->buffer[i]; j++) {
            printf("*");
        }
        puts("");
    }
    puts("");

    little_matchbook->release();
    free(little_matchbook);
}
