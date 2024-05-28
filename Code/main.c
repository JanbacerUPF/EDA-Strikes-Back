#include "main.h"

// Function to save Character (name, attributes, and skill names)
cJSON* save_character(Character* character) {
    cJSON* json_character = cJSON_CreateObject();
    cJSON_AddStringToObject(json_character, "name", character->name);
    cJSON_AddNumberToObject(json_character, "hp", character->hp);
    cJSON_AddNumberToObject(json_character, "atk", character->atk);
    cJSON_AddNumberToObject(json_character, "def", character->def);
    cJSON_AddNumberToObject(json_character, "vel", character->vel);
    cJSON_AddNumberToObject(json_character, "soul", character->soul);

    cJSON* json_skills = cJSON_CreateArray();
    for (int i = 0; i < PLAYER_SKILLS; i++) {
        cJSON_AddItemToArray(json_skills, cJSON_CreateString(character->character_skills[i].name));
    }
    cJSON_AddItemToObject(json_character, "character_skills", json_skills);
    return json_character;
}

// Function to save Session (only the player)
cJSON* save_session(Session* session) {
    cJSON* json_session = cJSON_CreateObject();
    cJSON_AddItemToObject(json_session, "player", save_character(&session->player));
    cJSON_AddNumberToObject(json_session, "ID", session->current_ID);
    return json_session;
}

// Save JSON to file
void save_session_to_file(Session* session) {
    cJSON* json_session = save_session(session);
    char* json_string = cJSON_Print(json_session);

    FILE* file = fopen("session.json", "w");
    if (file != NULL) {
        fputs(json_string, file);
        fclose(file);
    }

    cJSON_Delete(json_session);
    free(json_string);
}


//Load skills from the Jason
void load_session(Session* session){
    load_config(session);
    FILE *fp = fopen("session.json", "r");

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

    cJSON* player = cJSON_GetObjectItemCaseSensitive(json, "player");

    cJSON* name = cJSON_GetObjectItemCaseSensitive(player, "name");
    cJSON* hp = cJSON_GetObjectItemCaseSensitive(player, "hp");
    cJSON* atk = cJSON_GetObjectItemCaseSensitive(player, "atk");
    cJSON* def = cJSON_GetObjectItemCaseSensitive(player, "def");
    cJSON* vel = cJSON_GetObjectItemCaseSensitive(player, "vel");
    cJSON* soul = cJSON_GetObjectItemCaseSensitive(player, "soul");
    cJSON* skills = cJSON_GetObjectItemCaseSensitive(player, "character_skills");

    if (cJSON_IsString(name) && name->valuestring != NULL) {
        strncpy(session->player.name, name->valuestring, MAX_NAME - 1);
        session->player.name[MAX_NAME - 1] = '\0';
    }

    session->player.hp=hp->valueint;
    session->player.atk=atk->valueint;
    session->player.def=def->valueint;
    session->player.vel=vel->valueint;
    session->player.soul=soul->valueint;

    for(int i = 0; i<PLAYER_SKILLS; i++){
        cJSON* skill = cJSON_GetArrayItem(skills, i);
        Skills* findSkill = find_skill(session->hash_skills,skill->valuestring);
        if(find_skill==NULL){
            printf(RED"\nSkill NOT found\n");
            return;
        }
        session->player.character_skills[i]=*findSkill;

    }

    cJSON* ID = cJSON_GetObjectItemCaseSensitive(json, "ID");
    session->current_ID=ID->valueint;
    Scenario *current_scenario=session->first_Scenario;
    for(int i = 0; i<MAX_SCENARIOS; i++){
        if(current_scenario->ID==session->current_ID){
            session->current_scenario=current_scenario;
            break;
        }
        current_scenario->completed=1;
        current_scenario=current_scenario->Next;
    }

    if(session->current_scenario==NULL){
        printf(RED"\nScenario NOT found\n");
        return;
    }
    cJSON_Delete(json);

    printf(GREEN UNDERLINE BOLD"\n\nGAME LOADED SUCCESFULLY\n\n");
    sleep(1);
    play_game(session);
}


void play_game(Session* current_game){
    while(current_game->current_scenario->Next!=NULL){
        open_scenario(current_game->current_scenario, current_game);
        int option;
        bool valid_options[2]={false};
        if(current_game->current_scenario->Next!=NULL){
            valid_options[0]=true;
            printf("1. Continue your journey and go to the next scenario\n");
        }
        if(current_game->current_scenario->Previous!=NULL){
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
            current_game->current_scenario=current_game->current_scenario->Next;
        }
        else if(option==1){
            current_game->current_scenario=current_game->current_scenario->Previous;
        }
        current_game->current_ID=current_game->current_scenario->ID;
    }
    open_scenario(current_game->current_scenario, current_game);
    printf("\n\n\n THE GAME IS FINISHED\n");

}

void new_game(){
    Session current_game;
    load_config(&current_game);
    current_game.current_scenario=current_game.first_Scenario;
    current_game.player = story_character_creation(&current_game);
    play_game(&current_game);
    
}

int menu(){
    printf(RESET"___  ___                 \n");
    printf("|  \\/  |                 \n");
    printf("| .  . | ___ _ __  _   _ \n");
    printf("| |\\/| |/ _ \\ '_ \\| | | |\n");
    printf("| |  | |  __/ | | | |_| |\n");
    printf("\\_|  |_|\\___|_| |_|\\__,_|\n");
    printf("                         \n");
    printf("                         \n");
    int option=0;
    while(option!=EXIT){
        printf("*********************************\n");
        printf("*                               *\n");
        printf("*        SELECT AN OPTION       *\n");
        printf("*                               *\n");
        printf("*  1. New Game                  *\n");
        printf("*  2. Load Game                  *\n");
        printf("*  3. Exit Game                 *\n");
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
        else if(option==LOAD_GAME){
            Session loaded_session;
            load_session(&loaded_session);

        }
        else if(option==TEST_CODE){
            test_menu();

        }
    }

}


int main(){
    menu();
    return 0;
}