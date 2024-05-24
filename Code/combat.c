#include "combat.h"
#define TURNS 12
#define PLAYER 0
#define ENEMY 1

/*QUEUE FUNCTIONS, TURNS ASSIGMENTS, COMBAT LOGIC*/

//CODE FOR QUEUES
typedef struct Queue{
    int* array;
    int *head;
    int *tail;
    int elements;
}Queue;

Queue init_queue(int N){
    Queue queue;
    queue.array=(int *)malloc(N*sizeof(int));
    queue.head=queue.array;
    queue.tail=queue.array;
    queue.elements=0;
    return queue;
}

bool is_empty(Queue q) {
    return (q.elements == 0);
}

bool is_full(Queue q, int N) {
    return (q.elements == N);
}

int first(Queue q) {
    if (is_empty(q)) {
        printf("Queue is empty\n");
    } else {
        return *(q.head);
    }
}

void enqueue(Queue *q, int element, int N) {
    //printf("elements:%d,N:%d\n",q->elements,N);
    if (is_full(*q, N)) {
        printf("Queue is full\n");
    } else {
        *(q->tail) = element;
        q->elements++;
        q->tail++;
        if (q->tail >= q->array + N) //Go around if the maximum is reached
            q->tail = q->array;
    }
}

void dequeue(Queue *q, int N) {
    if (is_empty(*q)) {
        printf("Queue is empty\n");
    } else {
        q->head++;
        q->elements--;
        if (q->head >= q->array + N) // Wrap around
            q->head = q->array;
        
    }
}

void free_queue(Queue *q) {
    free(q->array);
}
//END OF THE CODE FOR QUEUES´

void combats(Character* player, Enemy enemies[], int number_enemies){
    //Random initialization
    srand(time(NULL));
    //We will use number_enemies + 1 as the player also will have a turn
    Queue turns = init_queue(TURNS);
    //Enqueuing all the turns
    int player_percentage = 50+player->vel;
    for(int i = 0; i<TURNS; i++){
        //Generate random number bearing in mind the player's chance to attack
        int r = rand() % 100;
        if(r<player_percentage){
            enqueue(&turns,PLAYER,TURNS);
        }
        else{
            enqueue(&turns,ENEMY,TURNS);
        }
    }
    //PLACEHOLDER COMBAT SIM
    for(int i = 0; i<TURNS;i++){
        int turn = first(turns);
        printf("TURN\n");
        if(turn==0){
            printf("Player's turn\n");
        }
        else{
            printf("Enemy's turn\n",turn);
        }
        dequeue(&turns,TURNS);
    }
}



bool is_finished(Character* player, Enemy* enemies[]) {
    if (player->hp != 0 && enemies[0]->hp == 0) {
        return true;
    }
    return false;
}