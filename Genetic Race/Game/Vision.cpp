//
//  Vision.cpp
//  t02
//
//  Created by Daniel Harvey on 23/02/2019.
//

#include "Vision.hpp"
#include <math.h>
#include "Helper.hpp"

Vision::Vision(Data &constantdata) : ConstantData(constantdata)
{
    SightLine MidLeft(constantdata);
    SightLine Center(constantdata);
    SightLine MidRight(constantdata);

    MidLeft.Init(Size, CircleSize, 240, sf::Color::Green );
    Center.Init(Size, CircleSize, 270, sf::Color::Blue);
    MidRight.Init(Size, CircleSize, 300, sf::Color::Magenta);
    
    Sight.push_back(MidLeft);
    Sight.push_back(Center);
    Sight.push_back(MidRight);
}
sf::Color Vision::GetColour(eSLine side)
{
    int SideIndex = 0;
    if(side == eSLine::eLeft)
    {
        SideIndex = 0;
    }
    else if(side == eSLine::eMiddle)
    {
        SideIndex = 1;
    }
    else if(side == eSLine::eRight)
    {
        SideIndex = 2;
    }
    else if(side == eSLine::eCenter)
    {
        SideIndex = 1;
    }
    
    return sf::Color(Sight[SideIndex].ALLPixelUnderCar());
}

void Vision::SetScale(int scaleX, int scaleY)
{
    for(int i{0}; i<Sight.size(); i++)
    {
        Sight[i].SetScale(sf::Vector2f(scaleX,scaleY));
    }
}
void Vision::setSize(float x, float y, bool LongerMiddle)
{
    Size = sf::Vector2f(x,y); 
    for(int i{0}; i<Sight.size(); i++)
    {
        Sight[i].SetSize(sf::Vector2f(x,y));
        
    }
    if(LongerMiddle)
        Sight[1].SetSize(sf::Vector2f(Size.x + Size.x/3,y));
    
}
void Vision::SetRotation(float rotation)
{
    for(int i{0}; i<Sight.size(); i++)
    {
        Sight[i].SetRotation(rotation);
    }
}
void Vision::SetPosition(sf::Vector2f position, std::pair<int, int> offset)
{
    CarPosition = position;
    
    for(int i{0}; i<Sight.size(); i++)
    {
        Sight[i].SetPosition(position, offset);
    }
}
void Vision::Render(sf::RenderWindow &window)
{
    for(int i{0}; i<Sight.size(); i++)
    {
        Sight[i].Render(window);
    }
}

int Vision::GetDistance(eSLine Line1, eSLine Line2)
{
    if(Line1 == Line2)
        return 0;
    if(Line1 == eSLine::eLeft)
    {
        if(Line2 == eSLine::eRight)
            return Helper::GetDistanceBetween2Points(Sight[0].GetPosition(),Sight[2].GetPosition());
        if(Line2 == eSLine::eMiddle)
            return Helper::GetDistanceBetween2Points(Sight[0].GetPosition(),Sight[1].GetPosition());
        if(Line2 == eSLine::eCenter)
            return Helper::GetDistanceBetween2Points(Sight[0].GetPosition(),CarPosition);
    }
    else if(Line1 == eSLine::eMiddle)
    {
        if(Line2 == eSLine::eRight)
            return Helper::GetDistanceBetween2Points(Sight[1].GetPosition(),Sight[2].GetPosition());
        if(Line2 == eSLine::eLeft)
            return Helper::GetDistanceBetween2Points(Sight[1].GetPosition(),Sight[0].GetPosition());
        if(Line2 == eSLine::eCenter)
            return Helper::GetDistanceBetween2Points(Sight[1].GetPosition(),CarPosition);
    }
    else if(Line1 == eSLine::eRight)
    {
        if(Line2 == eSLine::eLeft)
            return Helper::GetDistanceBetween2Points(Sight[2].GetPosition(),Sight[0].GetPosition());
        if(Line2 == eSLine::eMiddle)
            return Helper::GetDistanceBetween2Points(Sight[2].GetPosition(),Sight[1].GetPosition());
        if(Line2 == eSLine::eCenter)
            return Helper::GetDistanceBetween2Points(Sight[2].GetPosition(),CarPosition);
    }
    else if(Line1 == eSLine::eCenter)
    {
        if(Line2 == eSLine::eLeft)
            return Helper::GetDistanceBetween2Points(CarPosition,Sight[0].GetPosition());
        if(Line2 == eSLine::eMiddle)
            return Helper::GetDistanceBetween2Points(CarPosition,Sight[1].GetPosition());
        if(Line2 == eSLine::eRight)
            return Helper::GetDistanceBetween2Points(CarPosition,Sight[2].GetPosition());
    }
    
    return 0;
}

int Vision::breakingPointAlpha()
{
    std::vector<int> brakingalpha;
    brakingalpha.clear();
    for(int i{0}; i<Sight.size(); i++)
    {
        if(Sight[i].BrakingZone())
        {
            brakingalpha.push_back(Sight[i].getBrakingAlpha());
        }
    }
    int averageA = -1;
    if(brakingalpha.size() > 0)
    {
        averageA = 0;
        for(int i{0}; i<brakingalpha.size(); i++)
        {
            averageA+=brakingalpha[i];
        }
        averageA = averageA/brakingalpha.size();
    }
    return averageA;
}
