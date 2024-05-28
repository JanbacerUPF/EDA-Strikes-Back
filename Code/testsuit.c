#include <stdio.h>
#include "testsuit.h"



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



void test_menu(){
    Session test_session;
    int option=0;
    while(option!=4){
    printf("1. Test JSON loads\n");
    printf("2. Test combat function\n");
    printf("4. EXIT\n");
    printf("Enter a valid option: ");
    scanf("%d",&option);
        while((getchar()) != '\n');
        if(option==1){
            test_json(&test_session);
        }
        else if(option==2){
        }
    }
}
