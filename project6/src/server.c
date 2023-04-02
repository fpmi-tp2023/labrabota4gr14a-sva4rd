//
// created by sva4rd
//

// interface.h functions are implemented in this file

#include "../include/interface.h"
#include <stdlib.h>
#include <string.h>
#include "../include/commands.h"

static int callback(void *data, int argc, char **argv, char **azColName);
void select_int_lbl(char *sql, char *name, int column);
void select_char_lbl(char *sql, char *name, int column);
void select_with_where(char *sql);
void select_query(char *sql);
void enter_str(char *sql, char *name, int column);
void insert_query(char *sql);
void delete_query(char *sql);
void select_photo(char *sql, sqlite3 *db, int mode);
void insert_photo(char *sql, sqlite3 *db, int mode);


static int callback(void *data, int argc, char **argv, char **azColName){
    fprintf (stderr, "%s:\n", (const char *)data);
    for (int i = 0; i < argc; i++){
        printf("\n%s = %s", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

void select_int_lbl(char *sql, char *name, int column){
    printf("Enter %s: ", name);
    int field;
    scanf("%d", &field);
    strcat(sql, columns[column]);
    strcat(sql, " = ");
    int length = snprintf(NULL, 0, "%d", field);
    char *field_str = malloc(length+1);
    snprintf(field_str, length+1, "%d", field);
    strcat(sql, field_str);
    free(field_str);
}

void select_char_lbl(char *sql, char *name, int column){
    printf("Enter %s: ", name);
    char *field = malloc(50 * sizeof(char));
    scanf("%s", field);
    strcat(sql, columns[column]);
    strcat(sql, " = '");
    strcat(sql, field);
    strcat(sql, "'");
    free(field);
}

void select_with_where(char *sql){
    int field;
    strcat(sql, commands[WHERE]);
    while(1){
        printf("Choose the field:\n"
               "\t1 - ID\n"
               "\t2 - TYPE\n"
               "\t3 - FLOORS_NUM\n"
               "\t4 - APARTMENTS_NUM\n"
               "\t5 - SERVICE_LIFE\n"
               "\t6 - TO_OVERHAUL\n"
               "\t7 - DISTRICT_ID\n"
               "Enter: ");
        scanf("%d", &field);
        switch(field){
            case(1):
                select_int_lbl(sql, "ID", ID);
                break;
            case(2):
                select_char_lbl(sql, "TYPE", TYPE);
                break;
            case(3):
                select_int_lbl(sql, "FLOORS_NUM", FLOORS_NUM);
                break;
            case(4):
                select_int_lbl(sql, "APARTMENTS_NUM", APARTMENTS_NUM);
                break;
            case(5):
                select_int_lbl(sql, "SERVICE_LIFE", SERVICE_LIFE);
                break;
            case(6):
                select_int_lbl(sql, "TO_OVERHAUL", TO_OVERHAUL);
                break;
            case(7):
                select_int_lbl(sql, "DISTRICT_ID", DISTRICT_ID);
                break;
            default:
                printf("Error: Wrong input!\n");
                continue;
        }
        break;
    }
}

void select_query(char *sql){
    int select_type;
    strcat(sql, commands[SELECT]);
    while(1){
        printf("Choose select type:\n"
               "\t1 - without parametrs\n"
               "\t2 - with parametrs\n"
               "Enter: ");
        scanf("%d", &select_type);
        switch(select_type){
            case(1):
                return;
            case(2):
                select_with_where(sql);
                return;
            default:
                printf("Error: Wrong input!\n");
                continue;
        }
    }
}

void enter_str(char *sql, char *name, int column){
    char *field = malloc(50 * sizeof(char));
    printf("\t%s: ",columns[column]);
    scanf("%s", field);
    strcat(sql, "'");
    strcat(sql, field);
    strcat(sql, "'");
    free(field);
}

void insert_query(char *sql) {
    printf("Enter fields:\n");
    strcat(sql, commands[INSERT]);
    enter_str(sql, "ID", ID);
    strcat(sql, ", ");
    enter_str(sql, "TYPE", TYPE);
    strcat(sql, ", ");
    enter_str(sql, "FLOORS_NUM", FLOORS_NUM);
    strcat(sql, ", ");
    enter_str(sql, "APARTMENTS_NUM", APARTMENTS_NUM);
    strcat(sql, ", ");
    enter_str(sql, "SERVICE_LIFE", SERVICE_LIFE);
    strcat(sql, ", ");
    enter_str(sql, "TO_OVERHAUL", TO_OVERHAUL);
    strcat(sql, ", ");
    enter_str(sql, "DISTRICT_ID", DISTRICT_ID);
    strcat(sql, ", ?)");
}

void delete_query(char *sql){
    strcat(sql, commands[DELETE]);
    strcat(sql, commands[WHERE]);
    select_int_lbl(sql, "ID", ID);
}

void select_photo(char *sql, sqlite3 *db, int mode){
    char *ph_sql = malloc(800 * sizeof(char));
    strcpy(ph_sql, "SELECT photo FROM Building WHERE ");
    select_int_lbl(ph_sql, "ID", ID);
    strcat(sql, ph_sql);
    if (mode == 2)  //transaction
        strcat(sql, commands[COMMIT]);
    printf("Enter filename(pattern: file.jpg): ");
    char *filename = malloc(50 * sizeof(char));
    char *file = malloc(50 * sizeof(char));
    scanf("%s", filename);
    strcpy(file, "img/");
    strcat(file, filename);
    FILE *fp = fopen(file, "wb");
    free(filename);
    free(file);
    if (fp == NULL) {
        fprintf(stderr, "Cannot open image file\n");
        free(ph_sql);
        return;
    }
    sqlite3_stmt *pStmt;
    int rc = sqlite3_prepare_v2(db, ph_sql, -1, &pStmt, 0);
    if (rc != SQLITE_OK ) {
        fprintf(stderr, "Failed to prepare statement\n");
        free(ph_sql);
        return;
    }
    rc = sqlite3_step(pStmt);
    int bytes = 0;
    if (rc == SQLITE_ROW)
        bytes = sqlite3_column_bytes(pStmt, 0);

    fwrite(sqlite3_column_blob(pStmt, 0), bytes, 1, fp);
    if (ferror(fp)) {
        fprintf(stderr, "fwrite() failed\n");
        free(ph_sql);
        return;
    }
    int r = fclose(fp);
    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }
    rc = sqlite3_finalize(pStmt);
    fprintf(stdout, "\nOperation done successfully\n");
    free(ph_sql);
}

void insert_photo(char *sql, sqlite3 *db, int mode){
    printf("Enter filename(pattern: file.jpg): ");
    char *filename = malloc(50 * sizeof(char));
    char *file = malloc(50 * sizeof(char));
    scanf("%s", filename);
    strcpy(file, "img/");
    strcat(file, filename);
    FILE *fp = fopen(file, "rb");
    free(filename);
    free(file);
    
    if (fp == NULL) {
        fprintf(stderr, "Cannot open image file\n");
        return ;
    }

    fseek(fp, 0, SEEK_END);
    if (ferror(fp)) {
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return ;
    }

    long flen = ftell(fp);
    if (flen == -1) {
        perror("error occurred");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return;
    }

    fseek(fp, 0, SEEK_SET);
    if (ferror(fp)) {
        fprintf(stderr, "fseek() failed\n");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return ;
    }

    char data[flen+1];
    int size = (int)fread(data, 1, flen, fp);
    if (ferror(fp)) {
        fprintf(stderr, "fread() failed\n");
        int r = fclose(fp);
        if (r == EOF)
            fprintf(stderr, "Cannot close file handler\n");
        return ;
    }

    int r = fclose(fp);
    if (r == EOF) {
        fprintf(stderr, "Cannot close file handler\n");
    }
    
    sqlite3_stmt *pStmt;
    char *temp_sql = malloc(800 * sizeof(char));
    if (mode == 2)
        strcpy(temp_sql, sql+19);
    else
        strcpy(temp_sql, sql);
    int rc = sqlite3_prepare(db, temp_sql, -1, &pStmt, 0);
    free(temp_sql);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot prepare statement: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    sqlite3_bind_blob(pStmt, 1, data, size, SQLITE_STATIC);
    rc = sqlite3_step(pStmt);
    if (rc != SQLITE_DONE)
        printf("execution failed: %s", sqlite3_errmsg(db));
    
    sqlite3_finalize(pStmt);
    fprintf(stdout, "\nOperation done successfully\n");
}

int do_sql_query(sqlite3 *data_base) {
    int mode;
    int command;
    char *sql = malloc(sizeof(char)*800);
    printf("\n========SQLite-querry started========\n");
    while(1){
        printf("Choose mode:\n"
               "\t1 - autocommit mode\n"
               "\t2 - transaction mode\n"
               "\t3 - exit\n"
               "Enter: ");
        scanf("%d", &mode);
        switch(mode){
            case(1):
                break;
            case(2):
                strcat(sql, commands[BEGIN]);
                break;
            case(3):
                printf("Aborted\n");
                free(sql);
                return 1;
            default:
                printf("Error: Wrong input!\n");
                continue;
        }
        break;
    }
    while(1){
        printf("Choose command:\n"
               "\t1 - SELECT\n"
               "\t2 - INSERT\n"
               "\t3 - DELETE\n"
               "\t4 - SELECT PHOTO\n"
               "\t5 - EXIT\n"
               "Enter: ");
        scanf("%d", &command);
        switch(command){
            case(1):
                select_query(sql);
                break;
            case(2):
                insert_query(sql);
                if (mode == 2)  //transaction
                    strcat(sql, commands[COMMIT]);
                insert_photo(sql, data_base, mode);
                break;
            case(3):
                delete_query(sql);
                break;
            case(4):
                select_photo(sql, data_base, mode);
                break;
            case(5):
                printf("Aborted\n");
                free(sql);
                return 1;
            default:
                printf("Error: Wrong input!\n");
                continue;
        }
        break;
    }

    if (mode == 2 && command != 4 && command != 2)  //transaction
        strcat(sql, commands[COMMIT]);

    printf("\nQuery: %s\n", sql);

    char *zErrMsg = 0;
    const char *data = "\nCallback function called";
    int rc;
    if (command != 4 && command != 2){
        rc = sqlite3_exec(data_base, sql, callback, (void *)data, &zErrMsg);
        if(rc != SQLITE_OK) {
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
          fprintf(stdout, "\nOperation done successfully\n");
        }
    }
    free(sql);
    return 0;
}
