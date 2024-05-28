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
int partition(Enemy enemies[], int low, int high) {
    int pivot = enemies[high].atk;
    int i = (low - 1);
    for (int j = low; j < high; j++) {
        if (enemies[j].atk < pivot) {
            i++;
            swap(&enemies[i], &enemies[j]);
        }
    }
    swap(&enemies[i + 1], &enemies[high]);
    return (i + 1);
}


// Quick Sort function
void quickSort(Enemy enemies[], int low, int high) {
    if (low < high) {
        int pi = partition(enemies, low, high);
        quickSort(enemies, low, pi - 1);
        quickSort(enemies, pi + 1, high);
    }
}


void test_json(Session* session){
    printf(RED UNDERLINE BOLD"\nLOADING SKILLS\n"RESET);
    session->hash_skills = create_table_skills();
    skill_loader(session->hash_skills);

    for (int i = 0; i < MAX_SKILLS; i++) {
        HashNode* node = session->hash_skills->table[i];
        while (node != NULL) {          
            //printf("%d: %s => %s\n", count + 1, node->skill.name, node->skill.description);
            printf("%sLoaded Skill: %s %s\n",GREEN BOLD,RESET, node->skill.name);
            node = node->next;
        }
    }
    printf(RED UNDERLINE BOLD"\nLOADING ENEMIES\n"RESET);
    enemy_loader(session->enemies,session->hash_skills);
    //quickSort(session->enemies,0,MAX_ENEMIES-1);
    for(int i = 0; i<MAX_ENEMIES; i++){
        printf("%sLoaded Enemie: %s %s\n",GREEN BOLD,RESET, session->enemies[i].name);
        printf(BLUE BOLD"  ATK:%d DEF:%d HP:%d\n"RESET,session->enemies[i].atk,session->enemies[i].def,session->enemies[i].hp);
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
    printf("Available skills:\n");
    int skill_index = 1;
    for (int i = 0; i < MAX_SKILLS; i++) {
        HashNode* node = session->hash_skills->table[i];
        while (node != NULL) {
            printf("%d: %s => %s\n", skill_index, node->skill.name, node->skill.description);
            node = node->next;
            skill_index++;
        }
    }

    // Let the player choose 4 skills
    printf("Choose 4 skills for your character:\n");
    Skills selected_skills[4];
    int skill_count = 0;

    while (skill_count < 4) {
        int skill_idx = 0;
        printf("Select skill %d: ", skill_count + 1);
        if (scanf("%d", &skill_idx) != 1 || skill_idx < 1 || skill_idx > MAX_SKILLS) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a number between 1 and %d.\n", skill_index - 1);
        } else {
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
    }

    // Initialize the player character
    Character player;
    strcpy(player.name, "Player");

    printf(GREEN BOLD"What equipment do you choose?\n");
    printf(CYAN BOLD"\n1. Light Clothing:  500 HP, 80 ATK, 40 DEF, 80 VEL\n");
    printf(CYAN BOLD"\n2. Steel Armour:  500 HP, 60 ATK, 80 DEF, 50 VEL\n\n"RESET);
    int outfit;
    printf(YELLOW BOLD"Enter a valid option: "RESET);
    scanf("%d", &outfit);

    while (outfit < 1 || outfit > 2) {
        printf("Please enter a valid option: "RESET);
        scanf("%d", &outfit);
    }

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

    

    // Let the player choose an enemy
    printf("Choose an enemy for the fight:\n");
    for (int i = 0; i < MAX_ENEMIES; i++) {
        printf("%d: %s\n", i + 1, session->enemies[i].name);
    }

    int enemy_idx = 0;
    while (1) {
        printf("Select enemy: ");
        if (scanf("%d", &enemy_idx) != 1 || enemy_idx < 1 || enemy_idx > MAX_ENEMIES) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a number between 1 and %d.\n", MAX_ENEMIES);
        } else {
            break;
        }
    }
    Enemy selected_enemy = session->enemies[enemy_idx - 1];
    printf("Selected Enemy: %s\n", selected_enemy.name);

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
    while (1) {
        printf("Select scenario: ");
        if (scanf("%d", &option) != 1 || option < 1 || option > MAX_SCENARIOS) {
            while (getchar() != '\n'); // Clear the input buffer
            printf("Invalid input. Please enter a number between 1 and %d.\n", MAX_SCENARIOS);
        } else {
            break;
        }
    }

    for(i = MAX_SCENARIOS-option; i>0; i--){
        test_scenario=test_scenario->Previous;
    }
    open_scenario(test_scenario, session);

}


void test_menu(){
    Session test_session;
    int option=0;
    while(option!=4){
    printf("1. Test JSON loads\n");
    printf("2. Test combat function\n");
    printf("3. Test scenario function\n");
    printf("4. EXIT\n");
    printf("Enter a valid option: ");
    scanf("%d",&option);
        while((getchar()) != '\n');
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
