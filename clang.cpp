#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <cstdio>
#include <cstdlib>

using namespace llvm;

namespace {
  struct Footprint : public FunctionPass {
    static char ID;
    Footprint() : FunctionPass(ID) {}
    bool runOnFunction(Function &F) override {
      static int cnt=0;
      //errs()<<"loda"<<"\n";
      char buffer[256];
      std::string version;
      std::string command = "/home/cs22mtech11006/llvm-project/build/bin/./clang --version";
      std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
      if(cnt==0)
      {
      if (!pipe) {
        errs() << "Failed to execute command: " << command << "\n";
        return false;
      }
      while (!feof(pipe.get())) {
        if (fgets(buffer, 256, pipe.get()) != NULL)
          version += buffer;
      }
      //errs() << version;
      std::string ver, has, git, tar;
      int counter=0;
      for(char i: version){
        if(i == ' ')
          counter+=1;
        else if(counter==2)
        ver+=i;
        else if(counter == 3 && i!='(')
        git+=i;
        else if(counter == 4 && i!=')')
        has+=i;
        else if(i=='\n')
        tar+=i;
      }
      cnt++;
      errs()<<ver<<"\n"<<git<<"\n"<<has<<"\n"<<tar;
    }
    
      return false;
    }
  };
}

char Footprint::ID = 0;
static RegisterPass<Footprint> X("ftprint", "Print Clang version");
