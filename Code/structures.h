#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include "cJSON.h"
#include "common.h"


/*Skill: name and description, type (temporary modifier or direct attack), 
duration in turns (if temporary), and modifiers for atk/def/hp*/
typedef struct Skills{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    char effect[MAX_LENGTH];
    int type; //Type 0: no soul. Type 1: soul
    float hp_mod;
    float atk_mod;
    float def_mod;
}Skills;

//Enemy: name, atk/hp/def.
typedef struct Enemy{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    char appearance[MAX_LENGTH];
    float hp;
    float atk;
    float def;
    int turns;
    Skills skills[NUM_SKILLS];
    float multiplier_skill;
}Enemy;

/*Option: response text, narrative text (before battling the enemies), 
enemies (can be reused from other scenarios), narrative text (after battling the enemies)*/
typedef struct Option{
    char response[MAX_LENGTH];
    char previous_narrative[MAX_LENGTH];
    Enemy enemies[MAX_NUM_ENEMIES]; //3 enemies since it is the maximum number of enemies
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
    struct Scenario *Next;
    struct Scenario *Previous;
    Decision *decisions[];
}Scenario;

//Character: name, hp/atk/def points, and an array of 4 skills.
typedef struct Character{
    char name[MAX_NAME];
    float hp;
    float atk;
    float def;
    float vel;
    float soul;
    Skills character_skills[NUM_SKILLS];
    Scenario current_scenario;
}Character;

Character character_creation();

#endif 