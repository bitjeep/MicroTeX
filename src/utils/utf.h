#ifndef UTF_H_INCLUDED
#define UTF_H_INCLUDED

#include <string>

namespace tex {

/** Convert UTF-16 string to UTF-8 encoded string. */
std::string utf162utf8(const std::u16string& src);

/**
 * Convert an UTF-8 encoded char sequence to UTF-16 string,
 * the encoding of input char sequence must be known as UTF-8
 */
std::u16string utf82utf16(const std::string& src);

}  // namespace tex

#endif
