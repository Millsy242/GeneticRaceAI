//
//  PopulationManager.hpp
//  t02
//
//  Created by Daniel Harvey on 16/02/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
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
    bool isRacing(); 
    
    void BreedNewPopulation();
    
    unsigned int numCars();
    
    void UpdateCars();
    
    int getCameraID()
    {
        return CameraID;
    }
    
    void UpdateGUI(bool Pause); 
    
    unsigned long SizeOfCarArray(); 
    
    Car &GetCar(int id);
    
    void SetupChromosome(Chromosome &ch, float maxspeed); 
    
    void RenderCars(sf::RenderWindow &window);
    
    int getGeneration(){return Generation;};
    int getDead(){return numDead;};
    float getFastestLap(); 
    
    void WriteToCSV();
    
     std::vector<float> TotalFitnessHistory;
    std::vector<float> AverageLapTimes;
        std::vector<float> BestLapTimes; 
    
private:
    void sortCars(bool Fitness = true);
    void UpdateStandings(); 
    int RouletteWheelSelection(std::vector<int> Orderedcarindex,std::vector<Car> &tempCarArray);
    
    float totalFitness = 0;
    float FastestLap = 299; 
    
    std::vector<Car> carArray;
   
    
    Car Breed(Car M, Car D);
    
    Data &ConstantData;
    
    int numDead = 0; 
    
    bool Racing = false;
    int MutationRate = 70;
    
    int totalnumberofcars = 1;
    
    std::vector<int> orderedcarindex;
    
    const int CrossoverRate = 70;
    int Generation = 1;
    std::string nameofoutput;
    
    int CameraID = 0;
    int selectedCar = 0;
    char stringinput[128] = "Input text here"; 
    
};

#endif /* PopulationManager_hpp */
