#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "structures.h"

bool is_finished(Character* player, Enemy* enemies[]);

void combats(Character* player, Enemy enemies[], int number_enemies);

#endif 
