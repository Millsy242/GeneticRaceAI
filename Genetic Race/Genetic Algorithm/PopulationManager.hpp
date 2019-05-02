//
//  PopulationManager.hpp
//
//  Created by Daniel Harvey on 16/02/2019.
//
// Manager for all Agents and the camera,  handles specific GUI functionality and writing data to file. 
//
#ifndef PopulationManager_hpp
#define PopulationManager_hpp

#include "Helper.hpp"
#include <iostream>
#include <vector>

#include "DataStruct.hpp"

class Car;
class PopulationManager
{
public:
    PopulationManager(Data &constantdata);
    ~PopulationManager(){};
    
    void CreateStartingPopulation();
    void StartRace();
    void StopRace();
    void BreedNewPopulation();
    void UpdateCars();
    void UpdateGUI(bool Pause);
    void SetupChromosome(Chromosome &ch, float maxspeed);
    void WriteToCSV();
    void RenderCars(sf::RenderWindow &window);
    
    bool isRacing();
    
    int getCameraID();
    int getGeneration(){return Generation;};
    int getDead(){return numDead;};
    unsigned int numCars();
    unsigned long SizeOfCarArray();
    
    float getFastestLap();
    
    Car &GetCar(int id);
    Car CreateCar(int carnum);
    
    std::vector<float> TotalFitnessHistory;
    std::vector<float> AverageLapTimes;
    std::vector<float> BestLapTimes;
    
private:
    void sortCars(bool Fitness = true);
    void UpdateStandings();
    int RouletteWheelSelection(std::vector<int> Orderedcarindex,std::vector<Car> &tempCarArray);

    std::vector<Car> carArray;
    std::vector<int> orderedcarindex;
    std::vector<std::pair<std::string,Chromosome>> ForceAddedChromosomes;
    
    
    Car Breed(Car M, Car D);
    
    Data &ConstantData;
    
    float totalFitness = 0;
    float FastestLap = 299;
    float worstFitness = 0;
    
    const int CrossoverRate = 70;
    int Generation = 1;
    int numDead = 0;
    int totalnumberofcars = 1;
    int NumFinished = 0;
    int CameraID = 0;
    int selectedCar = 0;
    
    bool Racing = false;
    
    char stringinput[128] = "Input text here";
    
    
    std::string nameofoutput;
    
    
    
    
};

#endif /* PopulationManager_hpp */
