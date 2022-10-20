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

#ifndef _CODEGEN_AMDGPU_H
#define _CODEGEN_AMDGPU_H
#define arch_aarch64

class AddressSpace;
#include "arch-amdgpu.h"
#include <stdlib.h>
#include <cstring>
typedef unsigned codeBufIndex_t;
const unsigned int codeGenMinAlloc = (4 * 1024);
class CodeGen{
    public:
        CodeGen() :
            buffer_(NULL),
            offset_(0),
            size_(0),
            max_(0),
            allocated_(false){}
        ~CodeGen(){
            if(allocated_ && buffer_){
                free(buffer_);
            }
        }

        void insert(const void *buf, const unsigned size, const codeBufIndex_t offset){
            if(size ==0) return;
            assert(buffer_);
            realloc(used()+size);
            char * temp = ( char * ) get_ptr(offset);
            memmove(temp+size, temp, used() - offset);
            memcpy(temp,buf,size);
            moveIndex(size);
        }

        void realloc(unsigned newSize) {  
            if (newSize <= size_) return;

            unsigned increment = newSize - size_;
            if (increment < codeGenMinAlloc) increment = codeGenMinAlloc;

            size_ += increment;
            max_ += increment;
            buffer_ = (void *)::realloc(buffer_, max_);
            assert(buffer_);
            allocated_ = true;
        }

        void *get_ptr(unsigned offset) const {
            return (void*)  ((char * ) buffer_ + offset);
        }
        void *cur_ptr() const {
            assert(buffer_);
            //if (sizeof(codeBuf_t) != CODE_GEN_OFFSET_SIZE)
            //    fprintf(stderr, "ERROR: sizeof codeBuf %zu, OFFSET %u\n",
            //            sizeof(codeBuf_t), CODE_GEN_OFFSET_SIZE);
            //assert(sizeof(codeBuf_t) == CODE_GEN_OFFSET_SIZE);
            char *ret = (char *) buffer_;
            ret += offset_;
            return (void *)ret;
        }

        void copy(const void *buf, const unsigned size){
            if (size == 0) return;


            realloc(used() + size);

            assert(buffer_);
            memcpy(cur_ptr(), buf, size);
            uint32_t * cmd_ptr = (uint32_t* ) cur_ptr();
            
            printf("copied value to ptr %lx, value = %lx",cmd_ptr, *cmd_ptr);


            moveIndex(size);



        }
        void moveIndex(unsigned size){
            offset_ += size;
        }
        
//        void copy(const void *buf, const unsigned size, const codeBufIndex_t index){}
//        void copy(const std::vector<unsigned char> &buf){}
        unsigned used() const {
            return offset_;
        }
    private:

        void *buffer_;
        codeBufIndex_t offset_; // byte offset to insert into, = cur_ptr() in our implementation
        unsigned size_; // size_ allocated
        unsigned max_; // same as size_?
        bool allocated_; // whether buffer is allocated or not



};

using NS_amdgpu::instruction;
class __attribute__((visibility ("default")))  amdgpuCodeGen {
    public:
    enum SOP2_OP{
        S_ADD_U32  =0,
        S_SUB_U32  =1,
        S_ADDC_U32 =4,
        S_SUBB_U32 =5,
        S_AND_B32  =12,
        S_LHSL_B32 =28,
        S_LSHR_B32 =30,
        S_ASHR_I32 =32,
        S_MUL_I32  =36,
        S_MUL_HI_U32 = 44
    };
    enum SOPC_OP{
        S_CMP_EQ_I32 = 0,
        S_CMP_EQ_U64 = 18
    };

    enum SOPP_OP{
        S_NOP = 0,
        S_ENDPGM = 1,
        S_BRANCH = 2
    };

    static void *insnPtr(CodeGen &gen);

    static void generate(CodeGen &gen, instruction &insn);
    static void generate(CodeGen &gen, instruction &insn, unsigned position);
    static void generateTrap(CodeGen &gen);
    static void generateIllegal(CodeGen &gen);

    static void generate_s_add_u32( CodeGen & gen,  uint32_t sdst, uint32_t ssrc1, uint32_t ssrc0  , bool useImm ){ generate_SOP2(gen, SOP2_OP::S_ADD_U32, sdst, ssrc1 , ssrc0, useImm); };
    static void generate_s_addc_u32( CodeGen & gen,  uint32_t sdst, uint32_t ssrc1, uint32_t ssrc0  , bool useImm ){ generate_SOP2(gen, SOP2_OP::S_ADDC_U32, sdst, ssrc1 , ssrc0, useImm); };
    static void generate_SOP2( CodeGen & gen,  uint32_t OP, uint32_t SDST, uint32_t SSRC1 , uint32_t SSRC0, bool useImm);
    static void generate_SOPC( CodeGen & gen,  uint32_t OP, uint32_t SSRC1 , uint32_t SSRC0, bool useImm);


};

#endif
