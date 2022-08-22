#ifndef __LR_MODEL_H___
#define __LR_MODEL_H___

#include "mkl.h"
#include "mkl_lapacke.h"
#include <array>
#include <vector>
#include "util_.h"
namespace index505 {

    template<class key_t>
    class LinearRegressionModel {


    public:
        inline LinearRegressionModel();

        inline LinearRegressionModel(long double w, long double b);

        ~LinearRegressionModel();

        void train(const typename std::vector<key_t>::const_iterator &it, size_t size);

        void train(const std::vector<key_t> &keys,
                   const std::vector<size_t> &positions);

        void print_weights() const;

        size_t predict(const key_t &key) const;

        std::vector<size_t> predict(const std::vector<key_t> &keys) const;

        size_t max_error(const typename std::vector<key_t>::const_iterator &keys_begin,
                         uint32_t size);

        size_t max_error(const std::vector<key_t> &keys,
                         const std::vector<size_t> &positions);

        inline long double get_weight0() { return weights[0]; }

        inline long double get_weight1() { return weights[1]; }

        inline size_t get_maxErr() { return maxErr; }

    private:
        size_t maxErr = 0;
        std::array<long double, 2> weights;

    };
}


#endif