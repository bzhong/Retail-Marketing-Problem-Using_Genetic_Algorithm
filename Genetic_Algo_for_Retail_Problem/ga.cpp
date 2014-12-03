//
//  ga.cpp
//  Genetic_Algo_for_Retail_Problem
//
//  Created by Brady on 11/25/14.
//  Copyright (c) 2014 Qianqian Zhong. All rights reserved.
//

#include "ga.h"
using namespace std;

GA::GA(const int& num,
       const int& num_of_clusters,
       const int& num_of_iter,
       const int& c_probability,
       const int& m_probability,
       const int& select_m,
       const int& data_s):
        num_of_clusters_(num_of_clusters),
        num_of_chromosomes_(num),
        iteration_count_(num_of_iter),
        crossover_probability_(c_probability),
        mutate_probability_(m_probability),
        selection_method_(select_m),
        data_set_num_(data_s){
    srand(NULL);
}

int GA::RandomInput() {
    return rand() % 10;
}

void GA::ChangeDataSet(const int &data_s) {
    data_set_num_ = data_s;
}

void GA::InitChromosome() {
    set_of_chromosome_.clear();
    set_of_chromosome_.resize(num_of_chromosomes_);
    for (int count = 0; count < num_of_chromosomes_; ++count) {
        for (int elem = 0; elem < num_of_clusters_; ++elem) {
            for (int ans = 0; ans < k_num_of_answers; ++ans) {
                set_of_chromosome_[count].push_back(RandomInput());
            }
        }
    }
}

// no: very high->none, 0-4; yes: none->very high, 5-9;
void GA::MapAnsValue(vector<string>& data) {
    for (int i = 5, count = 5; i < 24; i += 2, ++count) {
        if (data[i] != "" && data[i] != "-" && stoi(data[i]) < 2 && stoi(data[i]) >= 0) {
            if ((data[i + 1] == "" || data[i + 1] == "-") || stoi(data[i + 1]) > 4 || stoi(data[i + 1]) < 0){
                data[count] = to_string(stoi(data[i]) * 5 + 2);
            }
            else if (data[i] == "0") {
                data[count] = to_string(5 + stoi(data[i + 1]));
            }
            else if (data[i] == "1") {
                data[count] = to_string(4 - stoi(data[i + 1]));
            }
        }
        else {
            data[count] = "4";
        }
    }
    data.resize(15);
}

void GA::AddData(vector<string>& data) {
    MapAnsValue(data);
    if (data[0] == "0" && data[1] == "0")
        real_data_set_[0].push_back(data);
    else if (data[0] == "0" && data[1] == "1")
        real_data_set_[1].push_back(data);
    else if (data[0] == "0" && data[1] == "2")
        real_data_set_[2].push_back(data);
    else if (data[0] == "0" && data[1] == "3")
        real_data_set_[3].push_back(data);
    else if (data[0] == "1" && data[1] == "0")
        real_data_set_[4].push_back(data);
    else if (data[0] == "1" && data[1] == "1")
        real_data_set_[5].push_back(data);
    else if (data[0] == "1" && data[1] == "2")
        real_data_set_[6].push_back(data);
    else if (data[0] == "1" && data[1] == "3")
        real_data_set_[7].push_back(data);
}

void GA::InitData(ifstream& file) {
    string value;
    vector<string> data;
    int count = 0;
    while (file.good()) {
        getline(file, value, ',');
        if (value.find("\r") != string::npos) {
            size_t pos = value.find("\r");
            data.push_back(value.substr(0, pos));
            if (data.size() == 25) {
                AddData(data);
                data.clear();
            }
            count = 0;
            //data.push_back(value.substr(pos + 1));
        }
        else {
            if (count == 0 || count == 3 || count == 7 || count == 8) {
                ++count;
                continue;
            }
            data.push_back(value);
        }
        ++count;
    }
    AddData(data);
}

void GA::ComputeAndUpdate(const vector<string> &data, int& pos, const int& count) {
    pos = 0;
    long double min_dist = 0, cur_dist = 0;
    for (int i = 0; i < k_num_of_answers; ++i) {
        min_dist += (stoi(data[i + 5]) - set_of_chromosome_[count][i]) * (stoi(data[i + 5]) - set_of_chromosome_[count][i]);
    }
    min_dist = sqrt(min_dist);
    for (int cluster = 1; cluster < num_of_clusters_; ++cluster) {
        for (int i = 0; i < k_num_of_answers; ++i) {
            cur_dist += (stoi(data[i + 5]) - set_of_chromosome_[count][cluster * k_num_of_answers + i]) * (stoi(data[i + 5]) - set_of_chromosome_[count][cluster * k_num_of_answers + i]);
        }
        cur_dist = sqrt(cur_dist);
        if (cur_dist < min_dist) {
            min_dist = cur_dist;
            pos = cluster;
        }
        cur_dist = 0;
    }
}

long double GA::FitnessEval(const int& rth_chromosome) {
    vector<vector<int> > counter(num_of_clusters_, vector<int>(k_num_of_answers, 0));
    vector<int> num_of_points(num_of_clusters_, 0);
    unordered_map<int, int> point_to_pos;
    int pos = -1;
    long double cur_dist = 0;
        for (int i = 0; i < real_data_set_[data_set_num_].size(); ++i) {
            ComputeAndUpdate(real_data_set_[data_set_num_][i], pos, rth_chromosome);
            ++num_of_points[pos];
            point_to_pos[i] = pos;
            for (int j = 0; j < k_num_of_answers; ++j) {
                counter[pos][j] += stoi(real_data_set_[data_set_num_][i][j + 5]);
            }
        }
        for (int i = 0; i < num_of_clusters_; ++i) {
            if (num_of_points[i] == 0)
                continue;
            for (int j = 0; j < k_num_of_answers; ++j) {
                set_of_chromosome_[rth_chromosome][i * k_num_of_answers + j] = counter[i][j] / num_of_points[i];
            }
        }
        long double sum = 0;
        for (int i = 0; i < real_data_set_[data_set_num_].size(); ++i) {
            for (int j = 0; j < k_num_of_answers; ++j) {
                cur_dist += (stoi(real_data_set_[data_set_num_][i][5 + j]) - set_of_chromosome_[rth_chromosome][point_to_pos.at(i) * k_num_of_answers + j]) * (stoi(real_data_set_[data_set_num_][i][5 + j]) - set_of_chromosome_[rth_chromosome][point_to_pos.at(i) * k_num_of_answers + j]);
            }
            cur_dist = sqrt(cur_dist);
            sum += cur_dist;
            cur_dist = 0;
        }
        return sum;
}

void GA::Selection_Roulette(const map<int, long double>& chromosome_to_value) {
    long double total_fit_value = 0, min_value = k_int_max;
    int pos = 0;
    vector<vector<int> > new_set_of_chromosome;
    for (auto iter = chromosome_to_value.begin(); iter != chromosome_to_value.end(); ++iter) {
        total_fit_value += 1 / iter->second;
    }
    for (auto iter = chromosome_to_value.begin(); iter != chromosome_to_value.end(); ++iter) {
        int produce_num = set_of_chromosome_.size() / iter->second / total_fit_value + 0.5;
        for (int i = 0; i < produce_num; ++i) {
            new_set_of_chromosome.push_back(set_of_chromosome_[iter->first]);
        }
        if (iter->second < min_value) {
            min_value = iter->second;
            pos = iter->first;
        }
    }
    for (int i = 0; i < set_of_chromosome_.size() - new_set_of_chromosome.size(); ++i) {
        new_set_of_chromosome.push_back(set_of_chromosome_[pos]);
    }
    set_of_chromosome_.clear();
    set_of_chromosome_ = new_set_of_chromosome;
    assert(set_of_chromosome_.size() == num_of_chromosomes_);
}

void GA::Selection_Tournament(const map<int, long double> &chromosome_to_value) {
    long double min_value = k_int_max;
    int pos = 0, paren1, paren2;
    vector<vector<int> > new_set_of_chromosome;
    for (auto iter = chromosome_to_value.begin(); iter != chromosome_to_value.end(); ++iter) {
        if (iter->second < min_value) {
            min_value = iter->second;
            pos = iter->first;
        }
    }
    new_set_of_chromosome.push_back(set_of_chromosome_[pos]);
    for (int i = 0; i < set_of_chromosome_.size() - 1; ++i) {
        paren1 = rand() % num_of_chromosomes_;
        paren2 = rand() % num_of_chromosomes_;
        if (chromosome_to_value.at(paren1) < chromosome_to_value.at(paren2)) {
            new_set_of_chromosome.push_back(set_of_chromosome_[paren1]);
        }
        else {
            new_set_of_chromosome.push_back(set_of_chromosome_[paren2]);
        }
    }
    set_of_chromosome_.clear();
    set_of_chromosome_ = new_set_of_chromosome;
    assert(set_of_chromosome_.size() == num_of_chromosomes_);
}

void GA::Crossover() {
    bool probability = rand() % 100 < crossover_probability_? true : false;
    if (probability) {
        assert(set_of_chromosome_.size() == num_of_chromosomes_);
        vector<vector<int> > copy_chromosome(set_of_chromosome_);
        set_of_chromosome_.clear();
        for (int iteration = 0; iteration < num_of_chromosomes_ / 2; ++iteration) {
            int first_parent = rand() % (num_of_chromosomes_ - iteration * 2);
            int second_parent = rand() % (num_of_chromosomes_ - iteration * 2);
            while (first_parent == second_parent) {
                second_parent = rand() % (num_of_chromosomes_ - iteration * 2);
            }
            int cross_point = rand() % num_of_clusters_;
            vector<int> tmp1, tmp2;
            tmp1.insert(tmp1.end(), copy_chromosome[first_parent].begin(), copy_chromosome[first_parent].begin() + cross_point * k_num_of_answers);
            tmp1.insert(tmp1.end(), copy_chromosome[second_parent].begin() + cross_point * k_num_of_answers, copy_chromosome[second_parent].end());
            tmp2.insert(tmp2.end(), copy_chromosome[second_parent].begin(), copy_chromosome[second_parent].begin() + cross_point * k_num_of_answers);
            tmp2.insert(tmp2.end(), copy_chromosome[first_parent].begin() + cross_point * k_num_of_answers, copy_chromosome[first_parent].end());
            set_of_chromosome_.push_back(tmp1);
            set_of_chromosome_.push_back(tmp2);
            if (first_parent > second_parent) {
                copy_chromosome.erase(copy_chromosome.begin() + first_parent);
                copy_chromosome.erase(copy_chromosome.begin() + second_parent);
            }
            else {
                copy_chromosome.erase(copy_chromosome.begin() + second_parent);
                copy_chromosome.erase(copy_chromosome.begin() + first_parent);
            }
        }
    }
}

void GA::Mutation() {
    bool probability = rand() % 1000 < mutate_probability_? true : false;
    if (probability) {
        assert(set_of_chromosome_.size() == num_of_chromosomes_);
        int mutate_parent = rand() % num_of_chromosomes_;
        int pos = rand() % (num_of_clusters_ * k_num_of_answers);
        if (rand() % 2 == 0)
            set_of_chromosome_[mutate_parent][pos] += 1;
        else
            set_of_chromosome_[mutate_parent][pos] -= 1;
    }
}

void GA::PrintResults(const int& pos) {
    vector<int> point_to_pos;
    for (int i = 0; i < real_data_set_[data_set_num_].size(); ++i) {
        int cluster = -1;
        ComputeAndUpdate(real_data_set_[data_set_num_][i], cluster, pos);
        point_to_pos.push_back(cluster);
    }
    //nationality, age, gender
    map<int, OutputFormat> results;
    for (int i = 0; i < real_data_set_[data_set_num_].size(); ++i) {
        if (results.find(point_to_pos[i]) != results.end()) {
            if (real_data_set_[data_set_num_][i][3] != "" && real_data_set_[data_set_num_][i][3] != "-" && stoi(real_data_set_[data_set_num_][i][3]) < 4)
                results[point_to_pos[i]].age[stoi(real_data_set_[data_set_num_][i][3])] += 1;
            if (real_data_set_[data_set_num_][i][4] != "" && real_data_set_[data_set_num_][i][4] != "-" && stoi(real_data_set_[data_set_num_][i][4]) < 2)
                results[point_to_pos[i]].gender[stoi(real_data_set_[data_set_num_][i][4])] += 1;
        }
        else {
            OutputFormat out;
            out.age.resize(4, 0);
            out.gender.resize(2, 0);
            if (real_data_set_[data_set_num_][i][3] != "" && real_data_set_[data_set_num_][i][3] != "-" && stoi(real_data_set_[data_set_num_][i][3]) < 4)
                out.age[stoi(real_data_set_[data_set_num_][i][3])] += 1;
            if (real_data_set_[data_set_num_][i][4] != "" && real_data_set_[data_set_num_][i][4] != "-" && stoi(real_data_set_[data_set_num_][i][4]) < 2)
                out.gender[stoi(real_data_set_[data_set_num_][i][4])] += 1;
            results[point_to_pos[i]] = out;
        }
    }
    for (auto iter = results.begin(); iter != results.end(); ++iter) {
        cout << "cluster " << iter->first << ":" << endl;
        for (int j = 0; j < k_num_of_answers; ++j) {
            cout << set_of_chromosome_[pos][iter->first * k_num_of_answers + j] << " ";
        }
        cout << endl;
        cout << "age distribution: " << endl;
        int total_age_num = 0;
        for (int i = 0; i < 4; ++i)
            total_age_num += iter->second.age[i];
        cout << "20-25: " << iter->second.age[3] << " " << (long double)iter->second.age[3] / total_age_num << endl;
        cout << "25-30: " << iter->second.age[2] << " " << (long double)iter->second.age[2] / total_age_num << endl;
        cout << "30-35: " << iter->second.age[1] << " " << (long double)iter->second.age[1] / total_age_num << endl;
        cout << "35+: " << iter->second.age[0] << " " << (long double)iter->second.age[0] / total_age_num << endl;
        cout << "gender distribution: " << endl;
        int total_gender_num = 0;
        for (int i = 0; i < 2; ++i)
            total_gender_num += iter->second.gender[i];
        cout << "male: " << iter->second.gender[0] << " " << (long double)iter->second.gender[0] / total_gender_num << endl;
        cout << "female: " << iter->second.gender[1] << " " << (long double)iter->second.gender[1] / total_gender_num << endl;
        cout << endl;
    }
}

void GA::Exec() {
    InitChromosome();
    map<int, long double> chromosome_to_value;
    for (int iteration = 0; iteration < iteration_count_; ++iteration) {
        long double min_distance = k_int_max;
        int pos = -1;
        for (int i = 0; i < set_of_chromosome_.size(); ++i) {
            chromosome_to_value[i] = FitnessEval(i);
            if (chromosome_to_value.at(i) < min_distance) {
                pos = i;
                min_distance = chromosome_to_value.at(i);
            }
        }
        if (iteration == iteration_count_ - 1) {
            PrintResults(pos);
            break;
        }
        if (selection_method_ == 0)
            Selection_Roulette(chromosome_to_value);
        else if (selection_method_ == 1)
            Selection_Tournament(chromosome_to_value);
        Crossover();
        Mutation();
        chromosome_to_value.clear();
    }
}









