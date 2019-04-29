//
//  PopulationManager.cpp
//  t02
//
//  Created by Daniel Harvey on 16/02/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
//

#include "PopulationManager.hpp"
#include "Car.h"
#include "Chromosome.hpp"
#include <string>
#include <fstream>
#include <sstream>
#include "Database.hpp"
#include <algorithm>
#include <utility>
#include "imgui.h"
#include "imgui-SFML.h"
#include "imgui_user.h"
#include <string>

PopulationManager::PopulationManager(Data &constantdata) : ConstantData(constantdata)
{
}

void PopulationManager::CreateStartingPopulation()
{
    ConstantData.FastestLapTimeData = new LapTimeTableStruct();
    std::string sql = "SELECT * FROM LapTimeRecords WHERE TRACKID = " + std::to_string(ConstantData.TrackNumber);
    ConstantData.database->Select(sql, *ConstantData.FastestLapTimeData);
    for(unsigned int i=0; i<ConstantData.PopSize; i++)
    {
        Car newCar(ConstantData);
        newCar.setPosX(ConstantData.StartPos.x);
        newCar.setPosY(ConstantData.StartPos.y);
        newCar.id = std::to_string(totalnumberofcars);
        newCar.LoadFromFile();
        SetupChromosome(newCar.chromosome,newCar.GetMaxSpeed());
        if(i<ConstantData.Chromosomes.size())
        {           
            newCar.chromosome = ConstantData.Chromosomes[i];
            newCar.id = ConstantData.savedChromosomes[i].first;
        }
        newCar.Init();
        carArray.push_back(newCar);
        totalnumberofcars++;
    }
    nameofoutput = Helper::GetDateTime();
}
void PopulationManager::SetupChromosome(Chromosome &ch, float maxspeed)
{
    ch.AddGene(2,255,0);
    ch.AddGene(1,1,0);
    ch.AddGene(1,200,1);
    ch.AddGene(1,maxspeed,0.5);
    /*
     ch.AddGene(3,255,0);
     ch.AddGene(2,1,0);
     ch.AddGene(1,100,1);
     ch.AddGene(2,1,0);
     ch.AddGene(1,maxspeed,0.5);
     ch.AddGene(3,255,0);
     */
    //ch.AddGene(1, 4294967295,255); //replace colour genes with this
    ch.EndAddingGenes();
}
void PopulationManager::WriteToCSV()
{
    std::ofstream output("GenesOutput/"+nameofoutput+".csv", std::ios::app);
    
    for(int i{0}; i<carArray.size();i++)
    {
        output << carArray[i].chromosome.ToString() + ", " + std::to_string(carArray[i].getFastestLap())+ "\n";
    }
    output << "\n\n\n";
    output.close();
}
void PopulationManager::StartRace()
{
    Racing = true;
}
void PopulationManager::StopRace()
{
    Racing = false;
}
bool PopulationManager::isRacing()
{
    return Racing;
}
float PopulationManager::getFastestLap()
{
    return FastestLap;
}
unsigned long PopulationManager::SizeOfCarArray()
{
    return carArray.size();
}
Car& PopulationManager::GetCar(int id)
{
    if(carArray.size() > id)
    {
        return carArray[id];
    }
    std::cerr<<"looking for id that doesnt exist, returning first car"<<std::endl;
    return carArray[0];
}
Car PopulationManager::CreateCar()
{
    Car newCar(ConstantData);
    newCar.setPosX(ConstantData.StartPos.x);
    newCar.setPosY(ConstantData.StartPos.y);
    newCar.id = std::to_string(totalnumberofcars);
    newCar.LoadFromFile();
    totalnumberofcars++;
    SetupChromosome(newCar.chromosome,newCar.GetMaxSpeed());
    
    return newCar;
}
Car PopulationManager::Breed(Car M, Car D)
{
    auto newCar = CreateCar();
    
    if(Helper::random(0, 100) < CrossoverRate)
    {
        newCar.chromosome.Crossover(M.chromosome, D.chromosome);
    }
    else
    {
        if(Helper::random(0, 100) > 50)
        {
            newCar.chromosome = M.chromosome;
        }
        else
        {
            newCar.chromosome = D.chromosome;
        }
    }
    if(Helper::random(0, 100) == 1)
    {
        newCar.chromosome.Mutate(ConstantData.MutationRate);
    }
    newCar.Init();
    
    return newCar;
}
void PopulationManager::BreedNewPopulation()
{

    numDead = 0;
    NumFinished = 0;
    WriteToCSV();
    totalFitness = 0;
    float bestFitness = 0;
    //We can just use the first cars fitness as the temp worst fitness
    //because if another car has a worse fitness, it will get replaced.
    //This avoids errors where a large random number ends up somehow being smaller than the worst fitness in a population!
    worstFitness = carArray[0].getFitness();
    float bestLap = 300;
    float SumofLaptime = 0;
    int NumCompletedaLap = 0;
    for(unsigned int i{0}; i<carArray.size(); i++)
    {
        if(carArray[i].getFitness() > bestFitness )
            bestFitness = carArray[i].getFitness();
        if(carArray[i].getFitness() < worstFitness )
            worstFitness = carArray[i].getFitness();
        
        if(carArray[i].getFastestLap() < 299)
        {
            SumofLaptime+=carArray[i].getFastestLap();
            
            NumCompletedaLap++;
            
            if(carArray[i].getFastestLap() < FastestLap)
            {
                FastestLap = carArray[i].getFastestLap();
            }
            if(carArray[i].getFastestLap() < bestLap)
            {
                bestLap = carArray[i].getFastestLap();
            }
        }
        
        totalFitness+=carArray[i].getFitness();
        
    }
    std::cout<<"\nTotal Fitness = " << totalFitness << "\nBestFitness = " << bestFitness<< "\nFastestLap = " << sf::seconds(FastestLap).asSeconds()<<std::endl;
    
    TotalFitnessHistory.push_back(totalFitness/ConstantData.GenerationLength.asSeconds());
    AverageLapTimes.push_back(SumofLaptime/NumCompletedaLap);
    BestLapTimes.push_back(bestLap);
    
    if(bestLap < ConstantData.FastestLapTimeData->LapTime)
    {
        ConstantData.FastestLapTimeData->LapTime = bestLap;
        
        std::string sql = "INSERT INTO LapTimeRecords (TrackID,DateTime,LapTime,Name,Gene) VALUES('" + std::to_string(ConstantData.TrackNumber) + "', '" + Helper::GetDateTime() + "','" + std::to_string(bestLap) + "','Player" + carArray[orderedcarindex[carArray.size()-1]].id + "', '" + carArray[orderedcarindex[carArray.size()-1]].chromosome.ToString()+"');";
        
        if(!ConstantData.database->Insert(sql))
        {
            ConstantData.database->Delete("DELETE FROM LapTimeRecords WHERE TrackID = '" + std::to_string(ConstantData.TrackNumber) + "';");
            ConstantData.database->Insert(sql);
        }
    }
    totalFitness -= (carArray.size() * worstFitness);
    sortCars();
    std::vector<Car> tempCarArray = carArray;
    carArray.clear();
    
    for (unsigned int i = 0; i < orderedcarindex.size(); i++)
    {
        //We should leave the elites in the population https://stackoverflow.com/questions/14622342/elitism-in-ga-should-i-let-the-elites-be-selected-as-parents
        if(ConstantData.Elitism && i < ConstantData.ElitismNum )
        {
            auto newcar = CreateCar();
            newcar.chromosome = tempCarArray[orderedcarindex[ i]].chromosome;
            newcar.Init();
            newcar.id = tempCarArray[orderedcarindex[ i]].id;
            if(ConstantData.MutateElites)
            {
                newcar.chromosome.Mutate(ConstantData.MutationRate);
            }
            carArray.push_back(newcar);
        }
        else
        {
            int a = RouletteWheelSelection(orderedcarindex,tempCarArray);
            int b = RouletteWheelSelection(orderedcarindex,tempCarArray);
            carArray.push_back(Breed (tempCarArray[a], tempCarArray[b]));
        }
    }
    tempCarArray.clear();
    orderedcarindex.clear();
    Generation++;
}

int PopulationManager::RouletteWheelSelection(std::vector<int> Orderedcarindex, std::vector<Car> &tempCarArray)
{
    float chosen = Helper::random(0, totalFitness);
    float partialSum = 0;
    
    for (int i = 0; i < Orderedcarindex.size(); i++)
    {
        if(partialSum + (tempCarArray[Orderedcarindex[i]].getFitness() - worstFitness) < chosen)
        {
            partialSum+=(tempCarArray[Orderedcarindex[i]].getFitness() - worstFitness);
        }
        else
            return Orderedcarindex[i];
        
    }
    std::cout<<"ERROR in RouletteWheelSelection"<<std::endl;
    return 0;
}
void PopulationManager::UpdateCars()
{
    if(Racing)
    {
        numDead = 0;
        for(int i{0}; i<carArray.size(); i++)
        {
            if(!carArray[i].STOPPED)
            {
                if(ConstantData.StopOnLaps && (ConstantData.laps < carArray[i].getLapsComplete(true)))
                {
                    NumFinished++;
                    GetCar(i).STOP();
                }
                
                
                carArray[i].Timing(ConstantData.clock);
                carArray[i].UpdateLaptime(ConstantData.elapsedTime);
                
                carArray[i].AIMovement();
                if(!carArray[i].GetActive())
                {
                    numDead++;
                }
            }
        }
        if(numDead == numCars() ||  (numDead + NumFinished >= numCars()))
        {
            BreedNewPopulation();
        }
    }
}

unsigned int PopulationManager::numCars()
{
    return (unsigned int)carArray.size();
}

void PopulationManager::RenderCars(sf::RenderWindow &window)
{
    if(Racing)
    {
        for(int i=0; i<carArray.size(); i++)
        {
            carArray[i].Render(window,Helper::UpdateOffset(carArray[CameraID].getPosX(),carArray[CameraID].getPosY(), ConstantData.ScreenHeight, ConstantData.ScreenWidth));
        }
    }
}

void PopulationManager::sortCars(bool Fitness)
{
    orderedcarindex.clear();
    for(int i{0}; i<carArray.size();i++)
    {
        orderedcarindex.push_back(i);
    }
    unsigned int count{0};
    while(count != carArray.size())
    {
        for(unsigned int i{0}; i<orderedcarindex.size()-1; i++)
        {
            if(Fitness)
            {
                if(carArray[orderedcarindex[i]].getFitness() < carArray[orderedcarindex[i+1]].getFitness())
                {
                    std::swap(orderedcarindex[i], orderedcarindex[i+1]);
                }
            }
            else
            {
                if(carArray[orderedcarindex[i]].getLapsComplete(true) < carArray[orderedcarindex[i+1]].getLapsComplete(true))
                {
                    std::swap(orderedcarindex[i], orderedcarindex[i+1]);
                }
            }
        }
        count++;
    }
}
void PopulationManager::UpdateGUI(bool Pause)
{
    ImGui::Begin("Standings");
    UpdateStandings();
    if(Pause) //Only do this when Pause is true to avoid running through potentially 1000 cars every frame just incase the game is paused
    {
        for(unsigned int i{0}; i<carArray.size(); i++)
        {
            carArray[i].TheGameIsPaused(true);
        }
        carArray[CameraID].Highlight(true);
        
        if(Pause)
        {
            // assigning value to string s
            std::string defaultname = "Player" + carArray[CameraID].id;
            
            // declaring character array
            char hint[defaultname.length() + 1];
            
            // copying the contents of the
            // string to char array
            strcpy(hint, defaultname.c_str());
            
            
            ImGui::InputTextWithHint("Input a Savename", hint, stringinput, IM_ARRAYSIZE(stringinput));
            if(ImGui::Button("Save Selected Car"))
            {
                std::string sql = "INSERT INTO GENE (Name,DateTime,TrackNumber,FastestLapTime,Genes,Generation) VALUES('"+ std::string(stringinput) + "', '" + Helper::GetDateTime() + "',  '" + std::to_string(ConstantData.TrackNumber) + "',   '" + std::to_string(carArray[CameraID].getFastestLap()) +   "',    '" + carArray[CameraID].chromosome.ToString()+"',     '" + std::to_string(Generation) +"');";
                
                ConstantData.database->Insert(sql);
            }
        }
        
        
    }
    else
    {
        ImGui::Text("Pause to save any car here!");
        
        if(!carArray[CameraID].GetActive())
        {
            CameraID = orderedcarindex[0];
        }
    }
    ImGui::End();
}
void PopulationManager::UpdateStandings()
{
    sortCars(false);
    std::vector<std::string> Standings;
    
    
    for(unsigned int i{0}; i<ConstantData.PopSize; i++)
    {
        if(GetCar(orderedcarindex[i]).GetActive())
        {
            std::string entry = std::to_string(i+1) + ": Car " + GetCar(orderedcarindex[i]).id + " " + std::to_string(GetCar(orderedcarindex[i]).getLapsComplete(true));
            if(GetCar(orderedcarindex[i]).STOPPED)
            {
                entry += " FIN";
            }           
            
            Standings.push_back(entry);
        }
        else
        {
            std::string entry = std::to_string(i+1) + ": Car " + GetCar(orderedcarindex[i]).id + " DEAD"  ;
            Standings.push_back(entry);
        }
    }
    if(ImGui::ListBox("User Saves", &selectedCar,Standings ))
    {
        CameraID = orderedcarindex[selectedCar];
    }
    
    
    
    
}
