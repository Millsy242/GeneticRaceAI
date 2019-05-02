//Game class
// Created by Dnaiel Harvey
// Manages the SFML Window, time, rendering, etc. 
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
    
private:
    void GamePlay();
    void Render();
    void OrganiseStats();
    void UserInterface();
    void SetupPostRace();
    void UpdateOverlays();
    
    bool PAUSE = false;
    bool Exit = false;
    bool Racing = true;
    
    PopulationManager popManager;
    Overlay TextOverlay;
    
    sf::RenderWindow GameWindow;
    
    sf::Sprite BackgroundSprite;
    sf::Texture BackgroundTexture;
    
    Data &ConstantData;
    
    int Generation = 0;
    
    const sf::Time TimePerFrame = sf::seconds(1.f/60.f);
    sf::Time OverallPlayerLapTime;
    sf::Time mStatisticsUpdateTime;
    sf::Time TotalTime;
    sf::Time timeSinceLastUpdate;
    
    std::size_t	mStatisticsNumFrames;
};


#endif // GAME_H

