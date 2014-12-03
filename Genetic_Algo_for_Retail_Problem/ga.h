//
//  ga.h
//  Genetic_Algo_for_Retail_Problem
//
//  Created by Brady on 11/25/14.
//  Copyright (c) 2014 Qianqian Zhong. All rights reserved.
//

#ifndef Genetic_Algo_for_Retail_Problem_ga_h
#define Genetic_Algo_for_Retail_Problem_ga_h

#include "common.h"
using namespace std;

class GA {
public:
    GA(const int& num = 100,
       const int& num_of_clusters = 4,
       const int& num_of_iter = 5,
       const int& c_probability = 80,
       const int& m_probability = 2,
       const int& select_m = 1,
       const int& data_s = 0);
    void AddData(vector<string>& data);
    void MapAnsValue(vector<string>& data);
    void InitData(ifstream& file);
    void InitChromosome();
    int RandomInput();
    void ChangeDataSet(const int& data_s);
    long double FitnessEval(const int& rth_chromosome);
    void ComputeAndUpdate(const vector<string>& data, int& pos, const int& count);
    void Selection_Roulette(const map<int, long double>& chromosome_to_value);
    void Selection_Tournament(const map<int, long double>& chromosome_to_value);
    void Crossover();
    void Mutation();
    void PrintResults(const int& pos);
    void Exec();
private:
    int num_of_clusters_;
    int num_of_chromosomes_;
    int iteration_count_;
    int crossover_probability_;
    int mutate_probability_;
    int selection_method_;
    int data_set_num_;
    vector<vector<int> > set_of_chromosome_;
    vector<vector<string> > real_data_set_[8]; // first evening: mk, op, fn, hr; second morning: mk, op, fn, hr
};

#endif
