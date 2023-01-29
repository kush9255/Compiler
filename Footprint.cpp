#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/Pass.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <set>
#include <string>

using namespace llvm;

namespace {
cl::opt<std::string> var_name("var-name", cl::desc("Specify variable name"),
                              cl::value_desc("variablename"));
struct Footprint : public FunctionPass {
  static char ID;
  Footprint() : FunctionPass(ID) {}

  std::map<StringRef, std::set<unsigned int>> varLines;

  bool runOnFunction(Function &F) override {
    errs() << "Var name: " << var_name << "\n";
    for (auto &BB : F) {
      for (auto &I : BB) {
        if (auto *DbgDecl = dyn_cast<DbgVariableIntrinsic>(&I)) {
          auto *Var = DbgDecl->getVariable();
          StringRef Variable = Var->getName();
          std::string Variable_name=Variable.str();
         if (Variable_name==var_name) {
            if (DILocation *Loc = I.getDebugLoc()) {
              if (!Loc)
                continue;
              unsigned int Line = Loc->getLine();
              varLines[Variable].insert(Line);
            }
          }
        }
      }
    }

    for (auto &entry : varLines) {
      errs() << "Variable name: " << entry.first << " used at lines: ";
      for (auto &line : entry.second) {
        errs() << line << " ";
      }
      errs() << "\n";
    }
    return false;
  }
};
} // namespace

char Footprint::ID = 0;
static RegisterPass<Footprint> Y("ftprint", "Variable Line Numbers Pass", false,
                                 false);

int main(int argc, char **argv) { cl::ParseCommandLineOptions(argc, argv); }
