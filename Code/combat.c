#include "Combat.h"

// SKILLS_CHARACTER -----------------------------------------------------------------
// Function to draw a progress bar with square symbols
void drawProgressBar(int current, int max) {
    const int barWidth = 20;
    int numSquares = (double)current / max * barWidth;
    printf("[");
    for (int i = 0; i < barWidth; ++i) {
        if (i < numSquares) {
            printf("■"); // Square symbol
        } else {
            printf(" "); // Empty space
        }
    }
    printf("] %d/%d\n", current, max);
}


void apply_effects(int* hp, int max_hp, int* def, int* atk, float hp_mod, float def_mod, float atk_mod, float multiplier, char* color) {
    int initial_hp = *hp;
    *hp = fmin(max_hp, *hp + multiplier * hp_mod);
    *def += multiplier * def_mod;
    *atk += multiplier * atk_mod;

    int hp_gained = *hp - initial_hp;
    if (hp_mod != 0 || def_mod != 0 || atk_mod != 0) {
        printf("%s", color);
        if (hp_mod != 0) {
            printf("%.1f HP ", hp_gained);
        }
        if (def_mod != 0) {
            printf("%.1f DEF ", multiplier * def_mod);
        }
        printf("gained%s\n", RESET);
    }
}

int calculate_damage(int atk, int def, float multiplier, char* color) {
    int damage = fmax(0, multiplier * atk - def);
    printf("%s%d Damage Dealt%s\n", color, damage, RESET);
    return damage;
}

void use_skill(Character* player, Enemy* enemy, Skills* skill, int is_enemy) {
    int damage = 0;
    float multiplier = is_enemy ? enemy->multiplier_skill : 1.0;
    int *hp, *atk, *def;
    int max_hp;
    char *color;

    if (is_enemy) {
        hp = &enemy->hp;
        atk = &enemy->atk;
        def = &enemy->def;
        max_hp = enemy->max_hp;
        color = RED;
    } else {
        hp = &player->hp;
        atk = &player->atk;
        def = &player->def;
        max_hp = 200;
        color = BLUE;
    }

    printf("%s%s: ", color, skill->name);

    if (strcmp(skill->name, "Berserk Strike") == 0) {
        damage = calculate_damage(*atk, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);
        if (is_enemy) {
            player->hp -= damage;
        } else {
            enemy->hp -= damage;
        }

    } else if (strcmp(skill->name, "Iron Defense") == 0) {
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);

    } else if (strcmp(skill->name, "Regeneration") == 0) {
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);

    } else if (strcmp(skill->name, "Precision Strike") == 0) {
        damage = calculate_damage(*atk * 1.25, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);
        if (is_enemy) {
            player->hp -= damage;
        } else {
            enemy->hp -= damage;
        }

    } else if (strcmp(skill->name, "Rapid Assault") == 0) {
        damage = calculate_damage(*atk * 1.5, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);
        if (is_enemy) {
            player->hp -= damage;
        } else {
            enemy->hp -= damage;
        }

    } else if (strcmp(skill->name, "Shield Bash") == 0) {
        damage = calculate_damage(*atk + 0.5 * *def, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);
        if (is_enemy) {
            player->hp -= damage;
        } else {
            enemy->hp -= damage;
        }

    } else if (strcmp(skill->name, "Vital Strike") == 0) {
        float strike_multiplier = (rand() % 2 == 0) ? 2.0 : 0.2;
        damage = calculate_damage(*atk, is_enemy ? player->def : enemy->def, strike_multiplier * multiplier, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, multiplier, color);
        if (is_enemy) {
            player->hp -= damage;
        } else {
            enemy->hp -= damage;
        }
        
    // Soul Skills -------------------------------------------------------------------------------------
    } else if (strcmp(skill->name, "Soul Strike") == 0 && !is_enemy && player->soul >= 50) {
        damage = calculate_damage(*atk + *atk * (player->soul / 100.0), enemy->def, 1.0, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, 1, color);
        enemy->hp -= damage;
        player->soul -= 50; // The soul skills cost SOUL

    } else if (strcmp(skill->name, "Guardian's Shield") == 0 && !is_enemy && player->soul >= 50) {
        apply_effects(hp, max_hp, def, atk, skill->hp_mod * player->soul, skill->def_mod, skill->atk_mod, 1, color);
        player->soul -= 50; // The soul skills cost SOUL

    } else if (strcmp(skill->name, "Soul Armour") == 0 && !is_enemy && player->soul >= 50) {
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod * player->soul, skill->atk_mod, 1, color);
        player->soul -= 50; // The soul skills cost SOUL

    } else if (strcmp(skill->name, "Soul Infusion") == 0 && !is_enemy && player->soul >= 50) {
        damage = calculate_damage((player->soul / 100.0) * (player->soul / 100.0) * *atk, enemy->def, 1.0, color);
        apply_effects(hp, max_hp, def, atk, skill->hp_mod, skill->def_mod, skill->atk_mod, 1, color);
        enemy->hp -= damage;
        player->soul -= 50; // The soul skills cost SOUL
    }

    printf("%sPlayer HP: ", BLUE);
    drawProgressBar(player->hp, 200);
    printf("%sEnemy HP: ", RED);
    drawProgressBar(enemy->hp, enemy->max_hp);
    printf("%s\n", RESET);
}

// QUEUE IMPLEMENTATION ---------------------------------------------------------------------
// Function to create a new node
Node* newNode(int data) {
    Node* temp = (Node*)malloc(sizeof(Node));
    temp->data = data;
    temp->next = NULL;
    return temp;
}

// Function to create an empty queue
Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

// Function to add an item to the queue
void enqueue(Queue* q, int data) {
    Node* temp = newNode(data);
    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

// Function to remove an item from the queue
int dequeue(Queue* q) {
    if (q->front == NULL) {
        printf("Queue is empty\n");
        return -1;
    }
    Node* temp = q->front;
    int data = temp->data;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(temp);
    return data;
}

// Function to check if the queue is empty
int isEmpty(Queue* q) {
    return q->front == NULL;
}

// Function to display the queue
void displayQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    Node* temp = q->front;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n"); //womp womp (Tribut a Aguilar)
}

//COMBAT FUNCTIONS ----------------------------------------------------------------
void fill_fight_queue(Queue* q, int velocity, int turns) {
    int player_probability = 500 + velocity;
    srand(time(NULL)); // Seed the random number generator
    printf("Turn Queue Generated, Chance of player turn %0.1f%%\n", (player_probability) / 10.0);

    for (int i = 0; i < turns; i++) {
        int r = rand() % 1000;
        if (r < player_probability) {
            enqueue(q, 0); // Player's turn
        } else {
            enqueue(q, 1); // Enemy's turn
        }
    }
}

// Function to show player and enemy stats
void view_stats(Character *player, Enemy *enemy) {
    printf("%-20s%-10s%-10s%-10s%-10s\n", "Name", "HP", "ATK", "DEF", "SOUL");
    printf("%s%-20s%-10d%-10d%-10d%-10d\n", BLUE, player->name, player->hp, player->atk, player->def, player->soul);
    printf("%s%-20s%-10d%-10d%-10d%-10s%s\n", RED, enemy->name, enemy->hp, enemy->atk, enemy->def, "-", RESET);
}

void player_turn(Character* player, Enemy* enemy, Session* session) {
    int skill_idx;
    int valid_input = 0; // Variable to check if input is valid

    // Display the 4 skills
    printf("%s0) View Stats\n%s", BOLD, RESET);
    for (int i = 0; i < 4; i++) {
        if (player->character_skills[i].type == 0) {
            printf("%s%d) %s => %s %s\n", BOLD, i + 1, player->character_skills[i].name, RESET, player->character_skills[i].effect);
        } else {
            printf("%s%d) %s (40 SOUL) => %s %s\n", BOLD, i + 1, player->character_skills[i].name, RESET, player->character_skills[i].effect);
        }
    }

    while (!valid_input) {
        printf("Choose a skill: ");
        if (scanf("%d", &skill_idx) != 1) {
            // If scanf() does not read an integer, clear the input buffer
            while (getchar() != '\n');
            printf("Invalid input\n");
        } else if (skill_idx < 0 || skill_idx > 4) {
            printf("Invalid choice\n");
        } else {
            valid_input = 1;
        }
    }

    // Check if the skill is one of the character's skills
    if (skill_idx == 0) {
        view_stats(player, enemy);
        player_turn(player, enemy,session);
    } else if (skill_idx >= 1 && skill_idx <= 4) {
        use_skill(player, enemy, &player->character_skills[skill_idx-1], 0);
        HashNode *skill_node=find_node(session->hash_skills,player->character_skills[skill_idx-1].name);
        skill_node->uses++;
        printf("%s has ben used %d times.\n",skill_node->skill.name,skill_node->uses );
    }
}


void enemy_turn(Character* player, Enemy* enemy) {
    printf("%s%s's Turn\n", RED, enemy->name);
    use_skill(player, enemy, &enemy->skills[rand() % 4], 1); // Choose a skill at random
}

int fight(Character* player, Enemy enemy, Session* session){ 
    printf("\nPreparing for battle...\n");
    sleep(2); // 5-second delay
    Queue* q = createQueue(); // Create an empty queue
    fill_fight_queue(q, player->vel, enemy.turns);
    int base_atk = player->atk;
    int base_def = player->def;
    int base_atk_e = enemy.atk;

    for(int i = 0; i<enemy.turns;i++){
        printf("%s Turn %d/%d %s\n", INVERT, i+1, enemy.turns, RESET);
        if(dequeue(q) == 0){ // PLAYER TURN
            player->soul += (player->soul <= 100) ? 10 : 0; // Add 10 SOUL each turn
            printf("SOUL AVAILABLE:  %d\n", player->soul);
            player->atk = base_atk; // Reset ATK each turn
            player_turn(player, &enemy, session);
        }else{ // ENEMY TURN
            enemy.atk = base_atk_e; // Reset enemy ATK each turn
            enemy_turn(player, &enemy); 
        } sleep(1); // Wait between turns, to give time for the player to understand what's happening

        if(enemy.hp <= 0){
            printf(YELLOW BOLD"__  __               _       __          \n"
                   "\\ \\/ /___  __  __   | |     / /___  ____ \n"
                   " \\  / __ \\/ / / /   | | /| / / __ \\/ __ \\\n"
                   " / / /_/ / /_/ /    | |/ |/ / /_/ / / / /\n"
                   "/_/\\____/\\__,_/     |__/|__/\\____/_/ /_/ %s\n", RESET);
            player->def = base_def; // Reset DEF each fight
            return 0; // player win
        }else if (player->hp <= 0){
            break;
        }
    }
printf(RED BOLD" __  __               __               __ \n"
       "\\ \\/ /___  __  __   / /   ____  _____/ /_\n"
       " \\  / __ \\/ / / /  / /   / __ \\/ ___/ __/\n"
       " / / /_/ / /_/ /  / /___/ /_/ (__  ) /_  \n"
       "/_/\\____/\\__,_/  /_____\\____/____/\\__/ %s\n", RESET);
    player->def = base_def; // Reset DEF each fight
    return 1; // player loss
}
