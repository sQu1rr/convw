# Convw
Convenient Callback Wrapper is a header only library that allows to omit any
arguments in a callback:
```cpp
using Cb = void (int, float, double); // required callback signature
void work(Convw<Cb> callback) { callback(1, 2.0f, 3.0); } // return some values

// then call function, but wee need only x from the passed arguments
work([](float x) { cout << x << std::endl; }); // outputs: 2
```

## Usage
```cpp
#include "convw/convw.hpp"

using Callback = /* callback signature */;

// conveniently wrapping a callback
auto wrapped = sq::convw::convw<Callback>(callback);
wrapped(...);

// conveniently wrapping a callback into object (redundant in this case)
auto wrappedObj = sq::Convw<Callback>{callback};
wrappedObj(...);
```

## More complicated example
Simplified real-life example that was a motivation for the library
```cpp
#include "convw/convw.hpp"

#include <iostream>

// lets make it brief
using namespace sq;
using convw::Convw;
using convw::convw;
using namespace std;
using Peer = int;
using Packet = int;

using Cb = void (uint64_t time, const Peer&, Packet&&);

struct Host {
    void onConnect(Convw<Cb> cb) { cb_ = std::move(cb); }
    void connect() { cb_(1, 2, 3); }
    Convw<Cb> cb_;
};

// NOTE: Neither server nor client override onConnect method to supply different
// arguments that hosts sends that may not be applicable to their specific
// use cases
struct Client : public Host { };
struct Server : public Host { };

// normally client and server are created in different executables
int main()
{
    Server server{};
    Client client{};

    // we don't need to know time in this particular case
    server.onConnect([](const Peer& peer, Packet&& packet){
        cout << peer << " Connected saying: " << packet << endl;
    });

    // we don't need to know who is sending packet since we are only connected
    // to one server
    client.onConnect([](Packet&& packet) {
        cout << "Server says: " << packet << endl;
    });

    // some task that handles connection calls internally
    client.connect();
    server.connect();
}
```
- Think how it would be implemented without this library (not too hard though)
- Now imagine we add another argument to the callback - only 2 places must be
changed if you are using this library - callback type and the call

## Brief Docs
### sq::convw::convw&lt;Callback&gt; function
Wraps provided callable type into Convenient Callback
### sq::convw::Convw&lt;Callback&gt; class
Automatically wraps its constructor argument into Convenient callback. Quite
useful as a function argument, since automatically wraps lambdas and other
callable types

Contains a single std::function object and mimics its behavior

## Performance
Yes, we lose it. There should be a balance between development time and
execution performance, in this case its negligible. By ***ineffectively*** using
this library the worst achieved performance loss was **30%**.

That said, if we store wrapped callback and not regenerating it, the benchmark
shows following results (BM_Target) is a wrapped callback
```
Benchmark            Time(ns)    CPU(ns) Iterations
BM_Hardcoded_Full        1072       1072     648148
BM_Hardcoded_Wrap        1065       1067     654206
BM_Hardcoded_Empty       1065       1067     654206
BM_Target_Full           1081       1083     648148
BM_Target_Wrap           1141       1143     619469
BM_Target_Empty          1121       1122     614035
```
When compiled with -O3 performance loss isn't noticeable
```
Benchmark            Time(ns)    CPU(ns) Iterations
BM_Hardcoded_Full         133        133    5263158
BM_Hardcoded_Wrap         132        132    5185185
BM_Hardcoded_Empty        132        133    5303030
BM_Target_Full            133        133    5223881
BM_Target_Wrap            134        135    5223881
BM_Target_Empty           135        135    5223881
```

## How many object copy/moves are performed

- Copy/Moves - reference/pointer passing which may take place are not taken into
account
- Function either match in signature or arguments are omitted. Either way
callable is wrapped at least once (not necessarily by this library)
which affects performance
- Left side shows how parameters are passed **into** the callback
- Typically callback can only receive values the same way the are passed

|           | match | omitted |
|-----------|-------|---------|
| value*    | 1 / 1 | 1 / 3   |
| ref**     | 0 / 0 | 0 / 0   |
| rvalue    | 0 / 0 | 0 / 0   |
| rvalue*** | 0 / 2 | 0 / 2   |
&#42; can also be received by rvalue
&#42;&#42; includes const ref, pointer and const pointer
&#42;&#42;&#42; if received by value

## Requirements
- c++14 compiler (tested on GCC 5.3 (linux) only)
- Microsoft's [gsl](https://github.com/microsoft/gsl)
- [belks](https://github.com/sQu1rr/belks) header only library
- [Catch](https://github.com/philsquared/Catch/) for testing
- [Google Benchmark](https://github.com/google/benchmark) for benchmarking

## Build all tests and benchmarks
```bash
mkdir -p build && cd build && cmake .. && make
```
- Benchmarks will reside in /benchmark-bin
- Tests will reside in /test-bin
- Testing and benchmarking was only done on linux using GCC 5.3

## Run All Tests
Build first
```bash
cd build # have to be in build directory
make test
```
