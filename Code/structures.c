#include "structures.h"

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

void insert_skill(HashTable* hashTable, Skill skill) {
    unsigned int index = hash(skill.name);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->key, skill.name);
    newNode->skill = skill;
    newNode->next = hashTable->table[index];
    hashTable->table[index] = newNode;
    hashTable->size++;
}

Skill* find_skill(HashTable* hashTable, char* name) {
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


//Load enemies from the Jason
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
        Skill currentskill;
        if (cJSON_IsString(name) && name->valuestring != NULL) {
            strncpy(currentskill.name, name->valuestring, MAX_NAME - 1);
            currentskill.name[MAX_NAME - 1] = '\0';
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

//Load skills from the Jason
/*void enemy_loader(Enemy enemies_array[], HashTable* hash_skills){
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

    for(int i = 0; i<MAX_ENEMIES; i++){
        cJSON* enemy = cJSON_GetArrayItem(enemies, i);
        if (enemy == NULL) continue;

        cJSON* name = cJSON_GetObjectItemCaseSensitive(enemy, "name");
        cJSON* hp = cJSON_GetObjectItemCaseSensitive(enemy, "hp");
        cJSON* atk = cJSON_GetObjectItemCaseSensitive(enemy, "atk");
        cJSON* def = cJSON_GetObjectItemCaseSensitive(enemy, "def");
        cJSON* turns = cJSON_GetObjectItemCaseSensitive(enemy, "turns");
        Enemy current_enemy;
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
        cJSON* enemy_skill_array = cJSON_GetObjectItemCaseSensitive(enemy, "skills");
        int size = cJSON_GetArraySize(enemy_skill_array);
        for(int j=0; j<size; j++){
            cJSON* skill = cJSON_GetArrayItem(enemy_skill_array, j);
            printf("\nTESTING: %s\n",skill->valuestring);
            Skills* findSkill = find_skill(hash_skills,skill->valuestring);
            current_enemy.skills[j]=*findSkill;
        }

        enemies_array[i]=current_enemy;
    }

    cJSON_Delete(json);
}*/

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
                    Skill* findSkill = find_skill(hash_skills, skill->valuestring);
                    if (findSkill != NULL) {
                        current_enemy.skills[j] = *findSkill;
                    }
                    else{
                        printf("SKILL ERROR");
                    }
                }
            }
        }
        printf("ENEMY FLOAT: %f", current_enemy.multiplier_skill);

        enemies_array[i] = current_enemy;
    }

    cJSON_Delete(json);
}


//Load scenarios from the Jason
/*void scene_loader(Session* session){
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
    int esc_size = cJSON_GetArraySize(scenarios);
    Scenario array_Scenarios[esc_size];
    Scenario current_scenario;
    Option current_option;
    Decision current_decision;
    for(int i = 0; i<MAX_SCENARIOS; i++){
        cJSON* scene = cJSON_GetArrayItem(scenarios, i);
        if (scene == NULL) continue;
        cJSON* name = cJSON_GetObjectItemCaseSensitive(scene, "name");
        cJSON* description = cJSON_GetObjectItemCaseSensitive(scene, "description");
        cJSON* decisions = cJSON_GetObjectItemCaseSensitive(scene, "decisions");
        //Allocate memory for the decision array
        int dec_size = cJSON_GetArraySize(decisions);
        current_scenario.decisions=(Decision*)malloc(dec_size*sizeof(Decision));
        for(int j=0; j<dec_size;j++){
            cJSON* decision = cJSON_GetArrayItem(decisions, j);;
            cJSON* question = cJSON_GetObjectItemCaseSensitive(decision, "question");
            cJSON* number_options = cJSON_GetObjectItemCaseSensitive(decision, "n_options");

            cJSON* options = cJSON_GetObjectItemCaseSensitive(decision, "options");
            //Allocate space for the options
            current_decision.options=(Option*)malloc(number_options->valueint*sizeof(Option));
            for(int k=0; k<number_options->valueint;k++){
                cJSON* option = cJSON_GetArrayItem(options, k);

                cJSON* response = cJSON_GetObjectItemCaseSensitive(option, "response");
                printf("\nTESTING: %s\n", response->valuestring);

                cJSON* previous = cJSON_GetObjectItemCaseSensitive(option, "previous");
                cJSON* after = cJSON_GetObjectItemCaseSensitive(option, "after");
                cJSON* enemies = cJSON_GetObjectItemCaseSensitive(option, "enemies");
                int en_size = cJSON_GetArraySize(enemies);
                //Create the enemies array
                int enemies_array_option[en_size];
                current_option.enemies=(Enemy*)malloc(en_size*sizeof(Enemy));
                for(int l = 0; l<en_size;l++){
                    cJSON* enemie = cJSON_GetArrayItem(enemies, l);
                    current_option.enemies[l]=session->enemies[enemie->valueint];
                }
                //PUT THE VALUES INTO THEIR POSITION
                strncpy(current_option.response, response->valuestring, MAX_LENGTH - 1);
                current_option.response[MAX_LENGTH - 1] = '\0';
                strncpy(current_option.previous_narrative, previous->valuestring, MAX_LENGTH - 1);
                current_option.previous_narrative[MAX_LENGTH - 1] = '\0';
                strncpy(current_option.after_narrative, after->valuestring, MAX_LENGTH - 1);
                current_option.after_narrative[MAX_LENGTH - 1] = '\0';
                //PUT THE OPTION IN THE DECISION
                current_decision.options[k]=current_option;
                free(current_option.enemies);
            }
            //Set all the other values of the decision
            strncpy(current_decision.question_text, question->valuestring, MAX_LENGTH - 1);
            current_decision.question_text[MAX_LENGTH - 1] = '\0';
            current_decision.options_number=number_options->valueint;
            //PUT THE DECISION IN THE SCENARIO
            current_scenario.decisions[j]=current_decision;
            free(current_decision.options);
        }
        strncpy(current_scenario.name, name->valuestring, MAX_NAME - 1);
        current_scenario.name[MAX_NAME - 1] = '\0';
        strncpy(current_scenario.description, description->valuestring, MAX_LENGTH - 1);
        current_scenario.description[MAX_LENGTH - 1] = '\0';
        array_Scenarios[i]=current_scenario;

    }
session->first_Scenario=array_Scenarios[0];
cJSON_Delete(json);
}*/





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

        if (name == NULL || description == NULL || decisions == NULL || !cJSON_IsArray(decisions)) continue;

        Scenario* current_scenario = (Scenario*)malloc(sizeof(Scenario));
        if (current_scenario == NULL) {
            printf("Memory allocation failed\n");
            continue;
        }

        strncpy(current_scenario->name, name->valuestring, MAX_NAME - 1);
        current_scenario->name[MAX_NAME - 1] = '\0';
        strncpy(current_scenario->description, description->valuestring, MAX_LENGTH - 1);
        current_scenario->description[MAX_LENGTH - 1] = '\0';
        current_scenario->Next = NULL;
        current_scenario->Previous = previousScenario;

        int dec_size = cJSON_GetArraySize(decisions);
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
            if (number_options == NULL) {
                number_options = cJSON_GetObjectItemCaseSensitive(decision, "number"); // handle "number" field
            }
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

        if (previousScenario == NULL) {
            firstScenario = current_scenario;
        } else {
            previousScenario->Next = current_scenario;
        }
        previousScenario = current_scenario;
    }

    session->first_Scenario = *firstScenario;

    cJSON_Delete(json);
}




//Function to print the possible skills to choose from
void show_skills(Skill skills[],int size){
    for(int i = 0; i<size; i++){
        printf("%d: %s=>%s\n",i+1,skills[i].name,skills[i].description);
    }
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
    scanf("%s", player.name); // Set the input as the character's name
    printf("\n");
    /*for(int i = 0; i<PLAYER_SKILLS; i++){
        printf("Choose the player's skills: \n");
        for (int j = 0; j < MAX_SKILLS; j++) {
            HashNode* node = session->hash_skills->table[j];
            Skills skill = node->skill;
            printf("%d: %s=>%s\n",j+1,skill.name,skill.description);
        }
        int option = read_int();
        while(option>=MAX_SKILLS||option<=0){
            printf("Please enter a valid option!");
            option = read_int();
            }
            player.character_skills[i]= session->hash_skills->table[option-1]->skill;

        }*/
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
                    printf("%d: %s\n\tDescription: %s\n\tEffect: %s\n\t", count + 1, node->skill.name, node->skill.description, node->skill.effect);
                }
                count++;
                node = node->next;
            }
        }

        if (count == 0) {
            printf("No skills available.\n");
            break;
        }

        int option = read_int();

        while (option < 1 || option > count || selected[option - 1]) {
            printf("Please enter a valid option: ");
            option = read_int();
        }

        selected[option - 1] = true; // Mark the selected skill as chosen
        player.character_skills[i] = skill_map[option - 1]->skill;
    }

    printf("Your skills are: \n");
    for(int i = 0; i<PLAYER_SKILLS; i++){
        printf("%s ",player.character_skills[i].name);
    }
    
    return player;
}