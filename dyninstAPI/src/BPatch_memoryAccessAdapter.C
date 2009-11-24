/*
 * Copyright (c) 1996-2009 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
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

#include "BPatch_memoryAccessAdapter.h"
#include "BPatch_memoryAccess_NP.h"
#include "Instruction.h"
#include "arch.h"
using namespace Dyninst;
using namespace InstructionAPI;


BPatch_memoryAccess* BPatch_memoryAccessAdapter::convert(Instruction::Ptr insn,
							 Address current, bool is64)
{
  static unsigned int log2[] = { 0, 0, 1, 1, 2, 2, 2, 2, 3 };
    
  // TODO 16-bit registers
    
  int nac = 0;
    
  ia32_memacc mac[3];
  ia32_condition cnd;
  ia32_instruction i(mac, &cnd);
    
  const unsigned char* addr = reinterpret_cast<const unsigned char*>(insn->ptr());
  BPatch_memoryAccess* bmap = BPatch_memoryAccess::none;

  ia32_set_mode_64(is64);
  
  ia32_decode(IA32_DECODE_MEMACCESS|IA32_DECODE_CONDITION, addr, i);
  
  bool first = true;

  for(int j=0; j<3; ++j) {
    ia32_memacc& mac = const_cast<ia32_memacc&>(i.getMac(j));
    const ia32_condition& cond = i.getCond();
    int bmapcond = cond.is ? cond.tttn : -1;
    if(mac.is) {

      // here, we can set the correct address for RIP-relative addressing
      if (mac.regs[0] == mRIP) {
	mac.imm = current + insn->size() + mac.imm;
      }

      if(first) {
        if(mac.prefetch) {
          if(mac.prefetchlvl > 0) // Intel
            bmap = new BPatch_memoryAccess(NULL, current,
					   false, false,
                                           mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                           0, -1, -1, 0,
                                           bmapcond, false, mac.prefetchlvl);
          else // AMD
	    bmap = new BPatch_memoryAccess(NULL, current,
					   false, false,
                                           mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                           0, -1, -1, 0,
                                           bmapcond, false, mac.prefetchstt + IA32AMDprefetch);
        }
        else switch(mac.sizehack) { // translation to pseudoregisters
        case 0:
	  bmap = new BPatch_memoryAccess(NULL, current,
					 mac.read, mac.write,
                                         mac.size, mac.imm, mac.regs[0], mac.regs[1], mac.scale, 
                                         bmapcond, mac.nt);
          break;
        case shREP: // use ECX register to compute final size as mac.size * ECX
	  bmap = new BPatch_memoryAccess(NULL, current,
                                         mac.read, mac.write,
                                         mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                         0, -1, 1 , log2[mac.size],
                                         bmapcond, false);
          break;
        case shREPESCAS:
	  bmap = new BPatch_memoryAccess(NULL, current,
					 mac.read, mac.write,
                                         mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                         0, -1, IA32_ESCAS, log2[mac.size],
                                         bmapcond, false);
          break;
        case shREPNESCAS:
	  bmap = new BPatch_memoryAccess(NULL, current,
					 mac.read, mac.write,
                                         mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                         0, -1, IA32_NESCAS, log2[mac.size],
                                         bmapcond, false);
          break;
        case shREPECMPS:
	  bmap = new BPatch_memoryAccess(NULL, current,
					 mac.read, mac.write,
                                         mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                         0, -1, IA32_ECMPS, log2[mac.size],
                                         bmapcond, false);
          break;
        case shREPNECMPS:
	  bmap = new BPatch_memoryAccess(NULL, current,
					 mac.read, mac.write,
                                         mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                                         0, -1, IA32_NECMPS, log2[mac.size],
                                         bmapcond, false);
          break;
        default:
          assert(!"Unknown size hack");
        }
        first = false;
      }
      else
        switch(mac.sizehack) { // translation to pseudoregisters
        case 0:
          bmap->set2nd(mac.read, mac.write, mac.size, mac.imm,
                       mac.regs[0], mac.regs[1], mac.scale);
          break;
        case shREP: // use ECX register to compute final size as mac.size * ECX
          bmap->set2nd(mac.read, mac.write,
                       mac.imm, mac.regs[0], mac.regs[1], mac.scale,
		       0, -1, 1 , log2[mac.size],
                       bmapcond, false);
          break;
        case shREPESCAS:
        case shREPNESCAS:
          assert(!"Cannot happen");
          break;
        case shREPECMPS:
          bmap->set2nd(mac.read, mac.write,
                       mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                       0, -1, IA32_ECMPS, log2[mac.size],
                       bmapcond, false);
          break;
        case shREPNECMPS:
          //fprintf(stderr, "In set2nd[shREPNECMPS]!!!\n");
          bmap->set2nd(mac.read, mac.write,
                       mac.imm, mac.regs[0], mac.regs[1], mac.scale,
                       0, -1, IA32_NECMPS, log2[mac.size],
                       bmapcond, false);
          break;
        default:
          assert(!"Unknown size hack");
        }
      ++nac;
    }
  }
  assert(nac < 3);
  return bmap;
}


void BPatch_memoryAccessAdapter::visit(BinaryFunction* b)
{
}


void BPatch_memoryAccessAdapter::visit(Dereference* d)
{
}

void BPatch_memoryAccessAdapter::visit(RegisterAST* r)
{
}

void BPatch_memoryAccessAdapter::visit(Immediate* i)
{
}
