#ifndef GAME_H
#define GAME_H
#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <ctime>
#include <utility>
#include <vector>
#include "Car.h"
#include "overlay.h"
#include "trackpoints.h"
#include <math.h>
#include <fstream>
#include <chrono>
#include "DataStruct.hpp"
#include <string>
#include "PopulationManager.hpp"


class Game
{
public:
    Game(Data &constantdata);
    virtual ~Game();

    bool Start();
    bool Setup();
    bool Run();
    void GamePlay();
    void Render();
    void OrganiseStats();
    void Input();
    void Movement();
    void Collisions();

private:

    void SaveDataToFile();
    void SetupPostRace();
    void UpdateOverlays();
    
    bool PAUSE = false;
    
    PopulationManager popManager; 
    
    sf::RenderWindow GWindow;
    
    sf::Sprite BackgroundSprite;
    sf::Texture BackgroundTexture;
    
    Overlay TextOverlay;
    
    Data &ConstantData;


    int r{0},l{0};
    int updateCamera = 0;
    int Generation = 0; 

    std::string floatTo3dpString(float f);
    bool Exit = false;
    bool Racing = true;

    const sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time OverallPlayerLapTime;
    sf::Time mStatisticsUpdateTime;
    
    std::size_t	mStatisticsNumFrames;
    
    sf::Time TotalTime;
    sf::Time timeSinceLastUpdate;
    
    std::vector<std::string> Standings; 

};


#endif // GAME_H

