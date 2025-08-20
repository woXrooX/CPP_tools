# C++ SHA256 Implementation.

### Usage
```C++
#include <iostream>
#include "SHA256/SHA256.hpp"

int main(){
  std::string data = "Data";
  woXrooX::SHA256 hash;
  std::cout << hash.digest(data) << '\n';

  return 0;
}
```

### Output
```
cec3a9b89b2e391393d0f68e4bc12a9fa6cf358b3cdf79496dc442d52b8dd528
```
