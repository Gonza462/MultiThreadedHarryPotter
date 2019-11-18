//||/////////////////////|
//|| by Luis Gonzalez  //|
//||     CS 537 HW 06  //|
//||     11/02/19     ///|
//||/////////////////////|

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>


//Team consist: 3 Chasers, 2 Beaters, 1 Keeper, 1 Seeker
//Neutral: 2 Blutgers
//phthreads represent teams, four balls, two goal posts
//Balls:snitch, quaffle, bluger_gryff, bluger_huffle
//Fourteen players two teams: Gryffindor vs HufflePuff

//Team Gryffindor
pthread_t g_chaser_1,g_chaser_2,g_chaser_3;
pthread_t g_beater_1,g_beater_2;
pthread_t g_keeper_1;
pthread_t g_seeker_1;

//Team Hufflepuff
pthread_t h_chaser_1,h_chaser_2,h_chaser_3;
pthread_t h_beater_1,h_beater_2;
pthread_t h_keeper_1;
pthread_t h_seeker_1;

//pthreads balls define
pthread_t snitch;
pthread_t quaffle;
pthread_t bludger_A, bludger_B;

//goals
pthread_t goal_g, goal_h;


//player object representation and attributes
//this struct should store id of player thread and current state of play
//14 instances of players
struct Player {
    pthread_t *p_thread;
    char my_name[32];
    char team_name;
    int state;
    int id;
};
// Teams structs mapped to the corresponding teams
//team gryffindor
struct Player g_chaser1 = {
        .p_thread = &g_chaser_1,
        .my_name = "g_chaser_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 1                     //unique id
};
struct Player g_chaser2 = {
        .p_thread = &g_chaser_2,
        .my_name = "g_chaser_2",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 2                     //unique id
};
struct Player g_chaser3 = {
        .p_thread = &g_chaser_3,
        .my_name = "g_chaser_3",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 3                     //unique id
};

struct Player g_beater1 = {
        .p_thread = &g_beater_1,
        .my_name = "g_beater_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 4                     //unique id
};
struct Player g_beater2 = {
        .p_thread = &g_beater_2,
        .my_name = "g_beater_2",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 5                    //unique id
};
struct Player g_keeper1 = {
        .p_thread = &g_keeper_1,
        .my_name = "g_keeper_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 6                     //unique id
};
struct Player g_seeker1 = {
        .p_thread = &g_seeker_1,
        .my_name = "g_seeker_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'G',
        .state = 1,                 //current status
        .id = 7                     //unique id
};
//Team hufflepuff
struct Player h_chaser1 = {
        .p_thread = &h_chaser_1,
        .my_name = "h_chaser_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 8                     //unique id
};
struct Player h_chaser2 = {
        .p_thread = &h_chaser_2,
        .my_name = "h_chaser_2",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 9                     //unique id
};
struct Player h_chaser3 = {
        .p_thread = &h_chaser_3,
        .my_name = "h_chaser_3",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 10                     //unique id
};

struct Player h_beater1 = {
        .p_thread = &h_beater_1,
        .my_name = "h_beater_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 11                     //unique id
};
struct Player h_beater2 = {
        .p_thread = &h_beater_2,
        .my_name = "h_beater_2",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 12                    //unique id
};
struct Player h_keeper1 = {
        .p_thread = &h_keeper_1,
        .my_name = "h_keeper_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 13                     //unique id
};
struct Player h_seeker1 = {
        .p_thread = &h_seeker_1,
        .my_name = "h_seeker_1",    //naming them exactly how they correspond to for debugging later
        .team_name = 'H',
        .state = 1,                 //current status
        .id = 14                     //unique id
};

//global array of pointers of players
struct Player *player_arrays[14] = {
        &g_chaser1, &g_chaser2, &g_chaser3,
        &g_beater1, &g_beater2,
        &g_keeper1,
        &g_seeker1,

        &h_chaser1, &h_chaser2, &h_chaser3,
        &h_beater1, &h_beater2,
        &h_keeper1,
        &h_seeker1,
};
//threads functions
void *beater_pthread(void *);
void *seeker_pthread(void *);
void *keeper_pthread(void *);
void *chaser_pthread(void *);
void *snitch_pthread(void *);
void *goal_pthread(void *);

//sig hanlders
void init();
void bludger_hit(int);
void save_block(int);
void catch_quaffle(int);


//global vars
int MAX_PLAYERS = 14;
int CHASERS = 6;


static volatile sig_atomic_t caught_flag = 0;
static volatile sig_atomic_t team_G,team_H = 0;



int main() {
    init();

    return 0;
}


//set-up
void init(){
    struct sigaction sig_action_throw;
    sig_action_throw.sa_handler = bludger_hit;
    sig_action_throw.sa_flags = 0;
    sigemptyset(&sig_action_throw.sa_mask);
    sigaction(SIGINT,&sig_action_throw,NULL);

    struct sigaction sig_action_save;
    sig_action_save.sa_handler = save_block;
    sig_action_save.sa_flags =0;
    sigemptyset(&sig_action_save.sa_mask);
    sigaction(SIGUSR1, &sig_action_save,NULL);

    struct sigaction sig_action_caught;
    sig_action_caught.sa_handler = catch_quaffle;
    sig_action_caught.sa_flags = 0;
    sigemptyset(&sig_action_caught.sa_mask);
    sigaction(SIGUSR2,&sig_action_caught,NULL);

    //create bludgers
    char bludger_ss = 'b';
    pthread_create(&bludger_A,NULL,&beater_pthread,(void*)&bludger_ss);
    pthread_create(&bludger_B,NULL,&beater_pthread,(void*)&bludger_ss);

    //quaffles
    char quaffle_ss = 'q';
    pthread_create(&quaffle,NULL,&beater_pthread,(void*)&quaffle_ss);

    //snitch
    pthread_create(&snitch,NULL,&snitch_pthread,NULL);

    //chasers
    pthread_create(&g_chaser_1,NULL, &chaser_pthread,NULL);
    pthread_create(&g_chaser_2,NULL, &chaser_pthread,NULL);
    pthread_create(&g_chaser_3,NULL, &chaser_pthread,NULL);

    pthread_create(&h_chaser_1,NULL, &chaser_pthread,NULL);
    pthread_create(&h_chaser_2,NULL, &chaser_pthread,NULL);
    pthread_create(&h_chaser_3,NULL, &chaser_pthread,NULL);

    //keepers
    pthread_create(&g_keeper_1,NULL, &keeper_pthread,NULL);
    pthread_create(&h_keeper_1,NULL, &keeper_pthread,NULL);

    //beaters
    char beat = 'v';
    pthread_create(&h_beater_1,NULL, &beater_pthread, (void*)&beat);
    pthread_create(&h_beater_2,NULL, &beater_pthread, (void*)&beat);

    pthread_create(&g_beater_1,NULL, &beater_pthread, (void*)&beat);
    pthread_create(&g_beater_2,NULL, &beater_pthread, (void*)&beat);

    //seekers
    pthread_create(&g_seeker_1,NULL,&seeker_pthread,NULL);
    pthread_create(&h_seeker_1,NULL,&seeker_pthread,NULL);

    //goals
    pthread_create(&goal_g,NULL,&goal_pthread,NULL);
    pthread_create(&goal_h,NULL,&goal_pthread,NULL);
    for(int i = 0; i<14; i++){
        pthread_join(*player_arrays[i]->p_thread,NULL);
    }
    exit(0);

}

//helper
void h_sleep(int temp){
    int period = rand() % temp;
    period += 1;
    sleep((unsigned int) period);
}
void *snitch_pthread(void*p){
    while(1){
        h_sleep(5);
        caught_flag = 1;
        sleep(1);
        caught_flag = 0;
    }
}
void *chaser_pthread(void *p){
    //run inf
    while(1);

}
void *goal_pthread(void *p){
    while(1);

}
void *beater_pthread(void *p){
    int player_num = 0;
    int sig = 0;
    int sleep_timer = 0;
    int max_sleep = 25;

    //case bludger 'b'
    if( *((char*)p) == 'b' ){
        player_num = MAX_PLAYERS;
        sig = SIGINT;
        sleep_timer = 0;

    }
    //case beater -> 'v'
    if(*((char*)p) == 'v'){
        player_num = MAX_PLAYERS;
        sig = SIGUSR1;
        sleep_timer = 0;

    }
    //case quaffle 'q'
    if(*((char*)p) == 'q'){
        player_num = CHASERS;
        sig = SIGUSR2;
        sleep_timer = 5;
    }


    for(;;){
        h_sleep(25);

        int temp_p = rand() % player_num + sleep_timer;
        while(player_arrays[temp_p]->state == 0){
            temp_p = rand() % player_num + sleep_timer;
        }
        pthread_t *select_rand_player = player_arrays[temp_p]->p_thread; //just gets random player index from players_arrays
        pthread_kill(*select_rand_player,sig);

    }


}

//runs inf where it sends SIGUSR1 sig to goal post respectively
//sleeps rand time
void *keeper_pthread(void *p){
    for(;;){
        h_sleep(25);
        for(int i = 0; i<MAX_PLAYERS; i++){
            if(player_arrays[i]->state==0) continue;
            if(*(player_arrays[i]->p_thread)==pthread_self()){
                char temp = player_arrays[i]->team_name;
                pthread_t goal_ss;
                if(temp == 'G'){
                    goal_ss = goal_g;

                }else if(temp == 'H'){
                    goal_ss = goal_h;
                }else{
                    printf("Unknown case keeper function");
                }
                pthread_kill(goal_ss,SIGUSR1);
            }
        }
    }
}

//Seeker pthread wakes up and checks value of "Caught_Snitch"
//Case True: -> seeker adds 150 to the team
//else -> goes back to sleep
void *seeker_pthread(void *p){
    //printf("seeker function called\n");

    for(;;){
        h_sleep(25);
        if(caught_flag == 1){
            printf("Caught Snitch!\n");
            for(int i = 0; i<MAX_PLAYERS; i++){
             if(player_arrays[i]->state == 0){continue;}
             if(*(player_arrays[i]->p_thread) == pthread_self()){
                 char temp = player_arrays[i]->team_name;

                 if(temp == 'G'){
                     team_G += 150;

                 }else {
                     team_H += 150;
                 }

                 printf("-------------\n");
                 printf("%s Caught the Snitch!!\n",player_arrays[i]->my_name);
                 printf("Score: Team Gryffindor: %i\n", team_G);
                 printf("Score: Team Hufflepuff: %i\n", team_H);
                 printf("-------------\n");
                 exit(0);

             }
            }
        }else{
            printf("...\n");
        }

    }

}
void catch_quaffle(int s){
    for(int i = 0; i<MAX_PLAYERS; i++){
        if(player_arrays[i]->state==0) continue;
        if(*(player_arrays[i]->p_thread)==pthread_self()){
            char temp = player_arrays[i]->team_name;
            pthread_t q_catch = NULL;
            if(temp == 'G'){
                q_catch = goal_g;
                printf("%s has quaffle possession\n\n",player_arrays[i]->my_name);
            }else if(temp == 'H'){
                q_catch = goal_h;
                printf("%s has quaffle possession\n\n",player_arrays[i]->my_name);
            }else{
                printf("Unknown case catch quaffle function");
            }
            pthread_kill(q_catch,SIGUSR1);
        }
    }


}
void save_block(int s){
    //read signal
    int bool_player = 0;
    for(int i = 0; i<14; i++){
        if(*(player_arrays[i]->p_thread) == pthread_self()){bool_player=1;  break;}
    }
    if(bool_player==0){
        printf(" It's a save by keeper!!\n");
    }else{
        printf(" Saved by player!!\n");
    }
}
void bludger_hit(int s){
    int bool_player = 0;
    for(int i = 0; i<14; i++){
        if(*(player_arrays[i]->p_thread) == pthread_self()){bool_player=1;  break;}
    }

    if(!bool_player){
        if(pthread_self() == goal_g){
            printf("Goal for Gryffindor!\n\n");
            team_G +=10;
        }
        else if(pthread_self() == goal_h){
            printf("Goal for Hufflepuff!\n\n");
            team_H +=10;
        }

    } else{
        for(int i = 0; i<14; i++){
            if(player_arrays[i]->state == 0) continue;
            if(*(player_arrays[i]->p_thread)== pthread_self()){
                player_arrays[i]->state = 0;//out
                printf("%s is out!\n", player_arrays[i]->my_name);
            }
        }
        pthread_exit(0);
    }

}

