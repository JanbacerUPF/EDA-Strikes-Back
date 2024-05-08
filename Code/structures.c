#include "structures.h"
//Function to print the possible skills to choose from
void show_skills(Skills skills[], int size){
    for(int i = 0; i<size; i++){
        printf("%d: %s=>%s\n",i+1,skills[i].name,skills[i].description);
    }
}

Character character_creation(){
    printf("Welcome to the character creation\n");
    Character player;
    printf("Enter the character's name: ");
    scanf("%s", player.name); // Set the input as the character's name
    printf("\n");
    //PLACEHOLDER
    Skills possible_skills[2];
    strcpy(possible_skills[0].name,"Skill 1");
    strcpy(possible_skills[1].name,"Skill 2");
    strcpy(possible_skills[0].description,"This is the description for skill 1");
    strcpy(possible_skills[1].description,"This is the description for skill 2");
    printf("Choose the player's skills: \n");
    show_skills(possible_skills,sizeof(possible_skills)/sizeof(Skills));
    for(int i = 0; i<4; i++){
        int option;
        scanf("%d", &option);
        player.character_skills[i]=possible_skills[option-1];
    }
    printf("Your skills are: \n");
    for(int i = 0; i<4; i++){
        printf("%s ",player.character_skills[i].name);
    }


    return player;
}