#ifndef BASEGRAPH_UTIL_HPP
#define BASEGRAPH_UTIL_HPP

#include <vector>

namespace BaseGraph {

template<typename T>
inline void swapAndPop(std::vector<T>& vec, std::size_t index) {
    std::iter_swap(vec.begin() + index, vec.end() - 1);
    vec.pop_back();
}

template<typename T>
inline std::size_t findFirst(const std::vector<T>& vec, T value) {
    for (std::size_t i=0; i<vec.size(); i++)
        if (vec[i] == value)
            return i;
    return vec.size();
}

} // namespace BaseGraph

#endif
