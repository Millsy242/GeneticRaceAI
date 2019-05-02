//
//  SightLine.hpp
//
//  Created by Daniel Harvey on 04/03/2019.
//
// Sightline (feeler) to read data from the Track
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
    void SetScale(sf::Vector2f Scale);
    void SetMaxLength(int maxlength); 
    void SetSize(sf::Vector2f size);
    void SetRotation(float Rotation);
    void SetPosition(sf::Vector2f position, std::pair<int, int> offset);
    void Render(sf::RenderWindow &window);
    
    bool BrakingZone();
    bool GetOnEdge();
    
    int getBrakingAlpha();
    int RPixel(); //Red
    int GPixel();//Green
    int BPixel();//Blue
    int APixel(); //Alpha
    int ALLPixelUnderCar();//All
    
    sf::Vector2f GetPosition();
    
private:
    
    bool OnEdge = false;
    bool OnTrack();
    bool InBrakeZone = false;
    
    float rotation;
    float circleSize;
    
    const int incrementAmount = 3;
    int MaxLength = 200;
    int BrakingAlpha = -1;
    
    sf::Color colour;
    sf::RectangleShape line;
    sf::CircleShape circle;    
    sf::Vector2f CurrentOffset;
    sf::Vector2f Size;
    
    Data &ConstantData;
};

#endif /* SightLine_hpp */
