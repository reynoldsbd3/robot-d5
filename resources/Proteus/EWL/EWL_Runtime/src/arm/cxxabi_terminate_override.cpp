
#include <cstdlib>
#include <exception>

// gnu C++ override
namespace __cxxabiv1 {
std::terminate_handler __terminate_handler = std::abort;
}