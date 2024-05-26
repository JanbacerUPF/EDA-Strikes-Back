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

// Swap function
void swap(Scenario** a, Scenario** b) {
    Scenario* temp = *a;
    *a = *b;
    *b = temp;
}

// Partition function for Quick Sort
int partition(Scenario* scenarios[], int low, int high) {
    char* pivot = scenarios[high]->name;
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (strcmp(scenarios[j]->name, pivot) < 0) {
            i++;
            swap(&scenarios[i], &scenarios[j]);
        }
    }
    swap(&scenarios[i + 1], &scenarios[high]);
    return (i + 1);
}

// Quick Sort function
void quickSort(Scenario* scenarios[], int low, int high) {
    if (low < high) {
        int pi = partition(scenarios, low, high);
        quickSort(scenarios, low, pi - 1);
        quickSort(scenarios, pi + 1, high);
    }
}

// Binary search function
Scenario* binary_search_scenario(Scenario* scenarios[], int num_scenarios, char* name) {
    int low = 0;
    int high = num_scenarios - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int cmp = strcmp(scenarios[mid]->name, name);

        if (cmp == 0) {
            return scenarios[mid];
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return NULL;
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
    printf("5. Specific Scenario Info\n");
    printf("6. Specific Enemy Info\n");
    printf("7. EXIT\n");

    do {
        option = read_int();

        switch (option) {
            case NEW_GAME:
                printf("Starting new game...\n");
                new_game();
                break;

            case RESUME_GAME:
                printf("Resuming game...\n");
                
                break;

            case SAVE_GAME:
                printf("Saving game...\n");
                
                break;

            case LOAD_GAME:
                printf("Loading game...\n");
                
                break;
            
            case SPECIFIC_SCENARIO_INFO:
                // Declare the session variable
                Session session_variable;

                // Initialize the session
                init_session(&session_variable);

                printf("What scenario are you interested in?\n");
                
                break;

            case SPECIFIC_ENEMY_INFO:
                printf("Loading game...\n");
                
                break;

            case EXIT:
                printf("Exiting...\n");
                break;

            default:
                printf("%d is not an option.\n", option);
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