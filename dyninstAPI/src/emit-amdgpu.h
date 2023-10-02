/*
 * See the dyninst/COPYRIGHT file for copyright information.
 *
 * We provide the Paradyn Tools (below described as "Paradyn")
 * on an AS IS basis, and do not warrant its validity or performance.
 * We reserve the right to update, modify, or discontinue this
 * software at any time.  We shall have no obligation to supply such
 * updates or modifications or any other form of support to you.
 *
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef INST_AMDGPU_H
#define INST_AMDGPU_H

#include "dyninstAPI/src/emitter.h"
#include "dyninstAPI/src/registerSpace.h"

#include <assert.h>

class codeGen;

//
// Codegen functions for AMDGPU Vega ISA
//
// Misc notes on the codebase:
// - The type Register is actually an unsigned int typedef'd as Register
// - codeBuf_t is unsigned char for x86
//
//
namespace amdgpu_on_x86 {

namespace Vega {
// TODO : Map scalar regs to abstract registers?
enum ScalarRegs {
  SGPR_0 = 0,
  SGPR_1,
  SGPR_2,
  SGPR_3,
  SGPR_4,
  SGPR_5,
  SGPR_6,
  SGPR_7,
  SGPR_8,
  SGPR_9,
  SGPR_10,
  SGPR_11,
  SGPR_12,
  SGPR_13,
  SGPR_14,
  SGPR_15,
  SGPR_16,
  SGPR_17,
  SGPR_18,
  SGPR_19,
  SGPR_20,
  SGPR_21,
  SGPR_22,
  SGPR_23,
  SGPR_24,
  SGPR_25,
  SGPR_26,
  SGPR_27,
  SGPR_28,
  SGPR_29,
  SGPR_30,
  SGPR_31,
  SGPR_32,
  SGPR_33,
  SGPR_34,
  SGPR_35,
  SGPR_36,
  SGPR_37,
  SGPR_38,
  SGPR_39,
  SGPR_40,
  SGPR_41,
  SGPR_42,
  SGPR_43,
  SGPR_44,
  SGPR_45,
  SGPR_46,
  SGPR_47,
  SGPR_48,
  SGPR_49,
  SGPR_50,
  SGPR_51,
  SGPR_52,
  SGPR_53,
  SGPR_54,
  SGPR_55,
  SGPR_56,
  SGPR_57,
  SGPR_58,
  SGPR_59,
  SGPR_60,
  SGPR_61,
  SGPR_62,
  SGPR_63,
  SGPR_64,
  SGPR_65,
  SGPR_66,
  SGPR_67,
  SGPR_68,
  SGPR_69,
  SGPR_70,
  SGPR_71,
  SGPR_72,
  SGPR_73,
  SGPR_74,
  SGPR_75,
  SGPR_76,
  SGPR_77,
  SGPR_78,
  SGPR_79,
  SGPR_80,
  SGPR_81,
  SGPR_82,
  SGPR_83,
  SGPR_84,
  SGPR_85,
  SGPR_86,
  SGPR_87,
  SGPR_88,
  SGPR_89,
  SGPR_90,
  SGPR_91,
  SGPR_92,
  SGPR_93,
  SGPR_94,
  SGPR_95,
  SGPR_96,
  SGPR_97,
  SGPR_98,
  SGPR_99,
  SGPR_100,
  SGPR_101,
}; // ScalarRegs

// SOP2 instruction format in memory: [encoding] [opcode] [sdst] [ssrc1] [ssrc0]
//                   bits (total 32):   2(10)       7       7       8       8
// This enum contains particular scalar instructions of interest.
// Extend it later as needed.
enum SOP2_Opcode {
  S_ADD_U32,
  S_SUB_U32,
  S_ADD_I32,
  S_SUB_I32,
  S_ADDC_U32,
  S_SUBB_U32,
  S_MIN_I32,
  S_MIN_U32,
  S_MAX_I32,
  S_MAX_U32,
  S_MUL_I32 = 36
};

enum SOP2_ContentKind { Encoding, Opcode, Dst, Src1, Src0 };

uint32_t getMaskSop2(SOP2_ContentKind k);
void setEncodingSop2(uint32_t &rawInst);
void setOpcodeSop2(uint32_t value, uint32_t &rawInst);
void setDstSop2(uint32_t value, uint32_t &rawInst);
void setSrc1Sop2(uint32_t value, uint32_t &rawInst);
void setSrc0Sop2(uint32_t value, uint32_t &rawInst);

void emitSop2(SOP2_Opcode opcode, Register dest, Register src1, Register src0,
              codeGen &gen);

} // namespace Vega

} // namespace amdgpu_on_x86

class EmitterAMDGPUVega : public Emitter {
  // So far nothing specific to add here;
  codeBufIndex_t emitIf(Register expr_reg, Register target, RegControl rc,
                        codeGen &gen) {
    printf("emitIf not implemented yet\n");
  }
  void emitOp(unsigned opcode, Register dest, Register src1, Register src2,
              codeGen &gen);
  void emitOpImm(unsigned opcode1, unsigned opcode2, Register dest,
                 Register src1, RegValue src2imm, codeGen &gen) {
    printf("not implemented yet\n");
  }
  void emitRelOp(unsigned op, Register dest, Register src1, Register src2,
                 codeGen &gen, bool s) {
    printf("emitRelOp not implemented yet\n");
  }
  void emitRelOpImm(unsigned op, Register dest, Register src1, RegValue src2imm,
                    codeGen &gen, bool s) {
    printf("emitRelOpImm not implemented yet\n");
  }
  void emitDiv(Register dest, Register src1, Register src2, codeGen &gen,
               bool s) {
    printf("emitDiv not implemented yet\n");
  }
  void emitTimesImm(Register dest, Register src1, RegValue src2imm,
                    codeGen &gen) {
    printf("emitTimesImm not implemented yet\n");
  }
  void emitDivImm(Register dest, Register src1, RegValue src2imm, codeGen &gen,
                  bool s) {
    printf("emitDivImm not implemented yet\n");
  }
  void emitLoad(Register dest, Address addr, int size, codeGen &gen) {
    printf("emitLoad not implemented yet\n");
  }
  void emitLoadConst(Register dest, Address imm, codeGen &gen) {
    printf("emitLoadConst not implemented yet\n");
  }
  void emitLoadIndir(Register dest, Register addr_reg, int size, codeGen &gen) {
    printf("emitLoadIndir not implemented yet\n");
  }
  bool emitCallRelative(Register, Address, Register, codeGen &) {
    printf("emitCallRelative not implemented yet\n");
  }
  bool emitLoadRelative(Register dest, Address offset, Register base, int size,
                        codeGen &gen) {
    printf("emitLoadRelative not implemented yet\n");
  }
  void emitLoadShared(opCode op, Register dest, const image_variable *var,
                      bool is_local, int size, codeGen &gen, Address offset) {
    printf("emitLoadShared not implemented yet\n");
  }

  void emitLoadFrameAddr(Register dest, Address offset, codeGen &gen) {
    printf("emitLoadFrameAddr not implemented yet\n");
  }

  // These implicitly use the stored original/non-inst value
  void emitLoadOrigFrameRelative(Register dest, Address offset, codeGen &gen) {
    printf("emitLoadOrigFrameRelative not implemented yet\n");
  }
  void emitLoadOrigRegRelative(Register dest, Address offset, Register base,
                               codeGen &gen, bool store) {
    printf("emitLoadOrigRegRelative not implemented yet\n");
  }
  void emitLoadOrigRegister(Address register_num, Register dest, codeGen &gen) {
    printf("emitLoadOrigRegister not implemented yet\n");
  }

  void emitStoreOrigRegister(Address register_num, Register dest,
                             codeGen &gen) {
    printf("emitStoreOrigRegister not implemented yet\n");
  }

  void emitStore(Address addr, Register src, int size, codeGen &gen) {
    printf("emitStore not implemented yet\n");
  }
  void emitStoreIndir(Register addr_reg, Register src, int size, codeGen &gen) {
    printf("emitStoreIndir not implemented yet\n");
  }
  void emitStoreFrameRelative(Address offset, Register src, Register scratch,
                              int size, codeGen &gen) {
    printf("emitStoreFrameRelative not implemented yet\n");
  }
  void emitStoreRelative(Register source, Address offset, Register base,
                         int size, codeGen &gen) {
    printf("emitStoreRelative not implemented yet\n");
  }
  void emitStoreShared(Register source, const image_variable *var,
                       bool is_local, int size, codeGen &gen) {
    printf("emitStoreShared not implemented yet\n");
  }

  bool emitMoveRegToReg(Register src, Register dest, codeGen &gen) {
    printf("emitMoveRegToReg not implemented yet\n");
  }
  bool emitMoveRegToReg(registerSlot *src, registerSlot *dest, codeGen &gen) {
    printf("emitMoveRegToReg -- slot not implemented yet\n");
  }

  Register emitCall(opCode op, codeGen &gen,
                    const std::vector<AstNodePtr> &operands, bool noCost,
                    func_instance *callee) {
    printf("emitCall not implemented yet\n");
  }

  void emitGetRetVal(Register dest, bool addr_of, codeGen &gen) {
    printf("emitGetRetVal not implemented yet\n");
  }
  void emitGetRetAddr(Register dest, codeGen &gen) {
    printf("emitGetRetAddr not implemented yet\n");
  }
  void emitGetParam(Register dest, Register param_num, instPoint::Type pt_type,
                    opCode op, bool addr_of, codeGen &gen) {
    printf("emitGetParam not implemented yet\n");
  }
  void emitASload(int ra, int rb, int sc, long imm, Register dest,
                  int stackShift, codeGen &gen) {
    printf("emitASload not implemented yet\n");
  }
  void emitCSload(int ra, int rb, int sc, long imm, Register dest,
                  codeGen &gen) {
    printf("emitCSload not implemented yet\n");
  }
  void emitPushFlags(codeGen &gen) {
    printf("emitPushFlags not implemented yet\n");
  }
  void emitRestoreFlags(codeGen &gen, unsigned offset) {
    printf("emitRestoreFlags not implemented yet\n");
  }
  // Built-in offset...
  void emitRestoreFlagsFromStackSlot(codeGen &gen) {
    printf("emitRestoreFlagsFromStackSlot not implemented yet\n");
  }
  bool emitBTSaves(baseTramp *bt, codeGen &gen) {
    printf("emitBTSaves not implemented yet\n");
  }
  bool emitBTRestores(baseTramp *bt, codeGen &gen) {
    printf("emitBTRestores not implemented yet\n");
  }
  void emitStoreImm(Address addr, int imm, codeGen &gen, bool noCost) {
    printf("emitStoreImm not implemented yet\n");
  }
  void emitAddSignedImm(Address addr, int imm, codeGen &gen, bool noCost) {
    printf("emitAddSignedImm not implemented yet\n");
  }
  bool emitPush(codeGen &, Register) {
    printf("emitPush not implemented yet\n");
  }
  bool emitPop(codeGen &, Register) { printf("emitPop not implemented yet\n"); }
  bool emitAdjustStackPointer(int index, codeGen &gen) {
    printf("emitAdjustStackPointer not implemented yet\n");
  }

  bool clobberAllFuncCall(registerSpace *rs, func_instance *callee) {
    printf("clobberAllFuncCall not implemented yet\n");
  }
};

#endif
