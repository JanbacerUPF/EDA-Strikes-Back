#include "structures.h"
#include "story.h"

//Dictionary of skills
unsigned int hash(char *str) {
    unsigned int hash = 0;
    while (*str) {
        hash = (hash << 5) + *str++;
    }
    return hash % MAX_SKILLS;
}

HashTable* create_table_skills() {
    HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < MAX_SKILLS; i++) {
        hashTable->table[i] = NULL;
    }
    hashTable->size = 0;
    return hashTable;
}

void insert_skill(HashTable* hashTable, Skills skill) {
    unsigned int index = hash(skill.name);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->key, skill.name);
    newNode->skill = skill;
    newNode->uses=0;
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
    hashTable->size++;
}

Skills* find_skill(HashTable* hashTable, char* name) {
    unsigned int index = hash(name);
    HashNode* node = hashTable->table[index];
    while (node != NULL) {
        if (strcmp(node->key, name) == 0) {
            return &node->skill;
        }
        node = node->next;
    }
    return NULL;
}

HashNode* find_node(HashTable* hashTable, char* name) {
    unsigned int index = hash(name);
    HashNode* node = hashTable->table[index];
    while (node != NULL) {
        if (strcmp(node->key, name) == 0) {
            return node;
        }
        node = node->next;
    }
    return NULL;
}

void delete_skill(HashTable* hashTable, char* name) {
    unsigned int index = hash(name);
    HashNode* node = hashTable->table[index];
    HashNode* prev = NULL;
    while (node != NULL && strcmp(node->key, name) != 0) {
        prev = node;
        node = node->next;
    }
    if (node == NULL) return; // Not found
    if (prev == NULL) {
        hashTable->table[index] = node->next;
    } else {
        prev->next = node->next;
    }
    free(node);
    hashTable->size--;
}

void free_table(HashTable* hashTable) {
    for (int i = 0; i < MAX_SKILLS; i++) {
        HashNode* node = hashTable->table[i];
        while (node != NULL) {
            HashNode* temp = node;
            node = node->next;
            free(temp);
        }
    }
    free(hashTable);
}


//Load skills from the Jason
void skill_loader(HashTable* hashTable){
    FILE *fp = fopen("presets.json", "r");

    if (fp == NULL) { 
        printf("ERROR: Could not open file.\n"); 
        return; 
    } 

    // Determine the file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory to read the file
    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    fread(fileContent, 1, fileSize, fp);
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(fp);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    cJSON* skills = cJSON_GetObjectItemCaseSensitive(json, "skills");

    for(int i = 0; i<MAX_SKILLS; i++){
        cJSON* skill = cJSON_GetArrayItem(skills, i);
        if (skill == NULL) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(skill, "name");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(skill, "description");
        cJSON* type = cJSON_GetObjectItemCaseSensitive(skill, "type");
        cJSON* hp_mod = cJSON_GetObjectItemCaseSensitive(skill, "hp_mod");
        cJSON* atk_mod = cJSON_GetObjectItemCaseSensitive(skill, "atk_mod");
        cJSON* def_mod = cJSON_GetObjectItemCaseSensitive(skill, "def_mod");
        cJSON* effect = cJSON_GetObjectItemCaseSensitive(skill, "effect");
        Skills currentskill;
        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(currentskill.name, name->valuestring, MAX_NAME - 1);
            currentskill.name[MAX_NAME - 1] = '\0';
        }
        if (cJSON_IsString(effect) && effect->valuestring != NULL) {
            strncpy(currentskill.effect, effect->valuestring, MAX_LENGTH - 1);
            currentskill.effect[MAX_LENGTH - 1] = '\0';
        }

        if (cJSON_IsString(description) && description->valuestring != NULL) {
            strncpy(currentskill.description, description->valuestring, MAX_LENGTH - 1);
            currentskill.description[MAX_LENGTH - 1] = '\0';
        }

        if (cJSON_IsNumber(type)) {
            currentskill.type = type->valueint;
        }


        if (cJSON_IsNumber(hp_mod)) {
            currentskill.hp_mod = hp_mod->valueint;
        }

        if (cJSON_IsNumber(atk_mod)) {
            currentskill.atk_mod = atk_mod->valueint;
        }

        if (cJSON_IsNumber(def_mod)) {
            currentskill.def_mod = def_mod->valueint;
        }

        insert_skill(hashTable, currentskill);
    }

    cJSON_Delete(json);
}

//Load enemies from the Jason
void enemy_loader(Enemy enemies_array[], HashTable* hash_skills) {
    FILE *fp = fopen("presets.json", "r");

    if (fp == NULL) {
        printf("ERROR: Could not open file.\n");
        return;
    }

    // Determine the file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory to read the file
    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    fread(fileContent, 1, fileSize, fp);
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(fp);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    cJSON* enemies = cJSON_GetObjectItemCaseSensitive(json, "enemies");
    if (enemies == NULL || !cJSON_IsArray(enemies)) {
        printf("Invalid JSON format\n");
        cJSON_Delete(json);
        return;
    }

    int num_enemies = cJSON_GetArraySize(enemies);
    for (int i = 0; i < num_enemies && i < MAX_ENEMIES; i++) {
        cJSON* enemy = cJSON_GetArrayItem(enemies, i);
        if (enemy == NULL) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(enemy, "name");
        cJSON* hp = cJSON_GetObjectItemCaseSensitive(enemy, "hp");
        cJSON* atk = cJSON_GetObjectItemCaseSensitive(enemy, "atk");
        cJSON* def = cJSON_GetObjectItemCaseSensitive(enemy, "def");
        cJSON* turns = cJSON_GetObjectItemCaseSensitive(enemy, "turns");
        cJSON* enemy_skill_array = cJSON_GetObjectItemCaseSensitive(enemy, "skills");
        cJSON* multiplier_skill = cJSON_GetObjectItemCaseSensitive(enemy, "multiplier_skill");
        


        Enemy current_enemy = {0}; // Initialize the structure to zero

        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(current_enemy.name, name->valuestring, MAX_NAME - 1);
            current_enemy.name[MAX_NAME - 1] = '\0';
        }

        if (cJSON_IsNumber(hp)) {
            current_enemy.hp = hp->valueint;
        }

        if (cJSON_IsNumber(atk)) {
            current_enemy.atk = atk->valueint;
        }

        if (cJSON_IsNumber(def)) {
            current_enemy.def = def->valueint;
        }

        if (cJSON_IsNumber(turns)) {
            current_enemy.turns = turns->valueint;
        }

        if (cJSON_IsNumber(multiplier_skill)) {
            current_enemy.multiplier_skill = multiplier_skill->valuedouble;
        }

        if (enemy_skill_array != NULL && cJSON_IsArray(enemy_skill_array)) {
            int skill_count = cJSON_GetArraySize(enemy_skill_array);
            for (int j = 0; j < skill_count && j < MAX_SKILLS; j++) {
                cJSON* skill = cJSON_GetArrayItem(enemy_skill_array, j);
                if (cJSON_IsString(skill) && skill->valuestring != NULL) {
                    Skills* findSkill = find_skill(hash_skills, skill->valuestring);
                    if (findSkill != NULL) {
                        current_enemy.skills[j] = *findSkill;
                    }
                    else{
                        printf("SKILL ERROR");
                    }
                }
            }
        }
        current_enemy.max_hp=current_enemy.hp;
        enemies_array[i] = current_enemy;
    }

    cJSON_Delete(json);
}


//Load scenarios from the JSON
void scene_loader(Session* session) {
    FILE *fp = fopen("scenarios.json", "r");

    if (fp == NULL) {
        printf("ERROR: Could not open file.\n");
        return;
    }

    // Determine the file size
    fseek(fp, 0, SEEK_END);
    long fileSize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    // Allocate memory to read the file
    char* fileContent = (char*)malloc(fileSize + 1);
    if (fileContent == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    fread(fileContent, 1, fileSize, fp);
    fileContent[fileSize] = '\0'; // Null-terminate the string
    fclose(fp);

    cJSON* json = cJSON_Parse(fileContent);
    free(fileContent);

    if (json == NULL) {
        printf("Error parsing JSON\n");
        return;
    }

    cJSON* scenarios = cJSON_GetObjectItemCaseSensitive(json, "scenarios");
    if (scenarios == NULL || !cJSON_IsArray(scenarios)) {
        printf("Invalid JSON format\n");
        cJSON_Delete(json);
        return;
    }

    Scenario* firstScenario = NULL;
    Scenario* previousScenario = NULL;

    int esc_size = cJSON_GetArraySize(scenarios);
    for (int i = 0; i < esc_size; i++) {
        cJSON* scene = cJSON_GetArrayItem(scenarios, i);
        if (scene == NULL) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(scene, "name");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(scene, "description");
        cJSON* decisions = cJSON_GetObjectItemCaseSensitive(scene, "decisions");
        cJSON* ID = cJSON_GetObjectItemCaseSensitive(scene, "ID");

        if (name == NULL || description == NULL || decisions == NULL || !cJSON_IsArray(decisions)) continue;

        Scenario* current_scenario = (Scenario*)malloc(sizeof(Scenario));
        if (current_scenario == NULL) {
            printf("Memory allocation failed\n");
            continue;
        }

        current_scenario->ID=ID->valueint;
        strncpy(current_scenario->name, name->valuestring, MAX_NAME - 1);
        current_scenario->name[MAX_NAME - 1] = '\0';
        strncpy(current_scenario->description, description->valuestring, MAX_LENGTH - 1);
        current_scenario->description[MAX_LENGTH - 1] = '\0';
        current_scenario->Next = NULL;
        current_scenario->Previous = previousScenario;

        int dec_size = cJSON_GetArraySize(decisions);
        current_scenario->dec_num=dec_size;
        current_scenario->decisions = (Decision*)malloc(dec_size * sizeof(Decision));
        if (current_scenario->decisions == NULL) {
            printf("Memory allocation failed\n");
            free(current_scenario);
            continue;
        }

        for (int j = 0; j < dec_size; j++) {
            cJSON* decision = cJSON_GetArrayItem(decisions, j);
            if (decision == NULL) continue;

            cJSON* question = cJSON_GetObjectItemCaseSensitive(decision, "question");
            cJSON* number_options = cJSON_GetObjectItemCaseSensitive(decision, "n_options");
            cJSON* options = cJSON_GetObjectItemCaseSensitive(decision, "options");

            if (question == NULL || number_options == NULL || options == NULL || !cJSON_IsArray(options)) continue;

            Decision current_decision;
            strncpy(current_decision.question_text, question->valuestring, MAX_LENGTH - 1);
            current_decision.question_text[MAX_LENGTH - 1] = '\0';
            current_decision.options_number = number_options->valueint;
            current_decision.options = (Option*)malloc(current_decision.options_number * sizeof(Option));
            if (current_decision.options == NULL) {
                printf("Memory allocation failed\n");
                continue;
            }

            for (int k = 0; k < current_decision.options_number; k++) {
                cJSON* option = cJSON_GetArrayItem(options, k);
                if (option == NULL) continue;

                cJSON* response = cJSON_GetObjectItemCaseSensitive(option, "response");
                cJSON* previous = cJSON_GetObjectItemCaseSensitive(option, "previous");
                cJSON* after = cJSON_GetObjectItemCaseSensitive(option, "after");
                cJSON* enemies = cJSON_GetObjectItemCaseSensitive(option, "enemies");

                if (response == NULL || previous == NULL || after == NULL || enemies == NULL || !cJSON_IsArray(enemies)) continue;

                Option current_option;
                strncpy(current_option.response, response->valuestring, MAX_LENGTH - 1);
                current_option.response[MAX_LENGTH - 1] = '\0';
                strncpy(current_option.previous_narrative, previous->valuestring, MAX_LENGTH - 1);
                current_option.previous_narrative[MAX_LENGTH - 1] = '\0';
                strncpy(current_option.after_narrative, after->valuestring, MAX_LENGTH - 1);
                current_option.after_narrative[MAX_LENGTH - 1] = '\0';

                int en_size = cJSON_GetArraySize(enemies);
                current_option.en_num=en_size;
                current_option.enemies = (Enemy*)malloc(en_size * sizeof(Enemy));
                if (current_option.enemies == NULL) {
                    printf("Memory allocation failed\n");
                    continue;
                }

                for (int l = 0; l < en_size; l++) {
                    cJSON* enemie = cJSON_GetArrayItem(enemies, l);
                    if (enemie != NULL && cJSON_IsNumber(enemie)) {
                        current_option.enemies[l] = session->enemies[enemie->valueint];
                    }
                }

                current_decision.options[k] = current_option;
                
            }

            current_scenario->decisions[j] = current_decision;
            
            
        }
        current_scenario->completed=0;
        if (previousScenario == NULL) {
            firstScenario = current_scenario;
        } else {
            previousScenario->Next = current_scenario;
        }
        previousScenario = current_scenario;
    }

    session->first_Scenario = firstScenario;

    cJSON_Delete(json);
}



void load_config(Session* session){
    session->hash_skills = create_table_skills();
    skill_loader(session->hash_skills);
    enemy_loader(session->enemies, session->hash_skills);
    scene_loader(session);
    

}

Character character_creation(Session* session){
    printf("Welcome to the character creation\n");
    Character player;
    player.vel=0;
    printf("Enter the character's name: ");
    scanf("%[^\n]s",player.name); // Set the input as the character's name
    while((getchar()) != '\n');

    printf("\n");
    player.hp=200;
    player.atk=50;
    player.def=50;
    player.soul=0;

    bool selected[MAX_SKILLS] = { false };  // Array to keep track of selected skills

    for (int i = 0; i < PLAYER_SKILLS; i++) {
        printf("Choose the player's skills: \n");

        int count = 0;
        HashNode* skill_map[MAX_SKILLS] = { 0 }; // Array to map displayed options to hash table nodes

        for (int j = 0; j < MAX_SKILLS; j++) {
            HashNode* node = session->hash_skills->table[j];
            while (node != NULL) {
                if (!selected[count]) { // Only display non-selected skills
                    skill_map[count] = node;
                    //printf("%d: %s => %s\n", count + 1, node->skill.name, node->skill.description);
                    printf("%s%d) %s => %s %s\n", BOLD, count + 1, node->skill.name, RESET, node->skill.description);
                }
                count++;
                node = node->next;
            }
        }

        if (count == 0) {
            printf("No skills available.\n");
            break;
        }

        int option;
        printf("Enter a valid option: ");
        scanf("%d", &option);

        while (option < 1 || option > count || selected[option - 1]) {
            printf("Please enter a valid option: ");
            scanf("%d", &option);
        }

        selected[option - 1] = true; // Mark the selected skill as chosen
        player.character_skills[i] = skill_map[option - 1]->skill;
        printf(CLEAR_SCREEN);
    }

    printf("Your skills are: \n");
    for(int i = 0; i<PLAYER_SKILLS; i++){
        printf("%s ",player.character_skills[i].name);
    }
    
    return player;
}


Character story_character_creation(Session* session){
    Intro intro= introduction();
    Character player;
    player.vel=0;
    printWrapped(intro.prev_name);
    printf(YELLOW BOLD"\nEnter the character's name: "RESET);
    scanf("%[^\n]s",player.name); // Set the input as the character's name
    while((getchar()) != '\n');

    printWrapped(intro.after_name);
    printf("\n");
    sleep(2);
    printWrapped(intro.prophecy);
    printf("\n");
    sleep(5);
    printWrapped(intro.last);
    sleep(3);

    printf(GREEN BOLD"What equipment do you choose?\n");
    printf(CYAN BOLD"\n1. Light Clothing:  500 HP, 80 ATK, 40 DEF, 80 VEL\n");
    printf(CYAN BOLD"\n2. Steel Armour:  500 HP, 60 ATK, 80 DEF, 50 VEL\n"RESET);
    int outfit;
    printf(YELLOW BOLD"Enter a valid option: ");
    scanf("%d", &outfit);

    while (outfit < 1 || outfit>2) {
        printf("Please enter a valid option: ");
        scanf("%d", &outfit);
    }

    if(outfit==1){
        player.hp=500;
        player.atk=80;
        player.def=40;
        player.vel=80;
        player.soul=0;
    }
    else if(outfit==2){
        player.hp=500;
        player.atk=60;
        player.def=80;
        player.vel=50;
        player.soul=0;
    }
    else{ //Default settings just in case
        player.hp=200;
        player.atk=50;
        player.def=50;
        player.vel=0;
        player.soul=0;
    }

    printf("\n");
    

    bool selected[MAX_SKILLS] = { false };  // Array to keep track of selected skills
    printf(RESET"Before starting your journey traveler, tell me, what skills make you special? \n");
    for (int i = 0; i < PLAYER_SKILLS; i++) {
        printf(GREEN UNDERLINE BOLD"\nChoose the player's skills: \n"RESET);

        int count = 0;
        HashNode* skill_map[MAX_SKILLS] = { 0 }; // Array to map displayed options to hash table nodes

        for (int j = 0; j < MAX_SKILLS; j++) {
            HashNode* node = session->hash_skills->table[j];
            while (node != NULL) {
                if (!selected[count]) { // Only display non-selected skills
                    skill_map[count] = node;
                    //printf("%d: %s => %s\n", count + 1, node->skill.name, node->skill.description);
                    printf("%s%d) %s => %s %s\n", BOLD, count + 1, node->skill.name, RESET, node->skill.description);
                }
                count++;
                node = node->next;
            }
        }

        if (count == 0) {
            printf("No skills available.\n");
            break;
        }

        int option;
        printf(YELLOW BOLD"Enter a valid option: ");
        scanf("%d", &option);

        while (option < 1 || option > count || selected[option - 1]) {
            printf("Please enter a valid option: ");
            scanf("%d", &option);
        }

        selected[option - 1] = true; // Mark the selected skill as chosen
        player.character_skills[i] = skill_map[option - 1]->skill;
        printf(CLEAR_SCREEN);
    }

    printf(RESET"Your skills are: \n");
    for(int i = 0; i<PLAYER_SKILLS; i++){
        printf("%s ",player.character_skills[i].name);
    }
    
    return player;
}

