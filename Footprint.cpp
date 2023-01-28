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

    bool runOnFunction(Function &F) override {
      for (auto &BB : F) {
        for (auto &I : BB) {
          if (auto *DbgDecl = dyn_cast<DbgVariableIntrinsic>(&I)) {
            auto *Var = DbgDecl->getVariable();
            StringRef Variable = Var->getName();
            errs() << "Variable name: " << Variable << "\n";
          }
        }
      }
      return false;
    }
  };
}

char Footprint::ID = 0;
static RegisterPass<Footprint> X("ftprint", "Variable Name Pass", false, false);
