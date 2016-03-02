#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "convw/convw.hpp"

namespace sq {

namespace convw {

using Callback1 = bool (int);
using Callback2 = bool (int, int);
using Callback3 = bool (int, int, int);

bool call3(Convw<Callback3> cb)
{
    return cb(1, 2, 3);
}

bool call2(Convw<Callback2> cb)
{
    return call3([cb](int a, int b, int c) { return cb(a, b); });
}

bool call1(Convw<Callback1> cb)
{
    return call2([cb](int a, int b) { return cb(a); });
}

SCENARIO( "Class automagically converts arguments" ) {
    bool works = true;
    REQUIRE( call3([&works](int, int, int) { return works; }) );
}

SCENARIO( "Nested callback tree" ) {
    bool works = true;
    REQUIRE( call1([&works]() { return works; }) );
}

} // \convw

} // \sq
