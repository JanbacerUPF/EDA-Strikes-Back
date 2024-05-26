#ifndef COMBAT_H
#define COMBAT_H

#include "structures.h"

// SKILLS_CHARACTER -----------------------------------------------------------------
// Function to draw a progress bar with square symbols
void drawProgressBar(int current, int max);

// Function to clear the console screen (platform dependent)
// void clear_screen();

void apply_effects(int* hp, int max_hp, int* def, int* atk, float hp_mod, float def_mod, float atk_mod, float multiplier, char* color);

int calculate_damage(int atk, int def, float multiplier, char* color);

void use_skill(Character* player, Enemy* enemy, Skills* skill, int is_enemy);

// QUEUE IMPLEMENTATION ---------------------------------------------------------------
// Define a structure for the nodes of the queue
typedef struct Node {
    int data;
    struct Node* next;
} Node;

// Define a structure for the queue
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

// Function to create a new node
Node* newNode(int data);

// Function to create an empty queue
Queue* createQueue();

// Function to add an item to the queue
void enqueue(Queue* q, int data);

// Function to remove an item from the queue
int dequeue(Queue* q);

// Function to check if the queue is empty
int isEmpty(Queue* q);

// Function to display the queue
void displayQueue(Queue* q);

// COMBAT FUNCTIONS ----------------------------------------------------------------
void fill_fight_queue(Queue* q, int velocity, int turns);

// Function to show player and enemy stats
void view_stats(Character* player, Enemy* enemy);

void player_turn(Character* player, Enemy* enemy, Session* session);

void enemy_turn(Character* player, Enemy* enemy);

// FIGHT FUNCTION ------------------------------------------------------------------
int fight(Character* player, Enemy enemy, Session* session); // OUTPUT: Returns 0 -> player won, 1 -> player lost

#endif // COMBAT_H
