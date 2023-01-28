#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/DebugLoc.h"

#include <map>

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
        auto debugLoc = DbgDecl->getDebugLoc();
        if ((I.getDebugLoc())) {
          unsigned Line = debugLoc.getLine();
          errs() << "Variable name: " << Variable << " used on line: " << Line << "\n";
        }
      }
    }
  }
  return false;
}

  };
}

char Footprint::ID = 0;
static RegisterPass<Footprint> X("ftprint", "Pass to count the usage of variables", false, false);
