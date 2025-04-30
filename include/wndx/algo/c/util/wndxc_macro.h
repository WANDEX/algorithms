#pragma once

// NOTE:
// Rule of thumb - macro should not be used in any of C++ related files.
// To not pollute the global namespace of the C++ project code.
// NESTING INTO C++ NAMESPACE WILL NOT WORK! (i.e. namespace ... { }) =>
// Macros are handled before the compiler ever sees the code,
// and preprocessor knows nothing about C++ namespaces.

// WNDXC_IS_SMALL - sizeof type/object <= 2 addresses of memory.
// -> cheap to copy (only if has no setup costs!). Cost of common types:
// cheap: all fundamental types, enumerated types, std::string_view.
// expns: std::array, std::string, std::vector, std::ostream, most of the classes.
#define WNDXC_IS_SMALL(T) (sizeof(T) <= 2 * sizeof(void*))

