#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Instructions.h"

using namespace llvm;

namespace {
    struct Footprint : public FunctionPass {
        static char ID;
        Footprint() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {
          /*
            errs() << "Clang version: " << CLANG_VERSION << "\n";
            errs() << "LLVM Source Repository: " << LLVM_REPOSITORY << "\n";
            errs() << "LLVM Commit Hash: " << LLVM_COMMIT_HASH << "\n";
            errs() << "Target: " << F.getParent()->getTargetTriple() << "\n";
          */
            for (auto &B : F) {
                for (auto &I : B) {
                    if (auto *op = dyn_cast<Instruction>(&I)) {
                        for (unsigned i = 0, e = op->getNumOperands(); i != e; ++i) {
                            Value *v = op->getOperand(i);
                            if (isa<GlobalVariable>(v)) {
                                errs() << "Variable Name: " << v->getName() << "\n";
                                errs() << "1\n";
                                errs() << "Variable Scope: " << op->getDebugLoc().getLine() << "\n";
                                errs() << "Footprint: " << op->getDebugLoc().getCol() << "\n";
                                errs() << "Number of Reads: " << v->getNumUses() << "\n";
                                errs() << "Number of Writes: " << v->getNumUses() << "\n";
                            }
                        }
                    }
                }
            }
            return false;
        }
    };
}

char Footprint::ID = 0;
static RegisterPass<Footprint> X("ftprint", "Counts the number of occurrences of a variable");
