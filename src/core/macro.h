#ifndef MACRO_H_INCLUDED
#define MACRO_H_INCLUDED

#include "atom/atom.h"
#include "common.h"

#include <map>
#include <string>

namespace tex {

class TeXParser;

class Macro {
public:
  virtual void execute(TeXParser& tp, std::vector<std::u16string>& args) = 0;

  virtual ~Macro() = default;
};

class NewCommandMacro : public Macro {
protected:
  static std::map<std::u16string, std::u16string> _codes;
  static std::map<std::u16string, std::u16string> _replacements;
  static Macro* _instance;

  static void checkNew(const std::u16string& name);

  static void checkRenew(const std::u16string& name);

public:
  /**
   * If notify a fatal error when defining a new command but it has been
   * defined already or redefine a command but it has not been defined,
   * default is true.
   */
  static bool _errIfConflict;

  void execute(TeXParser& tp, std::vector<std::u16string>& args) override;

  static void addNewCommand(
    const std::u16string& name,
    const std::u16string& code,
    int argc
  );

  static void addNewCommand(
    const std::u16string& name,
    const std::u16string& code,
    int argc,
    const std::u16string& def
  );

  static void addRenewCommand(
    const std::u16string& name,
    const std::u16string& code,
    int argc
  );

  static void addRenewCommand(
    const std::u16string& name,
    const std::u16string& code,
    int argc,
    const std::u16string& def
  );

  static bool isMacro(const std::u16string& name);

  static void _init_();

  static void _free_();

  ~NewCommandMacro() override = default;
};

class NewEnvironmentMacro : public NewCommandMacro {
public:
  static void addNewEnvironment(
    const std::u16string& name,
    const std::u16string& begDef,
    const std::u16string& endDef,
    int argc
  );

  static void addRenewEnvironment(
    const std::u16string& name,
    const std::u16string& begDef,
    const std::u16string& endDef,
    int argc
  );
};

class MacroInfo {
public:
  static std::map<std::u16string, MacroInfo*> _commands;

  /** Add a macro, replace it if the macro is exists. */
  static void add(const std::u16string& name, MacroInfo* mac);

  /** Get the macro info from given name, return nullptr if not found. */
  static MacroInfo* get(const std::u16string& name);

  // Number of arguments
  const int _argc;
  // Options' position, can be  0, 1 and 2
  // 0 represents this macro has no options
  // 1 represents the options appear after the command name, e.g.:
  //      \sqrt[3]{2}
  // 2 represents the options appear after the first argument, e.g.:
  //      \scalebox{0.5}[2]{\LaTeX}
  const int _posOpts;

  MacroInfo() : _argc(0), _posOpts(0) {}

  MacroInfo(int argc, int posOpts) : _argc(argc), _posOpts(posOpts) {}

  explicit MacroInfo(int argc) : _argc(argc), _posOpts(0) {}

  virtual sptr<Atom> invoke(
    TeXParser& tp,
    std::vector<std::u16string>& args) {
    return nullptr;
  }

  virtual ~MacroInfo() = default;

  static void _free_();
};

class InflationMacroInfo : public MacroInfo {
private:
  // The actual macro to execute
  Macro* const _macro;

public:
  InflationMacroInfo(Macro* macro, int argc)
    : _macro(macro), MacroInfo(argc) {}

  InflationMacroInfo(Macro* macro, int argc, int posOpts)
    : _macro(macro), MacroInfo(argc, posOpts) {}

  sptr<Atom> invoke(
    TeXParser& tp,
    std::vector<std::u16string>& args
  ) override {
    _macro->execute(tp, args);
    return nullptr;
  }
};

typedef sptr<Atom> (* MacroDelegate)(
  TeXParser& tp,
  std::vector<std::u16string>& args
);

class PreDefMacro : public MacroInfo {
private:
  MacroDelegate _delegate;

public:
  PreDefMacro() = delete;

  PreDefMacro(int argc, int posOpts, MacroDelegate delegate)
    : MacroInfo(argc, posOpts), _delegate(delegate) {}

  PreDefMacro(int argc, MacroDelegate delegate)
    : MacroInfo(argc), _delegate(delegate) {}

  sptr<Atom> invoke(
    TeXParser& tp,
    std::vector<std::u16string>& args
  ) override;
};

}  // namespace tex

#endif  // MACRO_H_INCLUDED
