//
//  SightLine.cpp
//  t02
//
//  Created by Daniel Harvey on 04/03/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
//

#include "SightLine.hpp"

SightLine::SightLine(Data &constantdata) : ConstantData(constantdata)
{
    
}
void SightLine::Init(sf::Vector2f size,float CircleSize, float Rotation, sf::Color Colour)
{
    Size = size;
    circleSize = CircleSize;
    rotation = Rotation;
    colour = Colour;
    
    line = sf::RectangleShape(Size);
    line.setFillColor(colour);
    line.rotate(rotation);
    line.setOrigin(0, line.getSize().y/2);
    line.setSize( Size);
    
    circle = sf::CircleShape(circleSize, 20);
    circle.setFillColor(colour);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
}
int SightLine::RPixel()
{
    if(OnTrack())
        return ConstantData.Track->getPixel((circle.getPosition().x+CurrentOffset.x)*ConstantData.ScaleX,(circle.getPosition().y+CurrentOffset.y)*ConstantData.ScaleY).r;
    else
        return -1;
}
int SightLine::GPixel()
{
    if(OnTrack())
        return ConstantData.Track->getPixel((circle.getPosition().x+CurrentOffset.x)*ConstantData.ScaleX,(circle.getPosition().y+CurrentOffset.y)*ConstantData.ScaleY).g;
    else
        return -1;
}
int SightLine::BPixel()
{
    if(OnTrack())
        return ConstantData.Track->getPixel((circle.getPosition().x+CurrentOffset.x)*ConstantData.ScaleX,(circle.getPosition().y+CurrentOffset.y)*ConstantData.ScaleY).b;
    else
        return -1;
}
int SightLine::APixel()
{
    if(OnTrack())
        return ConstantData.Track->getPixel((circle.getPosition().x+CurrentOffset.x)*ConstantData.ScaleX,(circle.getPosition().y+CurrentOffset.y)*ConstantData.ScaleY).a;
    else
        return -1;
}
int SightLine::ALLPixelUnderCar()
{
    if(OnTrack())
        return ConstantData.Track->getPixel((circle.getPosition().x+CurrentOffset.x)*ConstantData.ScaleX,(circle.getPosition().y+CurrentOffset.y)*ConstantData.ScaleY).toInteger();
    else
        return 0;
}
sf::Vector2f SightLine::GetPosition()
{
    return circle.getPosition(); 
}
void SightLine::SetMaxLength(int maxlength)
{
    MaxLength = maxlength; 
}
void SightLine::Update()
{

    

        
}
int SightLine::getBrakingAlpha()
{
    return BrakingAlpha; 
}
bool SightLine::BrakingZone()
{
    return InBrakeZone; 
}
void SightLine::SetScale(sf::Vector2f Scale)
{
    line.setScale(Scale);
    circle.setScale(Scale);
}
void SightLine::SetSize(sf::Vector2f size)
{
    Size = size;
    line.setSize(Size);
}
void SightLine::SetRotation(float Rotation)
{
    line.setRotation(Rotation + rotation);
}
void SightLine::SetPosition(sf::Vector2f position, std::pair<int, int> offset)
{
    CurrentOffset = sf::Vector2f(offset.first,offset.second);
    line.setPosition(position);
    
    sf::FloatRect rect = line.getLocalBounds();

    for(int x = rect.left; x<rect.width;x++)
    {
        sf::Vector2f localLine(x,rect.height/2);
        
        sf::Vector2f lineTransformed = line.getTransform() * localLine;
        
        circle.setPosition(lineTransformed);
        
        if((RPixel() == 64 && GPixel() == 64 && BPixel() == 64) || (RPixel() == 0 && GPixel() == 255 && BPixel() >= 0 )|| (RPixel() == 255 && GPixel() == 1 && BPixel() >= 0 ))
        {
            if(RPixel() == 0 && GPixel() == 255 && BPixel() >= 0 )
            {
                InBrakeZone = true;
                BrakingAlpha = BPixel();
            }
            else if(RPixel() == 255 && GPixel() == 1 && BPixel() >= 0 )
            {
                
            }
            else
            {
                InBrakeZone = false;
                BrakingAlpha = -1;
            }
            OnEdge = true; 
            break;
        }
    }
}
void SightLine::Render(sf::RenderWindow &window)
{
    window.draw(line);
    window.draw(circle);
}

bool SightLine::GetOnEdge()
{ 
    return OnEdge;
}
bool SightLine::OnTrack()
{
    if(ConstantData.Track->getSize().x >= circle.getPosition().x/ConstantData.ScaleX
       && ConstantData.Track->getSize().y >= circle.getPosition().y/ConstantData.ScaleY
       && 0 <= circle.getPosition().x && 0 <= circle.getPosition().y)
    {
        return true;
    }
    else
        return false;
        
        
}



/*
 //if on track - avoids trying to read outside the texture
 if(OnTrack())
 {
 if(RPixel() == 64 && GPixel() == 64 && BPixel() == 64)
 {
 OnEdge = true;
 }
 else
 {
 OnEdge = false;
 
 //This code needs to change to stop the circle from moving once it hits the grey
 if(RPixel() == 255 && GPixel() == 255 && BPixel() == 255)
 line.setSize( sf::Vector2f(line.getSize().x - incrementAmount*2, line.getSize().y));
 else
 line.setSize( sf::Vector2f(line.getSize().x + incrementAmount, line.getSize().y));
 
 
 }
 }
 */
