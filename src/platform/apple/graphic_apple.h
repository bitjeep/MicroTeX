#include "config.h"

#if defined(BUILD_APPLE) && !defined(MEM_CHECK)

#ifndef GRAPHIC_APPLE_H_INCLUDED
#define GRAPHIC_APPLE_H_INCLUDED

#include "graphic/graphic.h"

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>

using namespace std;

namespace tex {

class Font_CoreText : public Font {
private:
  CTFontRef _font;
  int _style;
  double _size;
  string _family;

  static CTFontSymbolicTraits convertStyle(int style);
  static int convertTraits(CTFontSymbolicTraits traits);
  static bool convertCFSTRToStdStr(CFStringRef cfStr, string& str);
  void updateFontParameters();

public:
  Font_CoreText(string family, int style, float size);

  Font_CoreText(const string& file, float size);

  Font_CoreText(CTFontRef font);
    
  virtual ~Font_CoreText();
    
  CTFontRef getCTFont() const;

  string getFamily() const;

  int getStyle() const;

  float getSize() const override;

  sptr<Font> deriveFont(int style) const override;

  bool operator==(const Font& f) const override;

  bool operator!=(const Font& f) const override;
};

/**************************************************************************************************/

class TextLayout_CoreText : public TextLayout {
private:
  CTLineRef _line;
  CGFloat _ascent;

public:
  TextLayout_CoreText(const u16string& src, const sptr<Font_CoreText>& font);
    
  virtual ~TextLayout_CoreText();

  void getBounds(Rect& r) override;

  void draw(Graphics2D& g2, float x, float y) override;
};

/**************************************************************************************************/

class Graphics2D_Quartz2D : public Graphics2D {
private:
  static Font_CoreText _default_font;

  CGContextRef _context;
  color _color;
  Stroke _stroke;
  sptr<Font_CoreText> _font;
  float _sx, _sy;
    
  void roundRect(float x, float y, float w, float h, float rx, float ry);

public:
  Graphics2D_Quartz2D(CGContextRef context);
    
  virtual ~Graphics2D_Quartz2D();
    
  CGContextRef getCGContext() const;

  void setColor(color c) override;

  color getColor() const override;

  void setStroke(const Stroke& s) override;

  const Stroke& getStroke() const override;

  void setStrokeWidth(float w) override;

  const Font* getFont() const override;

  void setFont(const Font* font) override;

  void translate(float dx, float dy) override;

  void scale(float sx, float sy) override;

  void rotate(float angle) override;

  void rotate(float angle, float px, float py) override;

  void reset() override;

  float sx() const override;

  float sy() const override;

  void drawChar(char16_t c, float x, float y) override;

  void drawText(const u16string& t, float x, float y) override;

  void drawLine(float x, float y1, float x2, float y2) override;

  void drawRect(float x, float y, float w, float h) override;

  void fillRect(float x, float y, float w, float h) override;

  void drawRoundRect(float x, float y, float w, float h, float rx, float ry) override;

  void fillRoundRect(float x, float y, float w, float h, float rx, float ry) override;
};

}  // namespace tex

#endif  // GRAPHIC_APPLE_H_INCLUDED
#endif  // BUILD_APPLE && !MEM_CHECK
