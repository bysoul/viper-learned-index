//
// Created by bysoulwarden on 2022/7/3.
//

#ifndef VIPER_Index505_CARE_H
#define VIPER_Index505_CARE_H

#include "finedex.h"
#include "finedex_impl.h"

namespace viper::index {
    template<typename K>
    class Index505Care : public BaseIndex<K> {
    public:
        aidel::FINEdex<K, uint64_t> *map;
        std::unordered_map<K, uint64_t> *map_c;
        Index505Care() {
            map = new aidel::FINEdex<K, uint64_t>();
        }

        ~Index505Care() {

        }

        void bulk_load(std::vector<uint64_t> &ks, std::vector<uint64_t> &vs) {
            map->train(ks, vs, 64);
            for(int i=0;i<ks.size();i++){
                map_c->insert(std::pair<K, uint64_t>(ks[i],vs[i]));
            }
        }

        KeyValueOffset CoreInsert(const K &k, KeyValueOffset o) {
            return KeyValueOffset();
        }

        KeyValueOffset CoreGet(const K &k) {
            uint64_t v = 0;
            map->find(k, v);
            if(v!=*map_c->find(k)){
                std::cout<<"========================fault"<<std::endl;
                std::cout<<*map_c->find(k)<<" "<<v<<std::endl;
            }
            return KeyValueOffset(v);
        }

    };
}


#endif //VIPER_Index505_CARE_H
