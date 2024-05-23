#include "combat.h"
//#include "structures.h"


#define NEW_GAME 1
#define EXIT 2


void new_game(){
    Session current_game;
    load_config(&current_game);
    current_game.current_scenario=current_game.first_Scenario;
    current_game.player = character_creation(&current_game);
    printf("\n ENEMIES \n");
    for(int i=0; i<MAX_ENEMIES; i++){
        printf("Enemy number %d: %s\n", i, current_game.enemies[i].name);
    }

    

    
    

    Enemy enemies[3];
    combats(&current_game.player,enemies,3);
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