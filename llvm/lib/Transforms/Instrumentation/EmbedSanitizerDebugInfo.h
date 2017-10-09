//===-- Extension to ThreadSanitizer.cpp - detecting races, Embeded ARM --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// (c) 2017 - Hassan Salehe Matar, Koc University
//            Email: hmatar@ku.edu.tr
//===----------------------------------------------------------------------===//


#include "llvm/Transforms/Instrumentation.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Utils/ModuleUtils.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/Analysis/CallGraph.h"
#include "llvm/Analysis/CaptureTracking.h"
#include "llvm/Analysis/MemoryBuiltins.h"
#include "llvm/Analysis/TargetFolder.h"
#include "llvm/Analysis/TargetLibraryInfo.h"
#include "llvm/ADT/SmallSet.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/IntrinsicInst.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Metadata.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/DiagnosticInfo.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm-c/Core.h"
#include "llvm/Pass.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <cxxabi.h>

using namespace llvm;

// Implements helper functions for manipulating debugging
// information that is sent to the runtime for races
// reporting of EmbedSanitizer.
namespace EmbedSanitizer {

/**
 * Demangles names of attributes, functions, etc
 */
StringRef demangleName(StringRef name) {
  int status = -1;
  std::string name_(name.str());
  char* d =abi::__cxa_demangle(name_.c_str(), nullptr, nullptr, &status);
  if(! status) {
    StringRef ab(d);
    return ab;
  }
  return name;
}

  /**
   * Returns the name of the file which an instruction belongs to.
   */
  Value* getFileName(Instruction* I) {

    StringRef name = "Unknown";

    // get debug information to retrieve file name
    const DebugLoc &location = I->getDebugLoc();
    if( location ) {
      auto *Scope = cast<DIScope>( location->getScope() );
      if(Scope)
         name = Scope->getFilename();
    }

    Function* F = I->getFunction();
    IRBuilder<> IRB(F->getEntryBlock().getFirstNonPHI());

    return IRB.CreateGlobalStringPtr(name, "fName");
  }

/**
 * Returns name of the function under instrumentation.
 * The name is retrieved later at runtime.
 */
Value* getFuncName(Function & F) {
  StringRef name = demangleName(F.getName());
  auto idx = name.find('(');
  if(idx != StringRef::npos)
    name = name.substr(0, idx);

  IRBuilder<> IRB(F.getEntryBlock().getFirstNonPHI());
  return IRB.CreateGlobalStringPtr(name, "func_name");
}

  /**
   * Returns the name of the memory location involved.
   * By object, this refers to the name of the variable.
   */
  Value * getObjectName(Value *V, Instruction* I, const DataLayout &DL) {
    Value* obj = GetUnderlyingObject(V, DL);

    Function* F = I->getFunction();
    IRBuilder<> IRB(F->getEntryBlock().getFirstNonPHI());

    if( !obj )
      return IRB.CreateGlobalStringPtr("unknown", "variable");

    return IRB.CreateGlobalStringPtr(obj->getName(), "variable");
  }

  /**
   * Retrieves the line number of the instruction
   * being instrumented.
   */
  Value* getLineNumber(Instruction* I) {

    if (auto Loc = I->getDebugLoc()) { // Here I is an LLVM instruction
      IRBuilder<> IRB(I);
      unsigned Line = Loc->getLine();
      return ConstantInt::get(Type::getInt32Ty(I->getContext()), Line);
    }
    else {
      Constant* zero = ConstantInt::get(Type::getInt32Ty(I->getContext()), 0);
      return zero;
    }
  }

} // EmbedSanitizer
