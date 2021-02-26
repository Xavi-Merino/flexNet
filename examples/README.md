# Examples
## 
1. First fit algorithm
2. Exact fit Algorithm 
2. Exact fit Algorithm with bitrate file.

## Definitions

All of the following definitions can be used to simplify the creation of a personalized allocation algorithm to be used inside the simulator to allocate resources for the simulated connections.

BEGIN_ALLOC_FUNCTION(name__) can be used to create an allocation algorithm that will belong to the allocator class. The parameter name__ corresponds to the name given to the new method, and so it can be used to call it when required. END_ALLOC_FUNCTION must be written after the algorithm to indicate the end of the function.

USE_ALLOC_FUNCTION(fun, simObject) can be used on the main file to set the allocation algorithm to be used by the simulation. The name of said algorithm is passed as the 'fun' parameter, and simObject corresponds to the simulator object whose allocator will be set.

SRC and DST correspond to the source and destination nodes of an event.

REQ_SLOTS(pos) can be used to get the number of slots required for a connection between nodes. The pos paremeter refers to the required position in the slots vector of the bitRate object.

LINK_IN_ROUTE(route, link) can be used to access a specific link object on a route between a source and destination node.

LINK_IN_ROUTE_ID(route, link) can be used to access the id of a specific link on a route between a source and destination node.

NUMBER_OF_ROUTES represents the amount of routes that exist between the current source and destination node.

NUMBER_OF_LINKS(route) represents the amount of links between a source and destination node on the specified route.

ALLOC_SLOTS(link, from, to) can be used to create a connection on the required interval of slots [from, from+to] of the specified link.

## Requirements

Library must be installed. by example, in ubuntu must be installed in /usr/local/lib/ (the lfnsim.a file) and /usr/local/include/ (include files)

## Compilation instructions:
Inside the code, you must have the next line including the library:

```C++
#include <fnsim/simulator.hpp>
```

Then, inside the folder you have to link the library:

```
 $ g++ main.cpp -lfnsim
```

Basically, you have to call the fnsim library with the **-l** flag