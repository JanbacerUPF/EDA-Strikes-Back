#include <stdio.h>
#include "testsuit.h"
#include "combat.h"
#include "story.h"



// Swap function
void swap(Enemy* a, Enemy* b) {
    Enemy temp = *a;
    *a = *b;
    *b = temp;
}


// Partition function for Quick Sort
int partition(Enemy enemies[], int low, int high, int type) {
    int pivot;
    int i;
    switch (type){
        case 0:
            pivot = enemies[high].hp;
            i = (low - 1);
            for (int j = low; j < high; j++) {
                if (enemies[j].hp < pivot) {
                    i++;
                    swap(&enemies[i], &enemies[j]);
                }
            }
            swap(&enemies[i + 1], &enemies[high]);
            return (i + 1);
        case 1:
            pivot = enemies[high].atk;
            i = (low - 1);
            for (int j = low; j < high; j++) {
                if (enemies[j].atk < pivot) {
                    i++;
                    swap(&enemies[i], &enemies[j]);
                }
            }
            swap(&enemies[i + 1], &enemies[high]);
            return (i + 1);
        case 2:
            pivot = enemies[high].def;
            i = (low - 1);
            for (int j = low; j < high; j++) {
                if (enemies[j].def < pivot) {
                    i++;
                    swap(&enemies[i], &enemies[j]);
                }
            }
            swap(&enemies[i + 1], &enemies[high]);
            return (i + 1);
    }
    
}


// Quick Sort function
void quickSort(Enemy enemies[], int low, int high, int type) {
    if (low < high) {
        int pi = partition(enemies, low, high, type);
        quickSort(enemies, low, pi - 1, type);
        quickSort(enemies, pi + 1, high, type);
    }
}


void test_json(Session* session){
    printf(RED UNDERLINE BOLD"\nLOADING SKILLS\n"RESET);
    session->hash_skills = create_table_skills();
    skill_loader(session->hash_skills);

    for (int i = 0; i < MAX_SKILLS; i++) {
        HashNode* node = session->hash_skills->table[i];
        while (node != NULL) {          
            printf("%sLoaded Skill: %s %s\n",GREEN BOLD,RESET, node->skill.name);
            node = node->next;
        }
    }
    printf(RED UNDERLINE BOLD"\nLOADING ENEMIES\n"RESET);
    enemy_loader(session->enemies,session->hash_skills);
    
    for(int i = 0; i<MAX_ENEMIES; i++){
        printf("%sLoaded Enemie: %s %s\n",GREEN BOLD,RESET, session->enemies[i].name);
        printf(BLUE BOLD"   ATK:%d DEF:%d HP:%d\n"RESET,session->enemies[i].atk,session->enemies[i].def,session->enemies[i].hp);
    }

    printf(RED UNDERLINE BOLD"\nLOADING SCENARIOS\n"RESET);
    scene_loader(session);
    Scenario *current=session->first_Scenario;
    while(current->Next!=NULL){
        printf("%sLoaded Scenario: %s %s\n",GREEN BOLD,RESET, current->name);
        current=current->Next;
    }
    printf("%sLoaded Scenario: %s %s\n",GREEN BOLD,RESET, current->name);


    printf(YELLOW BOLD"\n\tJSONS LOADED\n\n\n"RESET);

}


//Function to easily create a character to perform the tests.
Character test_character(Session* session){
    // Print available skills
    printf("\nAvailable skills:\n");
    int skill_index = 1;
    for (int i = 0; i < MAX_SKILLS; i++) {
        HashNode* node = session->hash_skills->table[i];
        while (node != NULL) {
            char *color = node->skill.dmg_type ? MAGENTA : GREEN;
            printf("%s%s%d: %s =>%s %s\n",color,BOLD, skill_index, node->skill.name, RESET,node->skill.description);
            node = node->next;
            skill_index++;
        }
    }

    // Let the player choose 4 skills
    printf("Choose 4 skills for your character:\n");
    Skills selected_skills[4];
    int skill_count = 0;

    while (skill_count < 4) {
        char message[50];  // Adjust the size as necessary
        snprintf(message, sizeof(message), "Select skill %d: ", skill_count + 1);
        int skill_idx = input_integer(message,1,MAX_SKILLS);

        // Find the skill in the hash table
        int current_index = 1;
        for (int i = 0; i < MAX_SKILLS; i++) {
            HashNode* node = session->hash_skills->table[i];
            while (node != NULL) {
                if (current_index == skill_idx) {
                    selected_skills[skill_count] = node->skill;
                    printf("Selected Skill: %s\n", node->skill.name);
                    skill_count++;
                    node = NULL; // Break out of the inner loop
                } else {
                    node = node->next;
                }
                current_index++;
            }
        }
        
    }

    // Initialize the player character
    Character player;
    strcpy(player.name, "Player");

    printf(GREEN BOLD"What equipment do you choose?\n");
    printf(CYAN BOLD"\n1. Light Clothing:  500 HP, 80 ATK, 40 DEF, 80 VEL\n");
    printf(CYAN BOLD"\n2. Steel Armour:  500 HP, 60 ATK, 80 DEF, 50 VEL\n\n"RESET);
    int outfit;
    outfit=input_integer("Enter a valid option: ",1,2);

    if(outfit == 1){
        player.hp=500;
        player.atk=80;
        player.def=40;
        player.vel=80;
        player.soul=0;
    }
    else if(outfit == 2){
        player.hp=500;
        player.atk=60;
        player.def=80;
        player.vel=50;
        player.soul=0;
    }
    else{ //Default settings just in case
        player.hp=200;
        player.atk=50;
        player.def=50;
        player.vel=0;
        player.soul=0;
    }

    printf("\n");

    for (int i = 0; i < 4; i++) {
        player.character_skills[i] = selected_skills[i];
    }
    return player;
}


// Function to test the combat system
void test_combat(Session* session) {
    // Load skills and enemies
    load_config(session);

    int enemy_idx = 0;
    int counter = 0;
     

    while(1){
        int shouldBreak = 0; // Flag to indicate whether to break
        quickSort(session->enemies,0,MAX_ENEMIES-1,counter % 3);
        // Let the player choose an enemy
        printf(RED UNDERLINE BOLD"\n\nChoose an enemy for the fight:\n"RESET);
        printf(BOLD"0: Ordered (from lowest to highest) by "RESET);
            switch (counter % 3) {
                case 0:
                    printf(GREEN BOLD"hp "RESET);
                    break;
                case 1:
                    printf(GREEN BOLD"atk "RESET);
                    break;
                case 2:
                    printf(GREEN BOLD"def "RESET);
                    break;
            }
        printf(BOLD"(toggle)\n"RESET);
        for (int i = 0; i < MAX_ENEMIES; i++) {
            printf(BOLD"%d: %s\n"RESET, i + 1, session->enemies[i].name);
            printf(BLUE BOLD"   ATK:%d DEF:%d HP:%d\n"RESET,session->enemies[i].atk,session->enemies[i].def,session->enemies[i].hp);
        }

        
        while (1) {
            enemy_idx=input_integer("Select enemy: ",0,MAX_ENEMIES);
            if(enemy_idx==0){
                counter++;
                break;
            }
            else {
                shouldBreak = 1;
                break;
            }
        }

        if (shouldBreak==1) {
            break; // Break the outer loop if the flag is set
        }

    }


    
    Enemy selected_enemy = session->enemies[enemy_idx - 1];
    printf("\nSelected Enemy: %s\n", selected_enemy.name);

    Character player = test_character(session);

    // Start the fight
    int result = fight(&player, selected_enemy, session);
}


void scenario_test(Session* session){

    load_config(session);

    session->player = test_character(session);

    Scenario* test_scenario = session->first_Scenario;
    printf("Choose an scenario\n");
    int i;
    for(i = 0; i<MAX_SCENARIOS-1; i++){
        printf("%d) %s\n", i+1, test_scenario->name);
        test_scenario=test_scenario->Next;
    }
    printf("%d) %s\n", i+1, test_scenario->name);
    int option = 0;
    option=input_integer("Select scenario: ",1,MAX_SCENARIOS);

    for(i = MAX_SCENARIOS-option; i>0; i--){
        test_scenario=test_scenario->Previous;
    }
    open_scenario(test_scenario, session);

}


void test_menu(){
    Session test_session;
    int option=0;
    while(option!=4){
    printf(RED UNDERLINE BOLD"\n    TESTSUIT MENU    \n\n"RESET);
    printf("1. Test JSON loads\n");
    printf("2. Test combat function\n");
    printf("3. Test scenario function\n");
    printf("4. EXIT\n");
    option=input_integer("Enter a valid option: ",1,4);
        if(option==1){
            test_json(&test_session);
        }
        else if(option==2){
            test_combat(&test_session);
        }
        else if(option==3){
            scenario_test(&test_session);
        }
    }
}
