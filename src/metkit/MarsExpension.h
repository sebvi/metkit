/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

/// @author Manuel Fuentes
/// @author Baudouin Raoult
/// @author Tiago Quintino

/// @date Sep 96

#ifndef metkit_MarsExpension_H
#define metkit_MarsExpension_H

#include "metkit/MarsRequest.h"
#include "eckit/memory/NonCopyable.h"
#include "metkit/MarsParsedRequest.h"


namespace metkit {

class MarsLanguage;
class MarsExpandContext;

//----------------------------------------------------------------------------------------------------------------------

class FlattenCallback {
public:
    virtual ~FlattenCallback();
    virtual void operator()(const MarsRequest&) = 0;
};

class ExpandCallback {
public:
    virtual ~ExpandCallback();
    virtual void operator()(const MarsExpandContext&, const MarsRequest&) = 0;
};

//----------------------------------------------------------------------------------------------------------------------

class MarsExpension : public eckit::NonCopyable {
public:
// -- Contructors

    MarsExpension(bool inherit);
    ~MarsExpension();

    void reset();

    MarsRequest expand(const MarsRequest&);
    std::vector<MarsRequest> expand(const std::vector<MarsParsedRequest>&);

    void expand(const MarsExpandContext& ctx,
                const MarsRequest& request,
                ExpandCallback& cb);


    void flatten(const MarsExpandContext& ctx,
        const MarsRequest& request,
                 FlattenCallback& callback);


private: // members

    MarsLanguage& language(const MarsExpandContext&, const std::string& verb);

    std::map<std::string, MarsLanguage*> languages_;
    bool inherit_;
};

//----------------------------------------------------------------------------------------------------------------------

} // namespace metkit

#endif
