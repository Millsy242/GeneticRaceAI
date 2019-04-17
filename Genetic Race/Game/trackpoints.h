
#define TRACKPOINTS_H

#include <sstream>
#include <iostream>
#include <ctime>
#include <utility>
#include<vector>

class TrackPoints
{
public:
    TrackPoints();
    ~TrackPoints();
    void AddPoint(int x, int y, int r, int g);
    void OrderPoints();
    std::vector<std::pair<int,int>> GetPoints();
    void SetStart(int x, int y);
    std::pair<int,int> GetStart();
    void Print();


private:
    void bubblesort();

    std::pair<int,int> position; //x,y
    std::pair<int,int> startposition; //x,y
    std::pair<int,int> colour; //first = red, second = green
    std::vector<std::pair<std::pair<int,int>,std::pair<int,int>>> PointVect;
    
    bool sorted = false;

};
