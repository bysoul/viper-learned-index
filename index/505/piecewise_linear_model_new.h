#pragma once

#include <cmath>
#include <limits>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include "function.h"

/*template<typename T>
using LargeSignedNew = typename std::conditional_t<std::is_floating_point_v<T>,
                                                long double,
                                                std::conditional_t<(sizeof(T) < 8), int64_t, __int128>>;*/

template<typename T>
using LargeSignedNew = __int128;


template<typename X, typename Y>
class OptimalPiecewiseLinearModelNew {
private:
    using SX = LargeSignedNew<X>;
    using SY = LargeSignedNew<Y>;

    struct Slope {
        SX dx{0};
        SY dy{0};

        Slope multiTwo() const {
            return Slope{dx, dy * 2};
        }

        bool operator<(const Slope &p) const {
            return dy * p.dx < dx * p.dy;
        }

        bool operator>(const Slope &p) const {
            return dy * p.dx > dx * p.dy;
        }

        bool operator==(const Slope &p) const {
            return dy * p.dx == dx * p.dy;
        }

        bool operator<=(const Slope &p) const {
            return dy * p.dx <= dx * p.dy;
        }

        bool operator!=(const Slope &p) const {
            return dy * p.dx != dx * p.dy;
        }

        explicit operator long double() const {
            return dy / (long double) dx;
        }
    };

    struct StoredPoint {
        X x;
        Y y;
    };

    struct Point {
        X x{};
        SY y{};

        Slope operator-(const Point &p) const {
            return {SX(x) - p.x, y - p.y};
        }
    };

    template<bool Upper>
    struct Hull : private std::vector<StoredPoint> {
        const SY epsilon;

        explicit Hull(SY epsilon) : std::vector<StoredPoint>(), epsilon(Upper ? epsilon : -epsilon) {}

        Point operator[](size_t i) const {
            auto &p = std::vector<StoredPoint>::operator[](i);
            return {p.x, SY(p.y) + epsilon};
        }

        void clear() { std::vector<StoredPoint>::clear(); }

        void resize(size_t n) { std::vector<StoredPoint>::resize(n); }

        void reserve(size_t n) { std::vector<StoredPoint>::reserve(n); }

        size_t size() const { return std::vector<StoredPoint>::size(); }

        void push(X x, Y y) { std::vector<StoredPoint>::emplace_back(StoredPoint{x, y}); };
    };

    X first_x = 0;
    X last_x = 0;
    std::vector<Point> hull;
    Slope k;
    const Y epsilon;
    Hull<false> lower;
    Hull<true> upper;

    size_t lower_start = 0;
    size_t upper_start = 0;
    size_t points_in_hull = 0;
    Point rectangle[4];

    auto cross(const Point &O, const Point &A, const Point &B) const {
        auto OA = A - O;
        auto OB = B - O;
        return (OA.dx * OB.dy) - (OA.dy * OB.dx);
    }

public:

    class CanonicalSegmentNew;

    explicit OptimalPiecewiseLinearModelNew(Y epsilon) : epsilon(epsilon), lower(epsilon), upper(epsilon) {
        if (epsilon < 0)
            throw std::invalid_argument("epsilon cannot be negative");

        upper.reserve(1u << 16);
        lower.reserve(1u << 16);
    }

    bool add_point(const X &x, const Y &y) {
        if (points_in_hull > 0 && x <= last_x)
            throw std::logic_error("Points must be increasing by x.");

        last_x = x;
        Point p{x, SY(y)};
        if (points_in_hull == 0) {
            first_x = x;
            //std::cout << "====================x" << x << std::endl;
            //std::cout << "====================y" << y << std::endl;
            hull.push_back(p);
            ++points_in_hull;
            return true;
        }
        if (points_in_hull == 1) {
            hull.push_back(p);
            k = hull[1] - hull[0];
            std::cout << "====================k0" << static_cast<long double>(k) << std::endl;
            /*std::cout << "====================k" << static_cast<long double>(k) << std::endl;
            std::cout << "====================x" << x << std::endl;
            std::cout << "====================y" << y << std::endl;
            long long intercept = -(long long) first_x * static_cast<long double>(k);
            std::cout << "====================intercept" << intercept << std::endl;
            std::cout << "====================dx" << std::to_string((long long) k.dx - 0) << std::endl;
            std::cout << "====================dy" << std::to_string((long long) k.dy - 0) << std::endl;

            std::cout << "====================p" << (static_cast<long double>(k) * x) + intercept << std::endl;*/
            ++points_in_hull;
            return true;
        }
        /*std::cout << "====================k" << static_cast<long double>(k) << std::endl;
        std::cout << "====================x" << x << std::endl;
        std::cout << "====================y" << y << std::endl;
        auto intercept = -(long long) first_x * static_cast<long double>(k);
        std::cout << "====================p" << (static_cast<long double>(k) * x) + intercept << std::endl;*/
        //floor double Y
        //todo opt
        auto diffY = floorl(static_cast<long double>(k) * x) - floorl(static_cast<long double>(k) * hull.back().x);
        if (diffY >= 1) {
            hull.push_back(p);
            ++points_in_hull;
            return true;
        }
        auto new_K = p - hull.back();
        std::cout << "====================new_K" << static_cast<long double>(new_K) << std::endl;
        if (new_K <= k.multiTwo()) {
            k = new_K;
            hull.push_back(p);
            ++points_in_hull;
            return true;
        }
        return false;
    }

    CanonicalSegmentNew get_segment() {
        std::cout << "====================k" << static_cast<long double>(k) << std::endl;
        std::cout << "====================cap" << hull.size() << std::endl;

        if (points_in_hull == 1)
            return CanonicalSegmentNew(Slope(), first_x);
        return CanonicalSegmentNew(k, first_x);
    }

    void reset() {
        points_in_hull = 0;
        hull.clear();
    }
};

template<typename X, typename Y>
class OptimalPiecewiseLinearModelNew<X, Y>::CanonicalSegmentNew {
    friend class OptimalPiecewiseLinearModelNew;

    Slope k;

    Point rectangle[4];
    X first;

    CanonicalSegmentNew(const Slope &k, X first) : k(k), first(first) {};

    CanonicalSegmentNew(X first) : first(first) {};

    CanonicalSegmentNew(const Point &p0, const Point &p1, X first) : rectangle{p0, p1, p0, p1}, first(first) {};

    CanonicalSegmentNew(const Point (&rectangle)[4], X first)
            : rectangle{rectangle[0], rectangle[1], rectangle[2], rectangle[3]}, first(first) {};

    bool one_point() const {
        return k.dx == 0 && k.dy == 0;
    }

public:

    CanonicalSegmentNew() = default;

    X get_first_x() const {
        return first;
    }

    CanonicalSegmentNew copy(X x) const {
        auto c(*this);
        c.first = x;
        return c;
    }

    std::pair<long double, long double> get_intersection() const {
        auto &p0 = rectangle[0];
        auto &p1 = rectangle[1];
        auto &p2 = rectangle[2];
        auto &p3 = rectangle[3];
        auto slope1 = p2 - p0;
        auto slope2 = p3 - p1;

        if (one_point() || slope1 == slope2)
            return {p0.x, p0.y};

        auto p0p1 = p1 - p0;
        auto a = slope1.dx * slope2.dy - slope1.dy * slope2.dx;
        auto b = (p0p1.dx * slope2.dy - p0p1.dy * slope2.dx) / static_cast<long double>(a);
        auto i_x = p0.x + b * slope1.dx;
        auto i_y = p0.y + b * slope1.dy;
        return {i_x, i_y};
    }

    std::pair<long double, long double> get_floating_point_segment(const X &origin) const {
        if (one_point())
            return {0, 0};
        long double slope = static_cast<long double>(k);
        long double intercept = 1-(long long) origin * slope;
        return {slope, intercept};
    }

    std::pair<long double, long double> get_slope_range() const {
        if (one_point())
            return {0, 0};

        auto min_slope = static_cast<long double>(rectangle[2] - rectangle[0]);
        auto max_slope = static_cast<long double>(rectangle[3] - rectangle[1]);
        return {min_slope, max_slope};
    }
};


template<typename Fin, typename Fout, typename lrmodel_type>
size_t make_segmentation_data_new(size_t n, size_t epsilon, Fin in, Fout out, lrmodel_type *lrmodel) {
    if (n == 0)
        return 0;

    using X = std::tuple_element_t<1, typename std::invoke_result_t<Fin, size_t>>;
    using Y = std::tuple_element_t<0, typename std::invoke_result_t<Fin, size_t>>;
    using Z = std::tuple_element_t<2, typename std::invoke_result_t<Fin, size_t>>;
    using canonical_segment = typename OptimalPiecewiseLinearModelNew<key_type, size_t>::CanonicalSegmentNew;
    size_t c = 0;
    size_t start = 0;
    auto p = in(0);

    OptimalPiecewiseLinearModelNew<X, Y> opt(epsilon);
    opt.add_point(std::get<1>(p), std::get<0>(p));
    std::vector<X> keys;
    std::vector<Z> vals;
    std::cout<<"zzz"<<std::get<1>(p)<<std::endl;
    std::cout<<"zzz"<<std::get<2>(p)<<std::endl;

    keys.push_back(std::get<1>(p));
    vals.push_back(std::get<2>(p));

    for (size_t i = 1; i < n; ++i) {
        auto next_p = in(i);
        if (i != start && std::get<1>(next_p) == std::get<1>(p))
            continue;
        p = next_p;

        bool add_success = opt.add_point(std::get<1>(p),
                                         std::get<0>(p) - start);  // alwayse start from 0 for each segment
        if (add_success) {
            keys.push_back(std::get<1>(p));
            vals.push_back(std::get<2>(p));
        } else {
            canonical_segment cs = opt.get_segment();
            auto[cs_slope, cs_intercept] = cs.get_floating_point_segment(cs.get_first_x());
            lrmodel_type model(cs_slope, cs_intercept);
            out(model, keys.begin(), vals.begin(), keys.size(), epsilon);
            // alwayse start from 0 for each segment
            std::vector<X>().swap(keys);
            std::vector<X>().swap(vals);
            assert(keys.size() == 0);
            start = std::get<0>(p);
            opt.reset();
            opt.add_point(std::get<1>(p), std::get<0>(p) - start);
            keys.push_back(std::get<1>(p));
            vals.push_back(std::get<2>(p));
            ++c;
        }
    }

    //out(start, n, opt.get_segment());
    canonical_segment cs = opt.get_segment();
    auto[cs_slope, cs_intercept] = cs.get_floating_point_segment(cs.get_first_x());
    lrmodel_type model(cs_slope, cs_intercept);
    out(model, keys.begin(), vals.begin(), keys.size(), epsilon);
    return ++c;
}