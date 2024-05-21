#include "combat.h"

#define NEW_GAME 1
#define EXIT 2


void new_game(){
    Character player = character_creation();
    Enemy enemies[3];
    combats(&player,enemies,3);
}

int menu(){
    printf("___  ___                 \n");
    printf("|  \\/  |                 \n");
    printf("| .  . | ___ _ __  _   _ \n");
    printf("| |\\/| |/ _ \\ '_ \\| | | |\n");
    printf("| |  | |  __/ | | | |_| |\n");
    printf("\\_|  |_|\\___|_| |_|\\__,_|\n");
    printf("                         \n");
    printf("                         \n");
    int option;
    while(option!=EXIT){
        printf("SELECT AN OPTION\n");
        printf("1. New Game\n");
        printf("2. Exit Game\n");
        scanf("%d",&option);
        if(option==NEW_GAME){
            new_game();
        }
    }

}


int main(){
    menu();
    return 0;
}