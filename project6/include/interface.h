//
// created by sva4rd
//

#ifndef INTERFACE_H
#define INTERFACE_H

// contains the declaration of functions that that handle SQLite queries

#include <stdio.h>
#include <sqlite3.h>

int do_sql_query(sqlite3 *data_base);

#endif
