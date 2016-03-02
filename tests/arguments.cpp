#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "convw/convw.hpp"

namespace sq {

namespace convw {

struct Obj {
    Obj() { construct++; }
    Obj(Obj&) { copy++; }
    Obj(const Obj&) { copy++; }
    Obj(Obj&&) { move++; }
    ~Obj() { destruct++; }

    void operator = (const Obj&) { throw "No assignment should be done"; }
    void operator = (Obj&&) { throw "No assignment should be done"; }
    void operator = (Obj&) { throw "No assignment should be done"; }

    static void clear() { construct = copy = move = destruct = 0; }
    
    static int construct;
    static int copy;
    static int move;
    static int destruct;
};
int Obj::construct = 0;
int Obj::copy = 0;
int Obj::move = 0;
int Obj::destruct = 0;

static bool test(int copy, int move)
{
    return Obj::construct == 0 && copy == Obj::copy && move == Obj::move;
}

SCENARIO( "Testing argument passing" ) {
    Obj obj; obj.clear();

    GIVEN( "Function arguments match" ) {
        WHEN ( "Passing by value" ) {
            THEN( "If receiving by value" ) {
                convw<void (Obj)>([](Obj) {})(obj);
                REQUIRE( test( 1, 1 ) );
            }
            THEN( "If receiving by rvalue" ) {
                convw<void (Obj)>([](Obj) {})(obj);
                REQUIRE( test( 1, 1 ) );
            }
        }
        WHEN ( "Passing by rvalue" ) {
            THEN( "If receiving by value" ) {
                convw<void (Obj&&)>([](Obj) {})(std::move(obj));
                REQUIRE( test( 0, 2 ) );
            }
            THEN( "If receiving by rvalue" ) {
                convw<void (Obj&&)>([](Obj&&) {})(std::move(obj));
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by reference" ) {
            THEN( "If receiving by reference" ) {
                convw<void (Obj&)>([](Obj&) {})(obj);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by const reference" ) {
            THEN( "If receiving by const reference" ) {
                convw<void (const Obj&)>([](const Obj&) {})(obj);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by pointer" ) {
            THEN( "If receiving by pointer" ) {
                convw<void (Obj*)>([](Obj*) {})(&obj);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by const pointer" ) {
            THEN( "If receiving by const pointer" ) {
                convw<void (const Obj*)>([](const Obj*) {})(&obj);
                REQUIRE( test( 0, 0 ) );
            }
        }
    }

    GIVEN( "Function arguments ommited" ) {
        WHEN ( "Passing by value" ) {
            THEN( "If receiving by value" ) {
                convw<void (Obj, int)>([](Obj) {})(obj, 0);
                REQUIRE( test( 1, 3 ) );
            }
            THEN( "If receiving by rvalue" ) {
                convw<void (Obj, int)>([](Obj) {})(obj, 0);
                REQUIRE( test( 1, 3 ) );
            }
        }
        WHEN ( "Passing by rvalue" ) {
            THEN( "If receiving by value" ) {
                convw<void (Obj&&, int)>([](Obj) {})(std::move(obj), 0);
                REQUIRE( test( 0, 2 ) );
            }
            THEN( "If receiving by rvalue" ) {
                convw<void (Obj&&, int)>([](Obj&&) {})(std::move(obj), 0);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by reference" ) {
            THEN( "If receiving by reference" ) {
                convw<void (Obj&, int)>([](Obj&) {})(obj, 0);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by const reference" ) {
            THEN( "If receiving by const reference" ) {
                convw<void (const Obj&, int)>([](const Obj&) {})(obj, 0);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by pointer" ) {
            THEN( "If receiving by pointer" ) {
                convw<void (Obj*, int)>([](Obj*) {})(&obj, 0);
                REQUIRE( test( 0, 0 ) );
            }
        }
        WHEN ( "Passing by const pointer" ) {
            THEN( "If receiving by const pointer" ) {
                convw<void (const Obj*, int)>([](const Obj*) {})(&obj, 0);
                REQUIRE( test( 0, 0 ) );
            }
        }
    }

    REQUIRE( Obj::construct + Obj::copy + Obj::move == Obj::destruct );
}

} // \convw

} // \sq
