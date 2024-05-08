#include "combat.h"

bool is_finished(Character* player, Enemy* enemies) {
    if (player->hp != 0 && enemies->hp == 0) {
        return true;
    }
    return false;
}