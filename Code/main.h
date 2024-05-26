#ifndef _MAIN_H_
#define _MAIN_H_

#include "combat.h"
#include "utils.h"
#include "story.h"
#include "structures.h"

void new_game();

void swap(Scenario** a, Scenario** b);

int partition(Scenario* scenarios[], int low, int high);

void quickSort(Scenario* scenarios[], int low, int high);

Scenario* binary_search_scenario(Scenario* scenarios[], int num_scenarios, char* name);

int menu();

int main();

#endif
