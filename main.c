#include <stdio.h>
#define Temp_Mod 0
#define Dir_Atk 1
/*Skill: name and description, type (temporary modifier or direct attack), 
duration in turns (if temporary), and modifiers for atk/def/hp*/
typedef struct Skills{
    char name;
    char description;
    int type; //Defint the type as an integer that can be 0 or 1(easier implementation)
    int duration;
    int hp_mod;
    int atk_mod;
    int def_mod;
}Skills;

//Character: name, hp/atk/def points, and an array of 4 skills.
typedef struct Character{
    char name;
    int hp;
    int atk;
    int def;
    Skills character_skills[4];
}Character;

//Enemy: name, atk/hp/def.
typedef struct Enemy{
    char name;
    int hp;
    int atk;
    int def;
}Enemy;

/*Option: response text, narrative text (before battling the enemies), 
enemies (can be reused from other scenarios), narrative text (after battling the enemies)*/
typedef struct Option{
    char response;
    char previous_narrative;
    Enemy enemies[3]; //3 enemies since it is the maximum type of enemies
    char after_narrative;
}Option;

//Decision: question text, options, number of options
typedef struct Decision{
    char question_text;
    int options_number;
    Option options[];
}Decision;

//Scenario: name and description, and decision (or decision list)
typedef struct Scenario{
    char name;
    char description;
    //Decision list
}Scenario;

int main(){
    return 0;
}