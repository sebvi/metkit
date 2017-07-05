/*
 * (C) Copyright 1996-2013 ECMWF.
 *
 * This software is licensed under the terms of the Apache Licence Version 2.0
 * which can be obtained at http://www.apache.org/licenses/LICENSE-2.0.
 * In applying this licence, ECMWF does not waive the privileges and immunities
 * granted to it by virtue of its status as an intergovernmental organisation nor
 * does it submit to any jurisdiction.
 */

// Baudouin Raoult - ECMWF Jan 2015

#ifndef DummyInputVariable_H
#define DummyInputVariable_H

#include "Variable.h"


class DummyInputVariable : public Variable {
public:

    DummyInputVariable(Field &owner, const Variable &parent);
    virtual ~DummyInputVariable();

protected:

    const Variable &parent_;

    // -- Methods

    virtual Variable *makeOutputVariable(Field &owner, const std::string &name, const std::vector<Dimension *> &dimensions) const;
    virtual bool dummy() const;
    virtual bool sameAsDummy(const Variable &) const;
    virtual const std::string &ncname() const;

    // From variable

    virtual void print(std::ostream &s) const;

    // From Endowed

    virtual int varid() const;
};

#endif
