#include "Combat.h"


// STACK IMPLEMENTATION ------------------------------------------------------------
// Function to push a skill onto the stack
void push(StackNode** top, Skills* skill) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    newNode->skill = skill;
    newNode->next = *top;
    *top = newNode;
}


// Function to pop a skill from the stack
Skills* pop(StackNode** top) {
    if (*top == NULL) return NULL;
    StackNode* temp = *top;
    *top = (*top)->next;
    Skills* poppedSkill = temp->skill;
    free(temp);
    return poppedSkill;
}


// Function to get the k-th skill from the stack
Skills* get_kth_skill(StackNode* top, int k) {
    StackNode* current = top;
    for (int i = 0; i < k && current != NULL; i++) {
        current = current->next;
    }
    return (current != NULL) ? current->skill : NULL;
}

int stack_size(StackNode* top) {
    int size = 0;
    StackNode* current = top;
    while (current != NULL) {
        size++;
        current = current->next;
    }
    return size;
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


void apply_effects(Character* player, Enemy* enemy, Skills* skill, float multiplier, char* color, int is_enemy) {
    int *hp, *def, *atk, *soul;
    int max_hp;


    if (is_enemy) {
        hp = &enemy->hp;
        atk = &enemy->atk;
        def = &enemy->def;
        soul = NULL;
        max_hp = enemy->max_hp;
    } else {
        hp = &player->hp;
        atk = &player->atk;
        def = &player->def;
        soul = &player->soul;
        max_hp = 500;
    }


    int initial_hp = *hp;
    *hp = fmin(max_hp, *hp + multiplier * skill->hp_mod);
    *def += multiplier * skill->def_mod;
    *atk += multiplier * skill->atk_mod;
    if (soul && skill->type) {
        *soul -= SOUL_COST;  // The soul skills cost SOUL
    }


    int hp_gained = *hp - initial_hp;
    if (skill->hp_mod != 0 || skill->def_mod != 0 || skill->atk_mod != 0) {
        printf("%s", color);
        if (skill->hp_mod != 0) {
            printf("%.1f HP ", hp_gained);
        }
        if (skill->def_mod != 0) {
            printf("%.1f DEF ", multiplier * skill->def_mod);
        }
        printf("gained%s\n", RESET);
    }
}


int calculate_damage(int atk, int def, float multiplier, char* color) {
    int damage = fmax(0, multiplier * atk - def);
    return damage;
}


void apply_damage(int* target_hp, int damage, char* color) {
    *target_hp -= damage;
    if (*target_hp < 0) {
        *target_hp = 0;
    }
    printf("%s%d Damage Dealt%s\n", color, damage, RESET);
}


void use_skill(Character* player, Enemy* enemy, Skills* skill, int is_enemy, int time_strike) {
    int damage = 0;
    float multiplier; // = is_enemy ? enemy->multiplier_skill : 1.0;
    char *color;


    if (is_enemy) {
        multiplier = enemy->multiplier_skill;
        color = RED;
    } else {
        multiplier  = time_strike ? 2.0 : 1.0;
        color = BLUE;
    }


    printf("%s%s: ", color, skill->name);


    if (strcmp(skill->name, "Berserk Strike") == 0) {
        damage = calculate_damage(is_enemy ? enemy->atk : player->atk, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);
        apply_damage(is_enemy ? &player->hp : &enemy->hp, damage, color);


    } else if (strcmp(skill->name, "Iron Defense") == 0) {
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);


    } else if (strcmp(skill->name, "Regeneration") == 0) {
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);


    } else if (strcmp(skill->name, "Precision Strike") == 0) {
        damage = calculate_damage((is_enemy ? enemy->atk : player->atk) * 1.25, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);
        apply_damage(is_enemy ? &player->hp : &enemy->hp, damage, color);


    } else if (strcmp(skill->name, "Rapid Assault") == 0) {
        damage = calculate_damage((is_enemy ? enemy->atk : player->atk) * 1.5, is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);
        apply_damage(is_enemy ? &player->hp : &enemy->hp, damage, color);


    } else if (strcmp(skill->name, "Shield Bash") == 0) {
        damage = calculate_damage((is_enemy ? enemy->atk : player->atk) + 0.5 * (is_enemy ? enemy->def : player->def), is_enemy ? player->def : enemy->def, multiplier, color);
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);
        apply_damage(is_enemy ? &player->hp : &enemy->hp, damage, color);


    } else if (strcmp(skill->name, "Vital Strike") == 0) {
        float strike_multiplier = (rand() % 2 == 0) ? 2.0 : 0.2;
        damage = calculate_damage(is_enemy ? enemy->atk : player->atk, is_enemy ? player->def : enemy->def, strike_multiplier * multiplier, color);
        apply_effects(player, enemy, skill, multiplier, color, is_enemy);
        apply_damage(is_enemy ? &player->hp : &enemy->hp, damage, color);


    // Soul Skills -------------------------------------------------------------------------------------
    } else if (strcmp(skill->name, "Soul Strike") == 0 && !is_enemy && player->soul >= SOUL_COST) {
        damage = calculate_damage(player->atk + player->atk * (player->soul / 100.0), enemy->def, 1.0, color);
        apply_effects(player, enemy, skill, 1, color, is_enemy);
        apply_damage(&enemy->hp, damage, color);


    } else if (strcmp(skill->name, "Guardian's Shield") == 0 && !is_enemy && player->soul >= SOUL_COST) {
        apply_effects(player, enemy, skill, 1, color, is_enemy);


    } else if (strcmp(skill->name, "Soul Armour") == 0 && !is_enemy && player->soul >= SOUL_COST) {
        apply_effects(player, enemy, skill, 1, color, is_enemy);


    } else if (strcmp(skill->name, "Soul Infusion") == 0 && !is_enemy && player->soul >= SOUL_COST) {
        damage = calculate_damage((player->soul / 100.0) * (player->soul / 100.0) * player->atk, enemy->def, 1.0, color);
        apply_effects(player, enemy, skill, 1, color, is_enemy);
        apply_damage(&enemy->hp, damage, color);
    }


    printf("%sPlayer HP: ", BLUE);
    drawProgressBar(player->hp, 500);
    printf("%sEnemy HP: ", RED);
    drawProgressBar(enemy->hp, enemy->max_hp);
    printf("%s", RESET);
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


void player_turn(Character* player, Enemy* enemy, Session* session, StackNode** skill_stack, int* time_strike_used) {
    int skill_idx;
    int valid_input = 0; // Variable to check if input is valid


    // Display the 4 skills
    printf("%s0) View Stats\n%s", BOLD, RESET);
    for (int i = 0; i < 4; i++) {
        if (player->character_skills[i].type == 0) {
            printf("%s%d) %s => %s %s\n", BOLD, i + 1, player->character_skills[i].name, RESET, player->character_skills[i].effect);
        } else {
            printf("%s%d) %s (%d SOUL) => %s %s\n", BOLD, i + 1, player->character_skills[i].name, SOUL_COST, RESET, player->character_skills[i].effect);
        }
    }
    // Display Time Strike if not used
    if (!(*time_strike_used)) {
        printf("%s5) Time Strike => Reuse a previous skill with double power%s\n", BOLD, RESET);
    }


    while (!valid_input) {
        printf("Choose a skill: ");
        if (scanf("%d", &skill_idx) != 1) {
            // If scanf() does not read an integer, clear the input buffer
            while (getchar() != '\n');
            printf("Invalid input\n");
        } else if (skill_idx < 0 || skill_idx > 5 || (skill_idx == 5 && *time_strike_used)) {
            printf("Invalid choice\n");
        } else {
            valid_input = 1;
        }
    }


    // Check if the skill is one of the character's skills
    if (skill_idx == 0) { // View stats
        view_stats(player, enemy);
        player_turn(player, enemy, session, skill_stack, time_strike_used);
    } else if (skill_idx == 5) { // Time Strike
        int k = stack_size(*skill_stack) ? rand() % stack_size(*skill_stack) : -1; // Randomly select k
        Skills* selected_skill = get_kth_skill(*skill_stack, k);
        if (selected_skill != NULL) {
            printf("Time Strike activates: Reusing %s with double power!\n", selected_skill->name);
            use_skill(player, enemy, selected_skill, 0, 1);
            *time_strike_used = 1; // Mark Time Strike as used
        } else {
            printf("No previous skills to reuse.\n");
            player_turn(player, enemy, session, skill_stack, time_strike_used);
        }
    } else if (player->character_skills[skill_idx - 1].type == 1 && player->soul < SOUL_COST) { // Not enough SOUL
        printf("Not enough SOUL\n");
        player_turn(player, enemy, session, skill_stack, time_strike_used);
    } else if (skill_idx >= 1 && skill_idx <= 4) { // valid skill
        use_skill(player, enemy, &player->character_skills[skill_idx - 1], 0, 0);
        push(skill_stack, &player->character_skills[skill_idx - 1]); // Push used skill onto stack
        HashNode* skill_node = find_node(session->hash_skills, player->character_skills[skill_idx - 1].name);
        skill_node->uses++;
        printf("%s has been used %d times.\n\n", skill_node->skill.name, skill_node->uses);
    }
}


void enemy_turn(Character* player, Enemy* enemy) {
    printf("%s%s's Turn\n", RED, enemy->name);
    use_skill(player, enemy, &enemy->skills[rand() % 4], 1, 0); // Choose a skill at random
    printf("\n");
}


int fight(Character* player, Enemy enemy, Session* session) {
    printf("\nPreparing for battle...\n");
    sleep(2); // 5-second delay
    Queue* q = createQueue(); // Create an empty queue
    fill_fight_queue(q, player->vel, enemy.turns);
    int base_atk = player->atk;
    int base_def = player->def;
    int base_atk_e = enemy.atk;


    StackNode* skill_stack = NULL; // Initialize the skill stack
    int time_strike_used = 0; // Initialize the Time Strike usage flag


    for (int i = 0; i < enemy.turns; i++) {
        printf("%s Turn %d/%d %s\n", INVERT, i + 1, enemy.turns, RESET);
        if (dequeue(q) == 0) { // PLAYER TURN
            player->soul += (player->soul <= 100) ? 10 : 0; // Add 10 SOUL each turn
            printf("SOUL AVAILABLE: %d\n", player->soul);
            player->atk = base_atk; // Reset ATK each turn
            player_turn(player, &enemy, session, &skill_stack, &time_strike_used);
        } else { // ENEMY TURN
            enemy.atk = base_atk_e; // Reset enemy ATK each turn
            enemy_turn(player, &enemy);
        }
        sleep(1); // Wait between turns, to give time for the player to understand what's happening


        if (enemy.hp <= 0) {
            printf(YELLOW BOLD"__  __               _       __          \n"
                   "\\ \\/ /___  __  __   | |     / /___  ____ \n"
                   " \\  / __ \\/ / / /   | | /| / / __ \\/ __ \\\n"
                   " / / /_/ / /_/ /    | |/ |/ / /_/ / / / /\n"
                   "/_/\\____/\\__,_/     |__/|__/\\____/_/ /_/ %s\n", RESET);
            player->def = base_def; // Reset DEF each fight
            return 0; // player win
        } else if (player->hp <= 0) {
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