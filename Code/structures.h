#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <math.h>

#include "cJSON.h"


#define TEMP_MOD 0  //Temporary modifier
#define DIR_ATK 1   //Direct attack
#define MAX_LENGTH 4096 //Max length of the texts
#define MAX_NAME 64 //Max length of the names
#define MAX_SKILLS 11 //Max number of skills created
#define MAX_ENEMIES 8 //Max number of enemies created
#define PLAYER_SKILLS 4 //Max number of skills the player can have
#define MAX_SCENARIOS 5 //MAx number of scenarios created

// Define ANSI color codes using #define
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"
#define BOLD    "\033[0;1m"
#define UNDERLINE "\033[0;4m"
#define INVERT  "\033[0;7m"
#define RESET   "\033[0m"  // Reset color

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
    int max_hp;
}Enemy;

/*Option: response text, narrative text (before battling the enemies), 
enemies (can be reused from other scenarios), narrative text (after battling the enemies)*/
typedef struct Option{
    char response[MAX_LENGTH];
    char previous_narrative[MAX_LENGTH];
    char after_narrative[MAX_LENGTH];
    Enemy* enemies;
    int en_num;
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
    int completed;
    struct Scenario *Next;
    struct Scenario *Previous;
    Decision* decisions;
    int dec_num;
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
    Scenario *current_scenario;
    Scenario *first_Scenario;
    Enemy enemies[MAX_ENEMIES];
} Session;


Character character_creation(Session* session);


HashTable* create_table_skills();


void skill_loader(HashTable* hashTable);

void load_config(Session* session);

Skills* find_skill(HashTable* hashTable, char* name);




#endif 

