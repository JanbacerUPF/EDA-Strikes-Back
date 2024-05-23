#include "main.h"
#include "structures.h"


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


int menu() {
    int option;

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
    printf("2. Resume Game\n");
    printf("3. Save Game\n");
    printf("4. Load Game\n");
    printf("5. Exit Game\n");

    do {
        option = read_int();

        switch (option) {
            case NEW_GAME:
                printf("Starting new game...\n");
                new_game();
                break;

            case RESUME_GAME:
                printf("Resuming game...\n");
                // Add the logic to resume game here
                break;

            case SAVE_GAME:
                printf("Saving game...\n");
                // Add the logic to save game here
                break;

            case LOAD_GAME:
                printf("Loading game...\n");
                // Add the logic to load game here
                break;

            case EXIT:
                printf("Exiting...\n");
                break;

            default:
                printf("That is not an option.\n");
                printf("Please, try it again.\n\n");
                break;
        }
    } while (option != EXIT);

    return 0;
}


int main(){
    printf("Welcome to Lost Souls!\n\n");

    menu();

    return 0;
}