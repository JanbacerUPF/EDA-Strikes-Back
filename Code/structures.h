#ifndef _STRUCTURES_H_
#define _STRUCTURES_H_

#include "cJSON.h"
#include "common.h"


/*Skill: name and description, type (temporary modifier or direct attack), 
duration in turns (if temporary), and modifiers for atk/def/hp*/
typedef struct Skill{
    char name[MAX_NAME];
    char description[MAX_LENGTH];
    char effect[MAX_LENGTH];
    int type; //Define the type as an integer that can be 0 or 1(easier implementation)
    float hp_mod;
    float atk_mod;
    float def_mod;
}Skill;

//Enemy: name, atk/hp/def.
typedef struct Enemy{
    char name[MAX_NAME];
    int hp;
    int atk;
    int def;
    int turns;
    Skill skills[PLAYER_SKILLS];
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
    Skill character_skills[PLAYER_SKILLS];
}Character;

//Structures for the dictionary of skills
typedef struct HashNode {
    char key[MAX_NAME];
    Skill skill;
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

unsigned int hash(char *str);




HashTable* create_table_skills();

void insert_skill(HashTable* hashTable, Skill skill);

Skill* find_skill(HashTable* hashTable, char* name);

void delete_skill(HashTable* hashTable, char* name);

void free_table(HashTable* hashTable);

void skill_loader(HashTable* hashTable);

void enemy_loader(Enemy enemies_array[], HashTable* hash_skills);

void scene_loader(Session* session);

void show_skills(Skill skills[],int size);

void load_config(Session* session);

Character character_creation(Session* session);




#endif 

