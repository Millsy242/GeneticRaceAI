#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <utility>
#include <vector>
#include "overlay.h"
#include <fstream>
#include <string>
#include "DataStruct.hpp"
#include "Chromosome.hpp"
#include "Vision.hpp"

class Car
{
    public:
    Car(Data &constantdata);

    ~Car();
    
    bool operator<(  Car& val )
    {
        return this->getFitness() < val.getFitness();
    }

    bool LoadFromFile();
    void AIMovement();

    void PlayerMovement(bool Drive,bool Right, bool Left, bool Brake);

    void Timing(sf::Clock &clock);
    
    void Render(sf::RenderWindow &window, std::pair<int,int> offset);

    void UpdatePlayerSpeedo(Overlay &TextOverlay);
    void UpdateLaptime(sf::Time& elapsedTime);
    void AdjustFitness(float adjustment)
    {
        
    }
    
    void ResetPosition();
    void Init();
    void STOP()
    {
        STOPPED = true; 
    }
    bool STOPPED = false;
    
    void TheGameIsPaused(bool Pause)
    {
        if(Pause )
        {
            carColour.a = 5;
            carSprite.setColor(carColour);
        }
        else if (!Pause)
        {
            carColour.a = 255;
            carSprite.setColor(carColour);
        }
            
    }
    void Highlight(bool highlight)
    {
        if(highlight)
        {
            carColour.a = 255;
            carSprite.setColor(carColour);            
        }
    }

    std::vector<float> BestSectors = {99.f,99.f,99.f};
    std::vector<float> CurrentSectors = {99.f,99.f,99.f};
    std::vector<float> laptime = {99,99,99};
    float BestLapTime = 300.f; 

    float maxSpeed=15.0;

    sf::Color carColour ;
    
     float getFitness();
    
    float getFastestLap();
    
    float getLapsComplete(bool playing);
    float getPosX() {return Posx;};
    float getPosY() {return Posy;};
    void  setPosX(float x) {Posx = x;};
    void setPosY(float y) {Posy = y;};
    
    bool PS1,PS2,PS3; //Purple sector 1,2,3
    bool GS1,GS2,GS3; //Green sector 1,2,3
    bool CS1=false,CS2=false; //Completed sector 1,2,3
    bool Started = false; //Set to true after they cross the finish line once
    
    void SetActive(bool active) {ACTIVE = active;};
    bool GetActive() {return ACTIVE;};
    
    float percentageComplete = 0.0;
    
    std::string Name;
    std::string id;
    Chromosome chromosome;
    
    bool IsOnTrack();
    
    float GetMaxSpeed();
    
private:
    bool defaultCar();
    
    int RPixelUnderCar(); //Red
    int GPixelUnderCar();//Green
    int BPixelUnderCar();//Blue
    int APixelUnderCar();//Alpha
    int ALLPixelUnderCar();//All
    void UpdateLastCheckpointPosition();

    std::string GetSector(int x);

    
    float LM =0;
    float MR = 0;
    float CM = 0;
    
    sf::Sprite carSprite;
    float fitness = 0.f;

    float acc=0.015, drag=0.01;
    float brakepower = 0.2;
    float turnSpeed=1.f;
    float offTrackSpeed = 1.f;
    
    Data &ConstantData;

    Vision eyes;
    int inputTime = 0; 

    bool MS1{false},MS2{false},MS3{false};
    bool D{false},B{false},L{false},R{false};
    sf::Vector2f oldPos; 
    
    float Posx,Posy,speed,angle;
    int nextPoint{0};
    sf::Vector2f LastTimingBeamPos;

    
    int CurrentLap=0;
    bool ACTIVE = true;

    sf::Time PlayerSectorTime;
    sf::Time PlayerLapTime;
    
    float TargetX{-1}, TargetY{-1}; 
    
    int frames = 0; //This is used to limit the sending of position data    
    
    sf::Vector2f movementVec; //normal vector based on current direction
    sf::Vector2f forwardVec = sf::Vector2f(0.f, -1.f); //normal vec pointing forward
    
    
    sf::Vector2f location;
    sf::Vector2f velocity = sf::Vector2f(0,0);
    sf::Vector2f acceleration = sf::Vector2f(0,0);
    float r = 3.0f;
    float maxforce = 0.1;
    float maxspeed = 4;
    
    void Seek(sf::Vector2f target);
    void ApplyForce(sf::Vector2f force);
    
    float Fitness = 0;
    
};

#endif // CAR_H


