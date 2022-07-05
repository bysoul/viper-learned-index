//
// Created by bysoulwarden on 2022/7/3.
//

#ifndef VIPER_RMI_CARE_H
#define VIPER_RMI_CARE_H

#include "./RMI-package/my_first_rmi.h"

namespace viper::index {
    template<typename K>
    class RMICare : public BaseIndex<K> {
    public:
        std::vector<std::pair<uint64_t, uint64_t>> vector;

        RMICare() {
        }

        ~RMICare() {
            my_first_rmi::cleanup();
        }

        bool SupportBulk(int threads) {
            return false;
        }

        void bulk_load(std::vector<std::pair<uint64_t, uint64_t>> &v) {
            vector = v;
            my_first_rmi::load("/lab505/gjk/viper/index/RMI/RMI-package/rmi_data");
        }

        KeyValueOffset CoreInsert(const K &k, KeyValueOffset o) {
            return KeyValueOffset();
        }

        KeyValueOffset CoreGet(const K &k) {
            size_t err;
            auto position = my_first_rmi::lookup(k, &err);
            uint64_t l, h, mid;
            if (vector[position].first > k) {
                l = position > err ? (position - err) : 0;
                h = position;
            } else {
                l = position;
                h = (position + err) < vector.size() ? (position + err) : (vector.size() - 1);
            }
            while (l <= h) {
                mid = (l + h) / 2;
                if (vector[mid].first==k){
                    return KeyValueOffset(vector[position].second);
                }else if(vector[mid].first > k){
                    h=mid;
                }else{
                    l=mid+1;
                }
            }
            return KeyValueOffset();
        }

    };
}


#endif //VIPER_RMI_CARE_H
