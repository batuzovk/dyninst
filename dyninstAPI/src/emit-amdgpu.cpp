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

#include "emit-amdgpu.h"
#include <stddef.h>

// typedef uint8_t codeBuf_t;

namespace amdgpu_on_x86 {
namespace Vega {
using namespace Dyninst;

uint32_t getMaskSop2(SOP2_ContentKind k) {
  switch (k) {
  case Encoding:
    return 0b11000000000000000000000000000000;
  case Opcode:
    return 0b00111111100000000000000000000000;
  case Dst:
    return 0b00000000011111110000000000000000;
  case Src1:
    return 0b00000000000000001111111100000000;
  case Src0:
    return 0b00000000000000000000000011111111;
  default:
    assert(false && "not valid SOP2 content kind");
  }
}

void setEncodingSop2(uint32_t rawInst) {
  uint32_t mask = getMaskSop2(SOP2_ContentKind::Encoding);
  rawInst = (rawInst & ~mask) | (2 & mask);
}

void setOpcodeSop2(uint32_t value, uint32_t rawInst) {
  uint32_t mask = getMaskSop2(SOP2_ContentKind::Opcode);
  rawInst = (rawInst & ~mask) | (value & mask);
}

void setDstSop2(uint32_t value, uint32_t rawInst) {
  uint32_t mask = getMaskSop2(SOP2_ContentKind::Dst);
  rawInst = (rawInst & ~mask) | (value & mask);
}
void setSrc1Sop2(uint32_t value, uint32_t rawInst) {
  uint32_t mask = getMaskSop2(SOP2_ContentKind::Src1);
  rawInst = (rawInst & ~mask) | (value & mask);
}
void setSrc0Sop2(uint32_t value, uint32_t rawInst) {
  uint32_t mask = getMaskSop2(SOP2_ContentKind::Src0);
  rawInst = (rawInst & ~mask) | (value & mask);
}

void emitSOP2(SOP2_Opcode opcode, Register dest, Register src1, Register src0,
              codeGen &gen) {
  uint32_t newRawInst = 0;
  setEncodingSop2(newRawInst);
  setOpcodeSop2(opcode, newRawInst);
  setDstSop2(dest, newRawInst);
  setSrc1Sop2(src1, newRawInst);
  setSrc0Sop2(src0, newRawInst);

  uint32_t *rawInstBuffer = (uint32_t *)gen.cur_ptr();
  append_memory_as</*PointerType = */ uint32_t, /*DataType = */ uint32_t>(
      rawInstBuffer, newRawInst);
  gen.update((uint8_t *)rawInstBuffer);
}
} // namespace Vega
} // namespace amdgpu_on_x86

void EmitterAMDGPUVega::emitOp(unsigned opcode, Register dest, Register src1,
                               Register src2, codeGen &gen) {
  amdgpu_on_x86::Vega::emitSOP2(amdgpu_on_x86::Vega::SOP2_Opcode(opcode), dest,
                                src2, src1, gen);
}
