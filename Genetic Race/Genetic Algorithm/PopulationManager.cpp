//
//  PopulationManager.cpp
//
//  Created by Daniel Harvey on 16/02/2019.
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
        newCar.id = "G" + std::to_string(Generation) + "C" + std::to_string(i);
        newCar.LoadFromFile();
        SetupChromosome(newCar.chromosome,newCar.GetMaxSpeed());
        if(i<ConstantData.Chromosomes.size())
        {
            newCar.chromosome = ConstantData.Chromosomes[i];
            newCar.id = ConstantData.savedChromosomes[i].first;
        }
        newCar.Init();
        carArray.push_back(newCar);
    }
    nameofoutput = Helper::GetDateTime();
}
void PopulationManager::SetupChromosome(Chromosome &ch, float maxspeed)
{
    ch.AddGene(2,255,0);
    ch.AddGene(1,1,0);
    ch.AddGene(1,200,1);
    ch.AddGene(1,maxspeed,1);

    ch.EndAddingGenes();
}
void PopulationManager::WriteToCSV()
{
    std::ofstream output("Genetic Race Data/GenesOutput/"+nameofoutput+".csv", std::ios::app);
    
    output << "Track Number:," << ConstantData.TrackNumber << "\nGeneration:," << Generation << "\nPopulation Size: ," << ConstantData.PopSize << "\nMutation Rate:," << ConstantData.MutationRate << "\nFastest Lap:,"<<FastestLap<<"\nAverage Fastest Lap:," << AverageLapTimes[AverageLapTimes.size()-1]<<"\nTotal Fitness:,"<< totalFitness<<"\nElitism:," << ConstantData.Elitism << "\nNumElites:,"<<ConstantData.ElitismNum;
    
    output << "\nBraking Point, Turning Point, Longer Middle, Feeler Length, Minimum Speed, Red, Green, Blue, Fastest Lap\n";
    
    for(int i{0}; i<carArray.size();i++)
    {
        output << carArray[i].chromosome.ToString() << carArray[i].getFastestLap() << "\n";
    }
    output << "\n\n";
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

void PopulationManager::BreedNewPopulation()
{
    numDead = 0;
    NumFinished = 0;
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
        
        std::string sql = "INSERT INTO LapTimeRecords (TrackID,DateTime,LapTime,Name,Gene) VALUES('" + std::to_string(ConstantData.TrackNumber) + "', '" + Helper::GetDateTime() + "','" + std::to_string(bestLap) + "','Player" + carArray[orderedcarindex[0]].id + "', '" + carArray[orderedcarindex[0]].chromosome.ToString()+"');";
        
        if(!ConstantData.database->Insert(sql))
        {
            ConstantData.database->Delete("DELETE FROM LapTimeRecords WHERE TrackID = '" + std::to_string(ConstantData.TrackNumber) + "';");
            ConstantData.database->Insert(sql);
        }
    }
    totalFitness -= (carArray.size() * worstFitness);
    WriteToCSV();
    sortCars();
    std::vector<Car> tempCarArray = carArray;
    carArray.clear();
    Generation++;
    bool UserHasForcedChromosomes = !ForceAddedChromosomes.empty();
    
    for (unsigned int i = 0; i < orderedcarindex.size(); i++)
    {
        //We should leave the elites in the population https://stackoverflow.com/questions/14622342/elitism-in-ga-should-i-let-the-elites-be-selected-as-parents
        if((ConstantData.Elitism && i < ConstantData.ElitismNum) || UserHasForcedChromosomes)
        {
            auto newcar = CreateCar(i);
            if(ForceAddedChromosomes.size() > i) //Prefer Users chosen agents
            {
                newcar.chromosome = ForceAddedChromosomes[i].second;
                newcar.id = ForceAddedChromosomes[i].first;
            }
            else
            {
                if(UserHasForcedChromosomes)
                {
                    newcar.chromosome = tempCarArray[orderedcarindex[i - ForceAddedChromosomes.size()]].chromosome;
                    newcar.id = tempCarArray[orderedcarindex[i - ForceAddedChromosomes.size()]].id;
                }
                else
                {
                    newcar.chromosome = tempCarArray[orderedcarindex[i]].chromosome;
                    newcar.id = tempCarArray[orderedcarindex[i]].id;
                }
            }
            
            newcar.Init();
            
            if(ConstantData.MutateElites && ConstantData.Elitism)
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
    ForceAddedChromosomes.clear();
    orderedcarindex.clear();
    
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
        
        std::string defaultname = "Player" + carArray[CameraID].id;
        char hint[defaultname.length() + 1];
        strcpy(hint, defaultname.c_str());
        
        ImGui::InputTextWithHint("Input a Savename", hint, stringinput, IM_ARRAYSIZE(stringinput));
        if(ImGui::Button("Save Selected Car"))
        {
            std::string sql = "INSERT INTO GENE (Name,DateTime,TrackNumber,FastestLapTime,Genes,Generation) VALUES('"+ std::string(stringinput) + "', '" + Helper::GetDateTime() + "',  '" + std::to_string(ConstantData.TrackNumber) + "',   '" + std::to_string(carArray[CameraID].getFastestLap()) +   "',    '" + carArray[CameraID].chromosome.ToString()+"',     '" + std::to_string(Generation) +"');";
            
            ConstantData.database->Insert(sql);
        }
        if(ImGui::Button("Force Add Selected Car to Next Generation"))
        {
            std::pair<std::string,Chromosome> addthis{std::string(stringinput),carArray[CameraID].chromosome};
            ForceAddedChromosomes.push_back(addthis);
        }
    }
    else
    {
        ImGui::Text("Pause to save any car here!");
        CameraID = orderedcarindex[0];
    }
    std::string genetext = "Genes for car: " + carArray[CameraID].id +
    "\nBraking Point = " + std::to_string(carArray[CameraID].chromosome.GetGene(0)) +
    "\nTurning Point = " + std::to_string(carArray[CameraID].chromosome.GetGene(1)) +
    "\nLonger Middle Feeler = " + std::to_string(carArray[CameraID].chromosome.GetGene(2)) +
    "\nFeeler Length = " + std::to_string(carArray[CameraID].chromosome.GetGene(3)) +
    "\nMinimum Speed = " + std::to_string(carArray[CameraID].chromosome.GetGene(4));
    ImGui::Text("%s", genetext.c_str());
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
                
                carArray[i].CarControls();
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
    sortCars();
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
Car PopulationManager::CreateCar(int carnum)
{
    Car newCar(ConstantData);
    newCar.setPosX(ConstantData.StartPos.x);
    newCar.setPosY(ConstantData.StartPos.y);
    newCar.id = newCar.id = "G" + std::to_string(Generation) + "C" + std::to_string(carnum);;
    newCar.LoadFromFile();
    SetupChromosome(newCar.chromosome,newCar.GetMaxSpeed());
    
    return newCar;
}
Car PopulationManager::Breed(Car M, Car D)
{
    auto newCar = CreateCar((int)carArray.size());
    
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
    if(Helper::random(0, ConstantData.MutationRate) == 1)
    {
        newCar.chromosome.Mutate(ConstantData.MutationRate);
    }
    newCar.Init();
    
    return newCar;
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
int PopulationManager::getCameraID()
{
    return CameraID;
}

unsigned int PopulationManager::numCars()
{
    return (unsigned int)carArray.size();
}

