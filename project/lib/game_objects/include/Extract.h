#pragma once

#include <boost/json.hpp>

// function for deducing parameters
template<typename T>
void extract(boost::json::object const &obj, T &t, boost::string_view key) {
    t = value_to<T>(obj.at(key));
}
