//
// created by sva4rd
//

#ifndef COMMANDS_H
#define COMMANDS_H

// Contains array of commands and structures with commands indexes

#include <stdio.h>

enum Commands_Idx{
    SELECT = 0,
    INSERT = 1,
    DELETE = 2,
    WHERE = 3,
    BEGIN = 4,
    COMMIT = 5
};

enum Columns_Idx{
    ID = 0,
    TYPE = 1,
    FLOORS_NUM = 2,
    APARTMENTS_NUM = 3,
    SERVICE_LIFE = 4,
    TO_OVERHAUL = 5,
    DISTRICT_ID = 6,
    PHOTO = 7,
};

const char *commands[] = {
    "SELECT * FROM Building", "INSERT INTO Building VALUES(", "DELETE FROM Building",
    " WHERE ", "BEGIN TRANSACTION; ", "; COMMIT;"
};

const char *columns[] = {
    "id", "type", "floors_num", "apartments_num", "service_life", "to overhal",
    "district_id", "photo"
};

#endif
