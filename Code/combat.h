#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "structures.h"
#include "common.h"
#include "utils.h"

bool is_finished(Character* player, Enemy* enemies[]);

bool is_game_over(Character* player, Enemy* enemy);

void combats(Character* player, Enemy enemies[], int number_enemies);

#endif

