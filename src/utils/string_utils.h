#ifndef STRING_UTILS_H_INCLUDED
#define STRING_UTILS_H_INCLUDED

#include "config.h"
#include <algorithm>
#include <cerrno>
#include <climits>
#include <sstream>
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <codecvt>

namespace tex {

std::map<std::string, std::string> parseOption(const std::string& options);

/** Convert a value to string */
template<class T>
inline std::string tostring(T val) {
  std::ostringstream os;
  os << val;
  return os.str();
}

/** Convert a value to wide string */
template<class T>
inline std::u16string tou16string(T val) {
  std::string s = tostring(val);
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  return convert.from_bytes(s);
}

template<>
inline std::string tostring(char16_t val) {
  char buf[16];
  auto len = wctomb(buf, val);
  return std::string(buf, len);
}

template<class T>
inline void valueof(const std::string& s, T& val) {
  std::stringstream ss;
  ss << s;
  ss >> val;
}

template<class T>
inline void valueof(const std::u16string& s, T& val) {
  std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
  valueof(convert.to_bytes(s), val);
}

inline bool str2int(const std::string& str, int& res, int radix) {
  char* endptr = nullptr;
  errno = 0;

  const long val = strtol(str.c_str(), &endptr, radix);

  if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE)
    return false;

  res = static_cast<int>(val);
  return endptr == str.c_str() + str.size();
}

/** Transform a string to lowercase */
inline std::string& tolower(std::string& src) {
  std::transform(src.begin(), src.end(), src.begin(), ::tolower);
  return src;
}

inline std::u16string& tolower(std::u16string& src) {
  std::transform(src.begin(), src.end(), src.begin(), ::tolower);
  return src;
}

/** Ignore left side whitespace in a string */
inline std::string& ltrim(std::string& s) {
#if CLATEX_CXX17
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not_fn(isspace)));
#else
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::cref<int(int)>(isspace))));
#endif
  return s;
}

/** Ignore right side whitespace in a string */
inline std::string& rtrim(std::string& s) {
#if CLATEX_CXX17
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not_fn(isspace)).base(), s.end());
#else
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::cref<int(int)>(isspace))).base(), s.end());
#endif
  return s;
}

/** Ignore left and right side whitespace in a string */
inline std::string& trim(std::string& s) {
  return ltrim(rtrim(s));
}

/** Split string with specified delimiter */
inline void split(const std::string& str, char del, std::vector<std::string>& res) {
  std::stringstream ss(str);
  std::string tok;
  while (std::getline(ss, tok, del)) res.push_back(tok);
}

inline bool startswith(const std::string& str, const std::string& cmp) {
  return str.find(cmp) == 0;
}

inline bool endswith(const std::string& str, const std::string& cmp) {
  return str.rfind(cmp) == (str.length() - cmp.length());
}

inline bool startswith(const std::u16string& str, const std::u16string& cmp) {
  return str.find(cmp) == 0;
}

inline bool endswith(const std::u16string& str, const std::u16string& cmp) {
  return str.rfind(cmp) == (str.length() - cmp.length());
}

/** Split string with delimiter */
class StrTokenizer {
private:
  std::string _str;
  std::string _del;
  bool _ret;
  int _pos;

public:
  StrTokenizer(std::string str);

  StrTokenizer(std::string  str, std::string  del, bool ret = false);

  int count();

  std::string next();
};

/**
 * Returns a replacement string for the given one that has all backslashes
 * and dollar signs escaped
 */
inline std::string& quotereplace(const std::string& src, std::string& out) {
  for (size_t i = 0; i < src.length(); i++) {
    char c = src[i];
    if (c == '\\' || c == '$') out.append(1, '\\');
    out.append(1, c);
  }
  return out;
}

inline std::u16string& quotereplace(const std::u16string& src, std::u16string& out) {
  for (size_t i = 0; i < src.length(); i++) {
    char16_t c = src[i];
    if (c == u'\\' || c == u'$') out.append(1, u'\\');
    out.append(1, c);
  }
  return out;
}

/** Replace string with specified string in the first */
inline std::string& replacefirst(std::string& src, const std::string& from, const std::string& to) {
  size_t start = src.find(from);
  if (start == std::string::npos) return src;
  src.replace(start, from.length(), to);
  return src;
}

inline std::string& replaceall(std::string& src, const std::string& from, const std::string& to) {
  if (from.empty()) return src;
  size_t start = 0;
  while ((start = src.find(from, start)) != std::string::npos) {
    src.replace(start, from.length(), to);
    start += to.length();
  }
  return src;
}

inline std::u16string& replacefirst(std::u16string& src, const std::u16string& from, const std::u16string& to) {
  size_t start = src.find(from);
  if (start == std::u16string::npos) return src;
  src.replace(start, from.length(), to);
  return src;
}

inline std::u16string& replaceall(std::u16string& src, const std::u16string& from, const std::u16string& to) {
  if (from.empty()) return src;
  size_t start = 0;
  while ((start = src.find(from, start)) != std::u16string::npos) {
    src.replace(start, from.length(), to);
    start += to.length();
  }
  return src;
}

}  // namespace tex

#endif
