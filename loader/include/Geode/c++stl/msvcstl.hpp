#pragma once

#include <map>
#include <vector>
#include <set>
#include <unordered_set>

namespace gd {
    template <class T>
    using vector = std::vector<T>;

    template <class K, class V>
    using map = std::map<K, V>;

    template <class K, class V>
    using unordered_map = std::unordered_map<K, V>;

    template <class K>
    using set = std::set<K>;

    template <class K>
    using unordered_set = std::unordered_set<K>;
}
