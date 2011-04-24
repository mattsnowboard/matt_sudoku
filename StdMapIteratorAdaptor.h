/**
 * Adapter class that allows us to use std::map with STL algorithms
 * Shamelessly copied from:
 * http://cpptruths.blogspot.com/2007/11/using-stdcopy-on-stdmap-iterator-pair.html
 * in order to avoid a Boost dependency (I mean c++11 is right arouund
 * the corner)
 */

#include <map>
#include <iostream>
#include <algorithm>
#include <string>
#include <list>
#include <iterator>

template <class BiDirIter>
class StdMapIteratorAdaptor
/* To make the custom iterator behave like a standard iterator by exposing
   required iterator_traits */
    : public
std::iterator <std::bidirectional_iterator_tag,
               typename BiDirIter::value_type::second_type>
{
    BiDirIter iter_;
public:

    explicit StdMapIteratorAdaptor(BiDirIter const & iter = BiDirIter())
  : iter_(iter) {}

    bool operator == (StdMapIteratorAdaptor const & rhs) const {
        return (iter_ == rhs.iter_);
    }

    bool operator != (StdMapIteratorAdaptor const & rhs) const {
        return !(*this == rhs);
    }

/* Return type is const to make it work with map::const_iterator */
    typename BiDirIter::value_type::second_type const & operator * () {
        return iter_->second;
    }

    typename BiDirIter::value_type::second_type const & operator * () const {
        return iter_->second;
    }

    typename BiDirIter::value_type::second_type const * operator -> () const {
        return &(iter_->second);
    }

// Pre-increment
    StdMapIteratorAdaptor & operator ++ () {
        ++iter_;
        return *this;
    }

// Post-increment
    const StdMapIteratorAdaptor operator ++ (int) {
        StdMapIteratorAdaptor temp (iter_);
        ++iter_;
        return temp;
    }

// Pre-decrement
    StdMapIteratorAdaptor & operator -- () {
        --iter_;
        return *this;
    }

// Post-decrement
    const StdMapIteratorAdaptor operator -- (int) {
        StdMapIteratorAdaptor temp (iter_);
        --iter_;
        return temp;
    }
};

/* An helper function to save some typing of tedious nested C++ types
   It is very similar to std::make_pair function */
template <class BiDirIter>
StdMapIteratorAdaptor <BiDirIter>
make_map_iterator_adaptor (BiDirIter const & iter)
{
    return StdMapIteratorAdaptor<BiDirIter> (iter);
}
