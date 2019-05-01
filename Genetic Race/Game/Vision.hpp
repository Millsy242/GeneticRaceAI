//
//  Vision.hpp
//  t02
//
//  Created by Daniel Harvey on 23/02/2019.
//

#ifndef Vision_hpp
#define Vision_hpp

#include <SFML/Graphics.hpp>
#include <iostream>
#include "DataStruct.hpp"
#include "SightLine.hpp"
#include <vector>

enum eSLine
{
    eLeft,
    eRight,
    eMiddle,
    eCenter
};

class Vision
{
public:
    Vision(Data &constantdata);
    
    void setSize(float x, float y, bool LongerMiddle);
    void SetScale(int scaleX, int scaleY);
    void SetRotation(float rotation);
    void SetPosition(sf::Vector2f position, std::pair<int, int> offset);
    void Render(sf::RenderWindow &window);
    
    int GetDistance(eSLine Line1, eSLine Line2);
    int breakingPointAlpha();
    
    sf::Color GetColour(eSLine side);
private:
    
    std::vector<SightLine> Sight;
    sf::Vector2f Size{1,1};
    float CircleSize{1};
    sf::Vector2f CarPosition;
    Data &ConstantData;
};

#endif /* Vision_hpp */
