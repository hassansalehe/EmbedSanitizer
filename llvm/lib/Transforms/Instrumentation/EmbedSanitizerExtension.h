//===-- Extension to ThreadSanitizer.cpp - detecting races, Embeded ARM --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// (c) 2017 - 2021  Hassan Salehe Matar, Koc University
//            Email: hassansalehe@gmail.com
//
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

// Implements some of the instrumentation callbacks for
// synchronization events of a program in ThreadSanitizer.
// Particularly, it extends ThreadSanitizer to serve the
// purpose of EmbedSanitizer: race detection for 32-bit ARM.
namespace EmbedSanitizer {

  /**
   * Check if the call instruction calls one of the
   * synchronization functions (eg. pthread_create,
   * pthread_join, pthread_mutex_lock, etc) and inserts
   * race detection callbacks to capture the HB dependencies.
   */
  void InstrIfSynchronization(llvm::Instruction & Inst) {

    llvm::CallInst *CI = llvm::dyn_cast<llvm::CallInst>(&Inst);
    if ( !CI ) return; // return if inconsistence (null)

    llvm::Function *F = CI->getCalledFunction();
    if ( !F ) return; // return if inconsistent function (null)

    llvm::StringRef name = F->getName();
    llvm::IRBuilder<> IRB(&Inst);
    llvm::Module *M = Inst.getModule();

    if (name.startswith("pthread_create")) {

      // get pointer to child id
      llvm::Value *threadIdPtr = CI->getArgOperand(0);

      // I struggled for 2 hours. Big-up to this SO question:
      // http://stackoverflow.com/q/33327097/1952879
      IRB.SetInsertPoint( Inst.getNextNode() );

      // Create callback function
      llvm::Function *tsan_create = checkSanitizerInterfaceFunction(
          M->getOrInsertFunction(
              "__tsan_thread_create", IRB.getVoidTy(),
              IRB.getInt8PtrTy(), nullptr));

      // insert the callback function
      IRB.CreateCall(tsan_create, {threadIdPtr} );
    } else if (name.startswith("pthread_join")) {

      // get pointer to child id
      llvm::Value *childThreadIdAddr = CI->getArgOperand(0);

      IRB.SetInsertPoint(Inst.getNextNode());

      // Create callback function
      llvm::Function *tsan_join = checkSanitizerInterfaceFunction(
          M->getOrInsertFunction(
              "__tsan_thread_join", IRB.getVoidTy(),
              IRB.getInt8PtrTy(), nullptr));

      // insert the callback function
      IRB.CreateCall(tsan_join, {childThreadIdAddr} );
    } else if (name.startswith("pthread_mutex_lock")) {

      llvm::Value * lockAddr = CI->getArgOperand(0); // lock pointer
      IRB.SetInsertPoint(Inst.getNextNode());

      // Create callback function
      llvm::Function *tsan_lock = checkSanitizerInterfaceFunction(
          M->getOrInsertFunction(
              "__tsan_thread_lock", IRB.getVoidTy(),
              IRB.getInt8PtrTy(), nullptr));

      // insert the callback function
      IRB.CreateCall(tsan_lock, {lockAddr} );
    } else if (name.startswith("pthread_mutex_unlock")) {

      llvm::Value * lockAddr = CI->getArgOperand(0); // lock pointer

      // Create callback function
      llvm::Function * tsan_unlock = checkSanitizerInterfaceFunction(
          M->getOrInsertFunction(
              "__tsan_thread_unlock",
              IRB.getVoidTy(),
              IRB.getInt8PtrTy(), nullptr));

      // insert the callback function
      IRB.CreateCall(tsan_unlock, {lockAddr} );
    } // end if
  } // end function
} // end namespace
