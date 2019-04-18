
#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include "overlay.h"
#include <iostream>
#include <cstring>
#include <string>
#include <thread>
#include "DataStruct.hpp"
#include <chrono>
#include <iomanip>
#include <ctime>
#include "Helper.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Chromosome.hpp"
#include "Database.hpp"


struct carstats
{
    float MaxSpeed = 15.0 * 12.f;
    float acceleration = 0.02;
    float Drag = 0.01;
    float BrakePower = 0.08;
    float TurnSpeed = 3.0;
    float OffTrackSpeed = 1.0 * 12.f;
    
    
    void SaveToFile(std::string filename)
    {
        std::ofstream myfile (filename);
        
        if (myfile.is_open())
        {
            myfile << "\n" << MaxSpeed/12.f << "\n"<< acceleration << "\n"<< Drag << "\n"<< BrakePower << "\n"<< TurnSpeed << "\n" << OffTrackSpeed/12.f << "\n\nEnd";
            myfile.close();
        }
    }
    
    void LoadFromFile(std::string filename)
    {
        std::string line;
        
        std::ifstream myfile (filename);
        bool exit = false;
        int i = 0;
        if (myfile.is_open())
        {
            while ( getline (myfile,line) && !exit)
            {
                if(!line.find("End"))
                    exit = true;
                switch(i)
                {
                    case(0):
                        break;
                    case(1):
                        MaxSpeed = std::stof(line) * 12.f;
                        break;
                    case(2):
                        acceleration = std::stof(line) ;
                        break;
                    case(3):
                        Drag = std::stof(line);
                        break;
                    case(4):
                        BrakePower = std::stof(line) ;
                        break;
                    case(5):
                        TurnSpeed = std::stof(line) ;
                        break;
                    case(6):
                        OffTrackSpeed = std::stof(line) * 12.f;
                        break;
                    default:
                        break;
                }
                i++;
            }
            myfile.close();

        }
    }
    
    
};


class Menu
{
    public:
        Menu(Data &constantdata);
        virtual ~Menu();
        void Run();

    private:
    
    Data &ConstantData;
    
    const std::string currentDateTime_();
    
    void Setup();
    void Loop(); 
    void Input();
    void Update();
    void Render();
    void OrganiseStats();
    void MainMenu();
    void CarEditor();
    void GeneChooser();
    void GeneEditor(); 
    void Load();
    void AlgorithmEditor();

    sf::Texture track;
    sf::Sprite tracksprite; 
   
    carstats CarStats; 

    sf::RenderWindow window;

    Overlay LobbyItems;
    bool GeneSelector = false;
    
    sf::Time mStatisticsUpdateTime;
    sf::Time elapsedTime;
    std::size_t mStatisticsNumFrames;
    sf::Clock clock;
    sf::Time TotalTime;
    sf::Time timeSinceLastUpdate;
    const sf::Time TimePerFrame = sf::seconds(1.f/60.f);

    bool user = false;
    bool Records = false;
    int selectedgenesplace = 0; 

    std::vector<GeneTableStruct> genereturns;
    std::vector<LapTimeTableStruct> Lapreturns;
    std::vector<std::string> listbox_items;
    std::vector<std::string> SelectedItems; 
    bool Loaded = false;
    int sizeGene = -1;
    int sizeLap = -1;

    int GeneListPlace = 0;
    int RecordListPlace = 0;
    
    std::pair<int,int> Breedingspot = {-1,-1}; 
    
    

};

#endif // MENU_H
