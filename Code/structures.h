#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define TEMP_MOD 0  //Temporary modifier
#define DIR_ATK 1   //Direct attack
#define MAX_LENGTH 500 //Max length of the strings
#define MAX_NAME 50 //Max length of the names

/*Skill: name and description, type (temporary modifier or direct attack), 
duration in turns (if temporary), and modifiers for atk/def/hp*/
typedef struct Skills{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    int type; //Defint the type as an integer that can be 0 or 1(easier implementation)
    int duration;
    int hp_mod;
    int atk_mod;
    int def_mod;
}Skills;

//Character: name, hp/atk/def points, and an array of 4 skills.
typedef struct Character{
    char name[MAX_NAME];
    int hp;
    int atk;
    int def;
    Skills character_skills[4];
}Character;

//Enemy: name, atk/hp/def.
typedef struct Enemy{
    char name[MAX_NAME];
    int hp;
    int atk;
    int def;
}Enemy;

/*Option: response text, narrative text (before battling the enemies), 
enemies (can be reused from other scenarios), narrative text (after battling the enemies)*/
typedef struct Option{
    char response[MAX_LENGTH];
    char previous_narrative[MAX_LENGTH];
    Enemy enemies[3]; //3 enemies since it is the maximum type of enemies
    char after_narrative;
}Option;

//Decision: question text, options, number of options
typedef struct Decision{
    char question_text[MAX_LENGTH];
    int options_number;
    Option options[];
}Decision;

//Scenario: name and description, and decision (or decision list)
typedef struct Scenario{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    Decision decisions[];
}Scenario;

Character character_creation();

