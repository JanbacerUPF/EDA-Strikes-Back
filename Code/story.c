#include "story.h"
#include "combat.h"
#include <unistd.h> // For sleep function


Intro introduction(){
    FILE *fp = fopen("extra.json", "r");
    Intro intro;

    if (fp == NULL) { 
        printf("ERROR: Could not open file.\n"); 
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
    }

    fread(fileContent, 1, fileSize, fp);
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(fp);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        printf("Error parsing JSON\n");
    }

    cJSON* introduction = cJSON_GetObjectItemCaseSensitive(json, "intro");
    cJSON* previous = cJSON_GetObjectItemCaseSensitive(introduction, "previous");
    cJSON* aftername = cJSON_GetObjectItemCaseSensitive(introduction, "aftername");
    cJSON* prophecy = cJSON_GetObjectItemCaseSensitive(introduction, "prophecy");
    cJSON* last = cJSON_GetObjectItemCaseSensitive(introduction, "last");

        if (cJSON_IsString(previous) && previous->valuestring != NULL) {
            strncpy(intro.prev_name, previous->valuestring, MAX_LENGTH - 1);
            intro.prev_name[MAX_LENGTH - 1] = '\0';
        }
        if (cJSON_IsString(aftername) && aftername->valuestring != NULL) {
            strncpy(intro.after_name, aftername->valuestring, MAX_LENGTH - 1);
            intro.after_name[MAX_LENGTH - 1] = '\0';
        }

        if (cJSON_IsString(prophecy) && prophecy->valuestring != NULL) {
            strncpy(intro.prophecy, prophecy->valuestring, MAX_LENGTH - 1);
            intro.prophecy[MAX_LENGTH - 1] = '\0';
        }

        if (cJSON_IsString(last) && last->valuestring != NULL) {
            strncpy(intro.last, last->valuestring, MAX_LENGTH - 1);
            intro.last[MAX_LENGTH - 1] = '\0';
        }

    cJSON_Delete(json);
    return intro;
}

//Pretty printing functions
void printCentered(char *text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    printf(MAGENTA BOLD  "|");
    for (int i = 0; i < padding; ++i)
        printf(" ");
    printf(MAGENTA BOLD  "%s", text);
    for (int i = 0; i < (padding + (width % 2)); ++i) // Handle odd width
        printf(" ");
    printf("|\n" RESET);
}

void printBorder(int width) {
    printf(MAGENTA BOLD  "+");
    for (int i = 0; i < width; ++i)
        printf("-");
    printf("+\n" RESET);
}

void printEnteringScenario(char *scenario) {
    int width = 60; // Adjust this value to change the width of the message box
    printBorder(width);
    printCentered("Entering Scenario...", width);
    printBorder(width);
    printCentered(scenario, width);
    printBorder(width);
    sleep(1); // Pause for a few seconds to create a dramatic effect
}


#define CONSOLE_WIDTH 100  // Adjust this to your console width

void printWrapped(const char *str) {
    int len = strlen(str);
    int start = 0;
    
    while (start < len) {
        // Find the end of the line
        int end = start + CONSOLE_WIDTH;
        if (end >= len) {
            // If the end goes past the string length, print the rest of the string
            printf("%s\n", &str[start]);
            break;
        }

        // Find the last space before the end of the line
        int lastSpace = end;
        while (lastSpace > start && str[lastSpace] != ' ') {
            lastSpace--;
        }

        // If no space is found, the word is too long and needs to be split
        if (lastSpace == start) {
            lastSpace = end;
        }

        // Print the substring from start to lastSpace
        for (int i = start; i < lastSpace; i++) {
            putchar(str[i]);
        }
        putchar('\n');

        // Move the start to the next character after the last printed space
        start = lastSpace + 1;
    }
}


void open_scenario(Scenario* scenario, Session* session){
    printf(CLEAR_SCREEN); 
    printf("\n\n\n");
    printEnteringScenario(scenario->name);
    if(scenario->completed==0){
         //printf("%s\n",scenario->description);
         printf("\n");
         printWrapped(scenario->description);
        for(int i = 0; i<scenario->dec_num;i++){
            printf(BLUE UNDERLINE BOLD   "\n%s\n" RESET,scenario->decisions[i].question_text);
            for(int j = 0; j<scenario->decisions[i].options_number;j++){
                printf(GREEN BOLD  "%d. %s\n\n" RESET, j+1, scenario->decisions[i].options[j].response);
            }
            int option;
            printf("Enter your option: ");
            scanf("%d",&option);
            option--;
            //printf("\n%s\n",scenario->decisions[i].options[option].previous_narrative);
            printf("\n");
            printWrapped(scenario->decisions[i].options[option].previous_narrative);
            sleep(3); // 5-second delay
            printf(CLEAR_SCREEN); 
            for(int j = 0; j<scenario->decisions[i].options[option].en_num;j++){
                printf(RED BOLD "\n//  BATTLE AGAINST %s  //\n" RESET, scenario->decisions[i].options[option].enemies[j].name);
                int win_num=fight(&session->player, scenario->decisions[i].options[option].enemies[j], session);   
            }
            //printf("\n%s\n",scenario->decisions[i].options[option].after_narrative);
            printf("\n");
            printWrapped(scenario->decisions[i].options[option].after_narrative);
        }
        scenario->completed=1;
    }
    else{
        printf("You retrace your steps, only to hear a familiar voice echo in your mind: \"You've already been here. Why are we stopping? We can't afford to waste any time. Move forward and continue your journey.\"\n");
    }
   

}

