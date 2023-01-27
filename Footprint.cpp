#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugInfoMetadata.h"

using namespace llvm;

namespace {
  struct Foo : public FunctionPass {
    static char ID;
    Foo() : FunctionPass(ID) {}

    bool runOnFunction(Function &F) override {
      for (auto &BB : F) {
        for (auto &I : BB) {
          if (auto *op = dyn_cast<Instruction>(&I)) {
             //errs()<<"1st if passed";
            for (unsigned int i = 0, e = op->getNumOperands(); i != e; ++i) {
             // errs()<<"inside for loop 1st\n";
              Value *v = op->getOperand(i);
              /*
              if(isa<Instruction>(v))
              {
                errs()<<"inst\n";
              }
              else if(isa<Constant>(v))
              errs()<<"const\n";
              else if ( isa<Argument>(v) || isa<GlobalValue>(v))
              errs()<<"something\n";
              */
              if(isa<Constant,GlobalValue,Argument,Instruction>(v)){
             // if (auto *var = dyn_cast<Argument>(v)) {
                //errs()<<"2nd if passed";
                if (DebugLoc loc = op->getDebugLoc()) {
                 // errs()<<"2nd if";
                  errs() << "Variable '" << v->getName() << "' used at line " << loc->getLine() << "\n";
                }
              }
            }
          }
        }
      }
      return false;
    }
  };
}

char Foo::ID = 0;
static RegisterPass<Foo> X("ftprint", "Prints the line number where a variable has been used");
