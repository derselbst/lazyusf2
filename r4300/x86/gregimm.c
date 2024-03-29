/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   Mupen64plus - gregimm.c                                               *
 *   Mupen64Plus homepage: http://code.google.com/p/mupen64plus/           *
 *   Copyright (C) 2002 Hacktarux                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.          *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef DYNAREC

#include <stdio.h>

#include "usf/usf.h"

#include "usf/usf_internal.h"

#include "assemble.h"
#include "interpret.h"

#include "r4300/cached_interp.h"
#include "r4300/recomph.h"
#include "r4300/recomp.h"
#include "r4300/r4300.h"
#include "r4300/ops.h"
#include "r4300/macros.h"

#include "memory/memory.h"

static void genbltz_test(usf_state_t * state)
{
   int rs_64bit = is64(state, (unsigned int *)state->dst->f.i.rs);
   
   if (!rs_64bit)
     {
    int rs = allocate_register(state, (unsigned int *)state->dst->f.i.rs);
    
    cmp_reg32_imm32(state, rs, 0);
    jge_rj(state, 12);
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 1); // 10
    jmp_imm_short(state, 10); // 2
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 0); // 10
     }
   else if (rs_64bit == -1)
     {
    cmp_m32_imm32(state, ((unsigned int *)state->dst->f.i.rs)+1, 0);
    jge_rj(state, 12);
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 1); // 10
    jmp_imm_short(state, 10); // 2
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 0); // 10
     }
   else
     {
    int rs2 = allocate_64_register2(state, (unsigned int *)state->dst->f.i.rs);
    
    cmp_reg32_imm32(state, rs2, 0);
    jge_rj(state, 12);
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 1); // 10
    jmp_imm_short(state, 10); // 2
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 0); // 10
     }
}

void genbltz(usf_state_t * state)
{
#ifdef INTERPRET_BLTZ
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZ, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZ, 1);
    return;
     }
   
   genbltz_test(state);
   gendelayslot(state);
   gentest(state);
#endif
}

void genbltz_out(usf_state_t * state)
{
#ifdef INTERPRET_BLTZ_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZ_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZ_OUT, 1);
    return;
     }
   
   genbltz_test(state);
   gendelayslot(state);
   gentest_out(state);
#endif
}

void genbltz_idle(usf_state_t * state)
{
#ifdef INTERPRET_BLTZ_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZ_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZ_IDLE, 1);
    return;
     }
   
   genbltz_test(state);
   gentest_idle(state);
   genbltz(state);
#endif
}

static void genbgez_test(usf_state_t * state)
{
   int rs_64bit = is64(state, (unsigned int *)state->dst->f.i.rs);
   
   if (!rs_64bit)
     {
    int rs = allocate_register(state, (unsigned int *)state->dst->f.i.rs);
    
    cmp_reg32_imm32(state, rs, 0);
    jl_rj(state, 12);
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 1); // 10
    jmp_imm_short(state, 10); // 2
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 0); // 10
     }
   else if (rs_64bit == -1)
     {
    cmp_m32_imm32(state, ((unsigned int *)state->dst->f.i.rs)+1, 0);
    jl_rj(state, 12);
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 1); // 10
    jmp_imm_short(state, 10); // 2
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 0); // 10
     }
   else
     {
    int rs2 = allocate_64_register2(state, (unsigned int *)state->dst->f.i.rs);
    
    cmp_reg32_imm32(state, rs2, 0);
    jl_rj(state, 12);
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 1); // 10
    jmp_imm_short(state, 10); // 2
    mov_m32_imm32(state, (unsigned int *)(&state->branch_taken), 0); // 10
     }
}

void genbgez(usf_state_t * state)
{
#ifdef INTERPRET_BGEZ
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZ, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZ, 1);
    return;
     }
   
   genbgez_test(state);
   gendelayslot(state);
   gentest(state);
#endif
}

void genbgez_out(usf_state_t * state)
{
#ifdef INTERPRET_BGEZ_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZ_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZ_OUT, 1);
    return;
     }
   
   genbgez_test(state);
   gendelayslot(state);
   gentest_out(state);
#endif
}

void genbgez_idle(usf_state_t * state)
{
#ifdef INTERPRET_BGEZ_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZ_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZ_IDLE, 1);
    return;
     }
   
   genbgez_test(state);
   gentest_idle(state);
   genbgez(state);
#endif
}

void genbltzl(usf_state_t * state)
{
#ifdef INTERPRET_BLTZL
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZL, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZL, 1);
    return;
     }
   
   genbltz_test(state);
   free_all_registers(state);
   gentestl(state);
#endif
}

void genbltzl_out(usf_state_t * state)
{
#ifdef INTERPRET_BLTZL_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZL_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZL_OUT, 1);
    return;
     }
   
   genbltz_test(state);
   free_all_registers(state);
   gentestl_out(state);
#endif
}

void genbltzl_idle(usf_state_t * state)
{
#ifdef INTERPRET_BLTZL_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZL_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZL_IDLE, 1);
    return;
     }
   
   genbltz_test(state);
   gentest_idle(state);
   genbltzl(state);
#endif
}

void genbgezl(usf_state_t * state)
{
#ifdef INTERPRET_BGEZL
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZL, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZL, 1);
    return;
     }
   
   genbgez_test(state);
   free_all_registers(state);
   gentestl(state);
#endif
}

void genbgezl_out(usf_state_t * state)
{
#ifdef INTERPRET_BGEZL_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZL_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZL_OUT, 1);
    return;
     }
   
   genbgez_test(state);
   free_all_registers(state);
   gentestl_out(state);
#endif
}

void genbgezl_idle(usf_state_t * state)
{
#ifdef INTERPRET_BGEZL_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZL_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZL_IDLE, 1);
    return;
     }
   
   genbgez_test(state);
   gentest_idle(state);
   genbgezl(state);
#endif
}

static void genbranchlink(usf_state_t * state)
{
   int r31_64bit = is64(state, (unsigned int*)&state->reg[31]);
   
   if (!r31_64bit)
     {
    int r31 = allocate_register_w(state, (unsigned int *)&state->reg[31]);
    
    mov_reg32_imm32(state, r31, state->dst->addr+8);
     }
   else if (r31_64bit == -1)
     {
    mov_m32_imm32(state, (unsigned int *)&state->reg[31], state->dst->addr + 8);
    if (state->dst->addr & 0x80000000)
      mov_m32_imm32(state, ((unsigned int *)&state->reg[31])+1, 0xFFFFFFFF);
    else
      mov_m32_imm32(state, ((unsigned int *)&state->reg[31])+1, 0);
     }
   else
     {
    int r311 = allocate_64_register1_w(state, (unsigned int *)&state->reg[31]);
    int r312 = allocate_64_register2_w(state, (unsigned int *)&state->reg[31]);
    
    mov_reg32_imm32(state, r311, state->dst->addr+8);
    if (state->dst->addr & 0x80000000)
      mov_reg32_imm32(state, r312, 0xFFFFFFFF);
    else
      mov_reg32_imm32(state, r312, 0);
     }
}

void genbltzal(usf_state_t * state)
{
#ifdef INTERPRET_BLTZAL
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZAL, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZAL, 1);
    return;
     }
   
   genbltz_test(state);
   genbranchlink(state);
   gendelayslot(state);
   gentest(state);
#endif
}

void genbltzal_out(usf_state_t * state)
{
#ifdef INTERPRET_BLTZAL_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZAL_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZAL_OUT, 1);
    return;
     }
   
   genbltz_test(state);
   genbranchlink(state);
   gendelayslot(state);
   gentest_out(state);
#endif
}

void genbltzal_idle(usf_state_t * state)
{
#ifdef INTERPRET_BLTZAL_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZAL_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZAL_IDLE, 1);
    return;
     }
   
   genbltz_test(state);
   genbranchlink(state);
   gentest_idle(state);
   genbltzal(state);
#endif
}

void genbgezal(usf_state_t * state)
{
#ifdef INTERPRET_BGEZAL
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZAL, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZAL, 1);
    return;
     }
   
   genbgez_test(state);
   genbranchlink(state);
   gendelayslot(state);
   gentest(state);
#endif
}

void genbgezal_out(usf_state_t * state)
{
#ifdef INTERPRET_BGEZAL_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZAL_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZAL_OUT, 1);
    return;
     }
   
   genbgez_test(state);
   genbranchlink(state);
   gendelayslot(state);
   gentest_out(state);
#endif
}

void genbgezal_idle(usf_state_t * state)
{
#ifdef INTERPRET_BGEZAL_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZAL_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZAL_IDLE, 1);
    return;
     }
   
   genbgez_test(state);
   genbranchlink(state);
   gentest_idle(state);
   genbgezal(state);
#endif
}

void genbltzall(usf_state_t * state)
{
#ifdef INTERPRET_BLTZALL
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZALL, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZALL, 1);
    return;
     }
   
   genbltz_test(state);
   genbranchlink(state);
   free_all_registers(state);
   gentestl(state);
#endif
}

void genbltzall_out(usf_state_t * state)
{
#ifdef INTERPRET_BLTZALL_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZALL_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZALL_OUT, 1);
    return;
     }
   
   genbltz_test(state);
   genbranchlink(state);
   free_all_registers(state);
   gentestl_out(state);
#endif
}

void genbltzall_idle(usf_state_t * state)
{
#ifdef INTERPRET_BLTZALL_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZALL_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BLTZALL_IDLE, 1);
    return;
     }
   
   genbltz_test(state);
   genbranchlink(state);
   gentest_idle(state);
   genbltzall(state);
#endif
}

void genbgezall(usf_state_t * state)
{
#ifdef INTERPRET_BGEZALL
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZALL, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZALL, 1);
    return;
     }
   
   genbgez_test(state);
   genbranchlink(state);
   free_all_registers(state);
   gentestl(state);
#endif
}

void genbgezall_out(usf_state_t * state)
{
#ifdef INTERPRET_BGEZALL_OUT
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZALL_OUT, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZALL_OUT, 1);
    return;
     }
   
   genbgez_test(state);
   genbranchlink(state);
   free_all_registers(state);
   gentestl_out(state);
#endif
}

void genbgezall_idle(usf_state_t * state)
{
#ifdef INTERPRET_BGEZALL_IDLE
   gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZALL_IDLE, 1);
#else
   if (((state->dst->addr & 0xFFF) == 0xFFC &&
       (state->dst->addr < 0x80000000 || state->dst->addr >= 0xC0000000))||state->no_compiled_jump)
     {
    gencallinterp(state, (unsigned int)state->current_instruction_table.BGEZALL_IDLE, 1);
    return;
     }
   
   genbgez_test(state);
   genbranchlink(state);
   gentest_idle(state);
   genbgezall(state);
#endif
}
#endif
