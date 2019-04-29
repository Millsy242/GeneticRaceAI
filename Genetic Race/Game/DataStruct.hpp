//
//  DataStruct.hpp
//  t02
//
//  Created by HARVEY, DANIEL on 26/11/2018.
//
//

#ifndef DataStruct_hpp
#define DataStruct_hpp

#include <stdio.h>
#include <vector>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include "Chromosome.hpp"


struct LapTimeTableStruct;
class Database;
struct Data
{
    void Init()
    {
        Track = new sf::Image();
        carSprite = new sf::Sprite();
        WindowPos = sf::Vector2i(0,0);
        StartPos = sf::Vector2f(0.f,0.f);
        Playing = false;
        if(!CarTexture.loadFromFile("images/car.png"))
        {
            std::cerr << "Cant load Car Texture" << std::endl;
        }
        CarTexture.setSmooth(true);
        carSprite->setTexture(CarTexture,true);
        LoadDataFromFile();
    }
    void End()
    {
        SaveDataToFile(); 
    }
    
    void LoadDataFromFile()
    {
        std::string line;        
        std::ifstream myfile ("Config.txt");
        bool exit = false;
        if (myfile.is_open())
        {
            while ( getline (myfile,line) && !exit)
            {
                if(!line.find("End"))
                {
                    exit = true;
                }
                if(!line.find("PopSize"))
                {
                    line = line.substr(std::string("PopSize:").size());
                    PopSize = std::stoi(line);
                }
                else if(!line.find("ScreenW"))
                {
                    line = line.substr(std::string("ScreenW:").size());
                    ScreenWidth = std::stoi(line);
                }
                else if(!line.find("ScreenH"))
                {
                    line = line.substr(std::string("ScreenH:").size());
                    ScreenHeight = std::stoi(line);
                }
                else if(!line.find("Laps"))
                {
                    line = line.substr(std::string("Laps:").size());
                    laps = std::stoi(line);
                }
                else if(!line.find("ScaleX"))
                {
                    line = line.substr(std::string("ScaleX:").size());
                    ScaleX = std::stoi(line);
                }
                else if(!line.find("ScaleY"))
                {
                    line = line.substr(std::string("ScaleY:").size());
                    ScaleY = std::stoi(line);
                }
                else if(!line.find("GenTime"))
                {
                    line = line.substr(std::string("GenTime:").size());
                    GenerationLength = sf::seconds(std::stof(line));
                }
                else if(!line.find("Track"))
                {
                    line = line.substr(std::string("Track:").size());
                    TrackNumber = std::stoi(line);
                }
                else if(!line.find("CarFilePath"))
                {
                    line = line.substr(std::string("CarFilePath:").size());
                    CarFilePath = (char*)line.c_str();
                }
                else if(!line.find("NumTracks"))
                {
                    line = line.substr(std::string("NumTracks:").size());
                    NumberOfTracks = std::stoi(line);
                }
                else if(!line.find("CarOrigin"))
                {
                    line = line.substr(std::string("CarOrigin:").size());
                    CarOrigin = sf::Vector2f(std::stof(line.substr(0,line.find(","))),std::stof(line.substr(line.find(",")+1, line.size())));
                }
                else if(!line.find("LapLimit"))
                {
                    line = line.substr(std::string("LapLimit:").size());
                    StopOnLaps = (bool)std::stoi(line);
                }
                else if(!line.find("EnableElitism"))
                {
                    line = line.substr(std::string("EnableElitism:").size());
                    Elitism = (bool)std::stoi(line);
                }
                else if(!line.find("ElitismNum"))
                {
                    line = line.substr(std::string("ElitismNum:").size());
                    ElitismNum = std::stoi(line);
                }
                else if(!line.find("MutateElities"))
                {
                    line = line.substr(std::string("MutateElities:").size());
                    MutateElites = (bool)std::stoi(line);
                }
                else if(!line.find("MutationRate"))
                {
                    line = line.substr(std::string("MutationRate:").size());
                    MutationRate = std::stoi(line);
                }
            }
            myfile.close();
        }
        else
        {
            SaveDataToFile();
        }
    }

    void SaveDataToFile()
    {
        std::ofstream myfile ("Config.txt");

        if (myfile.is_open())
        {
            myfile << "PopSize:" << PopSize <<
                    "\nScreenW:"<< ScreenWidth <<
                    "\nScreenH:"<< ScreenHeight <<
                    "\nLaps:"<< laps <<
                    "\nScaleX:"<< ScaleX <<
                    "\nScaleY:"<< ScaleY <<
                    "\nGenTime:"<< GenerationLength.asSeconds() <<
                    "\nTrack:"<< TrackNumber <<
                    "\nCarFilePath:"<< std::string(CarFilePath) <<
                    "\nNumTracks:"<< NumberOfTracks <<
                    "\nCarOrigin:"<< CarOrigin.x << ","<<CarOrigin.y<<
                    "\nLapLimit:"<< StopOnLaps <<
                    "\nEnableElitism:"<< Elitism <<
                    "\nElitismNum:" << ElitismNum <<
                    "\nMutateElities:"<<MutateElites <<
                    "\nMutationRate:"<<MutationRate<<
                    "\nEnd";
            myfile.close();
        }
    }
    int PopSize = 100;
    int ScreenHeight{900};
    int ScreenWidth{1200};
    int laps = 1;
    int ScaleX = 1;
    int ScaleY = 1;
    sf::Time GenerationLength = sf::seconds(10.f);
    int TrackNumber = 1;
    int NumberOfTracks = 1;
    sf::Vector2f CarOrigin; 
    
    std::string CarFilePath = "Players/Player.txt"; 
    sf::Image *Track;
    sf::Sprite *carSprite;
    sf::Vector2f StartPos;
    bool Playing = false;
    bool usePlayer = false;
    sf::Vector2i WindowPos;
    std::vector<std::pair<int,int>> pointvector;
    sf::Time elapsedTime;
    sf::Clock clock;
    Database *database;
    LapTimeTableStruct* FastestLapTimeData; 
    bool ShowEyes = false;
    sf::Texture CarTexture;
    std::vector<std::pair<std::string,std::string>> savedChromosomes;
    std::vector<Chromosome> Chromosomes;
    bool Elitism = false;
    bool MutateElites = false; 
    int ElitismNum = 0;
    int MutationRate = 70;
    bool StopOnLaps = false;

};

#endif /* DataStruct_hpp */
