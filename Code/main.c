#include "main.h"


void new_game(){
    Character player = character_creation();
    Enemy enemies[3];
    combats(&player,enemies,3);
}

int menu() {
    int option;

    do {
        printf("\n");
        printf("___  ___                 \n");
        printf("|  \\/  |                 \n");
        printf("| .  . | ___ _ __  _   _ \n");
        printf("| |\\/| |/ _ \\ '_ \\| | | |\n");
        printf("| |  | |  __/ | | | |_| |\n");
        printf("\\_|  |_|\\___|_| |_|\\__,_|\n");
        printf("                         \n");
        printf("                         \n");
        printf("SELECT AN OPTION\n");
        printf("1. New Game\n");
        printf("2. Exit Game\n");
        option = read_int();

        if (option == NEW_GAME) {
            printf("You have selected new game.\n");
            //new_game();
        } else if (option != EXIT) {
            printf("That is not an option.\n");
            printf("Please, try it again.\n");
        }
    } while (option != EXIT);

    return 0;
}


int main(){
    printf("Welcome to Lost Souls!\n\n");

    menu();

    return 0;
}