#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"

using namespace llvm;

namespace {
  struct Footprint : public FunctionPass {
    static char ID;
    Footprint() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      int count = 0;
      for (auto &BB : F) {
        for (auto &I : BB) {
         if (auto *op = dyn_cast<Value>(&I)) {
         // errs() << "First IF" << "\n";
            if (op->getName() == "res")
              count++; 
            // 
            // if (op->getOperand(0)->getName() == StringRef("res")) {
              // errs() << "2nd IF" << "\n";
              //errs() << count++ << " : "<< op->getNumUses() << "\n";
            // }
          }
        }
      }
      errs() << "Number of occurrences of variable: " << count << "\n";
      return false;
    }
  };
}

char Footprint::ID = 0;
static RegisterPass<Footprint> X("ftprint", "Counts the number of occurrences of a variable");
