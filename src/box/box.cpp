#include "box/box.h"
#include "unimath/uni_font.h"

using namespace tex;
using namespace std;

void Box::copyMetrics(const sptr<Box>& box) {
  _width = box->_width;
  _height = box->_height;
  _depth = box->_depth;
  _shift = box->_shift;
}

int Box::lastFontId() {
  return FontContext::NO_FONT;
}

std::string Box::name() const {
  return "Box";
}

std::string Box::toString() const {
  const auto h = _height + _depth;
  return sstr(
    name(), " [",
    _width,
    ", h(", _height, " + ", _depth, ")=", h, ", ",
    _shift, "]"
  );
}

int BoxGroup::lastFontId() {
  int id = FontContext::NO_FONT;
  for (int i = _children.size() - 1; i >= 0 && id == FontContext::NO_FONT; i--) {
    id = _children[i]->lastFontId();
  }
  return id;
}

int DecorBox::lastFontId() {
  return _base->lastFontId();
}
