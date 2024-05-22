#include "structures.h"


#define MAX_SKILLS 2
//Load skills from the Jason
void skill_loader(Skills skillsArray[]){
    FILE *fp = fopen("presets.json", "r");

    if (fp == NULL) { 
        printf("ERROR: Could not open file.\n"); 
        return; 
    } 

    // Determine the file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory to read the file
    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    fread(fileContent, 1, fileSize, fp);
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(fp);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    cJSON* skills = cJSON_GetObjectItemCaseSensitive(json, "skills");

    for(int i = 0; i<MAX_SKILLS; i++){
        cJSON* skill = cJSON_GetArrayItem(skills, i);
        if (skill == NULL) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(skill, "name");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(skill, "description");
        cJSON* type = cJSON_GetObjectItemCaseSensitive(skill, "type");
        cJSON* duration = cJSON_GetObjectItemCaseSensitive(skill, "duration");
        cJSON* hp_mod = cJSON_GetObjectItemCaseSensitive(skill, "hp_mod");
        cJSON* atk_mod = cJSON_GetObjectItemCaseSensitive(skill, "atk_mod");
        cJSON* def_mod = cJSON_GetObjectItemCaseSensitive(skill, "def_mod");

        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(skillsArray[i].name, name->valuestring, MAX_NAME - 1);
            skillsArray[i].name[MAX_NAME - 1] = '\0';
        }

        if (cJSON_IsString(description) && description->valuestring != NULL) {
            strncpy(skillsArray[i].description, description->valuestring, MAX_LENGTH - 1);
            skillsArray[i].description[MAX_LENGTH - 1] = '\0';
        }

        if (cJSON_IsNumber(type)) {
            skillsArray[i].type = type->valueint;
        }

        if (cJSON_IsNumber(duration)) {
            skillsArray[i].duration = duration->valueint;
        }

        if (cJSON_IsNumber(hp_mod)) {
            skillsArray[i].hp_mod = hp_mod->valueint;
        }

        if (cJSON_IsNumber(atk_mod)) {
            skillsArray[i].atk_mod = atk_mod->valueint;
        }

        if (cJSON_IsNumber(def_mod)) {
            skillsArray[i].def_mod = def_mod->valueint;
        }
    }

    cJSON_Delete(json);
}

//Load skills from the Jason
/*void enemy_loader(Enemy enemiesArray[]){
    FILE *fp = fopen("presets.json", "r");

    if (fp == NULL) { 
        printf("ERROR: Could not open file.\n"); 
        return; 
    } 

    // Determine the file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory to read the file
    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    fread(fileContent, 1, fileSize, fp);
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(fp);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    cJSON* skills = cJSON_GetObjectItemCaseSensitive(json, "skills");

    for(int i = 0; i<MAX_SKILLS; i++){
        cJSON* skill = cJSON_GetArrayItem(skills, i);
        if (skill == NULL) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(skill, "name");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(skill, "description");
        cJSON* type = cJSON_GetObjectItemCaseSensitive(skill, "type");
        cJSON* duration = cJSON_GetObjectItemCaseSensitive(skill, "duration");
        cJSON* hp_mod = cJSON_GetObjectItemCaseSensitive(skill, "hp_mod");
        cJSON* atk_mod = cJSON_GetObjectItemCaseSensitive(skill, "atk_mod");
        cJSON* def_mod = cJSON_GetObjectItemCaseSensitive(skill, "def_mod");

        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(enemiesArray[i].name, name->valuestring, MAX_NAME - 1);
            enemiesArray[i].name[MAX_NAME - 1] = '\0';
        }

        if (cJSON_IsString(description) && description->valuestring != NULL) {
            strncpy(enemiesArray[i].description, description->valuestring, MAX_LENGTH - 1);
            enemiesArray[i].description[MAX_LENGTH - 1] = '\0';
        }

        if (cJSON_IsNumber(type)) {
            enemiesArray[i].type = type->valueint;
        }

        if (cJSON_IsNumber(duration)) {
            enemiesArray[i].duration = duration->valueint;
        }

        if (cJSON_IsNumber(hp_mod)) {
            enemiesArray[i].hp_mod = hp_mod->valueint;
        }

        if (cJSON_IsNumber(atk_mod)) {
            enemiesArray[i].atk_mod = atk_mod->valueint;
        }

        if (cJSON_IsNumber(def_mod)) {
            enemiesArray[i].def_mod = def_mod->valueint;
        }
    }

    cJSON_Delete(json);
}*/



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
    Skills possible_skills[MAX_SKILLS];
    skill_loader(possible_skills);
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