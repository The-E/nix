// Copyright (c) 2013, German Neuroinformatics Node (G-Node)
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted under the terms of the BSD License. See
// LICENSE file in the root of the Project.

#ifndef IMPL_CONTAINER_H
#define IMPL_CONTAINER_H

#include <memory>
#include <vector>
#include <list>
#include <functional>

namespace nix {
namespace base {

template<typename T>
class ImplContainer {

protected:

    std::shared_ptr<T> impl_ptr;

    /**
     * Low level helper to get multiple entities via a getter function
     * that has to be provided.
     * 
     * Get multiple entities of given type and return them as vector.
     * The template param specifies which type.
     * The parameter "getEnt" is the function used to get the entities 
     * of given type T_ENT and has to be of return type T_ENT with 
     * parameter "int" specifying the index of the entity to get.
     * The parameter "nT" should give the number of entities to get and
     * should be =>0 & <= the total number of existing entities.
     * The parameter "filter" is defaulted to giving back all entities 
     * of given type. To use your own filter pass a lambda that accepts 
     * an entity of given type as parameter and returns a bool telling 
     * whether to get it or not.
     * NOTE: there is no need to specify 2nd template param TFUNC as it
     * should be automatically deduced.
     * 
     * @param class "get function": std::function of return type T_ENT and 
     *              param type "int" to get entities
     * @param int number of entities to get
     * @param class "filter function": std::function of return type bool
     *              and param type T_ENT to filter which entities to get
     * @return class entities of the given type as a vector
     */
    template<typename TENT, typename TFUNC>
    std::vector<TENT> getEntities(
        TFUNC const &getEntity,
        int nT, 
        std::function<bool(TENT)> filter) const 
    {
        std::vector<TENT> e;
        int i = 0;
         
        if(nT < 1) { return e; }
        e.resize(nT);

        for (typename std::vector<TENT>::iterator it = e.begin(); it!=e.end(); ++it) {
            if(filter(*it)) {
                *it = getEntity( i++ );
            }
        }

        return e;
    }
    

    /**
     * Helper struct for other low level helpers.
     */
    template<typename TENT>
    struct EntCont {
        EntCont(TENT s, size_t d = 0)
            : entity(s), depth(d)
        {}

        TENT entity;
        size_t depth;
    };
    
    
    /**
     * Go through the tree of sources originating from this source until
     * a max. level of "max_depth" and check for each source whether
     * to return it depending on predicate function "filter".
     * Return resulting vector of sources.
     * 
     * @param object filter function of type {@link nix::util::Filter::type}
     * @param int maximum depth to search tree
     * @return object vector of sources
     */
    template<typename TENT>
    std::vector<TENT> findEntities(TENT const self, 
                                   std::function<bool(TENT)> filter,
                                   size_t max_depth) const 
    {
        std::vector<TENT>  results;
        std::list<EntCont<TENT>> todo;

        todo.push_back(EntCont<TENT>(self));

        while(todo.size() > 0) 
        {
            EntCont<TENT> current = todo.front();
            todo.pop_front();

            bool filter_ok = filter(current.entity);
            if (filter_ok) {
                results.push_back(current.entity);
            }

            if (current.depth < max_depth) {
                std::vector<TENT> children = current.entity.children();
                size_t next_depth = current.depth + 1;

                for (auto it = children.begin(); it != children.end(); ++it) {
                    todo.push_back(EntCont<TENT>(*it, next_depth));
                }
            }
        }

        return results;
    }

public:

    ImplContainer()
        : impl_ptr(nullptr)
    {
    }


    ImplContainer(T *p_impl)
        : impl_ptr(p_impl)
    {
    }


    ImplContainer(const std::shared_ptr<T> &p_impl)
        : impl_ptr(p_impl)
    {
    }


    ImplContainer(const ImplContainer<T> &other)
        : impl_ptr(other.impl_ptr)
    {
    }


    virtual ImplContainer<T> &operator=(const ImplContainer<T> &other) {
        if (*this != other) {
            ImplContainer tmp(other);
            swap(tmp);
        }
        return *this;
    }


    virtual ImplContainer<T> &operator=(std::nullptr_t nullp) {
        impl_ptr = nullp;
        return *this;
    }


    virtual bool operator==(const ImplContainer<T> &other) const {
        return this->impl_ptr == other.impl_ptr;
    }


    virtual bool operator!=(const ImplContainer<T> &other) const {
        return !(*this == other);
    }


    virtual bool operator==(std::nullptr_t nullp) const {
        return impl_ptr == nullp;
    }

    virtual bool operator!=(std::nullptr_t nullp) const {
        return impl_ptr != nullp;
    }


    virtual void swap(ImplContainer<T> &second) {
        using std::swap;

        swap(impl_ptr, second.impl_ptr);
    }


    virtual ~ImplContainer() {}


    std::shared_ptr<T> impl() const {
        return impl_ptr;
    }

};


} // namespace base
} // namespace nix

#endif