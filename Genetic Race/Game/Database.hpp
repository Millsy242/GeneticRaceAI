//
//  Database.hpp
//  t02
//
//  Created by Daniel Harvey on 08/04/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
//

#ifndef Database_hpp
#define Database_hpp
#include <iostream>
#include <sqlite3.h>
#include "Helper.hpp"
#include <string>
#include <cstring>
#include <algorithm>

enum eTable
{
    eGene = 0,eLapTime = 1
};


struct GeneTableStruct
{
    const eTable type = eTable::eGene;
    int ID = -1;
    std::string Name= "ERR!";
    std::string DateTime= "ERR!";
    int TrackNum = -1;;
    float FastestLap = -1;
    std::string chromosome= "ERR!";
    int Generation = -1;
    
    std::string toString()
    {
        return DateTime + " - " + Name + " - " + std::to_string(TrackNum) + " - " + std::to_string(FastestLap);
    }

};

struct LapTimeTableStruct
{
    const eTable type = eTable::eLapTime;
    int TrackID = -1;
    std::string DateTime = "ERR!" ;
    std::string Name= "ERR!";
    float LapTime = 300;
    std::string chromosome= "ERR!";
    
    std::string toString()
    {
            return DateTime + " - " + Name + " - " + std::to_string(TrackID) + " - " + std::to_string(LapTime);
    }

};


class Database
{
public:
    Database();
    
    bool Open(const char *path);
    bool Create(const char *path);
    bool Close();
    bool Select(std::string query, GeneTableStruct &gts);
    bool Select(std::string query, LapTimeTableStruct &lts);
    int getSize(eTable table);
    bool Insert(std::string sql);
    bool Delete(std::string sql);
    
private:
    void Error();
    bool CreateTable(const char *path, std::string sql);
    sqlite3* DB;
    const char *Lastpath;
    


    
};



#endif /* Database_hpp */
