//
// created by sva4rd
//

// client file

#include <stdio.h>
#include "../include/interface.h"

int main() {
    sqlite3 *data_base;
    
    if (sqlite3_open("db/buildings.db", &data_base) != 0) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(data_base));
        sqlite3_close(data_base);
        return 1;
    }
    
    while (do_sql_query(data_base) == 0){};
    sqlite3_close(data_base);
    return 0;
}
