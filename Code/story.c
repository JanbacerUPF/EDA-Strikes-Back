#include "story.h"
#include <unistd.h> // For sleep function

//Pretty printing functions
void printCentered(char *text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    printf("|");
    for (int i = 0; i < padding; ++i)
        printf(" ");
    printf("%s", text);
    for (int i = 0; i < (padding + (width % 2)); ++i) // Handle odd width
        printf(" ");
    printf("|\n");
}

void printBorder(int width) {
    printf("+");
    for (int i = 0; i < width; ++i)
        printf("-");
    printf("+\n");
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





void open_scenario(Scenario* scenario){
    printf("\n\n\n");
    printEnteringScenario(scenario->name);
    if(scenario->completed==0){
         printf("%s\n",scenario->description);
        for(int i = 0; i<scenario->dec_num;i++){
            printf("\n%s\n",scenario->decisions[i].question_text);
            for(int j = 0; j<scenario->decisions[i].options_number;j++){
                printf("%d. %s\n", j+1, scenario->decisions[i].options[j].response);
            }
            int option;
            printf("Enter your option: ");
            scanf("%d",&option);
            option--;
            printf("\n%s\n",scenario->decisions[i].options[option].previous_narrative);
            for(int j = 0; j<scenario->decisions[i].options[option].en_num;j++){
                printf("\n//  BATTLE AGAINST %s  //\n", scenario->decisions[i].options[option].enemies[j].name);
            }
            printf("\n%s\n",scenario->decisions[i].options[option].after_narrative);
        }
        scenario->completed=1;
    }
    else{
        printf("You retrace your steps, only to hear a familiar voice echo in your mind: \"You've already been here. Why are we stopping? We can't afford to waste any time. Move forward and continue your journey.\"\n");
    }
   

}

