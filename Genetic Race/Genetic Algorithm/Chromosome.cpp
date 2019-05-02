//
//  Chromosome.cpp
//
//  Created by Daniel Harvey on 21/02/2019.
//
#include "Chromosome.hpp"
#include "Helper.hpp"

Chromosome::Chromosome()
{
}

void Chromosome::AddGene(int NumberOfGenes, int maxValue, int minValue)
{
    for(int i{0}; i<NumberOfGenes; i++)
    {
        dnaLength++;
        maxvalues.push_back(maxValue);
        minvalues.push_back(minValue);
    }
}
void Chromosome::EndAddingGenes()
{
    genes.clear();
    for (int i = 0; i < dnaLength; i++)
    {
        genes.push_back( Helper::random(minvalues[i], maxvalues[i]));
    }
    Colour[0] = Helper::random(0,255)/255.f;
    Colour[1] = Helper::random(0,255)/255.f;
    Colour[2] = Helper::random(0,255)/255.f;
}
void Chromosome::SetInt(int pos, int value)
{
    if (genes.size() > pos)
        genes [pos] = value;
    else
        std::cout<<"input position of " << pos << " is greater than size of genes"<<std::endl;
}
void Chromosome::Crossover(Chromosome &c1, Chromosome &c2)
{
    int point = Helper::random(0, dnaLength-1);
    for (int i = 0; i < dnaLength; i++)
    {
        if (i < point)
        {
            genes [i] = c1.genes [i];
        }
        else
        {
            genes [i] = c2.genes [i];
        }
    }
    
    Colour[0] = ((c1.Colour[0]*255+c2.Colour[0]*255)/2.f)/255.f;
    Colour[1] = ((c1.Colour[1]*255+c2.Colour[1]*255)/2.f)/255.f;
    Colour[2] = ((c1.Colour[2]*255+c2.Colour[2]*255)/2.f)/255.f;
}
void Chromosome::Mutate(int MutationRate)
{
    int chancethisgeneswaps = 0;
    for(int i{0}; i<genes.size();i++)
    {
        chancethisgeneswaps = Helper::random(0, 100);
        if(chancethisgeneswaps < MutationRate)
        {
            if(maxvalues[i] - minvalues[i] > 1)
            {
                bool higher = (bool)Helper::random(0, 1);
                if(higher)
                    genes[i] = Helper::random(genes[i], maxvalues[i]);
                else
                    genes[i] = Helper::random(minvalues[i], genes[i]);
            }
            else // if the gene is a binary value
            {
                if(genes[i] == maxvalues[i])
                    genes[i] = minvalues[i];
                else
                    genes[i] = maxvalues[i];
            }
        }
    }
    Colour[0] = Helper::random(0,255)/255.f;
    Colour[1] = Helper::random(0,255)/255.f;
    Colour[2] = Helper::random(0,255)/255.f;
}
void Chromosome::FromString(std::string ch)
{
    std::string delimiter = ",";
    int count = 0;
    size_t pos = 0;
    std::string token;
    int colourcount = 0;
    while ((pos = ch.find(delimiter)) != std::string::npos)
    {
        token = ch.substr(0, pos);
        
        if(token.find('c') == std::string::npos)
        {
            SetInt(count, std::stoi(token));
        }
        else
        {
            token = token.substr(token.find('c')+1,token.size());
            Colour[colourcount] = std::stof(token);
            colourcount++;
        }
        ch.erase(0, pos + delimiter.length());
        count++;
    }
}

int Chromosome::GetGene(int pos)
{
    if(genes.size() > pos)
        return genes [pos];
    else
    {
        std::cout<<"genes does not contain index " << pos << " genes contains " << genes.size() << std::endl;
        return 0;
    }
}
int Chromosome::GetMaxValue(int pos)
{
    if(maxvalues.size() > pos)
        return maxvalues [pos];
    else
    {
        std::cout<<"maxValues does not contain index " << pos << " maxValues contains " << maxvalues.size() << std::endl;
        return 0;
    }
}
int Chromosome::GetMinValue(int pos)
{
    if(minvalues.size() > pos)
        return minvalues [pos];
    else
    {
        std::cout<<"minvalues does not contain index " << pos << " minvalues contains " << minvalues.size() << std::endl;
        return 0;
    }
}

std::string Chromosome::ToString()
{
    std::string chromoString = "";
    
    for(int i{0}; i< dnaLength; i++)
    {
        chromoString += std::to_string(GetGene(i)) + ", ";
    }
    
    for(int i{0}; i<3; i++)
    {
        chromoString += "c" + std::to_string(Colour[i]) + ", ";
    }
    return chromoString;
}
