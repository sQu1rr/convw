#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "convw/convw.hpp"

namespace sq {

namespace convw {

using Callback = bool (int, float, double, int);

bool call(std::function<Callback> cb)
{
    return cb(1, 2.0f, 3.0, 4);
}

SCENARIO( "Testing wrapping function matching callback" ) {
    WHEN( "Using anonymous lambda" ) {
        REQUIRE( call(
            convw<Callback>([](int a, float b, double c, int d) {
                return a == 1 && b == 2.0f && c == 3.0 && d == 4;
            })
        ) );
    }
    WHEN( "Using lambda" ) {
        auto lambda = [](int a, float b, double c, int d) {
            return a == 1 && b == 2.0f && c == 3.0 && d == 4;
        };
        REQUIRE( call(convw<Callback>(lambda)) );
    }
    WHEN( "Using std::function" ) {
        auto function = std::function<Callback>(
            [](int a, float b, double c, int d) {
                return a == 1 && b == 2.0f && c == 3.0 && d == 4;
            }
        );
        REQUIRE( call(convw<Callback>(function)) );
    }
}

SCENARIO( "Testing wrapping an empty function" ) {
    WHEN( "Using anonymous lambda" ) {
        REQUIRE( call(
            convw<Callback>([]() { return true; })
        ) );
    }
    WHEN( "Using lambda" ) {
        auto lambda = []() { return true; };
        REQUIRE( call(convw<Callback>(lambda)) );
    }
    WHEN( "Using std::function" ) {
        auto function = std::function<bool ()>([]() { return true; });
        REQUIRE( call(convw<Callback>(function)) );
    }
}

SCENARIO( "Testing wrapping function ommiting arguments" ) {
    WHEN( "Using anonymous lambda" ) {
        REQUIRE( call(
            convw<Callback>([](int a) { return a == 1; })
        ) );
    }
    WHEN( "Using lambda" ) {
        auto lambda = [](float b) { return b == 2.0f; };
        REQUIRE( call(convw<Callback>(lambda)) );
    }
    WHEN( "Using std::function" ) {
        auto function = std::function<bool (float, double)>(
            [](float b, double c) {
                return b == 2.0f && c == 3.0;
            }
        );
        REQUIRE( call(convw<Callback>(function)) );
    }
}

SCENARIO( "Testing wrapping function argemnts same type" ) {
    WHEN( "Using anonymous lambda" ) {
        REQUIRE( call(
            convw<Callback>([](int a, int b) { return a + b == 5; })
        ) );
    }
    WHEN( "Using lambda" ) {
        auto lambda = [](int a, int b) { return a + b == 5; };
        REQUIRE( call(convw<Callback>(lambda)) );
    }
    WHEN( "Using std::function" ) {
        auto function = std::function<bool (int, int)>([](int a, int b) {
            return a + b == 5;
        });
        REQUIRE( call(convw<Callback>(function)) );
    }
}

SCENARIO( "Testing closure" ) {
    WHEN( "Using anonymous lambda" ) {
        int b = 2;
        REQUIRE( call(
            convw<Callback>([&b](int a) { return a + b == 3; })
        ) );
    }
}

} // \convw

} // \sq
