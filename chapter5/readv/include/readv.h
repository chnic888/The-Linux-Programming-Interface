#ifndef THE_LINUX_PROGRAMMING_INTERFACE_READV_H
#define THE_LINUX_PROGRAMMING_INTERFACE_READV_H

#endif //THE_LINUX_PROGRAMMING_INTERFACE_READV_H

#define NAME_SIZE 20
#define CLUB_NAME 20

struct player {
    char firstName[NAME_SIZE];
    char lastName[NAME_SIZE];
    int age;
};

struct club {
    char name[CLUB_NAME];
    int league;
    char delimiter1;
    int rating;
    char delimiter2;
    int point;
};

