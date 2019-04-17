//
//  SightLine.hpp
//  t02
//
//  Created by Daniel Harvey on 04/03/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
//

#ifndef SightLine_hpp
#define SightLine_hpp
#include <SFML/Graphics.hpp>
#include "DataStruct.hpp"
#include <iostream>

class SightLine
{
public:
    SightLine(Data &constantdata);
    void Init(sf::Vector2f size,float CircleSize, float Rotation, sf::Color Colour);
    void Update();
    void SetScale(sf::Vector2f Scale);
    void SetMaxLength(int maxlength); 
    void SetSize(sf::Vector2f size);
    void SetRotation(float Rotation);
    void SetPosition(sf::Vector2f position, std::pair<int, int> offset);
    sf::Vector2f GetPosition();
    void Render(sf::RenderWindow &window);
    
    bool BrakingZone();
    int getBrakingAlpha();
    
    bool GetOnEdge();
    
    int RPixel(); //Red
    int GPixel();//Green
    int BPixel();//Blue
    int APixel(); //Alpha
    int ALLPixelUnderCar();//All
    
private:
    
    bool OnEdge = false;
    
    
    bool OnTrack();
    bool InBrakeZone = false;
    
    sf::Color colour;
    float rotation;
    sf::Vector2f Size;
    float circleSize;
    const int incrementAmount = 3;
     int MaxLength = 200;
    int BrakingAlpha = -1; 
    
    sf::RectangleShape line;
    sf::CircleShape circle;
    
    sf::Vector2f CurrentOffset;
    
     Data &ConstantData;
};

#endif /* SightLine_hpp */
