#pragma once

namespace wndx {

/**
 * is sizeof type/object <= 2 addresses of memory.
 * -> cheap to copy (only if has no setup costs!). Cost of common types:
 * cheap: all fundamental types, enumerated types, std::string_view.
 * expns: std::array, std::string, std::vector, std::ostream, most of the classes.
 */
#define isSmall(T) (sizeof(T) <= 2 * sizeof(void*))

} // namespace wndx
