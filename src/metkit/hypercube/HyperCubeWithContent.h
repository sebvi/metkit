/*
 * (C) Copyright 2017- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Emanuele Danovaro
/// @date   Aug 2020


#ifndef metkit_HyperCubeWithContent_H
#define metkit_HyperCubeWithContent_H

#include "metkit/hypercube/HyperCube.h"


namespace metkit {
namespace hypercube {

template <typename T>
class Deduplicator {
public:
    virtual bool toReplace(const T& existing, const T& replacement) const = 0;
};

template <typename T>
class HyperCubeWithContent : public HyperCube {
public:
    HyperCubeWithContent(const metkit::mars::MarsRequest& request, const Deduplicator<T>& deduplicator) :
        HyperCube(request), dedup_(deduplicator) {


        // throw error for default dedup
    }


    void add(const metkit::mars::MarsRequest& request, T payload) {

        int idx = indexOf(request);

        ASSERT(0 <= idx);
        ASSERT(idx < size());

        auto entry = entries_.find(idx);
        if (entry == entries_.end()) {
            entries_.emplace(idx, payload);
            clear(idx);
        } else {
            if (dedup_.toReplace(entry->second, payload)) {
                entry->second=payload;
            }
        }
    }

    const T& at(size_t idx) {
        ASSERT(0 <= idx);
        ASSERT(idx < entries_.size());
        return entries_[idx];
    }

private:
    const Deduplicator<T>& dedup_;
    std::map<size_t, T> entries_;
};

}  // namespace hypercube
}  // namespace metkit


#endif
