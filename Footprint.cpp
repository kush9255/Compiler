#include <map>
#include <set>
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IntrinsicInst.h"

using namespace llvm;

namespace {
  struct Footprint : public FunctionPass {
    static char ID;
    Footprint() : FunctionPass(ID) {}

    std::map<StringRef, std::set<unsigned int>> varLines;

    bool runOnFunction(Function &F) override {
      for (auto &BB : F) {
        for (auto &I : BB) {
          if (auto *DbgDecl = dyn_cast<DbgVariableIntrinsic>(&I)) {
            auto *Var = DbgDecl->getVariable();
            StringRef Variable = Var->getName();
            if (DILocation *Loc = I.getDebugLoc()) {
              if (!Loc)
                continue;
              unsigned int Line = Loc->getLine();
              varLines[Variable].insert(Line);
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
}

char Footprint::ID = 0;
static RegisterPass<Footprint> Y("ftprint", "Variable Line Numbers Pass", false, false);
