//
//  Database.cpp
//
//  Created by Daniel Harvey on 08/04/2019.
//

#include "Database.hpp"
//Created with help from
//https://www.geeksforgeeks.org/sql-using-c-c-and-sqlite/
Database::Database()
{
}

bool Database::Open(const char *path)
{
    if(!Helper::is_file_exist(path))
    {
        return Create(path);
    }
    else
    { 
        Lastpath = path;
        int exit = 0;
        exit = sqlite3_open(path, &DB);
        
        if (exit)
        {
            Error();
            return false;
        }
    }
    return true;
}
bool Database::Create(const char *path)
{
    std::cout<<"CREATE"<<std::endl;
    Lastpath = path;
    std::string table_1 = "CREATE TABLE Gene ("
    "ID    INTEGER NOT NULL,"
    "Name    TEXT NOT NULL DEFAULT 'Name_NotGiven', "
    "DateTime    TEXT NOT NULL DEFAULT 'YYYY-MM-DD HH:MM:SS.SSS' UNIQUE, "
    "TrackNumber    INTEGER NOT NULL, "
    "FastestLapTime    NUMERIC, "
    "Genes    TEXT NOT NULL, "
    "Generation    INTEGER NOT NULL DEFAULT 1, "
    "PRIMARY KEY(ID) ); ";
    CreateTable(path, table_1);
    std::string table_2 = "CREATE TABLE LapTimeRecords ( "
    "TrackID    INTEGER NOT NULL DEFAULT 1 UNIQUE, "
    "DateTime    TEXT NOT NULL DEFAULT 'YYYY-MM-DD HH:MM:SS.SSS', "
    "Laptime    NUMERIC NOT NULL, "
    "Name    TEXT NOT NULL DEFAULT 'Name_NotGiven', "
    "Gene    TEXT NOT NULL, "
    "PRIMARY KEY(TrackID)); ";
    
    
    CreateTable(path, table_2);
    
    return true;
}
bool Database::CreateTable(const char *path, std::string sql)
{
    int exit = 0;
    exit = sqlite3_open(path, &DB);
    char* messaggeError;
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    
    if (exit != SQLITE_OK) {
        std::cerr << "Error Creating Table" << std::endl;
        sqlite3_free(messaggeError);
        return false; 
    }
    
    Close();
    return true;
}
bool Database::Close()
{
    sqlite3_close(DB);
    return true;
}
bool Database::Select(std::string query, GeneTableStruct &gts)
{
    Open(Lastpath);
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        Error();
        Close();
        return false;
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        gts.ID = sqlite3_column_int (stmt, 0);
        gts.Name = std::string((const char*)sqlite3_column_text(stmt, 1));
        gts.DateTime = std::string((const char*)sqlite3_column_text(stmt, 2));
        gts.TrackNum = sqlite3_column_int (stmt, 3);
        gts.FastestLap = sqlite3_column_double(stmt, 4);
        gts.chromosome = std::string((const char*)sqlite3_column_text(stmt, 5));
        gts.Generation = sqlite3_column_int (stmt, 6);
    }
    if (rc != SQLITE_DONE)
    {
        Error();
        Close();
        return false;
    }
    sqlite3_finalize(stmt);
    Close();
    return true;
}
bool Database::Select(std::string query, LapTimeTableStruct &lts)
{
    Open(Lastpath);
    sqlite3_stmt *stmt;
    
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        Error();
        Close();
        return false ;
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        lts.TrackID = sqlite3_column_int (stmt, 0);
        lts.DateTime = std::string((const char*)sqlite3_column_text(stmt, 1));
        lts.LapTime = sqlite3_column_double(stmt, 2);
        lts.Name = std::string((const char*)sqlite3_column_text(stmt, 3));
        lts.chromosome = std::string((const char*)sqlite3_column_text(stmt, 4));
    }
    if (rc != SQLITE_DONE)
    {
        Error();
        Close();
        return false;
    }
    sqlite3_finalize(stmt);
    Close();
    return true;
}
bool Database::Insert(std::string sql)
{
    Open(Lastpath);
    char* messaggeError;
    bool exit = Open(Lastpath);
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    
    if (exit != SQLITE_OK)
    {
        Error();
        sqlite3_free(messaggeError);
        Close();
        return false;
    }
    
    Close();
    return true;
}
bool Database::Delete(std::string sql)
{
    Open(Lastpath);
    char* messaggeError;
    bool exit = Open(Lastpath);
    exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messaggeError);
    
    if (exit != SQLITE_OK)
    {
        Error();
        sqlite3_free(messaggeError);
        return false;
    }
    
    Close();
    return true;
}

int Database::getSize(eTable table)
{
    Open(Lastpath);
    sqlite3_stmt *stmt;
    std::string query;
    int size = 0;
    if(table == eTable::eGene)
        query = "SELECT * FROM Gene ORDER BY ID DESC LIMIT 1";
    else
        query = "SELECT Count(*) FROM LapTimeRecords";
    
    int rc = sqlite3_prepare_v2(DB, query.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        Error();
        Close();
        return 0;
    }
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        size = sqlite3_column_int (stmt, 0);
    }
    if (rc != SQLITE_DONE)
    {
        Error();
        Close();
        return -1;
    }
    sqlite3_finalize(stmt);
    Close();
    return size;
}

void Database::Error()
{
    std::cerr << "Potential Error in Database: " << sqlite3_errmsg(DB) << std::endl;
}
