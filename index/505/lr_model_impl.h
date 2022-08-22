#ifndef __LR_MODEL_IMPL_H___
#define __LR_MODEL_IMPL_H___

#include "lr_model.h"
#include "util_.h"

namespace index505 {

    template<class key_t>
    inline LinearRegressionModel<key_t>::LinearRegressionModel() {}

    template<class key_t>
    inline LinearRegressionModel<key_t>::LinearRegressionModel(long double w, long double b) {
        weights[0] = w;
        weights[1] = b;
    }

    template<class key_t>
    LinearRegressionModel<key_t>::~LinearRegressionModel() {}


    template<class key_t>
    void LinearRegressionModel<key_t>::train(const typename std::vector<key_t>::const_iterator &it, size_t size) {
        std::vector<key_t> trainkeys(size);
        std::vector<size_t> positions(size);
        for (size_t i = 0; i < size; i++) {
            trainkeys[i] = *(it + i);
            positions[i] = i;
        }
        train(trainkeys, positions);
    }


    template<class key_t>
    void LinearRegressionModel<key_t>::train(const std::vector<key_t> &keys,
                                             const std::vector<size_t> &positions) {
        assert(keys.size() == positions.size());

        if (keys.size() == 0) return;

        if (positions.size() == 0) return;
        if (positions.size() == 1) {
            weights[0] = 0;
            weights[1] = positions[0];
            return;
        }
        Point start{keys[0], 0};
        Point p{keys[1], 0};
        auto k = p - start;
        for (int i = 2; i < positions.size(); i++) {
            //todo opt
            auto diffY = floorl(static_cast<long double>(k) * keys[i]) - floorl(static_cast<long double>(k) * keys[i-1]);
            if (diffY >= 1) {
                continue;
            }
            k = Point{keys[i],i} - Point{keys[i-1],i-1};
        }
        long double slope = static_cast<long double>(k);
        weights[0]=slope;
        weights[1]=1-(long long) start.x * slope;
        maxErr=0;
        //maxErr = max_error(keys, positions);
    }

    template<class key_t>
    void LinearRegressionModel<key_t>::print_weights() const {
        std::cout << "Weight[0]: " << weights[0] << " ,weight[1]: " << weights[1] << std::endl;
    }

// ============ prediction ==============
    template<class key_t>
    size_t LinearRegressionModel<key_t>::predict(const key_t &key) const {
        long double model_key = key;
        long double res = floorl(weights[0] * model_key + weights[1]);
        return res > 0 ? res : 0;
    }

    template<class key_t>
    std::vector<size_t> LinearRegressionModel<key_t>::predict(const std::vector<key_t> &keys) const {
        assert(keys.size() > 0);
        std::vector<size_t> pred(keys.size());
        for (int i = 0; i < keys.size(); i++) {
            pred[i] = predict(keys[i]);
        }
        return pred;
    }

// =========== max__error ===========
    template<class key_t>
    size_t LinearRegressionModel<key_t>::max_error(
            const typename std::vector<key_t>::const_iterator &keys_begin,
            uint32_t size) {
        size_t max = 0;

        for (size_t key_i = 0; key_i < size; ++key_i) {
            long long int pos_actual = key_i;
            long long int pos_pred = predict(*(keys_begin + key_i));
            int error = std::abs(pos_actual - pos_pred);
            if (error > max) {
                max = error;
            }
        }
        return max;
    }

    template<class key_t>
    size_t LinearRegressionModel<key_t>::max_error(const std::vector<key_t> &keys,
                                                   const std::vector<size_t> &positions) {
        size_t max = 0;

        for (size_t key_i = 0; key_i < keys.size(); ++key_i) {
            long long int pos_actual = positions[key_i];
            long long int pos_pred = predict(keys[key_i]);
            int error = std::abs(pos_actual - pos_pred);
            if (error > max) {
                max = error;
            }
        }
        return max;
    }

}

#endif