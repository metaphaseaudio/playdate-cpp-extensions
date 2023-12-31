# Playdate CPP Extensions
Being able to use C++ for Playdate development is a great start, but since the
SDK itself is all in C, you'll still end up writing a lot of C, and that wasn't
really the point, was it?

This library extends the core `playdate-cpp` library, and provides RAII
compliant C++ objects for the C API, as well as other useful objects to ease
development. The goal of this library to let you write as much C++, (and as 
little C) as possible.

## How to use
Start over at the core [`playdate-cpp` repo](https://github.com/nstbayless/playdate-cpp).
Once you've got that all set up, add this project as another submodule, and
follow these steps:

### Build
Like `playdate-cpp`, this library is designed to integrate into your CMake
project as a submodule, and requires the core library to be included first.
Once included, link your target with `pdcpp`. This will link in both the C++
boilerplate from `playdate-cpp` and the extension library.

```CMake
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/playdate-cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodules/playdate-cpp-extensions)

add_playdate_application(my_awesome_game)

# Note that you don't need to explicitly link against the `pdcpp_core` if
# linking against the extension library as it will come along transitively
target_link_libraries(my_awesome_game pdcpp)
```

It's possible we'll want to merge the two projects at some point, but for the
time being, an effort is being made to isolate things what are useful for
building large-scale projects vs. things what are required to run C++ at all.

### Initialization in the `eventHandler`
In order to support objects which wrap the C API, or indeed any class that
interacts with the C API, this library requires users to initialize a specific
global static pointer to the API in their project's event handler. Likewise,
best practice would dictate that this pointer is cleared during the teardown.

In order to achieve this, any event handler using the extensions library should
look similar to this annotated excerpt from the CPPong example:
```c++
// This header provides the interface for managing the global static pointer.
#include <pdcpp/core/GlobalPlaydateAPI.h>
...

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* pd, PDSystemEvent event, uint32_t arg)
{
    // this line is required to use C++ at all. Refer to the base PDCPP library
    // for more info.
    eventHandler_pdnewlib(pd, event, arg);

    if (event == kEventInit)
    {
        // this line is required to use the extensions library
        pdcpp::GlobalPlaydateAPI::initialize(pd);
        
        // Encapsulating all of a project's state into an object ensures that the 
        // C interface is as minimal as possible, and simplifies teardown.
        game = std::make_unique<pdcppong::Game>();
        
        pd->system->setUpdateCallback(update, pd);
    }

    // Best practice says clean up:
    if (event == kEventTerminate)
    {
        // Destroy your state *before* clearing the pointer to the C API! If
        // these lines are reversed, you will encounter all manner of crashes. 
        game = nullptr;
        pdcpp::GlobalPlaydateAPI::destroyInstance();
    }
    return 0;
}
#ifdef __cplusplus
}
#endif
```

### You're now ready to start using the C++ API
That is all that's required!

The vast majority of the C API is accessible through the C++ API, please send a
message or PR if you find an API you need is missing.

That said, there are some interfaces where providing a C++ API doesn't add much
value, (at least not yet,) and you'll need to use the `pdcpp::GlobalPlaydateAPI`
yourself. Generally these are either stand-alone functions, or stateless
methods. For these cases, calling the C API is still the best route:
* logging/errors
* the Time And Date APIs
* sparse graphics functions (most should be moving to `pdcpp::Graphics`)
* the "Miscellaneous" section of the C API

While it is unlikely to disappear completely, it's probable that this list will
shrink, as we create methods for interacting with these APIs that are more
semantic to C++. 

## Contributing
Contributions are highly encouraged. This project will be maintained, but
everyone has different things they care about, and if you need it, someone else
probably does too.

Short summary to leave something for the official document on the subject: go 
ahead and fire off that PR. Expect debate, but also expect your submission to be
considered seriously, and in good faith.

For more details, refer to the [CONTRIBUTING.md](CONTRIBUTING.md) file.
