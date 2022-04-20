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
        stx::btree<uint64_t, uint64_t> btree;
        BTreeCare(){
        }
        KeyValueOffset CoreInsert(const K & k, KeyValueOffset o) {
            btree.insert(std::pair<K,uint64_t>(k,o.get_offset()));
            typename stx::btree<K, uint64_t>::iterator i=btree.find(k);
            return KeyValueOffset();
        }
        KeyValueOffset CoreGet(const K & k) {
            typename stx::btree<K, uint64_t>::iterator i=btree.find(k);
            return KeyValueOffset((uint64_t)(i->second));
        }
        typename stx::btree<uint64_t, uint64_t>::iterator CoreGetIt(const uint64_t & k) {
            typename stx::btree<uint64_t, uint64_t>::iterator i=btree.find(k);
            return i;
        }
        typename stx::btree<K, uint64_t>::iterator CoreGetEnd() {
            typename stx::btree<K, uint64_t>::iterator i=btree.end();
            return i;
        }
        typename stx::btree<K, uint64_t>::iterator CoreGetBegin() {
            typename stx::btree<K, uint64_t>::iterator i=btree.begin();
            return i;
        }

    };
}


#endif //VIPER_B_CARE_HPP
