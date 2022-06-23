#include "config.h"

#if defined(BUILD_APPLE) && !defined(MEM_CHECK)

#include "platform/apple/graphic_apple.h"

using namespace tex;
using namespace std;

CTFontSymbolicTraits Font_CoreText::convertStyle(int style) {
    CTFontSymbolicTraits traits;
    switch (style) {
        case BOLD:
          traits = kCTFontTraitBold;
          break;
        case ITALIC:
          traits = kCTFontTraitItalic;
          break;
        case BOLDITALIC:
          traits = kCTFontTraitBold | kCTFontTraitItalic;
          break;
        case PLAIN:
        default:
            traits = 0;
            break;
    }
    return traits;
}

int Font_CoreText::convertTraits(CTFontSymbolicTraits traits) {
    int style;
    if (traits & kCTFontTraitBold) {
        if (traits & kCTFontTraitItalic) {
            style = BOLDITALIC;
        } else {
            style = BOLD;
        }
    } else if (traits & kCTFontTraitItalic) {
        style = ITALIC;
    } else {
        style = PLAIN;
    }
    return style;
}

bool Font_CoreText::convertCFSTRToStdStr(CFStringRef cfStr, string& str)
{
    auto maxSize = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfStr), kCFStringEncodingUTF8) + 1;
    str.resize(maxSize);
    bool retval = CFStringGetCString(cfStr, &str[0], maxSize, kCFStringEncodingUTF8);
    str.erase(std::find(str.begin(), str.end(), '\0'), str.end());
    return retval;
}

Font_CoreText::Font_CoreText(string family, int style, float size) : _font(nullptr){
    CFStringRef cfFamily = CFStringCreateWithCString(kCFAllocatorDefault, family.c_str(), kCFStringEncodingUTF8);
    if (cfFamily != nullptr) {
        CTFontRef font = CTFontCreateWithName(cfFamily, (CGFloat)size, nullptr);
        if (font != nullptr) {
            CTFontSymbolicTraits traits = convertStyle(style);
            if (traits != 0) {
                CTFontRef fontWithTraits = CTFontCreateCopyWithSymbolicTraits(font, 0.0, nullptr, traits, 0x3);
                if (fontWithTraits != nullptr) {
                    _font = fontWithTraits;
                    CFRelease(font);
                    font = nullptr;
                }
            }
            if (_font == nullptr) {
                _font = font;
            }
        }
        CFRelease(cfFamily);
    }
    if (_font == nullptr) {
        _font = CTFontCreateWithName(CFSTR("SansSerif"), size, nullptr);
    }
    updateFontParameters();
}

Font_CoreText::Font_CoreText(const string& file, float size) : _font(nullptr){
    CFStringRef cfFile = CFStringCreateWithCString(kCFAllocatorDefault, file.c_str(), kCFStringEncodingUTF8);
    if (cfFile != nullptr) {
        CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, cfFile, kCFURLPOSIXPathStyle, false);
        if (url != nullptr) {
            CFArrayRef array = CTFontManagerCreateFontDescriptorsFromURL(url);
            if (array != nullptr) {
                if (CFArrayGetCount(array) > 0) {
                    CTFontDescriptorRef fontDes = (CTFontDescriptorRef)CFArrayGetValueAtIndex(array, 0);
                    _font = CTFontCreateWithFontDescriptor(fontDes, size, nullptr);
                }
                CFRelease(array);
            }
            CFRelease(url);
        }
        CFRelease(cfFile);
    }
    if (_font == nullptr) {
        _font = CTFontCreateWithName(CFSTR("SansSerif"), size, nullptr);
    }
    updateFontParameters();
}

Font_CoreText::Font_CoreText(CTFontRef font) {
    _font = (CTFontRef)CFRetain(font);
    updateFontParameters();
}

void Font_CoreText::updateFontParameters() {
    CFStringRef cfFamily = CTFontCopyFamilyName(_font);
    convertCFSTRToStdStr(cfFamily, _family);
    _size = (float)CTFontGetSize(_font);
    _style = convertTraits(CTFontGetSymbolicTraits(_font));
    CFRelease(cfFamily);
}

Font_CoreText::~Font_CoreText() {
    CFRelease(_font);
    _font = nullptr;
}

CTFontRef Font_CoreText::getCTFont() const
{
    return _font;
}

string Font_CoreText::getFamily() const {
    return _family;
}

int Font_CoreText::getStyle() const {
    return _style;
}

float Font_CoreText::getSize() const {
    return _size;
}

sptr<Font> Font_CoreText::deriveFont(int style) const {
    return sptrOf<Font_CoreText>(_family, style, _size);
}

bool Font_CoreText::operator==(const Font& ft) const {
    const Font_CoreText& f = static_cast<const Font_CoreText&>(ft);
    return getFamily() == f.getFamily() && getSize() == f.getSize() && getStyle() == f.getStyle();
}

bool Font_CoreText::operator!=(const Font& f) const {
  return !(*this == f);
}

Font* Font::create(const string& file, float s) {
  return new Font_CoreText(file, s);
}

sptr<Font> Font::_create(const string& name, int style, float size) {
  return sptrOf<Font_CoreText>(name, style, size);
}

/**************************************************************************************************/

TextLayout_CoreText::TextLayout_CoreText(const u16string& src, const sptr<Font_CoreText>& f) {
    CTFontRef font = f->getCTFont();
    CFStringRef text = CFStringCreateWithCharacters(kCFAllocatorDefault, (const UniChar*)src.c_str(), src.length());
    CFStringRef keys[] = { kCTFontAttributeName };
    CFTypeRef values[] = { font };
    CFDictionaryRef font_attributes = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&keys,
                                                         (const void **)&values, 1,
                                                         &kCFTypeDictionaryKeyCallBacks,
                                                         &kCFTypeDictionaryValueCallBacks);
    CFAttributedStringRef attr_string = CFAttributedStringCreate(kCFAllocatorDefault, text, font_attributes);
    _line = CTLineCreateWithAttributedString(attr_string);
    _ascent = CTFontGetAscent(font);
    CFRelease(attr_string);
    CFRelease(font_attributes);
    CFRelease(text);
}

TextLayout_CoreText::~TextLayout_CoreText() {
    CFRelease(_line);
    _line = nullptr;
}

void TextLayout_CoreText::getBounds(Rect& r) {
    CGColorSpaceRef clrSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(nullptr, 100, 100, 8, 400,
                                                 clrSpace, kCGImageAlphaPremultipliedLast);
    CGRect bounds = CTLineGetImageBounds(_line, context);
    r.x = 0;
    r.y = -_ascent;
    r.w = bounds.size.width;
    r.h = bounds.size.height;
    CGColorSpaceRelease(clrSpace);
    CGContextRelease(context);
}

void TextLayout_CoreText::draw(Graphics2D& g2, float x, float y) {
    Graphics2D_Quartz2D &gq = static_cast<Graphics2D_Quartz2D&>(g2);
    CGContextRef context = gq.getCGContext();
    CGContextSaveGState(context);
    CGContextTranslateCTM(context, (CGFloat)x, (CGFloat)y);
    CGContextSetTextMatrix(context, CGAffineTransformMakeScale(1.0f, -1.0f));
    CTLineDraw(_line, context);
    CGContextRestoreGState(context);
}

sptr<TextLayout> TextLayout::create(const u16string& src, const sptr<Font>& font) {
    sptr<Font_CoreText> f = static_pointer_cast<Font_CoreText>(font);
    return sptrOf<TextLayout_CoreText>(src, f);
}

/**************************************************************************************************/

Font_CoreText Graphics2D_Quartz2D::_default_font("SansSerif", PLAIN, 10.);

Graphics2D_Quartz2D::Graphics2D_Quartz2D(CGContextRef context) {
    _context = (CGContextRef)CFRetain(context);
    _sx = _sy = 1.f;
    setColor(BLACK);
    setStroke(Stroke());
    setFont(&_default_font);
}

Graphics2D_Quartz2D::~Graphics2D_Quartz2D() {
    CGContextRelease(_context);
    _context = nullptr;
}

CGContextRef Graphics2D_Quartz2D::getCGContext() const {
    return _context;
}

void Graphics2D_Quartz2D::setColor(color c) {
    _color = c;
    const double a = color_a(c) / 255.;
    const double r = color_r(c) / 255.;
    const double g = color_g(c) / 255.;
    const double b = color_b(c) / 255.;
    CGContextSetRGBStrokeColor(_context, r, g, b, a);
    CGContextSetRGBFillColor(_context, r, g, b, a);
}

color Graphics2D_Quartz2D::getColor() const {
    return _color;
}

void Graphics2D_Quartz2D::setStroke(const Stroke& s) {
    _stroke = s;
    CGContextSetLineWidth(_context, (CGFloat)s.lineWidth);
    
    switch (s.cap) {
        case CAP_BUTT:
            CGContextSetLineCap(_context, kCGLineCapButt);
            break;
        case CAP_ROUND:
            CGContextSetLineCap(_context, kCGLineCapRound);
            break;
        case CAP_SQUARE:
            CGContextSetLineCap(_context, kCGLineCapSquare);
            break;
    }
    
    switch (s.join) {
        case JOIN_BEVEL:
            CGContextSetLineJoin(_context, kCGLineJoinBevel);
            break;
        case JOIN_ROUND:
            CGContextSetLineJoin(_context, kCGLineJoinRound);
            break;
        case JOIN_MITER:
            CGContextSetLineJoin(_context, kCGLineJoinMiter);
            break;
    }
    
    CGContextSetMiterLimit(_context, (CGFloat)s.miterLimit);
}

const Stroke& Graphics2D_Quartz2D::getStroke() const {
    return _stroke;
}

void Graphics2D_Quartz2D::setStrokeWidth(float w) {
    _stroke.lineWidth = w;
    CGContextSetLineWidth(_context, (CGFloat)w);
}

const Font* Graphics2D_Quartz2D::getFont() const {
    return static_cast<const Font*>(_font.get());
}

void Graphics2D_Quartz2D::setFont(const Font* font) {
    _font = sptr<Font_CoreText>(new Font_CoreText((static_cast<const Font_CoreText*>(font))->getCTFont()));
}

void Graphics2D_Quartz2D::translate(float dx, float dy) {
    CGContextTranslateCTM(_context, (CGFloat)dx, (CGFloat)dy);
}

void Graphics2D_Quartz2D::scale(float sx, float sy) {
    _sx *= sx;
    _sy *= sy;
    CGContextScaleCTM(_context, (CGFloat)sx, (CGFloat)sy);
}

void Graphics2D_Quartz2D::rotate(float angle) {
    CGContextRotateCTM(_context, (CGFloat)angle);
}

void Graphics2D_Quartz2D::rotate(float angle, float px, float py) {
    CGContextTranslateCTM(_context, (CGFloat)px, (CGFloat)py);
    CGContextRotateCTM(_context, (CGFloat)angle);
    CGContextTranslateCTM(_context, (CGFloat)-px, (CGFloat)-py);
}

void Graphics2D_Quartz2D::reset() {
    CGAffineTransform invert = CGAffineTransformInvert(CGContextGetCTM(_context));
    CGContextConcatCTM(_context, invert);
    _sx = _sy = 1.f;
}

float Graphics2D_Quartz2D::sx() const {
    return _sx;
}

float Graphics2D_Quartz2D::sy() const {
    return _sy;
}

void Graphics2D_Quartz2D::drawChar(char16_t c, float x, float y) {
    drawText(u16string(1, c), x, y);
}

void Graphics2D_Quartz2D::drawText(const u16string& t, float x, float y) {
    TextLayout_CoreText layout(t, _font);
    layout.draw(*this, x, y);
}

void Graphics2D_Quartz2D::drawLine(float x1, float y1, float x2, float y2) {
    CGContextBeginPath(_context);
    CGContextMoveToPoint(_context, x1, y1);
    CGContextAddLineToPoint(_context, x2, y2);
    CGContextClosePath(_context);
    CGContextStrokePath(_context);
}

void Graphics2D_Quartz2D::drawRect(float x, float y, float w, float h) {
    CGRect r = CGRectMake((CGFloat)x, (CGFloat)y, (CGFloat)w, (CGFloat)h);
    CGContextStrokeRect(_context, r);
}

void Graphics2D_Quartz2D::fillRect(float x, float y, float w, float h) {
    CGRect r = CGRectMake((CGFloat)x, (CGFloat)y, (CGFloat)w, (CGFloat)h);
    CGContextFillRect(_context, r);
}

void Graphics2D_Quartz2D::roundRect(float x, float y, float w, float h, float rx, float ry) {
    CGRect r = CGRectMake((CGFloat)x, (CGFloat)y, (CGFloat)w, (CGFloat)h);
    CGPathRef path = CGPathCreateWithRoundedRect(r, (CGFloat)rx, (CGFloat)ry, nullptr);
    CGContextBeginPath(_context);
    CGContextAddPath(_context, path);
    CGContextClosePath(_context);
    CGPathRelease(path);
}

void Graphics2D_Quartz2D::drawRoundRect(float x, float y, float w, float h, float rx, float ry) {
    roundRect(x, y, w, h, rx, ry);
    CGContextStrokePath(_context);
}

void Graphics2D_Quartz2D::fillRoundRect(float x, float y, float w, float h, float rx, float ry) {
    roundRect(x, y, w, h, rx, ry);
    CGContextFillPath(_context);
}

#endif
