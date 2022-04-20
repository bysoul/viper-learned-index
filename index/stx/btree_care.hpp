//
// Created by bysoulwarden on 2021/12/9.
//

#ifndef VIPER_B_CARE_HPP
#define VIPER_B_CARE_HPP

#include "../common_index.hpp"
#include "../FITing-tree/btree.h"

namespace viper::index {
    template<typename K>
    class BTreeCare :public BaseIndex<K>{
    public:
        std::map<uint64_t, uint64_t> btree;
        BTreeCare(){
        }
        KeyValueOffset CoreInsert(const uint64_t &k, KeyValueOffset o) {
            typename std::map<uint64_t, uint64_t>::iterator i=btree.find(k);
            if(i==btree.end()){
                btree.insert(std::pair<uint64_t,uint64_t>(k,o.get_offset()));
            }else{
                i->second=o.get_offset();
            }
            //std::cout<<"CoreInsert key :"<<k<<" value :"<<o.get_offset()<<std::endl;
            //typename std::map<uint64_t, uint64_t>::iterator ii=btree.find(k);
            //std::cout<<"CoreInsert key :"<<ii->first<<" value :"<<ii->second<<std::endl;
            return KeyValueOffset();
        }
        KeyValueOffset CoreGet(const K & k) {
            typename std::map<uint64_t, uint64_t>::iterator i=btree.find(k);
            return KeyValueOffset((uint64_t)(i->second));
        }
        typename std::map<uint64_t, uint64_t>::iterator CoreGetIt(const uint64_t & k) {
            typename std::map<uint64_t, uint64_t>::iterator i=btree.find(k);
            return i;
        }
        typename std::map<K, uint64_t>::iterator CoreGetEnd() {
            typename std::map<K, uint64_t>::iterator i=btree.end();
            return i;
        }
        typename std::map<K, uint64_t>::iterator CoreGetBegin() {
            typename std::map<K, uint64_t>::iterator i=btree.begin();
            return i;
        }
        int CoreGetSize() {
            return btree.size();
        }

    };
}


#endif //VIPER_B_CARE_HPP
