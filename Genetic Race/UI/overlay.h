#ifndef OVERLAY_H
#define OVERLAY_H

#include <SFML/Graphics.hpp>
#include<vector>
#include<unordered_map>
#include <iostream>
#include <sstream>
#include <string>

class Overlay
{
    public:
        Overlay();
        virtual ~Overlay();

        bool loadFont(std::string FontName, std::string Font_FilePath);
        bool CreateOverlay(std::string TextName, std::string FontName, int CharacterSize = 20, bool setOriginMiddle = true, std::string data = "_UNASSIGNED_");

        void ChangeFont(std::string TextName, std::string FontName, int CharacterSize = 10);
        void RenderAllOverlays(sf::RenderWindow &window);
        void RenderSpecificOverlay(std::string TextName,sf::RenderWindow &window, sf::Vector2f position = sf::Vector2f());
        void SetDataOutput(std::string TextName, std::string Data,sf::Vector2f position = sf::Vector2f());
        void SetDataOutput(std::string TextName, int Data,sf::Vector2f position = sf::Vector2f());
        void SetDataOutput(std::string TextName, float Data,sf::Vector2f position = sf::Vector2f());
        void AddToDataOutput(std::string TextName, std::string data);
        void ClearData(std::string TextName);
        void SetColour(std::string TextName, sf::Color Colour);
        void SetGLobalScale(float x, float y);
        void SetScale(std::string TextName, float x, float y);
        void DeleteOverlay(std::string TextName);
        void ChangeSize(std::string TextName, int CharacterSize);
        void SetPosition(std::string TextName, sf::Vector2f position);
        void SetStyle(std::string TextName, sf::Uint32 style);
        void ClearAll();
    
     std::vector<std::string> GetPlayerNames();
    
            sf::Color GetColour(std::string TextName);

    private:

        std::unordered_map<std::string, sf::Font> Fonts;
        std::unordered_map<std::string, sf::Text> TextOverlays;
        sf::Vector2f Scale = sf::Vector2f(1,1); 
};

#endif // OVERLAY_H
