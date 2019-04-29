//
//  Chromosome.hpp
//  t02
//
//  Created by Daniel Harvey on 21/02/2019.
//  Copyright © 2019 Cordry, Julien. All rights reserved.
//

#ifndef Chromosome_hpp
#define Chromosome_hpp
#include <iostream>
#include <vector>
#include <string> 

class Chromosome
{
public:
    Chromosome();
    
    void AddGene(int NumberOfGenes, int maxValue, int minValue = 0);
    void EndAddingGenes();
    void SetRandom();
    void SetInt(int pos, int value);
    void Crossover(Chromosome &c1, Chromosome &c2);
    void Mutate(int MutationRate);
    int GetGene(int pos);
    int GetMaxValue(int pos);
    int GetMinValue(int pos);
    std::string ToString();
    void FromString(std::string ch);
    float Colour[3] {0.f,0.f,0.f};
    
    
private:
    std::vector<int> genes;
    std::vector<int> maxvalues;
    std::vector<int> minvalues;
    int dnaLength = 0;
    bool Finished = false;
    
    
    
};


#endif /* Chromosome_hpp */
