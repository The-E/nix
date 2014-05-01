// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef NIX_I_NAMEDENTITY_H
#define NIX_I_NAMEDENTITY_H

#include <nix/base/IEntity.hpp>
#include <boost/optional.hpp>

namespace nix {
namespace base {


class NIXAPI INamedEntity : virtual public IEntity {

public:

    /**
     * Setter for the type of the entity.
     *
     * @param type The type to set.
     */
    virtual void type(const std::string &type) = 0;

    /**
     * Getter for the type of the entity.
     *
     * @return The current type.
     */
    virtual std::string type() const = 0;

    /**
     * Setter for the name of the entity.
     *
     * @param name The name of the entity.
     */
    virtual void name(const std::string &name) = 0;

    /**
     * Getter for the name of the entity.
     *
     * @return string The name of the entity.
     */
    virtual std::string name() const = 0;
    
    /**
     * Setter for the definition of the entity.
     *
     * @param definition The definition of the entity.
     */
    virtual void definition(const std::string &definition) = 0;

    /**
     * Getter for the definition of the entity.
     *
     * @return The definition of the entity.
     */
    virtual boost::optional<std::string> definition() const = 0;
    
    /**
     * Deleter for the definition of the entity.
     */
    virtual void definition(const none_t t) = 0;

    /**
     * Compare two named entities.
     *
     * @param other The entity to compare with.
     *
     * @return
     */
    virtual int compare(const INamedEntity &other) const = 0;


    virtual ~INamedEntity() {}

};


} // namespace base
} // namespace nix

#endif // NIX_I_NAMEDENTITY_H
