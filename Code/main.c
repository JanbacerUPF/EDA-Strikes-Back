#include "combat.h"
#include "story.h"
//#include "structures.h"


#define NEW_GAME 1
#define EXIT 2


void new_game(){
    Session current_game;
    load_config(&current_game);
    current_game.current_scenario=current_game.first_Scenario;
    current_game.player = story_character_creation(&current_game);
    while(current_game.current_scenario->Next!=NULL){
        open_scenario(current_game.current_scenario, &current_game);
        int option;
        bool valid_options[2]={false};
        if(current_game.current_scenario->Next!=NULL){
            valid_options[0]=true;
            printf("1. Continue your journey and go to the next scenario\n");
        }
        if(current_game.current_scenario->Previous!=NULL){
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
            current_game.current_scenario=current_game.current_scenario->Next;
        }
        else if(option==1){
            current_game.current_scenario=current_game.current_scenario->Previous;
        }
    }
    open_scenario(current_game.current_scenario, &current_game);
    printf("\n\n\n THE GAME IS FINISHED\n");
    

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
        printf("*********************************\n");
        printf("*                               *\n");
        printf("*        SELECT AN OPTION       *\n");
        printf("*                               *\n");
        printf("*  1. New Game                  *\n");
        printf("*  2. Exit Game                 *\n");
        printf("*                               *\n");
        printf("*********************************\n");
        printf("Enter your choice: ");
        scanf("%d",&option);
        while((getchar()) != '\n');
        if(option==NEW_GAME){
            printf("Starting a New Game...\n\n\n");
            sleep(2);
            printf(CLEAR_SCREEN);
            new_game();
        }
    }

}


int main(){
    menu();
    return 0;
}