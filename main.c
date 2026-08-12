#include <iso646.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int x;
    int y;
    //TILE_TYPE tile;
} Position;

typedef struct {

} Monster;

typedef struct {

} Items ;

typedef struct{
    Position position;
    int height;
    int width;

    Position ** doors;
    //Monster ** monsters;
    //Item ** items;
} Room;

typedef struct{
    Position position;
    int health;
    //Room * room;
} Player;

int screenSetup();
Room ** mapSetup();
Player* playerSetup();
int handleInput(int input, Player* user);
int playerMove(int x, int y, Player* user);
int checkPosition(int newX, int newY, Player* user);

/* room functions */

Room * createRoom(int x, int y, int height, int width);
int drawRoom(Room * room);

int main(){

    Player* user;
    int ch;

    screenSetup();

    mapSetup();

    user = playerSetup();

    /* Main game loop */
    while((ch = getch()) != 'q'){
        handleInput(ch,user);
    }
    endwin();

    return 0;
}

int screenSetup(){
    initscr();
    //printw("Hello world!");
    noecho();
    refresh();

    srand(time(NULL));

    return 1;
}

Room ** mapSetup(){

    Room ** rooms;
    rooms = malloc(sizeof(Room) *6);

    /*mvprintw(13,13,"--------");
    mvprintw(14,13,"|......|");
    mvprintw(15,13,"|......|");
    mvprintw(16,13,"|......|");
    mvprintw(17,13,"|......|");
    mvprintw(18,13,"--------");*/

    rooms[0] = createRoom(13,13,6,8);
    drawRoom(rooms[0]);

    /*mvprintw(2,40,"--------");
    mvprintw(3,40,"|......|");
    mvprintw(4,40,"|......|");
    mvprintw(5,40,"|......|");
    mvprintw(6,40,"|......|");
    mvprintw(7,40,"--------"); */

    rooms[1] = createRoom(40,2,6,8);
    drawRoom(rooms[1]);

    rooms[2] = createRoom(40,10,6,12);
    drawRoom(rooms[2]);

    return rooms;
}

Room * createRoom(int x, int y, int height, int width){
    Room * newRoom;
    newRoom = malloc(sizeof(Room));

    newRoom -> position.x = x;
    newRoom -> position.y = y;
    newRoom -> height = height;
    newRoom -> width = width;

    newRoom -> doors = malloc(sizeof(Position) * 4);

    /* top door */
    newRoom -> doors[0] = malloc(sizeof(Position));
    newRoom -> doors[0] -> x = rand() % (width - 2) + newRoom -> position.x + 1;
    newRoom -> doors[0] -> y = newRoom -> position.y;

    /* bottom door */
    newRoom -> doors[1] = malloc(sizeof(Position));
    newRoom -> doors[1] -> x = rand() % (width - 2) + newRoom -> position.x + 1;
    newRoom -> doors[1] -> y = newRoom -> position.y + newRoom -> height - 1;

    /* left door */
    newRoom -> doors[2] = malloc(sizeof(Position));
    newRoom -> doors[2] -> x = newRoom -> position.x;
    newRoom -> doors[2] -> y = rand() % (height - 2) + newRoom -> position.y + 1;

    /* right door */
    newRoom -> doors[3] = malloc(sizeof(Position));
    newRoom -> doors[3] -> x = newRoom -> position.x + width - 1;
    newRoom -> doors[3] -> y = rand() % (height - 2) + newRoom -> position.y + 1;

    return newRoom;
}

int drawRoom(Room * room){

    int x;
    int y;

    /* draw top and bottom */
    for(x = room -> position.x ; x < room -> position.x + room -> width ; x++){
        mvprintw(room -> position.y,x, "-"); /* top */
        mvprintw(room -> position.y + room -> height - 1,x, "-"); /* bottom */
    }

    /* draw floors and side of walls */
    for(y = room -> position.y + 1 ; y < room-> position.y + room->height - 1; y++){
        /* draw side walls */
        mvprintw(y,room-> position.x,"|");
        mvprintw(y,room-> position.x + room -> width - 1,"|");

        /* draw floor */
        for(x = room -> position.x + 1; x < room -> position.x + room -> width -1; x++){
            mvprintw(y,x,".");
        }
    }

    /* draw doors */
    mvprintw(room -> doors[0]->y, room -> doors[0]->x, "+");
    mvprintw(room -> doors[1]->y, room -> doors[1]->x, "+");
    mvprintw(room -> doors[2]->y, room -> doors[2]->x, "+");
    mvprintw(room -> doors[3]->y, room -> doors[3]->x, "+");

    return 1;
}


Player* playerSetup(){
    Player* newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer -> position.x = 14;
    newPlayer -> position.y = 14;

    newPlayer -> health = 20;

    playerMove(14, 14, newPlayer);

    return newPlayer;
}

int handleInput(int input, Player* user){

    int newX;
    int newY;

    switch (input) {

        /* Move up */
        case 'w':
        case 'W':
            newX = user -> position.x;
            newY = user -> position.y - 1;
            //playerMove(user -> yPosition - 1, user -> xPosition, user);
            break;

        /* Move left */
        case 'a':
        case 'A':
            newX = user -> position.x - 1;
            newY = user -> position.y;
            //playerMove(user -> yPosition , user -> xPosition - 1, user);
            break;

        /* Move down */
        case 's':
        case 'S':
            newX = user -> position.x;
            newY = user -> position.y + 1;
            //playerMove(user -> yPosition + 1, user -> xPosition , user);
            break;

        /* Move right */
        case 'd':
        case 'D':
            newX = user -> position.x + 1;
            newY = user -> position.y;
            //playerMove(user -> yPosition , user -> xPosition + 1, user);
            break;

        default:
            break;

    }

    checkPosition(newY,newX,user);

    return 1;
}

/* Check what's the next position */
int checkPosition(int newY, int newX, Player* user){
    int space;

    switch (mvinch(newY, newX)) {
        case '.':
            playerMove(newY,newX,user);
            break;
        default:
            move(user->position.y, user->position.x);
            break;
    }

    return 1;
}

int playerMove(int y, int x, Player* user){

    mvprintw(user->position.y, user -> position.x, ".");

    user -> position.x = x;
    user -> position.y = y;

    mvprintw(user->position.y, user -> position.x, "@");
    move(user->position.y, user -> position.x);

    return 1;
}
