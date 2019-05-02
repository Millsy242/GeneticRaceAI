//
//  Car.cpp
//
//  Created by Daniel Harvey
//
#include "Car.h"
#include "Helper.hpp"

Car::Car(Data &constantdata) : speed(1), angle(0),nextPoint(0), ConstantData(constantdata),eyes(ConstantData)
{
    
}
Car::~Car()
{
}

void Car::Init()
{
    carSprite.setTexture(*ConstantData.carSprite->getTexture());
    carSprite.setOrigin(ConstantData.CarOrigin);
    carSprite.setScale(2, 2);
    
    eyes.SetScale(carSprite.getScale().x, carSprite.getScale().y);
    eyes.setSize(chromosome.GetGene(3), 1.0,chromosome.GetGene(2));
    
    carColour = sf::Color(chromosome.Colour[0]*255.f,chromosome.Colour[1]*255.f,chromosome.Colour[2]*255.f);
    carSprite.setColor(carColour);
}

void Car::UpdateSpeedo(Overlay &TextOverlay)
{
    if((speed/maxSpeed) > 0.75)
    {
        TextOverlay.SetColour("Speedo", sf::Color::Red);
    }
    else
    {
        TextOverlay.SetColour("Speedo", sf::Color::White);
    }
    TextOverlay.SetDataOutput("Speedo",std::round(speed*12)); //12 roughly gives a reasonable number for the speed value in mph
    TextOverlay.AddToDataOutput("Speedo", " MPH");
    
    if(PS1)
        TextOverlay.SetColour("Sector1", sf::Color::Magenta);
    else if(GS1)
        TextOverlay.SetColour("Sector1", sf::Color::Green);
    else
        TextOverlay.SetColour("Sector1", sf::Color::White);
    
    if(PS2)
        TextOverlay.SetColour("Sector2", sf::Color::Magenta);
    else if(GS2)
        TextOverlay.SetColour("Sector2", sf::Color::Green);
    else
        TextOverlay.SetColour("Sector2", sf::Color::White);
    
    if(PS3)
        TextOverlay.SetColour("Sector3", sf::Color::Magenta);
    else if(GS3)
        TextOverlay.SetColour("Sector3", sf::Color::Green);
    else
        TextOverlay.SetColour("Sector3", sf::Color::White);
    
    TextOverlay.SetDataOutput("Sector1", "Sector 1: " + GetSector(1));
    TextOverlay.SetDataOutput("Sector2", "Sector 2: " + GetSector(2));
    TextOverlay.SetDataOutput("Sector3", "Sector 3: " + GetSector(3));
    TextOverlay.SetDataOutput("CurrentLap", "Laptime: " + Helper::floatTo3dpString(Helper::AddUpVecFloat(CurrentSectors),3));
    TextOverlay.SetDataOutput("BestLap", "Best Laptime: " + Helper::floatTo3dpString(laptime[laptime.size()-1],3));
}

void Car::UpdateLaptime(sf::Time& elapsedTime)
{
    if(ACTIVE)
    {
        PlayerLapTime+=elapsedTime;
        PlayerSectorTime+=elapsedTime;
    }
}

void Car::UpdateLastCheckpointPosition()
{
    LastTimingBeamPos = sf::Vector2f(Posx,Posy);
}

void Car::Timing(sf::Clock &clock)
{
    
    if(ACTIVE)
    {
        TheGameIsPaused(false);
        /*
         if(speed == 0 && B)
         {
         //Fitness = Helper::GetDistanceBetween2Points(ConstantData.StartPos, sf::Vector2f(Posx/ConstantData.ScaleX,Posy/ConstantData.ScaleY));
         ACTIVE = false;
         carSprite.setColor(sf::Color::Black);
         }
         */
        if(Started)
        {
            if(BPixelUnderCar() == 0 && GPixelUnderCar() == 255 && RPixelUnderCar() == 255)
            {
                if(!MS1)
                {
                    MS1 = true;
                    MS2 = false;
                    MS3 = false;
                    
                    percentageComplete+= 0.005;
                }
                UpdateLastCheckpointPosition();
            }
            else if(BPixelUnderCar() == 0 && GPixelUnderCar() == 255 && RPixelUnderCar() == 250)
            {
                if(!MS2)
                {
                    MS1 = false;
                    MS2 = true;
                    MS3 = false;
                    
                    percentageComplete+= 0.005;
                }
                UpdateLastCheckpointPosition();
            }
            else if(BPixelUnderCar() == 0 && GPixelUnderCar() == 255 && RPixelUnderCar() == 245)
            {
                if(!MS3)
                {
                    MS1 = false;
                    MS2 = false;
                    MS3 = true;
                    
                    percentageComplete+= 0.005;
                }
                UpdateLastCheckpointPosition();
            }
        }
        //StartLine
        if(BPixelUnderCar() == 228 && GPixelUnderCar() == 0 && RPixelUnderCar() == 0)
        {
            if(!Started)
                Started = true;
            if(CS1 && CS2)
            {
                if(BestSectors[2] > PlayerSectorTime.asSeconds())
                {
                    BestSectors[2] = PlayerSectorTime.asSeconds();
                    PS3 = true;
                    GS3 = false;
                }
                else if(CurrentSectors[2] > PlayerSectorTime.asSeconds())
                {
                    PS3 = false;
                    GS3 = true;
                }
                else
                {
                    PS3 = false;
                    GS3 = false;
                }
                CurrentSectors[2] = PlayerSectorTime.asSeconds();
                PlayerSectorTime = clock.restart();
                laptime.push_back(Helper::AddUpVecFloat(CurrentSectors));
                if(BestLapTime>Helper::AddUpVecFloat(CurrentSectors))
                {
                    BestLapTime = Helper::AddUpVecFloat(CurrentSectors);
                }
                PlayerLapTime=clock.restart();
                CurrentLap++;
                percentageComplete = 0;
                CS1 = false;
                CS2 = false;
                
                MS1 = false;
                MS2 = false;
                MS3 = false;
            }
            UpdateLastCheckpointPosition();
        }
        //End of Sector 1
        if(BPixelUnderCar() == 128 && GPixelUnderCar() == 128 && RPixelUnderCar() == 0 && Started)
        {
            if(!CS1)
            {
                if(BestSectors[0] > PlayerSectorTime.asSeconds())
                {
                    BestSectors[0] = PlayerSectorTime.asSeconds();
                    PS1 = true;
                    GS1 = false;
                }
                else if(CurrentSectors[0] > PlayerSectorTime.asSeconds())
                {
                    GS1 = true;
                    PS1 = false;
                }
                else
                {
                    PS1 = false;
                    GS1 = false;
                }
                CurrentSectors[0] = PlayerSectorTime.asSeconds();
                PlayerSectorTime = clock.restart();
                percentageComplete = 0.33;
                CS1 = true;
                
                MS1 = false;
                MS2 = false;
                MS3 = false;
            }
            UpdateLastCheckpointPosition();
        }
        //End of Sector 2
        if(BPixelUnderCar() == 128 && GPixelUnderCar() == 128 && RPixelUnderCar() == 128 && Started)
        {
            if(CS1 && !CS2)
            {
                if(BestSectors[1] > PlayerSectorTime.asSeconds())
                {
                    BestSectors[1] = PlayerSectorTime.asSeconds();
                    GS2 = false;
                    PS2 = true;
                }
                else if(CurrentSectors[1] > PlayerSectorTime.asSeconds())
                {
                    GS2 = true;
                    PS2 = false;
                }
                else
                {
                    GS2 = false;
                    PS2 = false;
                }
                CurrentSectors[1] = PlayerSectorTime.asSeconds();
                PlayerSectorTime = clock.restart();
                percentageComplete = 0.66;
                CS2 = true;
            }
            UpdateLastCheckpointPosition();
            MS1 = false;
            MS2 = false;
            MS3 = false;
        }
    }
}
void Car::CarControls()
{
    D = false;B = false;L = false;R = false;
    if(!ConstantData.usePlayer)
    {
        LM = std::llround(eyes.GetDistance(eSLine::eLeft, eSLine::eMiddle)*2) / 2.0;
        MR = std::llround(eyes.GetDistance(eSLine::eMiddle, eSLine::eRight)*2) / 2.0;
        CM = eyes.GetDistance(eSLine::eCenter, eSLine::eMiddle);
        
        D = true;
        B = false;
        
        if( LM<MR )
        {
            L = true;
        }
        else if(LM > MR)
        {
            R = true;
        }
        
        
        if(eyes.breakingPointAlpha() >= chromosome.GetGene(0) && speed > chromosome.GetGene(4))
        {
            B = true;
            
        }
        if(eyes.GetColour(eSLine::eLeft).r == 255 && eyes.GetColour(eSLine::eLeft).g == 1 && eyes.GetColour(eSLine::eLeft).b >= chromosome.GetGene(1))
        {
            R = true;
            L = false;
        }
        else if(eyes.GetColour(eSLine::eRight).r == 255 && eyes.GetColour(eSLine::eRight).g == 1 && eyes.GetColour(eSLine::eRight).b >= chromosome.GetGene(1))
        {
            L = true;
            R = false;
            
        }
    }
    else
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            R=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            B=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            L=1;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            D=1;
        }
    }
    MoveCar(D,R,L,B);
    
}

void Car::MoveCar(bool Drive,bool Right, bool Left, bool Brake)
{
    
    float turnSpeed_temp = turnSpeed * (1-(speed/maxSpeed));
    
    if(speed > 0)
    {
        if(Right || Left)
        {
            if(speed > 3)
                speed-=(acc*0.75);
            speed-=0.0001; // some speed is scrubbed off when turning.
            
            if(Left)
            {
                turnSpeed_temp = -turnSpeed_temp;
                carSprite.rotate(turnSpeed_temp);
            }
            else if(Right)
            {
                carSprite.rotate(turnSpeed_temp);
            }
        }
        if(Brake)
        {
            speed -= drag;
            speed -= brakepower;
        }
    }
    if(Drive)
    {
        if (speed<maxSpeed)
        {
            speed += acc;
        }
    }
    else
    {
        if (speed - drag > 0)
        {
            speed -= drag;
        }
        else if (speed + drag < 0)
        {
            speed += drag ;
        }
        else
        {
            speed = 0;
        }
    }
    if(Brake)
    {
        speed -= drag;
        speed -= brakepower;
    }
    //Slow the car down when it goes off circuit
    
    if(BPixelUnderCar() == 255 && GPixelUnderCar() == 255 && RPixelUnderCar() == 255)
    {
        if(speed > offTrackSpeed)
        {
            speed -= acc*2;
        }
    }
    
    if(BPixelUnderCar() == 0 && GPixelUnderCar() == 0 && RPixelUnderCar() == 255)
    {
        speed = 0;
        if(!ConstantData.usePlayer)
        {
            ACTIVE = false;
        }
    }
    if(speed < 0)
    {
        if(!ConstantData.usePlayer)
        {
            ACTIVE = false;
        }
        speed = 0; //make sure car doesn't go backwards.
    }
    
    sf::Vector2f oldVec = movementVec;
    
    sf::Transform t;
    t.rotate(carSprite.getRotation());
    movementVec = t.transformPoint(forwardVec);
    //calc the dot product so any rotation reduces the
    //current speed - gives the impression of wheel spin
    //when accelerating after a tight turn
    speed *=  Helper::dot(oldVec, movementVec);
    oldPos = sf::Vector2f(Posx,Posy);
    
    Posx += movementVec.x * speed;
    Posy += movementVec.y * speed;
}

void Car::ResetPosition()
{
    Posx = oldPos.x;
    Posy = oldPos.y;
}

void Car::Render(sf::RenderWindow &window, std::pair<int,int> offset)
{
    carSprite.setPosition(Posx-offset.first,Posy-offset.second);
    //carSprite.rotate(theta);
    eyes.SetRotation(carSprite.getRotation());
    eyes.SetPosition(sf::Vector2f(Posx-offset.first,Posy-offset.second),offset);
    
    if(!ACTIVE)
    {
        if(carColour.a > 0)
        {
            carColour.a--;
            carSprite.setColor(carColour);
        }
    }
    
    window.draw(carSprite);
    
    if(ACTIVE && ConstantData.ShowEyes)
        eyes.Render(window);
    
}
void Car::TheGameIsPaused(bool Pause)
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
void Car::STOP()
{
    STOPPED = true;
}
void Car::Highlight(bool highlight)
{
    if(highlight)
    {
        carColour.a = 255;
        carSprite.setColor(carColour);
    }
}


float Car::GetMaxSpeed()
{
    return maxSpeed;
}
float Car::getLapsComplete(bool playing)
{
    if(playing && !ACTIVE) //This is to help the sorting algorithm remove
        return 0.f;
    
    return float(CurrentLap+percentageComplete);
}

float Car::getFitness()
{
    auto fitnessTotal = getLapsComplete(false);
    fitnessTotal *= (301.f - BestLapTime);
    Fitness = fitnessTotal;
    return Fitness;
}

float Car::getFastestLap()
{
    return BestLapTime;
}

bool Car::LoadFromFile()
{
    std::string line;
    
    std::ifstream myfile(ConstantData.CarFilePath);
    bool exit = false;
    int i = 0;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) && !exit)
        {
            switch(i)
            {
                case(0):
                    break;
                case(1):
                    maxSpeed = std::stof(line);
                    break;
                case(2):
                    acc = std::stof(line) ;
                    break;
                case(3):
                    drag = std::stof(line);
                    break;
                case(4):
                    brakepower = std::stof(line) ;
                    break;
                case(5):
                    turnSpeed = std::stof(line) ;
                    break;
                case(6):
                    offTrackSpeed = std::stof(line);
                    break;
                case(7):
                    break;
                case(8):
                    exit = true;
                    break;
                default:
                    break;
            }
            i++;
        }
        myfile.close();
        return true;
    }
    return defaultCar();
    
}

std::string Car::GetSector(int x)
{
    switch (x)
    {
        case (1):
            if(CurrentSectors[0] < 80)
            {
                return Helper::floatTo3dpString(CurrentSectors[0],3);
            }
            else
            {
                return "_.______";
            }
            break;
        case (2):
            if(CurrentSectors[1] < 80)
            {
                return Helper::floatTo3dpString(CurrentSectors[1],3);
            }
            else
            {
                return "_.______";
            }
            break;
        case (3):
            if(CurrentSectors[2] < 80)
            {
                return Helper::floatTo3dpString(CurrentSectors[2],3);
            }
            else
            {
                return "_.______";
            }
            break;
        default:
            return "OOPS theres only sectors 1(0), 2(1) and 3(2)";
    }
}


//This is only used when a file cannot be loaded
//It should never be used.
//Probably out of date now
bool Car::defaultCar()
{
    std::cerr << "OOPS missing file for car " << id << "\nProbably missing a Players Folder" << std::endl;
    maxSpeed=15.0;
    acc=0.015;
    drag=0.02;
    brakepower = acc * 3.3; //3.3 calculated using braking sums.xlsx
    turnSpeed=0.095;
    offTrackSpeed = 1.f;
    return true;
}

bool Car::IsOnTrack()
{
    if(ConstantData.Track->getSize().x >= Posx/ConstantData.ScaleX && ConstantData.Track->getSize().y >= Posy/ConstantData.ScaleY && 0 <= Posx && 0 <= Posy)
        return true;
    else
        return false;
}

int Car::RPixelUnderCar()
{
    if(IsOnTrack())
        return ConstantData.Track->getPixel(Posx/ConstantData.ScaleX,Posy/ConstantData.ScaleY).r;
    else
        return 0;
}

int Car::GPixelUnderCar()
{
    if(IsOnTrack())
        return ConstantData.Track->getPixel(Posx/ConstantData.ScaleX,Posy/ConstantData.ScaleY).g;
    else
        return 0;
}

int Car::BPixelUnderCar()
{
    if(IsOnTrack())
        return ConstantData.Track->getPixel(Posx/ConstantData.ScaleX,Posy/ConstantData.ScaleY).b;
    else
        return 0;
}

int Car::APixelUnderCar()
{
    if(IsOnTrack())
        return ConstantData.Track->getPixel(Posx/ConstantData.ScaleX,Posy/ConstantData.ScaleY).a;
    else
        return 0;
}

int Car::ALLPixelUnderCar()
{
    if(IsOnTrack())
        return ConstantData.Track->getPixel(Posx/ConstantData.ScaleX,Posy/ConstantData.ScaleY).toInteger();
    else
        return 0;
}
