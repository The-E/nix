// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#include <nix/util/util.hpp>
#include <nix/hdf5/SourceHDF5.hpp>

using namespace std;

namespace nix {
namespace hdf5 {


SourceHDF5::SourceHDF5(const SourceHDF5 &source)
    : EntityWithMetadataHDF5(source.file(), source.group(), source.id(), source.type(), source.name())
{
    source_group = source.source_group;
}


SourceHDF5::SourceHDF5(File file, Group group, const std::string &id, const string &type, const string &name)
    : SourceHDF5(file, group, id, type, name, util::getTime())
{
}


SourceHDF5::SourceHDF5(File file, Group group, const std::string &id, const string &type, const string &name, time_t time)
    : EntityWithMetadataHDF5(file, group, id, type, name, time)
{
    source_group = group.openGroup("sources");
}


bool SourceHDF5::hasSource(const string &id) const {
    return source_group.hasGroup(id);
}


Source SourceHDF5::getSource(const string &id) const {
    if(source_group.hasGroup(id)) {
        Group group = source_group.openGroup(id, false);
        string type;
        string name;
        group.getAttr("type", type);
        group.getAttr("name", name);
        shared_ptr<SourceHDF5> tmp = make_shared<SourceHDF5>(file(), group, id, type, name);
        return Source(tmp);
    } else {
        return Source();
    }
}


Source SourceHDF5::getSource(size_t index) const {
    string id = source_group.objectName(index);
    // all checks done by "getSource(const string &id)"
    return getSource(id);
}


size_t SourceHDF5::sourceCount() const {
    return source_group.objectCount();
}


Source SourceHDF5::createSource(const string &name, const string &type) {
    string id = util::createId("source");

    while(source_group.hasObject(id)) {
        id = util::createId("source");
    }

    Group grp = source_group.openGroup(id, true);
    shared_ptr<SourceHDF5> tmp = make_shared<SourceHDF5>(file(), grp, id, type, name);

    return Source(tmp);
}


bool SourceHDF5::deleteSource(const string &id) {
    if (source_group.hasGroup(id)) {
        source_group.removeGroup(id);
        return true;
    } else {
        return false;
    }
}


void SourceHDF5::swap(SourceHDF5 &other) {
    using std::swap;

    EntityWithMetadataHDF5::swap(other);
    swap(source_group, other.source_group);
}


SourceHDF5& SourceHDF5::operator=(const SourceHDF5 &other) {
    if (*this != other) {
        SourceHDF5 tmp(other);
        swap(tmp);
    }
    return *this;
}


SourceHDF5::~SourceHDF5() {}


} // namespace hdf5
} // namespace nix
