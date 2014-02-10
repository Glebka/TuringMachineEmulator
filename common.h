#ifndef COMMON_H
#define COMMON_H

typedef enum {
    STOP='S',
    LEFT='L',
    RIGHT='R'
} MoveCommand;

struct Command
{
    int state;
    char symbol;
    MoveCommand move;
};

#endif // COMMON_H
