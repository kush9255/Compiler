#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/DebugInfoMetadata.h"
#include <map>
using namespace llvm;

namespace {
  struct Footprint : public FunctionPass {
    static char ID;
    Footprint() : FunctionPass(ID) {}
    std::map<StringRef, std::pair<unsigned,unsigned>> variableMap;
    std::set<unsigned> lineNumbers;
    bool runOnFunction(Function &F) override {
      for (auto &B : F) {
        for (auto &I : B) {
          if (auto *op = dyn_cast<Instruction>(&I)) {
            for (unsigned i = 0, e = op->getNumOperands(); i != e; ++i) {
              Value *v = op->getOperand(i);
              if (auto *var = dyn_cast<Instruction>(v)) {
                if (auto *md = var->getMetadata("dbg")) {
                  if (auto *di = dyn_cast<DILocalVariable>(md)) {
                    StringRef name = di->getName();
                    unsigned line = di->getLine();
                    lineNumbers.insert(line);
                    if (op->mayReadFromMemory()) {
                      if(variableMap.count(name)){
                        variableMap[name].first++;
                      }
                      else{
                        variableMap[name] = std::make_pair(1,0);
                      }
                    }
                    if (op->mayWriteToMemory()) {
                      if(variableMap.count(name)){
                        variableMap[name].second++;
                      }
                      else{
                        variableMap[name] = std::make_pair(0,1);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
      for (auto &var : variableMap) {
        errs() << "Variable: " << var.first << " used at line(s): ";
        for(auto &line:lineNumbers){
          errs()<<line<<" ";
        }
        errs()<<"\n";
        errs() << "Number of reads: " << var.second.first << "\n";
        errs() << "Number of writes: " << var.second.second << "\n\n";
      }
      return false;
    }
  };
}

char Footprint::ID = 0;
static RegisterPass<Footprint> X("ftprint", "It counts the footprint");
