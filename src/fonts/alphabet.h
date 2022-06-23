#ifndef ALPHABET_H_INCLUDED
#define ALPHABET_H_INCLUDED

#include <string>
#include <vector>

namespace tex {

class UnicodeBlock {
private:
  static std::vector<const UnicodeBlock*> _defined;

public:
  // predefined unicode-blocks
  static const UnicodeBlock BASIC_LATIN;
  static const UnicodeBlock LATIN1_SUPPLEMENT;
  static const UnicodeBlock CYRILLIC;
  static const UnicodeBlock GREEK;
  static const UnicodeBlock GREEK_EXTENDED;
  static const UnicodeBlock UNKNOWN;

  const char16_t _start, _end;

  UnicodeBlock(char16_t codePointStart, char16_t codePointEnd)
      : _start(codePointStart), _end(codePointEnd) {}

  bool contains(char16_t c) const;

  bool operator==(const UnicodeBlock& ub) const;

  bool operator<(const UnicodeBlock& ub) const;

  bool operator>(const UnicodeBlock& ub) const;

  static const UnicodeBlock& define(char16_t codePointStart, char16_t codePointEnd);

  static const UnicodeBlock& of(char16_t c);
};

class AlphabetRegistration {
public:
  virtual const std::vector<UnicodeBlock>& getUnicodeBlock() const = 0;

  virtual const std::string getPackage() const = 0;

  virtual const std::string getTeXFontFile() const = 0;

  virtual ~AlphabetRegistration();
};

class CyrillicRegistration : public AlphabetRegistration {
private:
  static const std::vector<UnicodeBlock> _block;
  static const std::string _package;
  static const std::string _font;

public:
  const std::vector<UnicodeBlock>& getUnicodeBlock() const override;

  const std::string getPackage() const override;

  const std::string getTeXFontFile() const override;
};

class GreekRegistration : public AlphabetRegistration {
private:
  static const std::vector<UnicodeBlock> _block;
  static const std::string _package;
  static const std::string _font;

public:
  const std::vector<UnicodeBlock>& getUnicodeBlock() const override;

  const std::string getPackage() const override;

  const std::string getTeXFontFile() const override;
};

}  // namespace tex

#endif  // ALPHABET_H_INCLUDED
