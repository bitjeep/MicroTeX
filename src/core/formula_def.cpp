#include "core/formula.h"

using namespace std;
using namespace tex;

map<u16string, u16string> Formula::_predefinedTeXFormulasAsString{
  {u"qquad",              u"\\quad\\quad"},
  {u" ",                  u"\\nbsp"},
  {u"ne",                 u"\\not\\equals"},
  {u"neq",                u"\\not\\equals"},
  {u"ldots",              u"\\mathinner{\\ldotp\\ldotp\\ldotp}"},
  {u"dotsc",              u"\\ldots"},
  {u"dots",               u"\\ldots"},
  {u"cdots",              u"\\mathinner{\\cdotp\\cdotp\\cdotp}"},
  {u"dotsb",              u"\\cdots"},
  {u"dotso",              u"\\ldots"},
  {u"dotsi",              u"\\!\\cdots"},
  {u"bowtie",             u"\\mathrel\\triangleright\\joinrel\\mathrel\\triangleleft"},
  {u"models",             u"\\mathrel|\\joinrel\\equals"},
  {u"Doteq",              u"\\doteqdot"},
  {u"{",                  u"\\lbrace"},
  {u"}",                  u"\\rbrace"},
  {u"|",                  u"\\Vert"},
  {u"&",                  u"\\textampersand"},
  {u"%",                  u"\\textpercent"},
  {u"_",                  u"\\underscore"},
  {u"$",                  u"\\textdollar"},
  {u"@",                  u"\\matharobase"},
  {u"#",                  u"\\mathsharp"},
  {u"relbar",             u"\\mathrel{\\smash-}"},
  {u"hookrightarrow",     u"\\lhook\\joinrel\\joinrel\\joinrel\\rightarrow"},
  {u"hookleftarrow",      u"\\leftarrow\\joinrel\\joinrel\\joinrel\\rhook"},
  {u"Longrightarrow",     u"\\Relbar\\joinrel\\Rightarrow"},
  {u"longrightarrow",     u"\\relbar\\joinrel\\rightarrow"},
  {u"Longleftarrow",      u"\\Leftarrow\\joinrel\\Relbar"},
  {u"longleftarrow",      u"\\leftarrow\\joinrel\\relbar"},
  {u"Longleftrightarrow", u"\\Leftarrow\\joinrel\\Rightarrow"},
  {u"longleftrightarrow", u"\\leftarrow\\joinrel\\rightarrow"},
  {u"iff",                u"\\;\\Longleftrightarrow\\;"},
  {u"implies",            u"\\;\\Longrightarrow\\;"},
  {u"impliedby",          u"\\;\\Longleftarrow\\;"},
  {u"mapsto",             u"\\mapstochar\\rightarrow"},
  {u"longmapsto",         u"\\mapstochar\\longrightarrow"},
  {u"log",                u"\\mathop{\\mathrm{log}}\\nolimits"},
  {u"lg",                 u"\\mathop{\\mathrm{lg}}\\nolimits"},
  {u"ln",                 u"\\mathop{\\mathrm{ln}}\\nolimits"},
  {u"ln",                 u"\\mathop{\\mathrm{ln}}\\nolimits"},
  {u"lim",                u"\\mathop{\\mathrm{lim}}"},
  {u"limsup",             u"\\mathop{\\mathrm{lim\\,sup}}"},
  {u"liminf",             u"\\mathop{\\mathrm{lim\\,inf}}"},
  {u"injlim",             u"\\mathop{\\mathrm{inj\\,lim}}"},
  {u"projlim",            u"\\mathop{\\mathrm{proj\\,lim}}"},
  {u"varinjlim",          u"\\mathop{\\underrightarrow{\\mathrm{lim}}}"},
  {u"varprojlim",         u"\\mathop{\\underleftarrow{\\mathrm{lim}}}"},
  {u"varliminf",          u"\\mathop{\\underline{\\mathrm{lim}}}"},
  {u"varlimsup",          u"\\mathop{\\overline{\\mathrm{lim}}}"},
  {u"sin",                u"\\mathop{\\mathrm{sin}}\\nolimits"},
  {u"arcsin",             u"\\mathop{\\mathrm{arcsin}}\\nolimits"},
  {u"sinh",               u"\\mathop{\\mathrm{sinh}}\\nolimits"},
  {u"cos",                u"\\mathop{\\mathrm{cos}}\\nolimits"},
  {u"arccos",             u"\\mathop{\\mathrm{arccos}}\\nolimits"},
  {u"cot",                u"\\mathop{\\mathrm{cot}}\\nolimits"},
  {u"arccot",             u"\\mathop{\\mathrm{arccot}}\\nolimits"},
  {u"cosh",               u"\\mathop{\\mathrm{cosh}}\\nolimits"},
  {u"tan",                u"\\mathop{\\mathrm{tan}}\\nolimits"},
  {u"arctan",             u"\\mathop{\\mathrm{arctan}}\\nolimits"},
  {u"tanh",               u"\\mathop{\\mathrm{tanh}}\\nolimits"},
  {u"coth",               u"\\mathop{\\mathrm{coth}}\\nolimits"},
  {u"sec",                u"\\mathop{\\mathrm{sec}}\\nolimits"},
  {u"arcsec",             u"\\mathop{\\mathrm{arcsec}}\\nolimits"},
  {u"arccsc",             u"\\mathop{\\mathrm{arccsc}}\\nolimits"},
  {u"sech",               u"\\mathop{\\mathrm{sech}}\\nolimits"},
  {u"csc",                u"\\mathop{\\mathrm{csc}}\\nolimits"},
  {u"csch",               u"\\mathop{\\mathrm{csch}}\\nolimits"},
  {u"max",                u"\\mathop{\\mathrm{max}}"},
  {u"min",                u"\\mathop{\\mathrm{min}}"},
  {u"sup",                u"\\mathop{\\mathrm{sup}}"},
  {u"inf",                u"\\mathop{\\mathrm{inf}}"},
  {u"arg",                u"\\mathop{\\mathrm{arg}}\\nolimits"},
  {u"ker",                u"\\mathop{\\mathrm{ker}}\\nolimits"},
  {u"dim",                u"\\mathop{\\mathrm{dim}}\\nolimits"},
  {u"hom",                u"\\mathop{\\mathrm{hom}}\\nolimits"},
  {u"det",                u"\\mathop{\\mathrm{det}}"},
  {u"exp",                u"\\mathop{\\mathrm{exp}}\\nolimits"},
  {u"Pr",                 u"\\mathop{\\mathrm{Pr}}"},
  {u"gcd",                u"\\mathop{\\mathrm{gcd}}"},
  {u"deg",                u"\\mathop{\\mathrm{deg}}\\nolimits"},
  {u"bmod",               u"\\:\\mathbin{\\mathrm{mod}}\\:"},
  {u"Mapsto",             u"\\Mapstochar\\Rightarrow"},
  {u"mapsfrom",           u"\\leftarrow\\mapsfromchar"},
  {u"Mapsfrom",           u"\\Leftarrow\\Mapsfromchar"},
  {u"Longmapsto",         u"\\Mapstochar\\Longrightarrow"},
  {u"longmapsfrom",       u"\\longleftarrow\\mapsfromchar"},
  {u"Longmapsfrom",       u"\\Longleftarrow\\Mapsfromchar"},
  {u"arrowvert",          u"\\vert"},
  {u"Arrowvert",          u"\\Vert"},
  {u"aa",                 u"\\mathring{a}"},
  {u"AA",                 u"\\mathring{A}"},
  {u"ddag",               u"\\ddagger"},
  {u"dag",                u"\\dagger"},
  {u"Doteq",              u"\\doteqdot"},
  {u"doublecup",          u"\\Cup"},
  {u"doublecap",          u"\\Cap"},
  {u"llless",             u"\\lll"},
  {u"gggtr",              u"\\ggg"},
  {u"Alpha",              u"\\mathord{\\mathrm{A}}"},
  {u"Beta",               u"\\mathord{\\mathrm{B}}"},
  {u"Epsilon",            u"\\mathord{\\mathrm{E}}"},
  {u"Zeta",               u"\\mathord{\\mathrm{Z}}"},
  {u"Eta",                u"\\mathord{\\mathrm{H}}"},
  {u"Iota",               u"\\mathord{\\mathrm{I}}"},
  {u"Kappa",              u"\\mathord{\\mathrm{K}}"},
  {u"Mu",                 u"\\mathord{\\mathrm{M}}"},
  {u"Nu",                 u"\\mathord{\\mathrm{N}}"},
  {u"Omicron",            u"\\mathord{\\mathrm{O}}"},
  {u"Rho",                u"\\mathord{\\mathrm{P}}"},
  {u"Tau",                u"\\mathord{\\mathrm{T}}"},
  {u"Chi",                u"\\mathord{\\mathrm{X}}"},
  {u"hdots",              u"\\ldots"},
  {u"restriction",        u"\\upharpoonright"},
  {u"celsius",            u"\\mathord{{}^\\circ\\mathrm{C}}"},
  {u"micro",              u"\\textmu"},
  {u"marker",             u"\\kern{0.25ex}\\rule{0.5ex}{1.2ex}\\kern{0.25ex}"},
  {u"hybull",             u"\\rule[0.6ex]{1ex}{0.2ex}"},
  {u"block",              u"\\rule{1ex}{1.2ex}"},
  {u"uhblk",              u"\\rule[0.6ex]{1ex}{0.6ex}"},
  {u"lhblk",              u"\\rule{1ex}{0.6ex}"},
  {u"notin",              u"\\not\\in"},
  {u"rVert",              u"\\Vert"},
  {u"lVert",              u"\\Vert"},
  {u"lvert",              u"\\vert"},
  {u"rvert",              u"\\vert"},
  {u"Android",            u"\\Huge{\\android}\\normalsize"},
  {u"AndroidTeX",         u"\\Android\\large{\\dT\\dE\\dX}\\normalsize"},
  {u"TeX",                u"\\large{\\dT\\dE\\dX}\\normalsize"},
  {u"LaTeX",              u"\\text{L\\kern-0.4em\\raisebox{.45ex}{\\scalebox{0.7}{A}}\\kern-.2em T\\kern-.15em\\raisebox{-.5ex}{E}\\kern-.15em X}"},
};
