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

#include "RegisterConversion.h"
#include "registerSpace.h"

#include <map>
#include <boost/assign/list_of.hpp>
#include "Register.h"
#include "dyn_regs.h"
#include "registerSpace.h"

using namespace Dyninst;
using namespace Dyninst::InstructionAPI;
using namespace std;
using namespace boost::assign;

// AMDGPU has 32-bit registers, and we use a pair of registers for 64-bit values.
// regToMachReg64 is here only to prevent linker error because parts of the x86
// code generator refer to it. This is a temporary hack until the conditional
// compilation and legacy cruft goes away.
multimap<Register, MachRegister> regToMachReg64 = map_list_of (0, 0);

multimap<Register, MachRegister> regToMachReg32 = map_list_of
  (registerSpace::sgpr0,  amdgpu_vega::sgpr0)
  (registerSpace::sgpr1,  amdgpu_vega::sgpr1)
  (registerSpace::sgpr2,  amdgpu_vega::sgpr2)
  (registerSpace::sgpr3,  amdgpu_vega::sgpr3)
  (registerSpace::sgpr4,  amdgpu_vega::sgpr4)
  (registerSpace::sgpr5,  amdgpu_vega::sgpr5)
  (registerSpace::sgpr6,  amdgpu_vega::sgpr6)
  (registerSpace::sgpr7,  amdgpu_vega::sgpr7)
  (registerSpace::sgpr8,  amdgpu_vega::sgpr8)
  (registerSpace::sgpr9,  amdgpu_vega::sgpr9)
  (registerSpace::sgpr10, amdgpu_vega::sgpr10)
  (registerSpace::sgpr11, amdgpu_vega::sgpr11)
  (registerSpace::sgpr12, amdgpu_vega::sgpr12)
  (registerSpace::sgpr13, amdgpu_vega::sgpr13)
  (registerSpace::sgpr14, amdgpu_vega::sgpr14)
  (registerSpace::sgpr15, amdgpu_vega::sgpr15)
  (registerSpace::sgpr16, amdgpu_vega::sgpr16)
  (registerSpace::sgpr17, amdgpu_vega::sgpr17)
  (registerSpace::sgpr18, amdgpu_vega::sgpr18)
  (registerSpace::sgpr19, amdgpu_vega::sgpr19)
  (registerSpace::sgpr20, amdgpu_vega::sgpr20)
  (registerSpace::sgpr21, amdgpu_vega::sgpr21)
  (registerSpace::sgpr22, amdgpu_vega::sgpr22)
  (registerSpace::sgpr23, amdgpu_vega::sgpr23)
  (registerSpace::sgpr24, amdgpu_vega::sgpr24)
  (registerSpace::sgpr25, amdgpu_vega::sgpr25)
  (registerSpace::sgpr26, amdgpu_vega::sgpr26)
  (registerSpace::sgpr27, amdgpu_vega::sgpr27)
  (registerSpace::sgpr28, amdgpu_vega::sgpr28)
  (registerSpace::sgpr29, amdgpu_vega::sgpr29)
  (registerSpace::sgpr30, amdgpu_vega::sgpr30)
  (registerSpace::sgpr31, amdgpu_vega::sgpr31)
  (registerSpace::sgpr32, amdgpu_vega::sgpr32)
  (registerSpace::sgpr33, amdgpu_vega::sgpr33)
  (registerSpace::sgpr34, amdgpu_vega::sgpr34)
  (registerSpace::sgpr35, amdgpu_vega::sgpr35)
  (registerSpace::sgpr36, amdgpu_vega::sgpr36)
  (registerSpace::sgpr37, amdgpu_vega::sgpr37)
  (registerSpace::sgpr38, amdgpu_vega::sgpr38)
  (registerSpace::sgpr39, amdgpu_vega::sgpr39)
  (registerSpace::sgpr40, amdgpu_vega::sgpr40)
  (registerSpace::sgpr41, amdgpu_vega::sgpr41)
  (registerSpace::sgpr42, amdgpu_vega::sgpr42)
  (registerSpace::sgpr43, amdgpu_vega::sgpr43)
  (registerSpace::sgpr44, amdgpu_vega::sgpr44)
  (registerSpace::sgpr45, amdgpu_vega::sgpr45)
  (registerSpace::sgpr46, amdgpu_vega::sgpr46)
  (registerSpace::sgpr47, amdgpu_vega::sgpr47)
  (registerSpace::sgpr48, amdgpu_vega::sgpr48)
  (registerSpace::sgpr49, amdgpu_vega::sgpr49)
  (registerSpace::sgpr50, amdgpu_vega::sgpr50)
  (registerSpace::sgpr51, amdgpu_vega::sgpr51)
  (registerSpace::sgpr52, amdgpu_vega::sgpr52)
  (registerSpace::sgpr53, amdgpu_vega::sgpr53)
  (registerSpace::sgpr54, amdgpu_vega::sgpr54)
  (registerSpace::sgpr55, amdgpu_vega::sgpr55)
  (registerSpace::sgpr56, amdgpu_vega::sgpr56)
  (registerSpace::sgpr57, amdgpu_vega::sgpr57)
  (registerSpace::sgpr58, amdgpu_vega::sgpr58)
  (registerSpace::sgpr59, amdgpu_vega::sgpr59)
  (registerSpace::sgpr60, amdgpu_vega::sgpr60)
  (registerSpace::sgpr61, amdgpu_vega::sgpr61)
  (registerSpace::sgpr62, amdgpu_vega::sgpr62)
  (registerSpace::sgpr63, amdgpu_vega::sgpr63)
  (registerSpace::sgpr64, amdgpu_vega::sgpr64)
  (registerSpace::sgpr65, amdgpu_vega::sgpr65)
  (registerSpace::sgpr66, amdgpu_vega::sgpr66)
  (registerSpace::sgpr67, amdgpu_vega::sgpr67)
  (registerSpace::sgpr68, amdgpu_vega::sgpr68)
  (registerSpace::sgpr69, amdgpu_vega::sgpr69)
  (registerSpace::sgpr70, amdgpu_vega::sgpr70)
  (registerSpace::sgpr71, amdgpu_vega::sgpr71)
  (registerSpace::sgpr72, amdgpu_vega::sgpr72)
  (registerSpace::sgpr73, amdgpu_vega::sgpr73)
  (registerSpace::sgpr74, amdgpu_vega::sgpr74)
  (registerSpace::sgpr75, amdgpu_vega::sgpr75)
  (registerSpace::sgpr76, amdgpu_vega::sgpr76)
  (registerSpace::sgpr77, amdgpu_vega::sgpr77)
  (registerSpace::sgpr78, amdgpu_vega::sgpr78)
  (registerSpace::sgpr79, amdgpu_vega::sgpr79)
  (registerSpace::sgpr80, amdgpu_vega::sgpr80)
  (registerSpace::sgpr81, amdgpu_vega::sgpr81)
  (registerSpace::sgpr82, amdgpu_vega::sgpr82)
  (registerSpace::sgpr83, amdgpu_vega::sgpr83)
  (registerSpace::sgpr84, amdgpu_vega::sgpr84)
  (registerSpace::sgpr85, amdgpu_vega::sgpr85)
  (registerSpace::sgpr86, amdgpu_vega::sgpr86)
  (registerSpace::sgpr87, amdgpu_vega::sgpr87)
  (registerSpace::sgpr88, amdgpu_vega::sgpr88)
  (registerSpace::sgpr89, amdgpu_vega::sgpr89)
  (registerSpace::sgpr90, amdgpu_vega::sgpr90)
  (registerSpace::sgpr91, amdgpu_vega::sgpr91)
  (registerSpace::sgpr92, amdgpu_vega::sgpr92)
  (registerSpace::sgpr93, amdgpu_vega::sgpr93)
  (registerSpace::sgpr94, amdgpu_vega::sgpr94)
  (registerSpace::sgpr95, amdgpu_vega::sgpr95)
  (registerSpace::sgpr96, amdgpu_vega::sgpr96)
  (registerSpace::sgpr97, amdgpu_vega::sgpr97)
  (registerSpace::sgpr98, amdgpu_vega::sgpr98)
  (registerSpace::sgpr99, amdgpu_vega::sgpr99)
  (registerSpace::sgpr100, amdgpu_vega::sgpr100)
  (registerSpace::sgpr101, amdgpu_vega::sgpr101)
  (registerSpace::flat_scratch_lo, amdgpu_vega::flat_scratch_lo)
  (registerSpace::flat_scratch_hi, amdgpu_vega::flat_scratch_hi)
  (registerSpace::xnack_mask_lo, amdgpu_vega::xnack_mask_lo)
  (registerSpace::xnack_mask_hi, amdgpu_vega::xnack_mask_hi)
  (registerSpace::vcc_lo, amdgpu_vega::vcc_lo)
  (registerSpace::vcc_hi, amdgpu_vega::vcc_hi)
  (registerSpace::ttmp0,  amdgpu_vega::ttmp0)
  (registerSpace::ttmp1,  amdgpu_vega::ttmp1)
  (registerSpace::ttmp2,  amdgpu_vega::ttmp2)
  (registerSpace::ttmp3,  amdgpu_vega::ttmp3)
  (registerSpace::ttmp4,  amdgpu_vega::ttmp4)
  (registerSpace::ttmp5,  amdgpu_vega::ttmp5)
  (registerSpace::ttmp6,  amdgpu_vega::ttmp6)
  (registerSpace::ttmp7,  amdgpu_vega::ttmp7)
  (registerSpace::ttmp8,  amdgpu_vega::ttmp8)
  (registerSpace::ttmp9,  amdgpu_vega::ttmp9)
  (registerSpace::ttmp10, amdgpu_vega::ttmp10)
  (registerSpace::ttmp11, amdgpu_vega::ttmp11)
  (registerSpace::ttmp12, amdgpu_vega::ttmp12)
  (registerSpace::ttmp13, amdgpu_vega::ttmp13)
  (registerSpace::ttmp14, amdgpu_vega::ttmp14)
  (registerSpace::ttmp15, amdgpu_vega::ttmp15)
  (registerSpace::m0,     amdgpu_vega::m0)
  (registerSpace::exec_lo, amdgpu_vega::exec_lo)
  (registerSpace::exec_hi, amdgpu_vega::exec_hi)

  // now do vector registers
  (registerSpace::vgpr0,  amdgpu_vega::vgpr0)
  (registerSpace::vgpr1,  amdgpu_vega::vgpr1)
  (registerSpace::vgpr2,  amdgpu_vega::vgpr2)
  (registerSpace::vgpr3,  amdgpu_vega::vgpr3)
  (registerSpace::vgpr4,  amdgpu_vega::vgpr4)
  (registerSpace::vgpr5,  amdgpu_vega::vgpr5)
  (registerSpace::vgpr6,  amdgpu_vega::vgpr6)
  (registerSpace::vgpr7,  amdgpu_vega::vgpr7)
  (registerSpace::vgpr8,  amdgpu_vega::vgpr8)
  (registerSpace::vgpr9,  amdgpu_vega::vgpr9)
  (registerSpace::vgpr10, amdgpu_vega::vgpr10)
  (registerSpace::vgpr11, amdgpu_vega::vgpr11)
  (registerSpace::vgpr12, amdgpu_vega::vgpr12)
  (registerSpace::vgpr13, amdgpu_vega::vgpr13)
  (registerSpace::vgpr14, amdgpu_vega::vgpr14)
  (registerSpace::vgpr15, amdgpu_vega::vgpr15)
  (registerSpace::vgpr16, amdgpu_vega::vgpr16)
  (registerSpace::vgpr17, amdgpu_vega::vgpr17)
  (registerSpace::vgpr18, amdgpu_vega::vgpr18)
  (registerSpace::vgpr19, amdgpu_vega::vgpr19)
  (registerSpace::vgpr20, amdgpu_vega::vgpr20)
  (registerSpace::vgpr21, amdgpu_vega::vgpr21)
  (registerSpace::vgpr22, amdgpu_vega::vgpr22)
  (registerSpace::vgpr23, amdgpu_vega::vgpr23)
  (registerSpace::vgpr24, amdgpu_vega::vgpr24)
  (registerSpace::vgpr25, amdgpu_vega::vgpr25)
  (registerSpace::vgpr26, amdgpu_vega::vgpr26)
  (registerSpace::vgpr27, amdgpu_vega::vgpr27)
  (registerSpace::vgpr28, amdgpu_vega::vgpr28)
  (registerSpace::vgpr29, amdgpu_vega::vgpr29)
  (registerSpace::vgpr30, amdgpu_vega::vgpr30)
  (registerSpace::vgpr31, amdgpu_vega::vgpr31)
  (registerSpace::vgpr32, amdgpu_vega::vgpr32)
  (registerSpace::vgpr33, amdgpu_vega::vgpr33)
  (registerSpace::vgpr34, amdgpu_vega::vgpr34)
  (registerSpace::vgpr35, amdgpu_vega::vgpr35)
  (registerSpace::vgpr36, amdgpu_vega::vgpr36)
  (registerSpace::vgpr37, amdgpu_vega::vgpr37)
  (registerSpace::vgpr38, amdgpu_vega::vgpr38)
  (registerSpace::vgpr39, amdgpu_vega::vgpr39)
  (registerSpace::vgpr40, amdgpu_vega::vgpr40)
  (registerSpace::vgpr41, amdgpu_vega::vgpr41)
  (registerSpace::vgpr42, amdgpu_vega::vgpr42)
  (registerSpace::vgpr43, amdgpu_vega::vgpr43)
  (registerSpace::vgpr44, amdgpu_vega::vgpr44)
  (registerSpace::vgpr45, amdgpu_vega::vgpr45)
  (registerSpace::vgpr46, amdgpu_vega::vgpr46)
  (registerSpace::vgpr47, amdgpu_vega::vgpr47)
  (registerSpace::vgpr48, amdgpu_vega::vgpr48)
  (registerSpace::vgpr49, amdgpu_vega::vgpr49)
  (registerSpace::vgpr50, amdgpu_vega::vgpr50)
  (registerSpace::vgpr51, amdgpu_vega::vgpr51)
  (registerSpace::vgpr52, amdgpu_vega::vgpr52)
  (registerSpace::vgpr53, amdgpu_vega::vgpr53)
  (registerSpace::vgpr54, amdgpu_vega::vgpr54)
  (registerSpace::vgpr55, amdgpu_vega::vgpr55)
  (registerSpace::vgpr56, amdgpu_vega::vgpr56)
  (registerSpace::vgpr57, amdgpu_vega::vgpr57)
  (registerSpace::vgpr58, amdgpu_vega::vgpr58)
  (registerSpace::vgpr59, amdgpu_vega::vgpr59)
  (registerSpace::vgpr60, amdgpu_vega::vgpr60)
  (registerSpace::vgpr61, amdgpu_vega::vgpr61)
  (registerSpace::vgpr62, amdgpu_vega::vgpr62)
  (registerSpace::vgpr63, amdgpu_vega::vgpr63)
  (registerSpace::vgpr64, amdgpu_vega::vgpr64)
  (registerSpace::vgpr65, amdgpu_vega::vgpr65)
  (registerSpace::vgpr66, amdgpu_vega::vgpr66)
  (registerSpace::vgpr67, amdgpu_vega::vgpr67)
  (registerSpace::vgpr68, amdgpu_vega::vgpr68)
  (registerSpace::vgpr69, amdgpu_vega::vgpr69)
  (registerSpace::vgpr70, amdgpu_vega::vgpr70)
  (registerSpace::vgpr71, amdgpu_vega::vgpr71)
  (registerSpace::vgpr72, amdgpu_vega::vgpr72)
  (registerSpace::vgpr73, amdgpu_vega::vgpr73)
  (registerSpace::vgpr74, amdgpu_vega::vgpr74)
  (registerSpace::vgpr75, amdgpu_vega::vgpr75)
  (registerSpace::vgpr76, amdgpu_vega::vgpr76)
  (registerSpace::vgpr77, amdgpu_vega::vgpr77)
  (registerSpace::vgpr78, amdgpu_vega::vgpr78)
  (registerSpace::vgpr79, amdgpu_vega::vgpr79)
  (registerSpace::vgpr80, amdgpu_vega::vgpr80)
  (registerSpace::vgpr81, amdgpu_vega::vgpr81)
  (registerSpace::vgpr82, amdgpu_vega::vgpr82)
  (registerSpace::vgpr83, amdgpu_vega::vgpr83)
  (registerSpace::vgpr84, amdgpu_vega::vgpr84)
  (registerSpace::vgpr85, amdgpu_vega::vgpr85)
  (registerSpace::vgpr86, amdgpu_vega::vgpr86)
  (registerSpace::vgpr87, amdgpu_vega::vgpr87)
  (registerSpace::vgpr88, amdgpu_vega::vgpr88)
  (registerSpace::vgpr89, amdgpu_vega::vgpr89)
  (registerSpace::vgpr90, amdgpu_vega::vgpr90)
  (registerSpace::vgpr91, amdgpu_vega::vgpr91)
  (registerSpace::vgpr92, amdgpu_vega::vgpr92)
  (registerSpace::vgpr93, amdgpu_vega::vgpr93)
  (registerSpace::vgpr94, amdgpu_vega::vgpr94)
  (registerSpace::vgpr95, amdgpu_vega::vgpr95)
  (registerSpace::vgpr96, amdgpu_vega::vgpr96)
  (registerSpace::vgpr97, amdgpu_vega::vgpr97)
  (registerSpace::vgpr98, amdgpu_vega::vgpr98)
  (registerSpace::vgpr99, amdgpu_vega::vgpr99)
  (registerSpace::vgpr100, amdgpu_vega::vgpr100)
  (registerSpace::vgpr101, amdgpu_vega::vgpr101)
  (registerSpace::vgpr102, amdgpu_vega::vgpr102)
  (registerSpace::vgpr103, amdgpu_vega::vgpr103)
  (registerSpace::vgpr104, amdgpu_vega::vgpr104)
  (registerSpace::vgpr105, amdgpu_vega::vgpr105)
  (registerSpace::vgpr106, amdgpu_vega::vgpr106)
  (registerSpace::vgpr107, amdgpu_vega::vgpr107)
  (registerSpace::vgpr108, amdgpu_vega::vgpr108)
  (registerSpace::vgpr109, amdgpu_vega::vgpr109)
  (registerSpace::vgpr110, amdgpu_vega::vgpr110)
  (registerSpace::vgpr111, amdgpu_vega::vgpr111)
  (registerSpace::vgpr112, amdgpu_vega::vgpr112)
  (registerSpace::vgpr113, amdgpu_vega::vgpr113)
  (registerSpace::vgpr114, amdgpu_vega::vgpr114)
  (registerSpace::vgpr115, amdgpu_vega::vgpr115)
  (registerSpace::vgpr116, amdgpu_vega::vgpr116)
  (registerSpace::vgpr117, amdgpu_vega::vgpr117)
  (registerSpace::vgpr118, amdgpu_vega::vgpr118)
  (registerSpace::vgpr119, amdgpu_vega::vgpr119)
  (registerSpace::vgpr120, amdgpu_vega::vgpr120)
  (registerSpace::vgpr121, amdgpu_vega::vgpr121)
  (registerSpace::vgpr122, amdgpu_vega::vgpr122)
  (registerSpace::vgpr123, amdgpu_vega::vgpr123)
  (registerSpace::vgpr124, amdgpu_vega::vgpr124)
  (registerSpace::vgpr125, amdgpu_vega::vgpr125)
  (registerSpace::vgpr126, amdgpu_vega::vgpr126)
  (registerSpace::vgpr127, amdgpu_vega::vgpr127)
  (registerSpace::vgpr128, amdgpu_vega::vgpr128)
  (registerSpace::vgpr129, amdgpu_vega::vgpr129)
  (registerSpace::vgpr130, amdgpu_vega::vgpr130)
  (registerSpace::vgpr131, amdgpu_vega::vgpr131)
  (registerSpace::vgpr132, amdgpu_vega::vgpr132)
  (registerSpace::vgpr133, amdgpu_vega::vgpr133)
  (registerSpace::vgpr134, amdgpu_vega::vgpr134)
  (registerSpace::vgpr135, amdgpu_vega::vgpr135)
  (registerSpace::vgpr136, amdgpu_vega::vgpr136)
  (registerSpace::vgpr137, amdgpu_vega::vgpr137)
  (registerSpace::vgpr138, amdgpu_vega::vgpr138)
  (registerSpace::vgpr139, amdgpu_vega::vgpr139)
  (registerSpace::vgpr140, amdgpu_vega::vgpr140)
  (registerSpace::vgpr141, amdgpu_vega::vgpr141)
  (registerSpace::vgpr142, amdgpu_vega::vgpr142)
  (registerSpace::vgpr143, amdgpu_vega::vgpr143)
  (registerSpace::vgpr144, amdgpu_vega::vgpr144)
  (registerSpace::vgpr145, amdgpu_vega::vgpr145)
  (registerSpace::vgpr146, amdgpu_vega::vgpr146)
  (registerSpace::vgpr147, amdgpu_vega::vgpr147)
  (registerSpace::vgpr148, amdgpu_vega::vgpr148)
  (registerSpace::vgpr149, amdgpu_vega::vgpr149)
  (registerSpace::vgpr150, amdgpu_vega::vgpr150)
  (registerSpace::vgpr151, amdgpu_vega::vgpr151)
  (registerSpace::vgpr152, amdgpu_vega::vgpr152)
  (registerSpace::vgpr153, amdgpu_vega::vgpr153)
  (registerSpace::vgpr154, amdgpu_vega::vgpr154)
  (registerSpace::vgpr155, amdgpu_vega::vgpr155)
  (registerSpace::vgpr156, amdgpu_vega::vgpr156)
  (registerSpace::vgpr157, amdgpu_vega::vgpr157)
  (registerSpace::vgpr158, amdgpu_vega::vgpr158)
  (registerSpace::vgpr159, amdgpu_vega::vgpr159)
  (registerSpace::vgpr160, amdgpu_vega::vgpr160)
  (registerSpace::vgpr161, amdgpu_vega::vgpr161)
  (registerSpace::vgpr162, amdgpu_vega::vgpr162)
  (registerSpace::vgpr163, amdgpu_vega::vgpr163)
  (registerSpace::vgpr164, amdgpu_vega::vgpr164)
  (registerSpace::vgpr165, amdgpu_vega::vgpr165)
  (registerSpace::vgpr166, amdgpu_vega::vgpr166)
  (registerSpace::vgpr167, amdgpu_vega::vgpr167)
  (registerSpace::vgpr168, amdgpu_vega::vgpr168)
  (registerSpace::vgpr169, amdgpu_vega::vgpr169)
  (registerSpace::vgpr170, amdgpu_vega::vgpr170)
  (registerSpace::vgpr171, amdgpu_vega::vgpr171)
  (registerSpace::vgpr172, amdgpu_vega::vgpr172)
  (registerSpace::vgpr173, amdgpu_vega::vgpr173)
  (registerSpace::vgpr174, amdgpu_vega::vgpr174)
  (registerSpace::vgpr175, amdgpu_vega::vgpr175)
  (registerSpace::vgpr176, amdgpu_vega::vgpr176)
  (registerSpace::vgpr177, amdgpu_vega::vgpr177)
  (registerSpace::vgpr178, amdgpu_vega::vgpr178)
  (registerSpace::vgpr179, amdgpu_vega::vgpr179)
  (registerSpace::vgpr180, amdgpu_vega::vgpr180)
  (registerSpace::vgpr181, amdgpu_vega::vgpr181)
  (registerSpace::vgpr182, amdgpu_vega::vgpr182)
  (registerSpace::vgpr183, amdgpu_vega::vgpr183)
  (registerSpace::vgpr184, amdgpu_vega::vgpr184)
  (registerSpace::vgpr185, amdgpu_vega::vgpr185)
  (registerSpace::vgpr186, amdgpu_vega::vgpr186)
  (registerSpace::vgpr187, amdgpu_vega::vgpr187)
  (registerSpace::vgpr188, amdgpu_vega::vgpr188)
  (registerSpace::vgpr189, amdgpu_vega::vgpr189)
  (registerSpace::vgpr190, amdgpu_vega::vgpr190)
  (registerSpace::vgpr191, amdgpu_vega::vgpr191)
  (registerSpace::vgpr192, amdgpu_vega::vgpr192)
  (registerSpace::vgpr193, amdgpu_vega::vgpr193)
  (registerSpace::vgpr194, amdgpu_vega::vgpr194)
  (registerSpace::vgpr195, amdgpu_vega::vgpr195)
  (registerSpace::vgpr196, amdgpu_vega::vgpr196)
  (registerSpace::vgpr197, amdgpu_vega::vgpr197)
  (registerSpace::vgpr198, amdgpu_vega::vgpr198)
  (registerSpace::vgpr199, amdgpu_vega::vgpr199)
  (registerSpace::vgpr200, amdgpu_vega::vgpr200)
  (registerSpace::vgpr201, amdgpu_vega::vgpr201)
  (registerSpace::vgpr202, amdgpu_vega::vgpr202)
  (registerSpace::vgpr203, amdgpu_vega::vgpr203)
  (registerSpace::vgpr204, amdgpu_vega::vgpr204)
  (registerSpace::vgpr205, amdgpu_vega::vgpr205)
  (registerSpace::vgpr206, amdgpu_vega::vgpr206)
  (registerSpace::vgpr207, amdgpu_vega::vgpr207)
  (registerSpace::vgpr208, amdgpu_vega::vgpr208)
  (registerSpace::vgpr209, amdgpu_vega::vgpr209)
  (registerSpace::vgpr210, amdgpu_vega::vgpr210)
  (registerSpace::vgpr211, amdgpu_vega::vgpr211)
  (registerSpace::vgpr212, amdgpu_vega::vgpr212)
  (registerSpace::vgpr213, amdgpu_vega::vgpr213)
  (registerSpace::vgpr214, amdgpu_vega::vgpr214)
  (registerSpace::vgpr215, amdgpu_vega::vgpr215)
  (registerSpace::vgpr216, amdgpu_vega::vgpr216)
  (registerSpace::vgpr217, amdgpu_vega::vgpr217)
  (registerSpace::vgpr218, amdgpu_vega::vgpr218)
  (registerSpace::vgpr219, amdgpu_vega::vgpr219)
  (registerSpace::vgpr220, amdgpu_vega::vgpr220)
  (registerSpace::vgpr221, amdgpu_vega::vgpr221)
  (registerSpace::vgpr222, amdgpu_vega::vgpr222)
  (registerSpace::vgpr223, amdgpu_vega::vgpr223)
  (registerSpace::vgpr224, amdgpu_vega::vgpr224)
  (registerSpace::vgpr225, amdgpu_vega::vgpr225)
  (registerSpace::vgpr226, amdgpu_vega::vgpr226)
  (registerSpace::vgpr227, amdgpu_vega::vgpr227)
  (registerSpace::vgpr228, amdgpu_vega::vgpr228)
  (registerSpace::vgpr229, amdgpu_vega::vgpr229)
  (registerSpace::vgpr230, amdgpu_vega::vgpr230)
  (registerSpace::vgpr231, amdgpu_vega::vgpr231)
  (registerSpace::vgpr232, amdgpu_vega::vgpr232)
  (registerSpace::vgpr233, amdgpu_vega::vgpr233)
  (registerSpace::vgpr234, amdgpu_vega::vgpr234)
  (registerSpace::vgpr235, amdgpu_vega::vgpr235)
  (registerSpace::vgpr236, amdgpu_vega::vgpr236)
  (registerSpace::vgpr237, amdgpu_vega::vgpr237)
  (registerSpace::vgpr238, amdgpu_vega::vgpr238)
  (registerSpace::vgpr239, amdgpu_vega::vgpr239)
  (registerSpace::vgpr240, amdgpu_vega::vgpr240)
  (registerSpace::vgpr241, amdgpu_vega::vgpr241)
  (registerSpace::vgpr242, amdgpu_vega::vgpr242)
  (registerSpace::vgpr243, amdgpu_vega::vgpr243)
  (registerSpace::vgpr244, amdgpu_vega::vgpr244)
  (registerSpace::vgpr245, amdgpu_vega::vgpr245)
  (registerSpace::vgpr246, amdgpu_vega::vgpr246)
  (registerSpace::vgpr247, amdgpu_vega::vgpr247)
  (registerSpace::vgpr248, amdgpu_vega::vgpr248)
  (registerSpace::vgpr249, amdgpu_vega::vgpr249)
  (registerSpace::vgpr250, amdgpu_vega::vgpr250)
  (registerSpace::vgpr251, amdgpu_vega::vgpr251)
  (registerSpace::vgpr252, amdgpu_vega::vgpr252)
  (registerSpace::vgpr253, amdgpu_vega::vgpr253)
  (registerSpace::vgpr254, amdgpu_vega::vgpr254)
  (registerSpace::vgpr255, amdgpu_vega::vgpr255)
  ;


map<MachRegister, Register> reverseRegisterMap = map_list_of
  (amdgpu_vega::sgpr0,  registerSpace::sgpr0)
  (amdgpu_vega::sgpr1,  registerSpace::sgpr1)
  (amdgpu_vega::sgpr2,  registerSpace::sgpr2)
  (amdgpu_vega::sgpr3,  registerSpace::sgpr3)
  (amdgpu_vega::sgpr4,  registerSpace::sgpr4)
  (amdgpu_vega::sgpr5,  registerSpace::sgpr5)
  (amdgpu_vega::sgpr6,  registerSpace::sgpr6)
  (amdgpu_vega::sgpr7,  registerSpace::sgpr7)
  (amdgpu_vega::sgpr8,  registerSpace::sgpr8)
  (amdgpu_vega::sgpr9,  registerSpace::sgpr9)
  (amdgpu_vega::sgpr10, registerSpace::sgpr10)
  (amdgpu_vega::sgpr11, registerSpace::sgpr11)
  (amdgpu_vega::sgpr12, registerSpace::sgpr12)
  (amdgpu_vega::sgpr13, registerSpace::sgpr13)
  (amdgpu_vega::sgpr14, registerSpace::sgpr14)
  (amdgpu_vega::sgpr15, registerSpace::sgpr15)
  (amdgpu_vega::sgpr16, registerSpace::sgpr16)
  (amdgpu_vega::sgpr17, registerSpace::sgpr17)
  (amdgpu_vega::sgpr18, registerSpace::sgpr18)
  (amdgpu_vega::sgpr19, registerSpace::sgpr19)
  (amdgpu_vega::sgpr20, registerSpace::sgpr20)
  (amdgpu_vega::sgpr21, registerSpace::sgpr21)
  (amdgpu_vega::sgpr22, registerSpace::sgpr22)
  (amdgpu_vega::sgpr23, registerSpace::sgpr23)
  (amdgpu_vega::sgpr24, registerSpace::sgpr24)
  (amdgpu_vega::sgpr25, registerSpace::sgpr25)
  (amdgpu_vega::sgpr26, registerSpace::sgpr26)
  (amdgpu_vega::sgpr27, registerSpace::sgpr27)
  (amdgpu_vega::sgpr28, registerSpace::sgpr28)
  (amdgpu_vega::sgpr29, registerSpace::sgpr29)
  (amdgpu_vega::sgpr30, registerSpace::sgpr30)
  (amdgpu_vega::sgpr31, registerSpace::sgpr31)
  (amdgpu_vega::sgpr32, registerSpace::sgpr32)
  (amdgpu_vega::sgpr33, registerSpace::sgpr33)
  (amdgpu_vega::sgpr34, registerSpace::sgpr34)
  (amdgpu_vega::sgpr35, registerSpace::sgpr35)
  (amdgpu_vega::sgpr36, registerSpace::sgpr36)
  (amdgpu_vega::sgpr37, registerSpace::sgpr37)
  (amdgpu_vega::sgpr38, registerSpace::sgpr38)
  (amdgpu_vega::sgpr39, registerSpace::sgpr39)
  (amdgpu_vega::sgpr40, registerSpace::sgpr40)
  (amdgpu_vega::sgpr41, registerSpace::sgpr41)
  (amdgpu_vega::sgpr42, registerSpace::sgpr42)
  (amdgpu_vega::sgpr43, registerSpace::sgpr43)
  (amdgpu_vega::sgpr44, registerSpace::sgpr44)
  (amdgpu_vega::sgpr45, registerSpace::sgpr45)
  (amdgpu_vega::sgpr46, registerSpace::sgpr46)
  (amdgpu_vega::sgpr47, registerSpace::sgpr47)
  (amdgpu_vega::sgpr48, registerSpace::sgpr48)
  (amdgpu_vega::sgpr49, registerSpace::sgpr49)
  (amdgpu_vega::sgpr50, registerSpace::sgpr50)
  (amdgpu_vega::sgpr51, registerSpace::sgpr51)
  (amdgpu_vega::sgpr52, registerSpace::sgpr52)
  (amdgpu_vega::sgpr53, registerSpace::sgpr53)
  (amdgpu_vega::sgpr54, registerSpace::sgpr54)
  (amdgpu_vega::sgpr55, registerSpace::sgpr55)
  (amdgpu_vega::sgpr56, registerSpace::sgpr56)
  (amdgpu_vega::sgpr57, registerSpace::sgpr57)
  (amdgpu_vega::sgpr58, registerSpace::sgpr58)
  (amdgpu_vega::sgpr59, registerSpace::sgpr59)
  (amdgpu_vega::sgpr60, registerSpace::sgpr60)
  (amdgpu_vega::sgpr61, registerSpace::sgpr61)
  (amdgpu_vega::sgpr62, registerSpace::sgpr62)
  (amdgpu_vega::sgpr63, registerSpace::sgpr63)
  (amdgpu_vega::sgpr64, registerSpace::sgpr64)
  (amdgpu_vega::sgpr65, registerSpace::sgpr65)
  (amdgpu_vega::sgpr66, registerSpace::sgpr66)
  (amdgpu_vega::sgpr67, registerSpace::sgpr67)
  (amdgpu_vega::sgpr68, registerSpace::sgpr68)
  (amdgpu_vega::sgpr69, registerSpace::sgpr69)
  (amdgpu_vega::sgpr70, registerSpace::sgpr70)
  (amdgpu_vega::sgpr71, registerSpace::sgpr71)
  (amdgpu_vega::sgpr72, registerSpace::sgpr72)
  (amdgpu_vega::sgpr73, registerSpace::sgpr73)
  (amdgpu_vega::sgpr74, registerSpace::sgpr74)
  (amdgpu_vega::sgpr75, registerSpace::sgpr75)
  (amdgpu_vega::sgpr76, registerSpace::sgpr76)
  (amdgpu_vega::sgpr77, registerSpace::sgpr77)
  (amdgpu_vega::sgpr78, registerSpace::sgpr78)
  (amdgpu_vega::sgpr79, registerSpace::sgpr79)
  (amdgpu_vega::sgpr80, registerSpace::sgpr80)
  (amdgpu_vega::sgpr81, registerSpace::sgpr81)
  (amdgpu_vega::sgpr82, registerSpace::sgpr82)
  (amdgpu_vega::sgpr83, registerSpace::sgpr83)
  (amdgpu_vega::sgpr84, registerSpace::sgpr84)
  (amdgpu_vega::sgpr85, registerSpace::sgpr85)
  (amdgpu_vega::sgpr86, registerSpace::sgpr86)
  (amdgpu_vega::sgpr87, registerSpace::sgpr87)
  (amdgpu_vega::sgpr88, registerSpace::sgpr88)
  (amdgpu_vega::sgpr89, registerSpace::sgpr89)
  (amdgpu_vega::sgpr90, registerSpace::sgpr90)
  (amdgpu_vega::sgpr91, registerSpace::sgpr91)
  (amdgpu_vega::sgpr92, registerSpace::sgpr92)
  (amdgpu_vega::sgpr93, registerSpace::sgpr93)
  (amdgpu_vega::sgpr94, registerSpace::sgpr94)
  (amdgpu_vega::sgpr95, registerSpace::sgpr95)
  (amdgpu_vega::sgpr96, registerSpace::sgpr96)
  (amdgpu_vega::sgpr97, registerSpace::sgpr97)
  (amdgpu_vega::sgpr98, registerSpace::sgpr98)
  (amdgpu_vega::sgpr99, registerSpace::sgpr99)
  (amdgpu_vega::sgpr100, registerSpace::sgpr100)
  (amdgpu_vega::sgpr101, registerSpace::sgpr101)
  (amdgpu_vega::flat_scratch_lo, registerSpace::flat_scratch_lo)
  (amdgpu_vega::flat_scratch_hi, registerSpace::flat_scratch_hi)
  (amdgpu_vega::xnack_mask_lo, registerSpace::xnack_mask_lo)
  (amdgpu_vega::xnack_mask_hi, registerSpace::xnack_mask_hi)
  (amdgpu_vega::vcc_lo, registerSpace::vcc_lo)
  (amdgpu_vega::vcc_hi, registerSpace::vcc_hi)
  (amdgpu_vega::ttmp0,  registerSpace::ttmp0)
  (amdgpu_vega::ttmp1,  registerSpace::ttmp1)
  (amdgpu_vega::ttmp2,  registerSpace::ttmp2)
  (amdgpu_vega::ttmp3,  registerSpace::ttmp3)
  (amdgpu_vega::ttmp4,  registerSpace::ttmp4)
  (amdgpu_vega::ttmp5,  registerSpace::ttmp5)
  (amdgpu_vega::ttmp6,  registerSpace::ttmp6)
  (amdgpu_vega::ttmp7,  registerSpace::ttmp7)
  (amdgpu_vega::ttmp8,  registerSpace::ttmp8)
  (amdgpu_vega::ttmp9,  registerSpace::ttmp9)
  (amdgpu_vega::ttmp10, registerSpace::ttmp10)
  (amdgpu_vega::ttmp11, registerSpace::ttmp11)
  (amdgpu_vega::ttmp12, registerSpace::ttmp12)
  (amdgpu_vega::ttmp13, registerSpace::ttmp13)
  (amdgpu_vega::ttmp14, registerSpace::ttmp14)
  (amdgpu_vega::ttmp15, registerSpace::ttmp15)
  (amdgpu_vega::m0,     registerSpace::m0)
  (amdgpu_vega::exec_lo, registerSpace::exec_lo)
  (amdgpu_vega::exec_hi, registerSpace::exec_hi)

  (amdgpu_vega::vgpr0,  registerSpace::vgpr0)
  (amdgpu_vega::vgpr1,  registerSpace::vgpr1)
  (amdgpu_vega::vgpr2,  registerSpace::vgpr2)
  (amdgpu_vega::vgpr3,  registerSpace::vgpr3)
  (amdgpu_vega::vgpr4,  registerSpace::vgpr4)
  (amdgpu_vega::vgpr5,  registerSpace::vgpr5)
  (amdgpu_vega::vgpr6,  registerSpace::vgpr6)
  (amdgpu_vega::vgpr7,  registerSpace::vgpr7)
  (amdgpu_vega::vgpr8,  registerSpace::vgpr8)
  (amdgpu_vega::vgpr9,  registerSpace::vgpr9)
  (amdgpu_vega::vgpr10, registerSpace::vgpr10)
  (amdgpu_vega::vgpr11, registerSpace::vgpr11)
  (amdgpu_vega::vgpr12, registerSpace::vgpr12)
  (amdgpu_vega::vgpr13, registerSpace::vgpr13)
  (amdgpu_vega::vgpr14, registerSpace::vgpr14)
  (amdgpu_vega::vgpr15, registerSpace::vgpr15)
  (amdgpu_vega::vgpr16, registerSpace::vgpr16)
  (amdgpu_vega::vgpr17, registerSpace::vgpr17)
  (amdgpu_vega::vgpr18, registerSpace::vgpr18)
  (amdgpu_vega::vgpr19, registerSpace::vgpr19)
  (amdgpu_vega::vgpr20, registerSpace::vgpr20)
  (amdgpu_vega::vgpr21, registerSpace::vgpr21)
  (amdgpu_vega::vgpr22, registerSpace::vgpr22)
  (amdgpu_vega::vgpr23, registerSpace::vgpr23)
  (amdgpu_vega::vgpr24, registerSpace::vgpr24)
  (amdgpu_vega::vgpr25, registerSpace::vgpr25)
  (amdgpu_vega::vgpr26, registerSpace::vgpr26)
  (amdgpu_vega::vgpr27, registerSpace::vgpr27)
  (amdgpu_vega::vgpr28, registerSpace::vgpr28)
  (amdgpu_vega::vgpr29, registerSpace::vgpr29)
  (amdgpu_vega::vgpr30, registerSpace::vgpr30)
  (amdgpu_vega::vgpr31, registerSpace::vgpr31)
  (amdgpu_vega::vgpr32, registerSpace::vgpr32)
  (amdgpu_vega::vgpr33, registerSpace::vgpr33)
  (amdgpu_vega::vgpr34, registerSpace::vgpr34)
  (amdgpu_vega::vgpr35, registerSpace::vgpr35)
  (amdgpu_vega::vgpr36, registerSpace::vgpr36)
  (amdgpu_vega::vgpr37, registerSpace::vgpr37)
  (amdgpu_vega::vgpr38, registerSpace::vgpr38)
  (amdgpu_vega::vgpr39, registerSpace::vgpr39)
  (amdgpu_vega::vgpr40, registerSpace::vgpr40)
  (amdgpu_vega::vgpr41, registerSpace::vgpr41)
  (amdgpu_vega::vgpr42, registerSpace::vgpr42)
  (amdgpu_vega::vgpr43, registerSpace::vgpr43)
  (amdgpu_vega::vgpr44, registerSpace::vgpr44)
  (amdgpu_vega::vgpr45, registerSpace::vgpr45)
  (amdgpu_vega::vgpr46, registerSpace::vgpr46)
  (amdgpu_vega::vgpr47, registerSpace::vgpr47)
  (amdgpu_vega::vgpr48, registerSpace::vgpr48)
  (amdgpu_vega::vgpr49, registerSpace::vgpr49)
  (amdgpu_vega::vgpr50, registerSpace::vgpr50)
  (amdgpu_vega::vgpr51, registerSpace::vgpr51)
  (amdgpu_vega::vgpr52, registerSpace::vgpr52)
  (amdgpu_vega::vgpr53, registerSpace::vgpr53)
  (amdgpu_vega::vgpr54, registerSpace::vgpr54)
  (amdgpu_vega::vgpr55, registerSpace::vgpr55)
  (amdgpu_vega::vgpr56, registerSpace::vgpr56)
  (amdgpu_vega::vgpr57, registerSpace::vgpr57)
  (amdgpu_vega::vgpr58, registerSpace::vgpr58)
  (amdgpu_vega::vgpr59, registerSpace::vgpr59)
  (amdgpu_vega::vgpr60, registerSpace::vgpr60)
  (amdgpu_vega::vgpr61, registerSpace::vgpr61)
  (amdgpu_vega::vgpr62, registerSpace::vgpr62)
  (amdgpu_vega::vgpr63, registerSpace::vgpr63)
  (amdgpu_vega::vgpr64, registerSpace::vgpr64)
  (amdgpu_vega::vgpr65, registerSpace::vgpr65)
  (amdgpu_vega::vgpr66, registerSpace::vgpr66)
  (amdgpu_vega::vgpr67, registerSpace::vgpr67)
  (amdgpu_vega::vgpr68, registerSpace::vgpr68)
  (amdgpu_vega::vgpr69, registerSpace::vgpr69)
  (amdgpu_vega::vgpr70, registerSpace::vgpr70)
  (amdgpu_vega::vgpr71, registerSpace::vgpr71)
  (amdgpu_vega::vgpr72, registerSpace::vgpr72)
  (amdgpu_vega::vgpr73, registerSpace::vgpr73)
  (amdgpu_vega::vgpr74, registerSpace::vgpr74)
  (amdgpu_vega::vgpr75, registerSpace::vgpr75)
  (amdgpu_vega::vgpr76, registerSpace::vgpr76)
  (amdgpu_vega::vgpr77, registerSpace::vgpr77)
  (amdgpu_vega::vgpr78, registerSpace::vgpr78)
  (amdgpu_vega::vgpr79, registerSpace::vgpr79)
  (amdgpu_vega::vgpr80, registerSpace::vgpr80)
  (amdgpu_vega::vgpr81, registerSpace::vgpr81)
  (amdgpu_vega::vgpr82, registerSpace::vgpr82)
  (amdgpu_vega::vgpr83, registerSpace::vgpr83)
  (amdgpu_vega::vgpr84, registerSpace::vgpr84)
  (amdgpu_vega::vgpr85, registerSpace::vgpr85)
  (amdgpu_vega::vgpr86, registerSpace::vgpr86)
  (amdgpu_vega::vgpr87, registerSpace::vgpr87)
  (amdgpu_vega::vgpr88, registerSpace::vgpr88)
  (amdgpu_vega::vgpr89, registerSpace::vgpr89)
  (amdgpu_vega::vgpr90, registerSpace::vgpr90)
  (amdgpu_vega::vgpr91, registerSpace::vgpr91)
  (amdgpu_vega::vgpr92, registerSpace::vgpr92)
  (amdgpu_vega::vgpr93, registerSpace::vgpr93)
  (amdgpu_vega::vgpr94, registerSpace::vgpr94)
  (amdgpu_vega::vgpr95, registerSpace::vgpr95)
  (amdgpu_vega::vgpr96, registerSpace::vgpr96)
  (amdgpu_vega::vgpr97, registerSpace::vgpr97)
  (amdgpu_vega::vgpr98, registerSpace::vgpr98)
  (amdgpu_vega::vgpr99, registerSpace::vgpr99)
  (amdgpu_vega::vgpr100, registerSpace::vgpr100)
  (amdgpu_vega::vgpr101, registerSpace::vgpr101)
  (amdgpu_vega::vgpr102, registerSpace::vgpr102)
  (amdgpu_vega::vgpr103, registerSpace::vgpr103)
  (amdgpu_vega::vgpr104, registerSpace::vgpr104)
  (amdgpu_vega::vgpr105, registerSpace::vgpr105)
  (amdgpu_vega::vgpr106, registerSpace::vgpr106)
  (amdgpu_vega::vgpr107, registerSpace::vgpr107)
  (amdgpu_vega::vgpr108, registerSpace::vgpr108)
  (amdgpu_vega::vgpr109, registerSpace::vgpr109)
  (amdgpu_vega::vgpr110, registerSpace::vgpr110)
  (amdgpu_vega::vgpr111, registerSpace::vgpr111)
  (amdgpu_vega::vgpr112, registerSpace::vgpr112)
  (amdgpu_vega::vgpr113, registerSpace::vgpr113)
  (amdgpu_vega::vgpr114, registerSpace::vgpr114)
  (amdgpu_vega::vgpr115, registerSpace::vgpr115)
  (amdgpu_vega::vgpr116, registerSpace::vgpr116)
  (amdgpu_vega::vgpr117, registerSpace::vgpr117)
  (amdgpu_vega::vgpr118, registerSpace::vgpr118)
  (amdgpu_vega::vgpr119, registerSpace::vgpr119)
  (amdgpu_vega::vgpr120, registerSpace::vgpr120)
  (amdgpu_vega::vgpr121, registerSpace::vgpr121)
  (amdgpu_vega::vgpr122, registerSpace::vgpr122)
  (amdgpu_vega::vgpr123, registerSpace::vgpr123)
  (amdgpu_vega::vgpr124, registerSpace::vgpr124)
  (amdgpu_vega::vgpr125, registerSpace::vgpr125)
  (amdgpu_vega::vgpr126, registerSpace::vgpr126)
  (amdgpu_vega::vgpr127, registerSpace::vgpr127)
  (amdgpu_vega::vgpr128, registerSpace::vgpr128)
  (amdgpu_vega::vgpr129, registerSpace::vgpr129)
  (amdgpu_vega::vgpr130, registerSpace::vgpr130)
  (amdgpu_vega::vgpr131, registerSpace::vgpr131)
  (amdgpu_vega::vgpr132, registerSpace::vgpr132)
  (amdgpu_vega::vgpr133, registerSpace::vgpr133)
  (amdgpu_vega::vgpr134, registerSpace::vgpr134)
  (amdgpu_vega::vgpr135, registerSpace::vgpr135)
  (amdgpu_vega::vgpr136, registerSpace::vgpr136)
  (amdgpu_vega::vgpr137, registerSpace::vgpr137)
  (amdgpu_vega::vgpr138, registerSpace::vgpr138)
  (amdgpu_vega::vgpr139, registerSpace::vgpr139)
  (amdgpu_vega::vgpr140, registerSpace::vgpr140)
  (amdgpu_vega::vgpr141, registerSpace::vgpr141)
  (amdgpu_vega::vgpr142, registerSpace::vgpr142)
  (amdgpu_vega::vgpr143, registerSpace::vgpr143)
  (amdgpu_vega::vgpr144, registerSpace::vgpr144)
  (amdgpu_vega::vgpr145, registerSpace::vgpr145)
  (amdgpu_vega::vgpr146, registerSpace::vgpr146)
  (amdgpu_vega::vgpr147, registerSpace::vgpr147)
  (amdgpu_vega::vgpr148, registerSpace::vgpr148)
  (amdgpu_vega::vgpr149, registerSpace::vgpr149)
  (amdgpu_vega::vgpr150, registerSpace::vgpr150)
  (amdgpu_vega::vgpr151, registerSpace::vgpr151)
  (amdgpu_vega::vgpr152, registerSpace::vgpr152)
  (amdgpu_vega::vgpr153, registerSpace::vgpr153)
  (amdgpu_vega::vgpr154, registerSpace::vgpr154)
  (amdgpu_vega::vgpr155, registerSpace::vgpr155)
  (amdgpu_vega::vgpr156, registerSpace::vgpr156)
  (amdgpu_vega::vgpr157, registerSpace::vgpr157)
  (amdgpu_vega::vgpr158, registerSpace::vgpr158)
  (amdgpu_vega::vgpr159, registerSpace::vgpr159)
  (amdgpu_vega::vgpr160, registerSpace::vgpr160)
  (amdgpu_vega::vgpr161, registerSpace::vgpr161)
  (amdgpu_vega::vgpr162, registerSpace::vgpr162)
  (amdgpu_vega::vgpr163, registerSpace::vgpr163)
  (amdgpu_vega::vgpr164, registerSpace::vgpr164)
  (amdgpu_vega::vgpr165, registerSpace::vgpr165)
  (amdgpu_vega::vgpr166, registerSpace::vgpr166)
  (amdgpu_vega::vgpr167, registerSpace::vgpr167)
  (amdgpu_vega::vgpr168, registerSpace::vgpr168)
  (amdgpu_vega::vgpr169, registerSpace::vgpr169)
  (amdgpu_vega::vgpr170, registerSpace::vgpr170)
  (amdgpu_vega::vgpr171, registerSpace::vgpr171)
  (amdgpu_vega::vgpr172, registerSpace::vgpr172)
  (amdgpu_vega::vgpr173, registerSpace::vgpr173)
  (amdgpu_vega::vgpr174, registerSpace::vgpr174)
  (amdgpu_vega::vgpr175, registerSpace::vgpr175)
  (amdgpu_vega::vgpr176, registerSpace::vgpr176)
  (amdgpu_vega::vgpr177, registerSpace::vgpr177)
  (amdgpu_vega::vgpr178, registerSpace::vgpr178)
  (amdgpu_vega::vgpr179, registerSpace::vgpr179)
  (amdgpu_vega::vgpr180, registerSpace::vgpr180)
  (amdgpu_vega::vgpr181, registerSpace::vgpr181)
  (amdgpu_vega::vgpr182, registerSpace::vgpr182)
  (amdgpu_vega::vgpr183, registerSpace::vgpr183)
  (amdgpu_vega::vgpr184, registerSpace::vgpr184)
  (amdgpu_vega::vgpr185, registerSpace::vgpr185)
  (amdgpu_vega::vgpr186, registerSpace::vgpr186)
  (amdgpu_vega::vgpr187, registerSpace::vgpr187)
  (amdgpu_vega::vgpr188, registerSpace::vgpr188)
  (amdgpu_vega::vgpr189, registerSpace::vgpr189)
  (amdgpu_vega::vgpr190, registerSpace::vgpr190)
  (amdgpu_vega::vgpr191, registerSpace::vgpr191)
  (amdgpu_vega::vgpr192, registerSpace::vgpr192)
  (amdgpu_vega::vgpr193, registerSpace::vgpr193)
  (amdgpu_vega::vgpr194, registerSpace::vgpr194)
  (amdgpu_vega::vgpr195, registerSpace::vgpr195)
  (amdgpu_vega::vgpr196, registerSpace::vgpr196)
  (amdgpu_vega::vgpr197, registerSpace::vgpr197)
  (amdgpu_vega::vgpr198, registerSpace::vgpr198)
  (amdgpu_vega::vgpr199, registerSpace::vgpr199)
  (amdgpu_vega::vgpr200, registerSpace::vgpr200)
  (amdgpu_vega::vgpr201, registerSpace::vgpr201)
  (amdgpu_vega::vgpr202, registerSpace::vgpr202)
  (amdgpu_vega::vgpr203, registerSpace::vgpr203)
  (amdgpu_vega::vgpr204, registerSpace::vgpr204)
  (amdgpu_vega::vgpr205, registerSpace::vgpr205)
  (amdgpu_vega::vgpr206, registerSpace::vgpr206)
  (amdgpu_vega::vgpr207, registerSpace::vgpr207)
  (amdgpu_vega::vgpr208, registerSpace::vgpr208)
  (amdgpu_vega::vgpr209, registerSpace::vgpr209)
  (amdgpu_vega::vgpr210, registerSpace::vgpr210)
  (amdgpu_vega::vgpr211, registerSpace::vgpr211)
  (amdgpu_vega::vgpr212, registerSpace::vgpr212)
  (amdgpu_vega::vgpr213, registerSpace::vgpr213)
  (amdgpu_vega::vgpr214, registerSpace::vgpr214)
  (amdgpu_vega::vgpr215, registerSpace::vgpr215)
  (amdgpu_vega::vgpr216, registerSpace::vgpr216)
  (amdgpu_vega::vgpr217, registerSpace::vgpr217)
  (amdgpu_vega::vgpr218, registerSpace::vgpr218)
  (amdgpu_vega::vgpr219, registerSpace::vgpr219)
  (amdgpu_vega::vgpr220, registerSpace::vgpr220)
  (amdgpu_vega::vgpr221, registerSpace::vgpr221)
  (amdgpu_vega::vgpr222, registerSpace::vgpr222)
  (amdgpu_vega::vgpr223, registerSpace::vgpr223)
  (amdgpu_vega::vgpr224, registerSpace::vgpr224)
  (amdgpu_vega::vgpr225, registerSpace::vgpr225)
  (amdgpu_vega::vgpr226, registerSpace::vgpr226)
  (amdgpu_vega::vgpr227, registerSpace::vgpr227)
  (amdgpu_vega::vgpr228, registerSpace::vgpr228)
  (amdgpu_vega::vgpr229, registerSpace::vgpr229)
  (amdgpu_vega::vgpr230, registerSpace::vgpr230)
  (amdgpu_vega::vgpr231, registerSpace::vgpr231)
  (amdgpu_vega::vgpr232, registerSpace::vgpr232)
  (amdgpu_vega::vgpr233, registerSpace::vgpr233)
  (amdgpu_vega::vgpr234, registerSpace::vgpr234)
  (amdgpu_vega::vgpr235, registerSpace::vgpr235)
  (amdgpu_vega::vgpr236, registerSpace::vgpr236)
  (amdgpu_vega::vgpr237, registerSpace::vgpr237)
  (amdgpu_vega::vgpr238, registerSpace::vgpr238)
  (amdgpu_vega::vgpr239, registerSpace::vgpr239)
  (amdgpu_vega::vgpr240, registerSpace::vgpr240)
  (amdgpu_vega::vgpr241, registerSpace::vgpr241)
  (amdgpu_vega::vgpr242, registerSpace::vgpr242)
  (amdgpu_vega::vgpr243, registerSpace::vgpr243)
  (amdgpu_vega::vgpr244, registerSpace::vgpr244)
  (amdgpu_vega::vgpr245, registerSpace::vgpr245)
  (amdgpu_vega::vgpr246, registerSpace::vgpr246)
  (amdgpu_vega::vgpr247, registerSpace::vgpr247)
  (amdgpu_vega::vgpr248, registerSpace::vgpr248)
  (amdgpu_vega::vgpr249, registerSpace::vgpr249)
  (amdgpu_vega::vgpr250, registerSpace::vgpr250)
  (amdgpu_vega::vgpr251, registerSpace::vgpr251)
  (amdgpu_vega::vgpr252, registerSpace::vgpr252)
  (amdgpu_vega::vgpr253, registerSpace::vgpr253)
  (amdgpu_vega::vgpr254, registerSpace::vgpr254)
  (amdgpu_vega::vgpr255, registerSpace::vgpr255)
  ;

Register convertRegID(MachRegister reg, bool &wasUpcast) {
    wasUpcast = false;

    MachRegister baseReg = MachRegister((reg.getBaseRegister().val() & ~reg.getArchitecture()) | Arch_amdgpu_vega);
//    RegisterAST::Ptr debug(new RegisterAST(baseReg));
//    fprintf(stderr, "DEBUG: converting %s", toBeConverted->format().c_str());
//    fprintf(stderr, " to %s\n", debug->format().c_str());
    map<MachRegister, Register>::const_iterator found =
      reverseRegisterMap.find(baseReg);
    if(found == reverseRegisterMap.end()) {
      // Yeah, this happens when we analyze trash code. Oops...
      return registerSpace::ignored;
    }

    return found->second;
}


Register convertRegID(RegisterAST::Ptr toBeConverted, bool& wasUpcast)
{
    return convertRegID(toBeConverted.get(), wasUpcast);
}

Register convertRegID(RegisterAST* toBeConverted, bool& wasUpcast)
{
     if(!toBeConverted) {
        //assert(0);
      return registerSpace::ignored;
    }
    return convertRegID(toBeConverted->getID(), wasUpcast);
}


MachRegister convertRegID(Register r, Dyninst::Architecture arch) {
    if( arch == Arch_aarch64 ) {
        switch(r) {
            case registerSpace::sgpr0: return amdgpu_vega::sgpr0;
            case registerSpace::sgpr1: return amdgpu_vega::sgpr1;
            case registerSpace::sgpr2: return amdgpu_vega::sgpr2;
            case registerSpace::sgpr3: return amdgpu_vega::sgpr3;
            case registerSpace::sgpr4: return amdgpu_vega::sgpr4;
            case registerSpace::sgpr5: return amdgpu_vega::sgpr5;
            case registerSpace::sgpr6: return amdgpu_vega::sgpr6;
            case registerSpace::sgpr7: return amdgpu_vega::sgpr7;
            case registerSpace::sgpr8: return amdgpu_vega::sgpr8;
            case registerSpace::sgpr9: return amdgpu_vega::sgpr9;
            case registerSpace::sgpr10: return amdgpu_vega::sgpr10;
            case registerSpace::sgpr11: return amdgpu_vega::sgpr11;
            case registerSpace::sgpr12: return amdgpu_vega::sgpr12;
            case registerSpace::sgpr13: return amdgpu_vega::sgpr13;
            case registerSpace::sgpr14: return amdgpu_vega::sgpr14;
            case registerSpace::sgpr15: return amdgpu_vega::sgpr15;
            case registerSpace::sgpr16: return amdgpu_vega::sgpr16;
            case registerSpace::sgpr17: return amdgpu_vega::sgpr17;
            case registerSpace::sgpr18: return amdgpu_vega::sgpr18;
            case registerSpace::sgpr19: return amdgpu_vega::sgpr19;
            case registerSpace::sgpr20: return amdgpu_vega::sgpr20;
            case registerSpace::sgpr21: return amdgpu_vega::sgpr21;
            case registerSpace::sgpr22: return amdgpu_vega::sgpr22;
            case registerSpace::sgpr23: return amdgpu_vega::sgpr23;
            case registerSpace::sgpr24: return amdgpu_vega::sgpr24;
            case registerSpace::sgpr25: return amdgpu_vega::sgpr25;
            case registerSpace::sgpr26: return amdgpu_vega::sgpr26;
            case registerSpace::sgpr27: return amdgpu_vega::sgpr27;
            case registerSpace::sgpr28: return amdgpu_vega::sgpr28;
            case registerSpace::sgpr29: return amdgpu_vega::sgpr29;
            case registerSpace::sgpr30: return amdgpu_vega::sgpr30;
            case registerSpace::sgpr31: return amdgpu_vega::sgpr31;
            case registerSpace::sgpr32: return amdgpu_vega::sgpr32;
            case registerSpace::sgpr33: return amdgpu_vega::sgpr33;
            case registerSpace::sgpr34: return amdgpu_vega::sgpr34;
            case registerSpace::sgpr35: return amdgpu_vega::sgpr35;
            case registerSpace::sgpr36: return amdgpu_vega::sgpr36;
            case registerSpace::sgpr37: return amdgpu_vega::sgpr37;
            case registerSpace::sgpr38: return amdgpu_vega::sgpr38;
            case registerSpace::sgpr39: return amdgpu_vega::sgpr39;
            case registerSpace::sgpr40: return amdgpu_vega::sgpr40;
            case registerSpace::sgpr41: return amdgpu_vega::sgpr41;
            case registerSpace::sgpr42: return amdgpu_vega::sgpr42;
            case registerSpace::sgpr43: return amdgpu_vega::sgpr43;
            case registerSpace::sgpr44: return amdgpu_vega::sgpr44;
            case registerSpace::sgpr45: return amdgpu_vega::sgpr45;
            case registerSpace::sgpr46: return amdgpu_vega::sgpr46;
            case registerSpace::sgpr47: return amdgpu_vega::sgpr47;
            case registerSpace::sgpr48: return amdgpu_vega::sgpr48;
            case registerSpace::sgpr49: return amdgpu_vega::sgpr49;
            case registerSpace::sgpr50: return amdgpu_vega::sgpr50;
            case registerSpace::sgpr51: return amdgpu_vega::sgpr51;
            case registerSpace::sgpr52: return amdgpu_vega::sgpr52;
            case registerSpace::sgpr53: return amdgpu_vega::sgpr53;
            case registerSpace::sgpr54: return amdgpu_vega::sgpr54;
            case registerSpace::sgpr55: return amdgpu_vega::sgpr55;
            case registerSpace::sgpr56: return amdgpu_vega::sgpr56;
            case registerSpace::sgpr57: return amdgpu_vega::sgpr57;
            case registerSpace::sgpr58: return amdgpu_vega::sgpr58;
            case registerSpace::sgpr59: return amdgpu_vega::sgpr59;
            case registerSpace::sgpr60: return amdgpu_vega::sgpr60;
            case registerSpace::sgpr61: return amdgpu_vega::sgpr61;
            case registerSpace::sgpr62: return amdgpu_vega::sgpr62;
            case registerSpace::sgpr63: return amdgpu_vega::sgpr63;
            case registerSpace::sgpr64: return amdgpu_vega::sgpr64;
            case registerSpace::sgpr65: return amdgpu_vega::sgpr65;
            case registerSpace::sgpr66: return amdgpu_vega::sgpr66;
            case registerSpace::sgpr67: return amdgpu_vega::sgpr67;
            case registerSpace::sgpr68: return amdgpu_vega::sgpr68;
            case registerSpace::sgpr69: return amdgpu_vega::sgpr69;
            case registerSpace::sgpr70: return amdgpu_vega::sgpr70;
            case registerSpace::sgpr71: return amdgpu_vega::sgpr71;
            case registerSpace::sgpr72: return amdgpu_vega::sgpr72;
            case registerSpace::sgpr73: return amdgpu_vega::sgpr73;
            case registerSpace::sgpr74: return amdgpu_vega::sgpr74;
            case registerSpace::sgpr75: return amdgpu_vega::sgpr75;
            case registerSpace::sgpr76: return amdgpu_vega::sgpr76;
            case registerSpace::sgpr77: return amdgpu_vega::sgpr77;
            case registerSpace::sgpr78: return amdgpu_vega::sgpr78;
            case registerSpace::sgpr79: return amdgpu_vega::sgpr79;
            case registerSpace::sgpr80: return amdgpu_vega::sgpr80;
            case registerSpace::sgpr81: return amdgpu_vega::sgpr81;
            case registerSpace::sgpr82: return amdgpu_vega::sgpr82;
            case registerSpace::sgpr83: return amdgpu_vega::sgpr83;
            case registerSpace::sgpr84: return amdgpu_vega::sgpr84;
            case registerSpace::sgpr85: return amdgpu_vega::sgpr85;
            case registerSpace::sgpr86: return amdgpu_vega::sgpr86;
            case registerSpace::sgpr87: return amdgpu_vega::sgpr87;
            case registerSpace::sgpr88: return amdgpu_vega::sgpr88;
            case registerSpace::sgpr89: return amdgpu_vega::sgpr89;
            case registerSpace::sgpr90: return amdgpu_vega::sgpr90;
            case registerSpace::sgpr91: return amdgpu_vega::sgpr91;
            case registerSpace::sgpr92: return amdgpu_vega::sgpr92;
            case registerSpace::sgpr93: return amdgpu_vega::sgpr93;
            case registerSpace::sgpr94: return amdgpu_vega::sgpr94;
            case registerSpace::sgpr95: return amdgpu_vega::sgpr95;
            case registerSpace::sgpr96: return amdgpu_vega::sgpr96;
            case registerSpace::sgpr97: return amdgpu_vega::sgpr97;
            case registerSpace::sgpr98: return amdgpu_vega::sgpr98;
            case registerSpace::sgpr99: return amdgpu_vega::sgpr99;
            case registerSpace::sgpr100: return amdgpu_vega::sgpr100;
            case registerSpace::sgpr101: return amdgpu_vega::sgpr101;
            case registerSpace::flat_scratch_lo: return amdgpu_vega::flat_scratch_lo;
            case registerSpace::flat_scratch_hi: return amdgpu_vega::flat_scratch_hi;
            case registerSpace::xnack_mask_lo: return amdgpu_vega::xnack_mask_lo;
            case registerSpace::xnack_mask_hi: return amdgpu_vega::xnack_mask_hi;
            case registerSpace::vcc_lo: return amdgpu_vega::vcc_lo;
            case registerSpace::vcc_hi: return amdgpu_vega::vcc_hi;
            case registerSpace::ttmp0: return amdgpu_vega::ttmp0;
            case registerSpace::ttmp1: return amdgpu_vega::ttmp1;
            case registerSpace::ttmp2: return amdgpu_vega::ttmp2;
            case registerSpace::ttmp3: return amdgpu_vega::ttmp3;
            case registerSpace::ttmp4: return amdgpu_vega::ttmp4;
            case registerSpace::ttmp5: return amdgpu_vega::ttmp5;
            case registerSpace::ttmp6: return amdgpu_vega::ttmp6;
            case registerSpace::ttmp7: return amdgpu_vega::ttmp7;
            case registerSpace::ttmp8: return amdgpu_vega::ttmp8;
            case registerSpace::ttmp9: return amdgpu_vega::ttmp9;
            case registerSpace::ttmp10: return amdgpu_vega::ttmp10;
            case registerSpace::ttmp11: return amdgpu_vega::ttmp11;
            case registerSpace::ttmp12: return amdgpu_vega::ttmp12;
            case registerSpace::ttmp13: return amdgpu_vega::ttmp13;
            case registerSpace::ttmp14: return amdgpu_vega::ttmp14;
            case registerSpace::ttmp15: return amdgpu_vega::ttmp15;
            case registerSpace::m0: return amdgpu_vega::m0;
            case registerSpace::exec_lo: return amdgpu_vega::exec_lo;
            case registerSpace::exec_hi: return amdgpu_vega::exec_hi;

            case registerSpace::vgpr0: return amdgpu_vega::vgpr0;
            case registerSpace::vgpr1: return amdgpu_vega::vgpr1;
            case registerSpace::vgpr2: return amdgpu_vega::vgpr2;
            case registerSpace::vgpr3: return amdgpu_vega::vgpr3;
            case registerSpace::vgpr4: return amdgpu_vega::vgpr4;
            case registerSpace::vgpr5: return amdgpu_vega::vgpr5;
            case registerSpace::vgpr6: return amdgpu_vega::vgpr6;
            case registerSpace::vgpr7: return amdgpu_vega::vgpr7;
            case registerSpace::vgpr8: return amdgpu_vega::vgpr8;
            case registerSpace::vgpr9: return amdgpu_vega::vgpr9;
            case registerSpace::vgpr10: return amdgpu_vega::vgpr10;
            case registerSpace::vgpr11: return amdgpu_vega::vgpr11;
            case registerSpace::vgpr12: return amdgpu_vega::vgpr12;
            case registerSpace::vgpr13: return amdgpu_vega::vgpr13;
            case registerSpace::vgpr14: return amdgpu_vega::vgpr14;
            case registerSpace::vgpr15: return amdgpu_vega::vgpr15;
            case registerSpace::vgpr16: return amdgpu_vega::vgpr16;
            case registerSpace::vgpr17: return amdgpu_vega::vgpr17;
            case registerSpace::vgpr18: return amdgpu_vega::vgpr18;
            case registerSpace::vgpr19: return amdgpu_vega::vgpr19;
            case registerSpace::vgpr20: return amdgpu_vega::vgpr20;
            case registerSpace::vgpr21: return amdgpu_vega::vgpr21;
            case registerSpace::vgpr22: return amdgpu_vega::vgpr22;
            case registerSpace::vgpr23: return amdgpu_vega::vgpr23;
            case registerSpace::vgpr24: return amdgpu_vega::vgpr24;
            case registerSpace::vgpr25: return amdgpu_vega::vgpr25;
            case registerSpace::vgpr26: return amdgpu_vega::vgpr26;
            case registerSpace::vgpr27: return amdgpu_vega::vgpr27;
            case registerSpace::vgpr28: return amdgpu_vega::vgpr28;
            case registerSpace::vgpr29: return amdgpu_vega::vgpr29;
            case registerSpace::vgpr30: return amdgpu_vega::vgpr30;
            case registerSpace::vgpr31: return amdgpu_vega::vgpr31;
            case registerSpace::vgpr32: return amdgpu_vega::vgpr32;
            case registerSpace::vgpr33: return amdgpu_vega::vgpr33;
            case registerSpace::vgpr34: return amdgpu_vega::vgpr34;
            case registerSpace::vgpr35: return amdgpu_vega::vgpr35;
            case registerSpace::vgpr36: return amdgpu_vega::vgpr36;
            case registerSpace::vgpr37: return amdgpu_vega::vgpr37;
            case registerSpace::vgpr38: return amdgpu_vega::vgpr38;
            case registerSpace::vgpr39: return amdgpu_vega::vgpr39;
            case registerSpace::vgpr40: return amdgpu_vega::vgpr40;
            case registerSpace::vgpr41: return amdgpu_vega::vgpr41;
            case registerSpace::vgpr42: return amdgpu_vega::vgpr42;
            case registerSpace::vgpr43: return amdgpu_vega::vgpr43;
            case registerSpace::vgpr44: return amdgpu_vega::vgpr44;
            case registerSpace::vgpr45: return amdgpu_vega::vgpr45;
            case registerSpace::vgpr46: return amdgpu_vega::vgpr46;
            case registerSpace::vgpr47: return amdgpu_vega::vgpr47;
            case registerSpace::vgpr48: return amdgpu_vega::vgpr48;
            case registerSpace::vgpr49: return amdgpu_vega::vgpr49;
            case registerSpace::vgpr50: return amdgpu_vega::vgpr50;
            case registerSpace::vgpr51: return amdgpu_vega::vgpr51;
            case registerSpace::vgpr52: return amdgpu_vega::vgpr52;
            case registerSpace::vgpr53: return amdgpu_vega::vgpr53;
            case registerSpace::vgpr54: return amdgpu_vega::vgpr54;
            case registerSpace::vgpr55: return amdgpu_vega::vgpr55;
            case registerSpace::vgpr56: return amdgpu_vega::vgpr56;
            case registerSpace::vgpr57: return amdgpu_vega::vgpr57;
            case registerSpace::vgpr58: return amdgpu_vega::vgpr58;
            case registerSpace::vgpr59: return amdgpu_vega::vgpr59;
            case registerSpace::vgpr60: return amdgpu_vega::vgpr60;
            case registerSpace::vgpr61: return amdgpu_vega::vgpr61;
            case registerSpace::vgpr62: return amdgpu_vega::vgpr62;
            case registerSpace::vgpr63: return amdgpu_vega::vgpr63;
            case registerSpace::vgpr64: return amdgpu_vega::vgpr64;
            case registerSpace::vgpr65: return amdgpu_vega::vgpr65;
            case registerSpace::vgpr66: return amdgpu_vega::vgpr66;
            case registerSpace::vgpr67: return amdgpu_vega::vgpr67;
            case registerSpace::vgpr68: return amdgpu_vega::vgpr68;
            case registerSpace::vgpr69: return amdgpu_vega::vgpr69;
            case registerSpace::vgpr70: return amdgpu_vega::vgpr70;
            case registerSpace::vgpr71: return amdgpu_vega::vgpr71;
            case registerSpace::vgpr72: return amdgpu_vega::vgpr72;
            case registerSpace::vgpr73: return amdgpu_vega::vgpr73;
            case registerSpace::vgpr74: return amdgpu_vega::vgpr74;
            case registerSpace::vgpr75: return amdgpu_vega::vgpr75;
            case registerSpace::vgpr76: return amdgpu_vega::vgpr76;
            case registerSpace::vgpr77: return amdgpu_vega::vgpr77;
            case registerSpace::vgpr78: return amdgpu_vega::vgpr78;
            case registerSpace::vgpr79: return amdgpu_vega::vgpr79;
            case registerSpace::vgpr80: return amdgpu_vega::vgpr80;
            case registerSpace::vgpr81: return amdgpu_vega::vgpr81;
            case registerSpace::vgpr82: return amdgpu_vega::vgpr82;
            case registerSpace::vgpr83: return amdgpu_vega::vgpr83;
            case registerSpace::vgpr84: return amdgpu_vega::vgpr84;
            case registerSpace::vgpr85: return amdgpu_vega::vgpr85;
            case registerSpace::vgpr86: return amdgpu_vega::vgpr86;
            case registerSpace::vgpr87: return amdgpu_vega::vgpr87;
            case registerSpace::vgpr88: return amdgpu_vega::vgpr88;
            case registerSpace::vgpr89: return amdgpu_vega::vgpr89;
            case registerSpace::vgpr90: return amdgpu_vega::vgpr90;
            case registerSpace::vgpr91: return amdgpu_vega::vgpr91;
            case registerSpace::vgpr92: return amdgpu_vega::vgpr92;
            case registerSpace::vgpr93: return amdgpu_vega::vgpr93;
            case registerSpace::vgpr94: return amdgpu_vega::vgpr94;
            case registerSpace::vgpr95: return amdgpu_vega::vgpr95;
            case registerSpace::vgpr96: return amdgpu_vega::vgpr96;
            case registerSpace::vgpr97: return amdgpu_vega::vgpr97;
            case registerSpace::vgpr98: return amdgpu_vega::vgpr98;
            case registerSpace::vgpr99: return amdgpu_vega::vgpr99;
            case registerSpace::vgpr100: return amdgpu_vega::vgpr100;
            case registerSpace::vgpr101: return amdgpu_vega::vgpr101;
            case registerSpace::vgpr102: return amdgpu_vega::vgpr102;
            case registerSpace::vgpr103: return amdgpu_vega::vgpr103;
            case registerSpace::vgpr104: return amdgpu_vega::vgpr104;
            case registerSpace::vgpr105: return amdgpu_vega::vgpr105;
            case registerSpace::vgpr106: return amdgpu_vega::vgpr106;
            case registerSpace::vgpr107: return amdgpu_vega::vgpr107;
            case registerSpace::vgpr108: return amdgpu_vega::vgpr108;
            case registerSpace::vgpr109: return amdgpu_vega::vgpr109;
            case registerSpace::vgpr110: return amdgpu_vega::vgpr110;
            case registerSpace::vgpr111: return amdgpu_vega::vgpr111;
            case registerSpace::vgpr112: return amdgpu_vega::vgpr112;
            case registerSpace::vgpr113: return amdgpu_vega::vgpr113;
            case registerSpace::vgpr114: return amdgpu_vega::vgpr114;
            case registerSpace::vgpr115: return amdgpu_vega::vgpr115;
            case registerSpace::vgpr116: return amdgpu_vega::vgpr116;
            case registerSpace::vgpr117: return amdgpu_vega::vgpr117;
            case registerSpace::vgpr118: return amdgpu_vega::vgpr118;
            case registerSpace::vgpr119: return amdgpu_vega::vgpr119;
            case registerSpace::vgpr120: return amdgpu_vega::vgpr120;
            case registerSpace::vgpr121: return amdgpu_vega::vgpr121;
            case registerSpace::vgpr122: return amdgpu_vega::vgpr122;
            case registerSpace::vgpr123: return amdgpu_vega::vgpr123;
            case registerSpace::vgpr124: return amdgpu_vega::vgpr124;
            case registerSpace::vgpr125: return amdgpu_vega::vgpr125;
            case registerSpace::vgpr126: return amdgpu_vega::vgpr126;
            case registerSpace::vgpr127: return amdgpu_vega::vgpr127;
            case registerSpace::vgpr128: return amdgpu_vega::vgpr128;
            case registerSpace::vgpr129: return amdgpu_vega::vgpr129;
            case registerSpace::vgpr130: return amdgpu_vega::vgpr130;
            case registerSpace::vgpr131: return amdgpu_vega::vgpr131;
            case registerSpace::vgpr132: return amdgpu_vega::vgpr132;
            case registerSpace::vgpr133: return amdgpu_vega::vgpr133;
            case registerSpace::vgpr134: return amdgpu_vega::vgpr134;
            case registerSpace::vgpr135: return amdgpu_vega::vgpr135;
            case registerSpace::vgpr136: return amdgpu_vega::vgpr136;
            case registerSpace::vgpr137: return amdgpu_vega::vgpr137;
            case registerSpace::vgpr138: return amdgpu_vega::vgpr138;
            case registerSpace::vgpr139: return amdgpu_vega::vgpr139;
            case registerSpace::vgpr140: return amdgpu_vega::vgpr140;
            case registerSpace::vgpr141: return amdgpu_vega::vgpr141;
            case registerSpace::vgpr142: return amdgpu_vega::vgpr142;
            case registerSpace::vgpr143: return amdgpu_vega::vgpr143;
            case registerSpace::vgpr144: return amdgpu_vega::vgpr144;
            case registerSpace::vgpr145: return amdgpu_vega::vgpr145;
            case registerSpace::vgpr146: return amdgpu_vega::vgpr146;
            case registerSpace::vgpr147: return amdgpu_vega::vgpr147;
            case registerSpace::vgpr148: return amdgpu_vega::vgpr148;
            case registerSpace::vgpr149: return amdgpu_vega::vgpr149;
            case registerSpace::vgpr150: return amdgpu_vega::vgpr150;
            case registerSpace::vgpr151: return amdgpu_vega::vgpr151;
            case registerSpace::vgpr152: return amdgpu_vega::vgpr152;
            case registerSpace::vgpr153: return amdgpu_vega::vgpr153;
            case registerSpace::vgpr154: return amdgpu_vega::vgpr154;
            case registerSpace::vgpr155: return amdgpu_vega::vgpr155;
            case registerSpace::vgpr156: return amdgpu_vega::vgpr156;
            case registerSpace::vgpr157: return amdgpu_vega::vgpr157;
            case registerSpace::vgpr158: return amdgpu_vega::vgpr158;
            case registerSpace::vgpr159: return amdgpu_vega::vgpr159;
            case registerSpace::vgpr160: return amdgpu_vega::vgpr160;
            case registerSpace::vgpr161: return amdgpu_vega::vgpr161;
            case registerSpace::vgpr162: return amdgpu_vega::vgpr162;
            case registerSpace::vgpr163: return amdgpu_vega::vgpr163;
            case registerSpace::vgpr164: return amdgpu_vega::vgpr164;
            case registerSpace::vgpr165: return amdgpu_vega::vgpr165;
            case registerSpace::vgpr166: return amdgpu_vega::vgpr166;
            case registerSpace::vgpr167: return amdgpu_vega::vgpr167;
            case registerSpace::vgpr168: return amdgpu_vega::vgpr168;
            case registerSpace::vgpr169: return amdgpu_vega::vgpr169;
            case registerSpace::vgpr170: return amdgpu_vega::vgpr170;
            case registerSpace::vgpr171: return amdgpu_vega::vgpr171;
            case registerSpace::vgpr172: return amdgpu_vega::vgpr172;
            case registerSpace::vgpr173: return amdgpu_vega::vgpr173;
            case registerSpace::vgpr174: return amdgpu_vega::vgpr174;
            case registerSpace::vgpr175: return amdgpu_vega::vgpr175;
            case registerSpace::vgpr176: return amdgpu_vega::vgpr176;
            case registerSpace::vgpr177: return amdgpu_vega::vgpr177;
            case registerSpace::vgpr178: return amdgpu_vega::vgpr178;
            case registerSpace::vgpr179: return amdgpu_vega::vgpr179;
            case registerSpace::vgpr180: return amdgpu_vega::vgpr180;
            case registerSpace::vgpr181: return amdgpu_vega::vgpr181;
            case registerSpace::vgpr182: return amdgpu_vega::vgpr182;
            case registerSpace::vgpr183: return amdgpu_vega::vgpr183;
            case registerSpace::vgpr184: return amdgpu_vega::vgpr184;
            case registerSpace::vgpr185: return amdgpu_vega::vgpr185;
            case registerSpace::vgpr186: return amdgpu_vega::vgpr186;
            case registerSpace::vgpr187: return amdgpu_vega::vgpr187;
            case registerSpace::vgpr188: return amdgpu_vega::vgpr188;
            case registerSpace::vgpr189: return amdgpu_vega::vgpr189;
            case registerSpace::vgpr190: return amdgpu_vega::vgpr190;
            case registerSpace::vgpr191: return amdgpu_vega::vgpr191;
            case registerSpace::vgpr192: return amdgpu_vega::vgpr192;
            case registerSpace::vgpr193: return amdgpu_vega::vgpr193;
            case registerSpace::vgpr194: return amdgpu_vega::vgpr194;
            case registerSpace::vgpr195: return amdgpu_vega::vgpr195;
            case registerSpace::vgpr196: return amdgpu_vega::vgpr196;
            case registerSpace::vgpr197: return amdgpu_vega::vgpr197;
            case registerSpace::vgpr198: return amdgpu_vega::vgpr198;
            case registerSpace::vgpr199: return amdgpu_vega::vgpr199;
            case registerSpace::vgpr200: return amdgpu_vega::vgpr200;
            case registerSpace::vgpr201: return amdgpu_vega::vgpr201;
            case registerSpace::vgpr202: return amdgpu_vega::vgpr202;
            case registerSpace::vgpr203: return amdgpu_vega::vgpr203;
            case registerSpace::vgpr204: return amdgpu_vega::vgpr204;
            case registerSpace::vgpr205: return amdgpu_vega::vgpr205;
            case registerSpace::vgpr206: return amdgpu_vega::vgpr206;
            case registerSpace::vgpr207: return amdgpu_vega::vgpr207;
            case registerSpace::vgpr208: return amdgpu_vega::vgpr208;
            case registerSpace::vgpr209: return amdgpu_vega::vgpr209;
            case registerSpace::vgpr210: return amdgpu_vega::vgpr210;
            case registerSpace::vgpr211: return amdgpu_vega::vgpr211;
            case registerSpace::vgpr212: return amdgpu_vega::vgpr212;
            case registerSpace::vgpr213: return amdgpu_vega::vgpr213;
            case registerSpace::vgpr214: return amdgpu_vega::vgpr214;
            case registerSpace::vgpr215: return amdgpu_vega::vgpr215;
            case registerSpace::vgpr216: return amdgpu_vega::vgpr216;
            case registerSpace::vgpr217: return amdgpu_vega::vgpr217;
            case registerSpace::vgpr218: return amdgpu_vega::vgpr218;
            case registerSpace::vgpr219: return amdgpu_vega::vgpr219;
            case registerSpace::vgpr220: return amdgpu_vega::vgpr220;
            case registerSpace::vgpr221: return amdgpu_vega::vgpr221;
            case registerSpace::vgpr222: return amdgpu_vega::vgpr222;
            case registerSpace::vgpr223: return amdgpu_vega::vgpr223;
            case registerSpace::vgpr224: return amdgpu_vega::vgpr224;
            case registerSpace::vgpr225: return amdgpu_vega::vgpr225;
            case registerSpace::vgpr226: return amdgpu_vega::vgpr226;
            case registerSpace::vgpr227: return amdgpu_vega::vgpr227;
            case registerSpace::vgpr228: return amdgpu_vega::vgpr228;
            case registerSpace::vgpr229: return amdgpu_vega::vgpr229;
            case registerSpace::vgpr230: return amdgpu_vega::vgpr230;
            case registerSpace::vgpr231: return amdgpu_vega::vgpr231;
            case registerSpace::vgpr232: return amdgpu_vega::vgpr232;
            case registerSpace::vgpr233: return amdgpu_vega::vgpr233;
            case registerSpace::vgpr234: return amdgpu_vega::vgpr234;
            case registerSpace::vgpr235: return amdgpu_vega::vgpr235;
            case registerSpace::vgpr236: return amdgpu_vega::vgpr236;
            case registerSpace::vgpr237: return amdgpu_vega::vgpr237;
            case registerSpace::vgpr238: return amdgpu_vega::vgpr238;
            case registerSpace::vgpr239: return amdgpu_vega::vgpr239;
            case registerSpace::vgpr240: return amdgpu_vega::vgpr240;
            case registerSpace::vgpr241: return amdgpu_vega::vgpr241;
            case registerSpace::vgpr242: return amdgpu_vega::vgpr242;
            case registerSpace::vgpr243: return amdgpu_vega::vgpr243;
            case registerSpace::vgpr244: return amdgpu_vega::vgpr244;
            case registerSpace::vgpr245: return amdgpu_vega::vgpr245;
            case registerSpace::vgpr246: return amdgpu_vega::vgpr246;
            case registerSpace::vgpr247: return amdgpu_vega::vgpr247;
            case registerSpace::vgpr248: return amdgpu_vega::vgpr248;
            case registerSpace::vgpr249: return amdgpu_vega::vgpr249;
            case registerSpace::vgpr250: return amdgpu_vega::vgpr250;
            case registerSpace::vgpr251: return amdgpu_vega::vgpr251;
            case registerSpace::vgpr252: return amdgpu_vega::vgpr252;
            case registerSpace::vgpr253: return amdgpu_vega::vgpr253;
            case registerSpace::vgpr254: return amdgpu_vega::vgpr254;
            case registerSpace::vgpr255: return amdgpu_vega::vgpr255;
            default:
                break;
        }
    } else {
        assert(!"Invalid architecture");
    }

    assert(!"Register not handled");
    return InvalidReg;
}
