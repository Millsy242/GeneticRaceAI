//
//  Helper.hpp
//  t02
//
//  Created by Daniel Harvey on 17/02/2019.
//  
//

#ifndef Helper_hpp
#define Helper_hpp

#include "Car.h"
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <utility>
#include <SFML/Graphics.hpp>
#include "SFMLVectorMaths.hpp"




namespace Helper
{
     inline void BubbleSort(std::vector<float> &vect)
    {
        unsigned int count{0};
        while(count != vect.size())
        {
            float temp = -1.f; // to show errors
            for(unsigned int i{0}; i<vect.size()-1; i++)
            {
                if(vect[i] > vect[i+1])
                {
                    temp = vect[i+1];
                    vect[i+1] = vect[i];
                    vect[i] = temp;
                }
            }
            count++;
        }
    }
    inline float dot(sf::Vector2f a, sf::Vector2f b)
    {
        return a.x * b.x + a.y * b.y;
    }
    
    //https://stackoverflow.com/questions/8888748/how-to-check-if-given-c-string-or-char-contains-only-digits
    inline bool is_digits(const std::string &str)
    {
        return std::all_of(str.begin(), str.end(), ::isdigit); // C++11
    }
    
   inline std::string floatTo3dpString(float f, int precision)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << f;
        std::string mystring = ss.str();
        
        return mystring;
    }
    
    inline std::pair<int,int> UpdateOffset(int x , int y, int ScreenHeight, int ScreenWidth)
    {
        //Make sure point is centre of screen at all times
        std::pair<int,int> offset = {0,0};
        
        if (x>(ScreenHeight/2))
        {
            offset.first = x-(ScreenHeight/2);
        }
        if (y>(ScreenWidth/2))
        {
            offset.second = y-(ScreenWidth/2);
        }
        
        return offset;
    }
    
    inline int random(int min, int max) //https://stackoverflow.com/a/7560171/9178399
    {
        return min + rand() % (( max + 1 ) - min);
    }
    
    inline bool is_file_exist(const std::string fileName) //https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    {
        std::ifstream infile(fileName);
        return infile.good();
    }
    inline float GetDistanceBetween2Points(sf::Vector2f Start, sf::Vector2f End)
    {
        return sqrt(std::abs((pow(End.x-Start.x, 2)+pow(End.y-Start.y, 2))));
    }
    //return angle of rotation for vector vect.
    inline float Heading(sf::Vector2f vect)
    {
        return std::atan2(vect.y,vect.x);
    }
    inline float AddUpVecFloat(std::vector<float> Fvec)
    {
        float total = 0;
        for(int i{0};i<Fvec.size(); i++)
        {
            total+=Fvec[i];
        }

        return total; 
    }
    
    inline float magnitudeSQ(sf::Vector2f vect)
    {
        return (vect.x*vect.x + vect.y*vect.y);
    }
    
    inline void limit(sf::Vector2f &vect, float max)
    {
        if(Helper::magnitudeSQ(vect) > max * max)
        {
            sf::normalize(vect);
            vect *= max; 
        }
    }
    
    inline std::string GetDateTime()
    {
        time_t now = time(0);
        struct tm  tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); //Date and time
        return buf;
    }

    
    
}


#endif /* Helper_hpp */

