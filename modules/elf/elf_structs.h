/*  Copyright 2021-2022 MarcosHCK
 *  This file is part of exview2.
 *
 *  exview2 is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  exview2 is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with exview2. If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __ELF_STRUCTS_INCLUDED__
#define __ELF_STRUCTS_INCLUDED__ 1
#include <stdint.h>

typedef uint64_t vaddr_t;

#pragma pack(push, 1)
#define PACKED \
__attribute__((packed, aligned(1)))

typedef struct __elf_header32 elf_header32_t;
struct __elf_header32 {
    uint8_t e_ident[4];
    uint8_t bitlen;
    uint8_t endianness;
    uint8_t tiny_ver;
    uint8_t abi;
    uint8_t abiver;
    uint8_t padding[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

typedef struct __elf_header64 elf_header64_t;
struct __elf_header64 {
    uint8_t e_ident[4];
    uint8_t bitlen;
    uint8_t endianess;
    uint8_t tiny_ver;
    uint8_t abi;
    uint8_t abiver;
    uint8_t padding[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} PACKED;

typedef enum {
  elf_bitlen_32 = 1,
  elf_bitlen_64 = 2,
} elf_bitlen_t;

typedef enum {
    elf_abi_systemv,
    elf_abi_hpux,
    elf_abi_netbsd,
    elf_abi_linux,
    elf_abi_gnuhurd,
    elf_abi_solaris = 6,
    elf_abi_aix,
    elf_abi_irix,
    elf_abi_freebsd,
    elf_abi_tru64,
    elf_abi_novellmodesto,
    elf_abi_openbsd,
    elf_abi_openvms,
    elf_abi_nonstop,
    elf_abi_aros,
    elf_abi_fenixos,
    elf_abi_cloudabi,
    elf_abi_openvos,
} elf_abi_t;

typedef enum {
    elf_type_none,
    elf_type_rel,
    elf_type_exec,
    elf_type_dyn,
    elf_type_core,
    elf_type_loos = 0xfe00,
    elf_type_hios = 0xfeff,
    elf_type_loproc = 0xff00,
    elf_type_hiproc = 0xffff,
} elf_type_t;

typedef enum {
    elf_machine_none,
    elf_machine_m32,
    elf_machine_sparc,
    elf_machine_386,
    elf_machine_68k,
    elf_machine_88k,
    elf_machine_860 = 7,
    elf_machine_mips,
    elf_machine_s370,
    elf_machine_mips_rs3_le,
    elf_machine_parisc = 15,
    elf_machine_vpp500 = 17,
    elf_machine_sparc32plus,
    elf_machine_960,
    elf_machine_ppc,
    elf_machine_ppc64,
    elf_machine_s390,
    elf_machine_v800 = 36,
    elf_machine_fr20,
    elf_machine_rh32,
    elf_machine_rce,
    elf_machine_arm,
    elf_machine_fake_alpha,
    elf_machine_sh,
    elf_machine_sparcv9,
    elf_machine_tricore,
    elf_machine_arc,
    elf_machine_h8_300,
    elf_machine_h8_300h,
    elf_machine_h8s,
    elf_machine_h8_500,
    elf_machine_ia_64,
    elf_machine_mips_x,
    elf_machine_coldfire,
    elf_machine_68hc12,
    elf_machine_mma,
    elf_machine_pcp,
    elf_machine_ncpu,
    elf_machine_ndr1,
    elf_machine_starcore,
    elf_machine_me16,
    elf_machine_st100,
    elf_machine_tinyj,
    elf_machine_x86_64,
    elf_machine_pdsp,
    elf_machine_fx66 = 66,
    elf_machine_st9plus,
    elf_machine_st7,
    elf_machine_68hc16,
    elf_machine_68hc11,
    elf_machine_68hc08,
    elf_machine_68hc05,
    elf_machine_svx,
    elf_machine_st19,
    elf_machine_vax,
    elf_machine_cris,
    elf_machine_javelin,
    elf_machine_firepath,
    elf_machine_zsp,
    elf_machine_mmix,
    elf_machine_huany,
    elf_machine_prism,
    elf_machine_avr,
    elf_machine_fr30,
    elf_machine_d10v,
    elf_machine_d30v,
    elf_machine_v850,
    elf_machine_m32r,
    elf_machine_mn10300,
    elf_machine_mn10200,
    elf_machine_pj,
    elf_machine_or1k,
    elf_machine_openrisc = 92,
    elf_machine_arc_a5,
    elf_machine_arc_compact = 93,
    elf_machine_xtensa,
    elf_machine_videocore,
    elf_machine_tmm_gpp,
    elf_machine_ns32k,
    elf_machine_tpc,
    elf_machine_snp1k,
    elf_machine_st200,
    elf_machine_ip2k,
    elf_machine_max,
    elf_machine_cr,
    elf_machine_f2mc16,
    elf_machine_msp430,
    elf_machine_blackfin,
    elf_machine_se_c33,
    elf_machine_sep,
    elf_machine_arca,
    elf_machine_unicore,
    elf_machine_excess,
    elf_machine_dxp,
    elf_machine_altera_nios2,
    elf_machine_crx,
    elf_machine_xgate,
    elf_machine_c166,
    elf_machine_m16c,
    elf_machine_dspic30f,
    elf_machine_ce,
    elf_machine_m32c,
    elf_machine_tsk3000 = 131,
    elf_machine_rs08,
    elf_machine_sharc,
    elf_machine_ecog2,
    elf_machine_score7,
    elf_machine_dsp24,
    elf_machine_videocore3,
    elf_machine_latticemico32,
    elf_machine_se_c17,
    elf_machine_ti_c6000,
    elf_machine_ti_c2000,
    elf_machine_ti_c5500,
    elf_machine_ti_arp32,
    elf_machine_ti_pru,
    elf_machine_mmdsp_plus = 160,
    elf_machine_cypress_m8c,
    elf_machine_r32c,
    elf_machine_trimedia,
    elf_machine_qdsp6,
    elf_machine_8051,
    elf_machine_stxp7x,
    elf_machine_nds32,
    elf_machine_ecog1x,
    elf_machine_maxq30,
    elf_machine_ximo16,
    elf_machine_manik,
    elf_machine_craynv2,
    elf_machine_rx,
    elf_machine_metag,
    elf_machine_mcst_elbrus,
    elf_machine_ecog16,
    elf_machine_cr16,
    elf_machine_etpu,
    elf_machine_sle9x,
    elf_machine_l10m,
    elf_machine_k10m,
    elf_machine_aarch64 = 183,
    elf_machine_avr32 = 185,
    elf_machine_stm8,
    elf_machine_tile64,
    elf_machine_tilepro,
    elf_machine_microblaze,
    elf_machine_cuda,
    elf_machine_tilegx,
    elf_machine_cloudshield,
    elf_machine_corea_1st,
    elf_machine_corea_2nd,
    elf_machine_arc_compact2,
    elf_machine_open8,
    elf_machine_rl78,
    elf_machine_videocore5,
    elf_machine_78kor,
    elf_machine_56800ex,
    elf_machine_ba1,
    elf_machine_ba2,
    elf_machine_xcore,
    elf_machine_mchp_pic,
    elf_machine_km32 = 210,
    elf_machine_kmx32,
    elf_machine_emx16,
    elf_machine_emx8,
    elf_machine_kvarc,
    elf_machine_cdp,
    elf_machine_coge,
    elf_machine_cool,
    elf_machine_norc,
    elf_machine_csr_kalimba,
    elf_machine_z80,
    elf_machine_visium,
    elf_machine_ft32,
    elf_machine_moxie,
    elf_machine_amdgpu,
    elf_machine_riscv = 243,
    elf_machine_bpf = 247,
    elf_machine_num,
    elf_machine_alpha = 36902,
} elf_machine_t;

typedef struct __elf_phent32 elf_phent32_t;
struct __elf_phent32 {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
} PACKED;

typedef struct __elf_phent64 elf_phent64_t;
struct __elf_phent64 {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} PACKED;

typedef enum {
    elf_phtype_null,
    elf_phtype_load,
    elf_phtype_dynamic,
    elf_phtype_interp,
    elf_phtype_note,
    elf_phtype_shlib,
    elf_phtype_phdr,
    elf_phtype_tls,
    elf_phtype_loos = 0x60000000,
    elf_phtype_hios = 0x6fffffff,
    elf_phtype_loproc = 0x70000000,
    elf_phtype_hiproc = 0x7fffffff,
} elf_phtype_t;

typedef struct __elf_shent32 elf_shent32_t;
struct __elf_shent32 {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
} PACKED;

typedef struct __elf_shent64 elf_shent64_t;
struct __elf_shent64 {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} PACKED;

typedef enum {
    elf_shtype_null,
    elf_shtype_progbits,
    elf_shtype_symtab,
    elf_shtype_strtab,
    elf_shtype_rela,
    elf_shtype_hash,
    elf_shtype_dynamic,
    elf_shtype_note,
    elf_shtype_nobits,
    elf_shtype_rel,
    elf_shtype_shlib,
    elf_shtype_dynsym,
    elf_shtype_init_array = 14,
    elf_shtype_fini_array,
    elf_shtype_preinit_array,
    elf_shtype_group,
    elf_shtype_symtab_shndx,
    elf_shtype_num,
    elf_shtype_loos = 0x60000000,
    elf_shtype_gnu_attributes = 0x6ffffff5,
    elf_shtype_gnu_hash,
    elf_shtype_gnu_liblist,
    elf_shtype_checksum,
    elf_shtype_losunw = 0x6ffffffa,
    elf_shtype_sunw_move = 0x6ffffffa,
    elf_shtype_sunw_comdat,
    elf_shtype_sunw_syminfo,
    elf_shtype_gnu_verdef,
    elf_shtype_gnu_verneed,
    elf_shtype_gnu_versym,
    elf_shtype_hisunw = 0x6fffffff,
    elf_shtype_hios = 0x6fffffff,
    elf_shtype_loproc = 0x70000000,
    elf_shtype_hiproc = 0x7fffffff,
    elf_shtype_louser = 0x80000000,
    elf_shtype_hiuser = 0x8fffffff,
} elf_shtype_t;

typedef struct __elf_sym32 elf_sym32_t;
struct __elf_sym32 {
  uint32_t st_name;
  uint32_t st_value;
  uint32_t st_size;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
} PACKED;

typedef struct __elf_sym64 elf_sym64_t;
struct __elf_sym64 {
  uint32_t st_name;
  uint8_t st_info;
  uint8_t st_other;
  uint16_t st_shndx;
  uint64_t st_value;
  uint64_t st_size;
} PACKED;

typedef enum {
    elf_bind_local,
    elf_bind_global,
    elf_bind_weak,
    elf_bind_num,
    elf_bind_loos = 0x10,
    elf_bind_gnu_unic = 0x10,
    elf_bind_hios = 0x12,
    elf_bind_loproc,
    elf_bind_hiproc = 0x15,
} elf_bind_type_t;

typedef enum {
    elf_sym_notype,
    elf_sym_object,
    elf_sym_func,
    elf_sym_section,
    elf_sym_file,
    elf_sym_common,
    elf_sym_tls,
    elf_sym_num,
    elf_sym_loos = 0x10,
    elf_sym_gnu_ifunc = 0x10,
    elf_sym_hios = 0x12,
    elf_sym_loproc,
    elf_sym_hiproc = 0x15,
} elf_sym_type_t;

typedef enum {
    elf_sym_vis_default,
    elf_sym_vis_internal,
    elf_sym_vis_hidden,
    elf_sym_vis_protected,
} elf_sym_vis_t;

typedef struct __elf_dyn32 elf_dyn32_t;
struct __elf_dyn32 {
    int32_t d_tag;
    union {
        uint32_t d_val;
        uint32_t d_ptr;
    };
} PACKED;

typedef struct __elf_dyn64 elf_dyn64_t;
struct __elf_dyn64 {
    int64_t d_tag;
    union {
        uint64_t d_val;
        uint64_t d_ptr;
    };
} PACKED;

typedef enum {
    elf_dyn_null,
    elf_dyn_needed,
    elf_dyn_pltrelsz,
    elf_dyn_pltgot,
    elf_dyn_hash,
    elf_dyn_strtab,
    elf_dyn_symtab,
    elf_dyn_rela,
    elf_dyn_relasz,
    elf_dyn_relaent,
    elf_dyn_strsz,
    elf_dyn_syment,
    elf_dyn_init,
    elf_dyn_fini,
    elf_dyn_soname,
    elf_dyn_rpath,
    elf_dyn_symbolic,
    elf_dyn_rel,
    elf_dyn_relsz,
    elf_dyn_relent,
    elf_dyn_pltrel,
    elf_dyn_debug,
    elf_dyn_textrel,
    elf_dyn_jmprel,
} elf_dyn_type_t;

#pragma pack(pop)

#endif // __ELF_STRUCTS_INCLUDED__
