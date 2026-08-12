#include <iso646.h>
#include <ncurses.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct {

} Monster;

typedef struct {

} Items ;

typedef struct{
    int xPosition;
    int yPosition;
    int height;
    int width;
    //Monster ** monsters;
    //Item ** items;
} Room;

typedef struct{
    int xPosition;
    int yPosition;
    int health;
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

    newRoom -> xPosition = x;
    newRoom -> yPosition = y;
    newRoom -> height = height;
    newRoom -> width = width;

    return newRoom;
}

int drawRoom(Room * room){

    int x;
    int y;

    /* draw top and bottom */
    for(x = room -> xPosition ; x < room -> xPosition + room -> width ; x++){
        mvprintw(room -> yPosition,x, "-"); /* top */
        mvprintw(room -> yPosition + room -> height,x, "-"); /* bottom */
    }

    /* draw floors and side of walls */
    for(y = room -> yPosition + 1 ; y < room->yPosition + room->height; y++){
        /* draw side walls */
        mvprintw(y,room-> xPosition,"|");
        mvprintw(y,room-> xPosition + room -> width - 1,"|");

        /* draw floor */
        for(x = room -> xPosition + 1; x < room -> xPosition + room -> width -1; x++){
            mvprintw(y,x,".");
        }
    }

    return 1;
}


Player* playerSetup(){
    Player* newPlayer;
    newPlayer = malloc(sizeof(Player));

    newPlayer -> xPosition = 14;
    newPlayer -> yPosition = 14;

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
            newX = user -> xPosition;
            newY = user -> yPosition - 1;
            //playerMove(user -> yPosition - 1, user -> xPosition, user);
            break;

        /* Move left */
        case 'a':
        case 'A':
            newX = user -> xPosition - 1;
            newY = user -> yPosition;
            //playerMove(user -> yPosition , user -> xPosition - 1, user);
            break;

        /* Move down */
        case 's':
        case 'S':
            newX = user -> xPosition;
            newY = user -> yPosition + 1;
            //playerMove(user -> yPosition + 1, user -> xPosition , user);
            break;

        /* Move right */
        case 'd':
        case 'D':
            newX = user -> xPosition + 1;
            newY = user -> yPosition;
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
            move(user->yPosition, user->xPosition);
            break;
    }

    return 1;
}

int playerMove(int y, int x, Player* user){

    mvprintw(user->yPosition, user -> xPosition, ".");

    user -> xPosition = x;
    user -> yPosition = y;

    mvprintw(user->yPosition, user -> xPosition, "@");
    move(user->yPosition, user -> xPosition);

    return 1;
}
