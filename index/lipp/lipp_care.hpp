//
// Created by bysoulwarden on 2021/12/10.
//

#ifndef LIPP_CARE_HPP
#define LIPP_CARE_HPP
#include "lipp.h"
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

namespace viper::index {
    template<typename K>
    class LippCare :public BaseIndex<K>{
    public:
        LIPP<K, uint64_t> *map;
        // std::unordered_map<K, uint64_t> *t;
        // std::ofstream myfile;
        // std::ofstream myfile_fail;
        // std::ofstream myfile_fail_lipp;
        // int count1=0;
        // int count2=0;
        // int insert_count=0;
        LippCare(){
            // std::string filepath = "/lab505/gjk/viper/example";
            // filepath += std::to_string((time(0)/60) % 60);
            // filepath += ".txt";
            // myfile.open (filepath,std::ios::trunc);

            // std::string filepath1 = "/lab505/gjk/viper/example_fail";
            // filepath1 += std::to_string((time(0)/60) % 60);
            // filepath1 += ".txt";
            // myfile_fail.open (filepath1,std::ios::trunc);

            // std::string filepath2 = "/lab505/gjk/viper/example_fail_lipp";
            // filepath2 += std::to_string((time(0)/60) % 60);
            // filepath2 += ".txt";
            // myfile_fail_lipp.open (filepath2,std::ios::trunc);
        }
        ~LippCare(){
            if(map!=nullptr)
                delete map;
            // if(t!= nullptr){
            //     delete t;
            // }
            // std::cout << "get fail " << count2 << std::endl;
            // std::cout << "get success " << count1 << std::endl;
            // std::cout << "insert " << insert_count << std::endl;
            // myfile.close();
            // myfile_fail.close();
            // myfile_fail_lipp.close();
        }
        bool SupportBulk(int threads){
            return false;
        }
        void bulk_load(std::vector<std::pair<uint64_t, uint64_t>> &vector) {
            map = new LIPP<K, uint64_t>{};
            map->bulk_load(vector.data(), vector.size());
            // map->show();
            // t= new std::unordered_map<K, uint64_t>();
            // for(auto &i:vector){
            //     map->insert(i.first,i.second);
            //     t->insert(i);
            //     myfile << i.first << " " << i.second << std::endl;
            // }
            // myfile.close();
            return;
        }
        KeyValueOffset CoreInsert(const K & k, KeyValueOffset o) {
            map->insert(k,(uint64_t)(o.get_offset()));

            // insert_count++;
            // t->insert(std::pair<K, uint64_t>(k,(uint64_t)(o.get_offset())));
            // myfile_insert << k << " " << o.get_offset() << std::endl;
            return KeyValueOffset();
        }
        KeyValueOffset CoreGet(const K & k) {
            // if(map->at(k, false)!=(t->find(k))->second){
            //     count2++;
            //     myfile_fail << k << " " << (uint64_t)(t->find(k)->second) << std::endl;
            //     myfile_fail_lipp << k << " " << (uint64_t)(map->at(k, false)) << std::endl;
            //     return KeyValueOffset();
            // }
            // else {
            //     count1++;
            // }

            return KeyValueOffset(map->at(k));
        }
    };
}
#endif //VIPER_BW_CARE_HPP
