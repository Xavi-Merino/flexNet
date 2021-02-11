# Examples
## 
1. First fit algorithm
2. Exact fit Algorithm 

## Requirements

Library must be installed. by example, in ubuntu must be installed in /usr/local/lib/ (the lfnsim.a file) and /usr/local/include/ (include files)

## Compilation instructions:
Inside the code, you must have the next line including the library:

```C++
#include <fnsim/simulator.hpp>
```

You have to link the library:

```
 $ g++ e<example number>_<example name>.cpp -lfnsim
```

Basically, you have to call the fnsim library with the **-l** flag