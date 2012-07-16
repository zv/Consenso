//===-- ConsensIS 
//===-- ConsensISTargetMachine.h - Define TargetMachine for ConsensIS ---*- C++ -*-===//
//
// This file is distributed under the MIT License.
//
// Zephyr Pellerin (zv)
// Jessy Exum      (diamondman)
//
// Pretty much no rights reserved (2012)
//
//===----------------------------------------------------------------------===//
//
// This file declares the ConsensIS specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef CONSENSISTARGETMACHINE_H
#define CONSENSISTARGETMACHINE_H

#include "ConsensISInstrInfo.h"
#include "ConsensISISelLowering.h"
#include "ConsensISFrameLowering.h"
#include "ConsensISSelectionDAGInfo.h"
#include "ConsensISSubtarget.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Target/TargetFrameLowering.h"

namespace llvm {

class ConsensISTargetMachine : public LLVMTargetMachine {
  ConsensISSubtarget Subtarget;
  const TargetData DataLayout;       // Calculates type size & alignment
  ConsensISInstrInfo InstrInfo;
  ConsensISTargetLowering TLInfo;
  ConsensISSelectionDAGInfo TSInfo;
  ConsensISFrameLowering FrameLowering;
public:
  ConsensISTargetMachine(const Target &T, StringRef TT,
                     StringRef CPU, StringRef FS, const TargetOptions &Options,
                     Reloc::Model RM, CodeModel::Model CM,
                     CodeGenOpt::Level OL, bool is64bit);

  virtual const ConsensISInstrInfo *getInstrInfo() const { return &InstrInfo; }
  virtual const TargetFrameLowering  *getFrameLowering() const {
    return &FrameLowering;
  }
  virtual const ConsensISSubtarget   *getSubtargetImpl() const{ return &Subtarget; }
  virtual const ConsensISRegisterInfo *getRegisterInfo() const {
    return &InstrInfo.getRegisterInfo();
  }
  virtual const ConsensISTargetLowering* getTargetLowering() const {
    return &TLInfo;
  }
  virtual const ConsensISSelectionDAGInfo* getSelectionDAGInfo() const {
    return &TSInfo;
  }
  virtual const TargetData       *getTargetData() const { return &DataLayout; }

  // Pass Pipeline Configuration
  virtual TargetPassConfig *createPassConfig(PassManagerBase &PM);
};

/// ConsensISV8TargetMachine - ConsensIS 32-bit target machine
///
class ConsensISV8TargetMachine : public ConsensISTargetMachine {
  virtual void anchor();
public:
  ConsensISV8TargetMachine(const Target &T, StringRef TT,
                       StringRef CPU, StringRef FS,
                       const TargetOptions &Options,
                       Reloc::Model RM, CodeModel::Model CM,
                       CodeGenOpt::Level OL);
};

} // end namespace llvm

#endif
