//
// Created by bysoulwarden on 2022/7/3.
//

#ifndef VIPER_FINEdex_CARE_H
#define VIPER_FINEdex_CARE_H

#include "aidel.h"
#include "aidel_impl.h"

namespace viper::index {
    template<typename K>
    class FINEdexCare : public BaseIndex<K> {
    public:
        aidel::AIDEL<K, uint64_t> *map;
        std::unordered_map<K, uint64_t> *map_c;
        FINEdexCare() {
            map = new aidel::AIDEL<K, uint64_t>();
            map_c = new std::unordered_map<K, uint64_t>();
        }

        ~FINEdexCare() {
            delete map;
            delete map_c;
        }

        void bulk_load(std::vector<uint64_t> &ks, std::vector<uint64_t> &vs) {
            map->train(ks, vs, 64);
        }

        KeyValueOffset CoreInsert(const K &k, KeyValueOffset o) {
            return KeyValueOffset();
        }

        KeyValueOffset CoreGet(const K &k) {
            uint64_t v = 0;
            map->find(k, v);
            return KeyValueOffset(v);
        }

    };
}


#endif //VIPER_FINEdex_CARE_H
