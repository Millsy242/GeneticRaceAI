#include "overlay.h"

template <typename T>
std::string toString(const T& value)
{
    std::stringstream stream;
    stream << value;
    return stream.str();
}

Overlay::Overlay()
{
    //ctor
}

Overlay::~Overlay()
{
    //dtor
}

bool Overlay::loadFont(std::string FontName, std::string Font_FilePath)
{
    sf::Font newFont;
    if (!newFont.loadFromFile(Font_FilePath))
    {
        std::cerr << "Cant load " << FontName << std::endl;
        return false;
    }
    else
    {
        Fonts[FontName] = newFont;
        return true;
    }
}

bool Overlay::CreateOverlay(std::string TextName, std::string FontName,int CharacterSize,  bool setOriginMiddle,std::string data)
{
    sf::Text outputThisData;
    
    if(TextOverlays.find(TextName) == TextOverlays.end()) 
    {
        outputThisData.setString(data);

        if (Fonts.find(FontName) == Fonts.end())
        {
            std::cerr << "Trying to create an overlay, but can't find this font:  " << FontName << std::endl;
            return false;
        }
        outputThisData.setFont(Fonts[FontName]);
        
        if(setOriginMiddle)
        {
            outputThisData.setOrigin(outputThisData.getLocalBounds().width/2, outputThisData.getLocalBounds().height/2);
        }
        outputThisData.setScale(Scale);
        TextOverlays[TextName] = outputThisData;
        ChangeSize(TextName, CharacterSize);
        return true;
    }
    return false;
}
void Overlay::DeleteOverlay(std::string TextName)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "can't delete " << TextName << std::endl;
        return;
    }
    TextOverlays.erase(TextName);
}

void Overlay::ChangeFont(std::string TextName, std::string FontName, int CharacterSize)
{
    if (Fonts.find(FontName) == Fonts.end())
    {
        std::cerr << "can't find this font: " << FontName << std::endl;
        return ;
    }
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "can't find this text: " << TextName << std::endl;
        return;
    }
    TextOverlays[TextName].setFont(Fonts[FontName]);
}

void Overlay::RenderAllOverlays(sf::RenderWindow &window)
{
    for (auto t : TextOverlays)
    {
        window.draw(t.second);
    }
}

void Overlay::RenderSpecificOverlay(std::string TextName,sf::RenderWindow &window, sf::Vector2f position)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "can't find this text: " << TextName << std::endl;
        return;
    }
    window.draw(TextOverlays[TextName]);
}

void Overlay::SetDataOutput(std::string TextName, std::string Data, sf::Vector2f position)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to setDataOutput, but can't find this text: " << TextName << std::endl;
        return;
    }

    SetPosition(TextName,position);

    TextOverlays[TextName].setString(Data);
}
void Overlay::SetGLobalScale(float x, float y)
{
    for (auto t : TextOverlays)
    {
        t.second.setScale(x, y);
    }
}
void Overlay::SetScale(std::string TextName,float x, float y)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to SetScale, but can't find this text: " << TextName << std::endl;
        return;
    }
    TextOverlays[TextName].setScale(x,y); 
}

void Overlay::SetDataOutput(std::string TextName, int Data, sf::Vector2f position)
{
    SetDataOutput(TextName,toString(Data),position);
}

void Overlay::SetDataOutput(std::string TextName, float Data, sf::Vector2f position)
{
    SetDataOutput(TextName,toString(Data),position);
}

void Overlay::AddToDataOutput(std::string TextName, std::string data)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to addToDataOutput but can't find this text: " << TextName << std::endl;
        return;
    }

    TextOverlays[TextName].setString(TextOverlays[TextName].getString() + data);
}

void Overlay::ClearData(std::string TextName)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to clear Data but can't find this text: " << TextName << std::endl;
        return;
    }

    TextOverlays[TextName].setString("");
}

void Overlay::ClearAll()
{
    TextOverlays.clear();

}

void Overlay::SetPosition(std::string TextName, sf::Vector2f position)
{
    if(position != sf::Vector2f())
    {
        if (TextOverlays.find(TextName) == TextOverlays.end())
        {
            std::cerr << "Trying to set position but can't find this text: " << TextName << std::endl;
            return;
        }
        TextOverlays[TextName].setPosition(position);
    }
}

void Overlay::SetColour(std::string TextName, sf::Color Colour)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to set Colour but can't find " << TextName << std::endl;
        return;
    }
    TextOverlays[TextName].setFillColor(Colour);
}
sf::Color Overlay::GetColour(std::string TextName)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to Get Colour but cant find " << TextName << std::endl;
        
    }
    return TextOverlays[TextName].getFillColor();
}

void Overlay::ChangeSize(std::string TextName, int CharacterSize)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to ChangeSize but can't find " << TextName << std::endl;
        return;
    }
    TextOverlays[TextName].setCharacterSize(CharacterSize);
}
std::vector<std::string> Overlay::GetPlayerNames()
{
    std::vector<std::string> temp;
    for (auto t : TextOverlays)
    {
        temp.push_back(t.first);
    }
    return temp; 
}
void Overlay::SetStyle(std::string TextName, sf::Uint32 style)
{
    if (TextOverlays.find(TextName) == TextOverlays.end())
    {
        std::cerr << "Trying to SetStyle but can't find " << TextName << std::endl;
        return;
    }
    TextOverlays[TextName].setStyle(style);
}
