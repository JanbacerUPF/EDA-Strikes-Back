#ifndef COMBAT_H
#define COMBAT_H

// Define constants
#include "structures.h"
#define SOUL_COST 40

// Define the StackNode structure
typedef struct StackNode {
    Skills* skill;
    struct StackNode* next;
} StackNode;

// Define the Node structure for the queue
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Define the Queue structure
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Function prototypes
// Stack functions
void push(StackNode** top, Skills* skill);
Skills* pop(StackNode** top);
Skills* get_kth_skill(StackNode* top, int k);

// Display functions
void drawProgressBar(int current, int max);
void view_stats(Character* player, Enemy* enemy);

// Effect and damage functions
void apply_effects(Character* player, Enemy* enemy, Skills* skill, float multiplier, char* color, int is_enemy);
int calculate_damage(int atk, int def, float multiplier, char* color);
void apply_damage(int* target_hp, int damage, char* color);

// Skill usage function
void use_skill(Character* player, Enemy* enemy, Skills* skill, int is_enemy);

// Queue functions
Node* newNode(int data);
Queue* createQueue();
void enqueue(Queue* q, int data);
int dequeue(Queue* q);
int isEmpty(Queue* q);
void displayQueue(Queue* q);

// Combat functions
void fill_fight_queue(Queue* q, int velocity, int turns);
void player_turn(Character* player, Enemy* enemy, Session* session, StackNode** skill_stack, int* time_strike_used);
void enemy_turn(Character* player, Enemy* enemy);
int fight(Character* player, Enemy enemy, Session* session);

#endif // COMBAT_H
