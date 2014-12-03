//
//  main.cpp
//  Genetic_Algo_for_Retail_Problem
//
//  Created by Brady on 11/25/14.
//  Copyright (c) 2014 Qianqian Zhong. All rights reserved.
//

#include "ga.h"
using namespace std;

int main(int argc, const char * argv[]) {
    cout << "Please input number of chromosomes and number of clusters in each chromosome: " << endl;
    int num_chro, num_cluster, num_iter, c_prob, m_prob, select_m, data_set_num;
    cin >> num_chro >> num_cluster;
    cout << "Please input termination condition (number of iterations): " << endl;
    cin >> num_iter;
    cout << "Please input crossover probability(%) and mutation probability(1/1000): " << endl;
    cin >> c_prob >> m_prob;
    cout << "Please choose selection method: 0 Roulette Wheel, 1 Tournament: " << endl;
    cin >> select_m;
    cout << "Please choose which data set you want to use: " << endl;
    cout << "evening: 0 MK, 1 OP, 2 FN, 3 HR" << endl;
    cout << "morning: 4 MK, 5 OP, 6 FN, 7 HR" << endl;
    cin >> data_set_num;
    GA ga(num_chro, num_cluster, num_iter, c_prob, m_prob, select_m, data_set_num);
    ifstream input("/Users/qianqianzhong/Documents/github/Genetic_Algo_for_Retail_Problem/Safeer_Data.csv");
    ga.InitData(input);
    input.close();
    input.open("/Users/qianqianzhong/Documents/github/Genetic_Algo_for_Retail_Problem/Safeer_Data2.csv");
    ga.InitData(input);
    input.close();
    for (int i = 0; i < 8; ++i) {
        ga.ChangeDataSet(i);
        ga.Exec();
        cout << "*****data set " << i << " finished.*****" << endl << endl;
    }
    cout << "execution finished." << endl;
    return 0;
}
