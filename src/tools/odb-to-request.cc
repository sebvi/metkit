/*
 * (C) Copyright 1996- ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

#include <string>
#include <vector>

#include "eckit/filesystem/PathName.h"
#include "eckit/io/FileHandle.h"
#include "eckit/log/Log.h"
#include "eckit/option/CmdArgs.h"
#include "eckit/option/Option.h"
#include "eckit/option/SimpleOption.h"
#include "eckit/runtime/Tool.h"

#include "metkit/MetkitTool.h"
#include "metkit/odb/OdbToRequest.h"


using namespace metkit;
using namespace eckit;
using namespace eckit::option;

//----------------------------------------------------------------------------------------------------------------------

class OdbToRequestTool : public MetkitTool {
public:
    OdbToRequestTool(int argc, char** argv) : MetkitTool(argc, argv) {
        options_.push_back(
            new SimpleOption<std::string>("verb", "Verb in the request [default:retrieve]"));
        options_.push_back(new SimpleOption<std::string>(
            "database", "for 'archive' verb which database to archive [default:local]"));
        options_.push_back(new SimpleOption<std::string>(
            "target", "for 'retrieve' verb to which target to retrieve  [default:data]"));
        options_.push_back(
            new SimpleOption<bool>("one", "Merge into only one request [default:false]"));
        options_.push_back(
            new SimpleOption<bool>("constant", "Only constant columns [default:true]"));
        options_.push_back(new SimpleOption<bool>("json", "Format request in json  [default:false]"));
    }

    virtual ~OdbToRequestTool() {}

private:  // methods
    int minimumPositionalArguments() const { return 1; }

    virtual void execute(const eckit::option::CmdArgs& args);

    virtual void init(const CmdArgs& args);

    virtual void usage(const std::string& tool) const;

private:  // members
    std::vector<PathName> paths_;
    std::string verb_     = "retrieve";
    std::string database_ = "local";
    std::string target_   = "data";
    bool one_             = false;
    bool constant_        = true;
    bool json_            = false;
};

//----------------------------------------------------------------------------------------------------------------------

void OdbToRequestTool::init(const CmdArgs& args) {
    args.get("one", one_);
    args.get("constant", constant_);
    args.get("verb", verb_);
    args.get("database", database_);
    args.get("target", target_);
    args.get("json", json_);

    if (json_) {
        porcelain_ = true;
    }
}


void OdbToRequestTool::usage(const std::string& tool) const {
    Log::info() << "Usage: " << tool << " [options] [request1] [request2] ..." << std::endl
                << std::endl;

    Log::info() << "Examples:" << std::endl
                << "=========" << std::endl
                << std::endl
                << tool << " --one --verb=archive data.odb" << std::endl
                << std::endl;
}

void OdbToRequestTool::execute(const eckit::option::CmdArgs& args) {
    PathName inFile(args(0));

    FileHandle dh(inFile);
    dh.openForRead();

    for (const MarsRequest& request : odb::OdbToRequest(verb_, one_, constant_).odbToRequest(dh)) {
        eckit::Log::info() << request << std::endl;
    }
}

//----------------------------------------------------------------------------------------------------------------------

int main(int argc, char** argv) {
    OdbToRequestTool tool(argc, argv);
    return tool.start();
}
