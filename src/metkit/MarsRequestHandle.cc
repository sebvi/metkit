/*
 * (C) Copyright 1996-2017 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// File MarsRequestHandle.cc
// Baudouin Raoult - (c) ECMWF Feb 12

#include "metkit/MarsRequestHandle.h"
#include "eckit/parser/StringTools.h"
#include "eckit/types/Types.h"



namespace metkit {

static eckit::Reanimator<MarsRequestHandle> marsRequestHandleReanimator;

bool shortName(const std::string& prefix, const std::string& s)
{
    if (prefix.size() > s.size())
        return false;
    return std::equal(prefix.begin(), prefix.end(), s.begin());
}

MarsRequestHandle::MarsRequestHandle(eckit::Stream& s)
    : eckit::DataHandle(s),
      request_(s),
      protocol_(eckit::Reanimator<BaseProtocol>::reanimate(s)) {
}

MarsRequestHandle::MarsRequestHandle(const MarsRequest& request,
                                     BaseProtocol* protocol):
    request_(request),
    protocol_(protocol)
{
    eckit::Log::debug() << "MarsRequestHandle::MarsRequestHandle: request: " << request << " protocol: " << protocol << std::endl;
    ASSERT(protocol);
}

MarsRequestHandle::MarsRequestHandle(const metkit::MarsRequest& request,
                                     const eckit::Configuration& database):
    request_(request),
    protocol_(ProtocolFactory::build(database)) {
}

MarsRequestHandle::~MarsRequestHandle() {}

const eckit::ReanimatorBase & MarsRequestHandle::reanimator() const {
    return marsRequestHandleReanimator;
}

const eckit::ClassSpec & MarsRequestHandle::classSpec() {
    static eckit::ClassSpec spec = { &eckit::DataHandle::classSpec(), "MarsRequestHandle" };
    return spec;
}

eckit::Length MarsRequestHandle::openForRead()
{
    eckit::Log::debug() << "MarsRequestHandle::openForRead: request_: " << request_ << std::endl;

    const std::string v (eckit::StringTools::lower(request_.verb()));
    ASSERT(v == "retrieve" || v == "stage" || v == "list");

    return protocol_->retrieve(request_);
}

void MarsRequestHandle::openForWrite(const eckit::Length& size)
{
    eckit::Log::debug() << "MarsRequestHandle::openForWrite: request_.name()=" << request_.verb() << std::endl;

    ASSERT(eckit::StringTools::lower(request_.verb()) == "archive");
    protocol_->archive(request_, size);
}

void MarsRequestHandle::openForAppend(const eckit::Length&)
{
    NOTIMP;
}

long MarsRequestHandle::read(void* buffer, long len)
{
    return protocol_->read(buffer, len);
}

long MarsRequestHandle::write(const void* buffer, long len)
{
    return protocol_->write(buffer, len);
}

void MarsRequestHandle::close()
{
    protocol_->cleanup();
}

void MarsRequestHandle::print(std::ostream& s) const
{
    s << "MarsRequestHandle[" << *protocol_ << "," << request_ << "]";
}

void MarsRequestHandle::encode(eckit::Stream& s) const {
    eckit::DataHandle::encode(s);
    s << request_;
    s << *protocol_;
}

} // namespace metkit
