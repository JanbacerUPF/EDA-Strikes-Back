#include "combat.h"
#include "story.h"
#include "testsuit.h"

#define TEST_CODE 7357
#define NEW_GAME 1
#define LOAD_GAME 2
#define EXIT 3

void save_session_to_file(Session* session);

void play_game(Session* current_game);

int menu();