#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#include "cJSON.h"


#define TEMP_MOD 0  //Temporary modifier
#define DIR_ATK 1   //Direct attack
#define MAX_LENGTH 1024 //Max length of the strings
#define MAX_NAME 64 //Max length of the names
#define MAX_SKILLS 11
#define MAX_ENEMIES 9
#define PLAYER_SKILLS 4
#define MAX_SCENARIOS 5


/*Skill: name and description, type (temporary modifier or direct attack), 
duration in turns (if temporary), and modifiers for atk/def/hp*/
typedef struct Skills{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    char effect[MAX_LENGTH];
    int type; //Define the type as an integer that can be 0 or 1(easier implementation)
    float hp_mod;
    float atk_mod;
    float def_mod;
}Skills;

//Enemy: name, atk/hp/def.
typedef struct Enemy{
    char name[MAX_NAME];
    int hp;
    int atk;
    int def;
    int turns;
    Skills skills[PLAYER_SKILLS];
    float multiplier_skill;
}Enemy;

/*Option: response text, narrative text (before battling the enemies), 
enemies (can be reused from other scenarios), narrative text (after battling the enemies)*/
typedef struct Option{
    char response[MAX_LENGTH];
    char previous_narrative[MAX_LENGTH];
    char after_narrative[MAX_LENGTH];
    Enemy* enemies;
}Option;

//Decision: question text, options, number of options
typedef struct Decision{
    char question_text[MAX_LENGTH];
    int options_number;
    Option* options;
}Decision;

//Scenario: name and description, and decision (or decision list)
typedef struct Scenario{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    struct Scenario *Next;
    struct Scenario *Previous;
    Decision* decisions;
}Scenario;

//Character: name, hp/atk/def points, and an array of 4 skills.
typedef struct Character{
    char name[MAX_NAME];
    int hp;
    int atk;
    int def;
    int vel;
    int soul;
    Skills character_skills[PLAYER_SKILLS];
}Character;



//Structures for the dictionary of skills
typedef struct HashNode {
    char key[MAX_NAME];
    Skills skill;
    struct HashNode* next;
} HashNode;

typedef struct HashTable {
    HashNode* table[MAX_SKILLS];
    int size;
} HashTable;

typedef struct Session {
    HashTable* hash_skills;
    Character player;
    Scenario current_scenario;
    Scenario first_Scenario;
    Enemy enemies[MAX_ENEMIES];
} Session;


Character character_creation(Session* session);


HashTable* create_table_skills();


void skill_loader(HashTable* hashTable);

void load_config(Session* session);

Skills* find_skill(HashTable* hashTable, char* name);




#endif 

