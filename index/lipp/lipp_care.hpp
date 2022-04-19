//
// Created by bysoulwarden on 2021/12/10.
//

#ifndef LIPP_CARE_HPP
#define LIPP_CARE_HPP
#include "lipp.h"
#include <unordered_map>

namespace viper::index {
    template<typename K>
    class LippCare :public BaseIndex<K>{
    public:
        LIPP<K, uint64_t> *map;
        std::unordered_map<K, uint64_t> *t;
        LippCare(){

        }
        ~LippCare(){
            if(map!=nullptr)
                delete map;
        }
        bool SupportBulk(int threads){
            return false;
        }
        void bulk_load(std::vector<std::pair<uint64_t, uint64_t>> * vector) {
            map = new LIPP<K, uint64_t>{};
            //map->bulk_load(vector->data(), vector->size());
            t= new std::unordered_map<K, uint64_t>();
            for(auto &i:(*vector)){
                map->insert(i.first,i.second);
                t->insert(i);
            }
            return;
        }
        KeyValueOffset CoreInsert(const K & k, KeyValueOffset o) {
            map->insert(k,o.get_offset());
            t->insert(std::pair<K, uint64_t>(k,o.get_offset()));
            return KeyValueOffset();
        }
        KeyValueOffset CoreGet(const K & k) {
            if(map->at(k)!=(*t->find(k)).second){
                std::cout<<"!!!!!!!!!! wrong"<<std::endl;
                std::cout<<map->at(k)<<std::endl;
                std::cout<<(uint64_t)(*t->find(k)).second<<std::endl;
            }
            return KeyValueOffset(map->at(k));
        }
    };
}
#endif //VIPER_BW_CARE_HPP
