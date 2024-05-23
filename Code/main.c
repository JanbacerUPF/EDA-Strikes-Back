#include "main.h"
#include "structures.h"


void new_game(){
    Session current_game;
    load_config(&current_game);
    current_game.current_scenario=current_game.first_Scenario;
    current_game.player = character_creation(&current_game);
    while(current_game.current_scenario.Next!=NULL){
        open_scenario(&current_game.current_scenario);
        printf("Updated COMPLETED NUM: %d\n", current_game.current_scenario.completed); // To verify the change
        int option;
        bool valid_options[2]={false};
        if(current_game.current_scenario.Next!=NULL){
            valid_options[0]=true;
            printf("1. Continue your journey and go to the next scenario\n");
        }
        if(current_game.current_scenario.Previous!=NULL){
            valid_options[1]=true;
            printf("2. Go back to the previous scenario\n");
        }
        printf("Enter your option: ");
        scanf("%d",&option);
        option--;
        while(option<0 || option>2 || valid_options[option]==false){
            printf("You can't do that at the moment, please enter a valid option: ");
            scanf("%d",&option);
            option--;
        }
        if(option==0){
            current_game.current_scenario=*current_game.current_scenario.Next;
        }
        else if(option==1){
            current_game.current_scenario=*current_game.current_scenario.Previous;
        }
    }
    open_scenario(&current_game.current_scenario);
    printf("\n\n\n THE GAME IS FINISHED\n");
    

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