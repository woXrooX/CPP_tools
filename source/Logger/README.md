# C++ Logger
Minimal And Simple Logger Implementation In C++

### How to use ?
First include the `Logger.h` into your project. Then follow the examples below 👇
```C++
#include "path/to/Logger.h"

int main(){
  // In Case "using namespace woXrooX;" Is Always Here :)
  // woXrooX::Log is Alias To woXrooX::Logger

  //// Log To File
  //// Currently No Path Setter Method :(
  //// Absolute Path Defined At -> "woXrooX::Logger::logsAbsolutePath"

  // To Enable Log To File.
  woXrooX::Logger::enable_log_to_file();

  // To Disable Log To File.
  woXrooX::Logger::disable_log_to_file();

  // Modes
  woXrooX::Log::success("Success");
  woXrooX::Log::info("Information");
  woXrooX::Log::warning("Warning");
  woXrooX::Log::important("Important");
  woXrooX::Log::error("Error");
  woXrooX::Log::custom("custom_type", "Custom");

  // New Line
  woXrooX::Log::newLine();

  // Line
  woXrooX::Log::line();

  //// Enable & Disable Square Brackets (By Default Disabled)
  // Enabler
  woXrooX::Logger::enable_square_brackets();

  // Disabler
  woXrooX::Logger::disable_square_brackets();

  return 0;
}
```
### Output
```
[19:50:36 2022-03-19] [SUCCESS] Success
[19:50:36 2022-03-19] [INFO] Information
[19:50:36 2022-03-19] [WARNING] Warning
[19:50:36 2022-03-19] [ERROR] Error
[19:50:36 2022-03-19] [custom_type] Custom

----------------------------------------------------------------


```
### Log Pattern
```
[HH:mm:ss yyyy-MM-dd][type] message
```
