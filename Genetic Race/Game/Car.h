//
//  Car.h
//
//  Created by Daniel Harvey 
//

#ifndef CAR_H
#define CAR_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <utility>
#include <vector>
#include "overlay.h"
#include <string>
#include "DataStruct.hpp"
#include "Chromosome.hpp"
#include "Vision.hpp"

class Car
{
    public:
    
    Car(Data &constantdata);
    ~Car();
    
    bool operator<( Car& val )
    {
        return this->getFitness() < val.getFitness();
    }

    void Init();
    void CarControls();
    void MoveCar(bool Drive,bool Right, bool Left, bool Brake);
    void Timing(sf::Clock &clock);
    void Render(sf::RenderWindow &window, std::pair<int,int> offset);
    void UpdateSpeedo(Overlay &TextOverlay);
    void UpdateLaptime(sf::Time& elapsedTime);
    void ResetPosition();
    
    void STOP();
    void TheGameIsPaused(bool Pause);
    void Highlight(bool highlight);
    void setPosX(float x) {Posx = x;};
    void setPosY(float y) {Posy = y;};
    void SetActive(bool active) {ACTIVE = active;};
  
    float getFitness();
    float getFastestLap();
    float GetMaxSpeed();
    float getLapsComplete(bool playing);
    float getPosX() {return Posx;};
    float getPosY() {return Posy;};
    
    bool LoadFromFile();
    bool GetActive() {return ACTIVE;};
    bool IsOnTrack();
    bool Started = false; //Set to true after they cross the finish line once

    std::string id;
    
    std::vector<float> BestSectors = {99.f,99.f,99.f};
    std::vector<float> CurrentSectors = {99.f,99.f,99.f};
    std::vector<float> laptime = {99,99,99};
    
    float BestLapTime = 300.f;
    float percentageComplete = 0.0;
    
    bool STOPPED = false;
    
    Chromosome chromosome;
    
private:
    bool defaultCar();
    
    int RPixelUnderCar(); //Red
    int GPixelUnderCar();//Green
    int BPixelUnderCar();//Blue
    int APixelUnderCar();//Alpha
    int ALLPixelUnderCar();//All
    void UpdateLastCheckpointPosition();

    std::string GetSector(int x);
    
    sf::Time PlayerSectorTime;
    sf::Time PlayerLapTime;
    sf::Color carColour;
    sf::Sprite carSprite;
    sf::Vector2f oldPos;
    sf::Vector2f LastTimingBeamPos;
    sf::Vector2f movementVec; //normal vector based on current direction
    sf::Vector2f forwardVec = sf::Vector2f(0.f, -1.f); //normal vec pointing forward

    bool PS1,PS2,PS3; //Purple sector 1,2,3
    bool GS1,GS2,GS3; //Green sector 1,2,3
    bool CS1=false,CS2=false; //Completed sector 1,2,3
    bool MS1{false},MS2{false},MS3{false};
    bool D{false},B{false},L{false},R{false};
    bool ACTIVE = true;
    
    float LM = 0; // Left Sightline to Middle Sightline
    float MR = 0; // Middle Sightline to Right Sightline
    float CM = 0; // Car Origin to Middle Sightline
    float fitness = 0.f;
    float maxSpeed=15.0;
    float acc=0.015;
    float drag=0.01;
    float brakepower = 0.2;
    float turnSpeed=1.f;
    float offTrackSpeed = 1.f;
    float Posx,Posy,speed,angle;
    float Fitness = 0;
    
    int inputTime = 0;
    int CurrentLap = 0;
    int nextPoint{0};
    
    Data &ConstantData;
    Vision eyes;
};
#endif // CAR_H
