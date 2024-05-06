#include <stdio.h>

typedef struct Skills {
    char skill_1[15];
    char skill_2[15];
    char skill_3[15];
    char skill_4[15];
} Skills;

typedef struct Character {
    char name[15];
    int hp;
    int attack;
    int defence;
    struct Skills skills;
} Character;

typedef struct Enemy {
    char name[15];
    int hp;
    int attack;
    int defence;
    struct Skills skills;
} Enemy;

int main() {
    // hola
    return 0;
}