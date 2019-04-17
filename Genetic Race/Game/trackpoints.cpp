#include "trackpoints.h"

TrackPoints::TrackPoints()
{
    //ctor
}

TrackPoints::~TrackPoints()
{
    //dtor
}

void TrackPoints::SetStart(int x, int y)
{
    startposition.first = x;
    startposition.second = y; 
}
std::pair<int,int> TrackPoints::GetStart()
{
    return startposition; 
}

void TrackPoints::AddPoint(int x, int y, int r, int g)
{
    if(!PointVect.empty())
    {
        for(int i{0}; i<PointVect.size(); i++)
        {
            if(g == PointVect[i].second.second)
            {
                return;
            }
        }
    }

    position.first = x;
    position.second = y;

    colour.first = r;
    colour.second = g;
    
    sorted = false;

    PointVect.push_back(std::make_pair(position,colour));
}
void TrackPoints::OrderPoints()
{
    bubblesort();
    sorted = true;
}
std::vector<std::pair<int,int>> TrackPoints::GetPoints()
{
    if(!sorted)
        OrderPoints();

    std::vector<std::pair<int,int>> temp;
    for(int i{0}; i<PointVect.size(); i++)
    {
        temp.push_back(PointVect[i].first);
    }

    return temp;
}

void TrackPoints::Print()
{
    if(PointVect.size() == 0)
        std::cout<<"Trackpoints was empty!"<<std::endl;
    else
    {
        for(int i{0}; i<PointVect.size(); i++)
        {
            std::cout<< i << ": x = " << PointVect[i].first.first << ", y = "<< PointVect[i].first.second
            << ": r = " << PointVect[i].second.first << ", g = "<< PointVect[i].second.second  <<std::endl;
        }
    }
}

void TrackPoints::bubblesort()
{
    unsigned int count{0};
    while(count != PointVect.size())
    {
        std::pair<std::pair<int,int>,std::pair<int,int>> temp = std::make_pair(std::make_pair(-1,-1), std::make_pair(-1, -1)); // to show errors
        for(unsigned int i{0}; i<PointVect.size()-1; i++)
        {
            if(PointVect[i].second.second > PointVect[i+1].second.second)
            {
                temp = PointVect[i+1];
                PointVect[i+1] = PointVect[i];
                PointVect[i] = temp;
            }
        }
        count++;
    }
}
