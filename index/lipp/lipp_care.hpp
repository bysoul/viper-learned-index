//
// Created by bysoulwarden on 2021/12/10.
//

#ifndef LIPP_CARE_HPP
#define LIPP_CARE_HPP
#include "lipp.h"

namespace viper::index {
    template<typename K>
    class LippCare :public BaseIndex<K>{
    public:
        LIPP<K, uint64_t> *map;
        LippCare(){

        }
        ~LippCare(){
            if(map!=nullptr)
                delete map;
        }
        bool SupportBulk(int threads){
            return false;
        }
        void bulk_load(std::vector<std::pair<uint64_t, KeyValueOffset>> * vector) {
            map = new LIPP<K, uint64_t>{};
            map->bulk_load(vector->data(), vector->size());
            return;
        }
        KeyValueOffset CoreInsert(const K & k, KeyValueOffset o) {
            map->insert(k,o.get_offset());
            return KeyValueOffset();
        }
        KeyValueOffset CoreGet(const K & k) {
            return map->at(k);
        }
    };
}
#endif //VIPER_BW_CARE_HPP
