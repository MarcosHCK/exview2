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
#include <config.h>
#include <elf_parser.h>

#define CASE(n,s) case(n):return(s);break;

const gchar* _s_elfabi(elf_abi_t abi) {
  switch(abi)
  {
    CASE(elf_abi_systemv, "System V")
    CASE(elf_abi_hpux, "HP-UX")
    CASE(elf_abi_netbsd, "NetBSD")
    CASE(elf_abi_linux, "Linux")
    CASE(elf_abi_gnuhurd, "GNU Hurd")
    CASE(elf_abi_solaris, "Solaris")
    CASE(elf_abi_aix, "AIX")
    CASE(elf_abi_irix, "IRIX")
    CASE(elf_abi_freebsd, "FreeBSD")
    CASE(elf_abi_tru64, "Tru64")
    CASE(elf_abi_novellmodesto, "Novell Modesto")
    CASE(elf_abi_openbsd, "OpenBSD")
    CASE(elf_abi_openvms, "OpenVMS")
    CASE(elf_abi_nonstop, "NonStop Kernel")
    CASE(elf_abi_aros, "AROS")
    CASE(elf_abi_fenixos, "Fenix OS")
    CASE(elf_abi_cloudabi, "CloudABI")
    CASE(elf_abi_openvos, "Stratus Technologies OpenVOS")
  }
return "System V";
}

const gchar* _s_elftype(elf_type_t type) {
  switch(type)
  {
    CASE(elf_type_none, "ET_NONE")
    CASE(elf_type_rel, "ET_REL")
    CASE(elf_type_exec, "ET_EXEC")
    CASE(elf_type_dyn, "ET_DYN")
    CASE(elf_type_core, "ET_CORE")
    default:
      if(type >= elf_type_loos && elf_type_hios >= type)
      {
        return "ET OS specific";
      } else
      if(type >= elf_type_loproc && elf_type_hiproc >= type)
      {
        return "ET processor specific";
      }
      break;
  }
return "ET_NONE";
}

const gchar* _s_elfmach(elf_machine_t type) {
  switch(type)
  {
    CASE(elf_machine_none, "NONE")
    CASE(elf_machine_m32, "M32")
    CASE(elf_machine_sparc, "SPARC")
    CASE(elf_machine_386, "386")
    CASE(elf_machine_68k, "68K")
    CASE(elf_machine_88k, "88K")
    CASE(elf_machine_860, "860")
    CASE(elf_machine_mips, "MIPS")
    CASE(elf_machine_s370, "S370")
    CASE(elf_machine_mips_rs3_le, "MIPS_RS3_LE")
    CASE(elf_machine_parisc, "PARISC")
    CASE(elf_machine_vpp500, "VPP500")
    CASE(elf_machine_sparc32plus, "SPARC32PLUS")
    CASE(elf_machine_960, "960")
    CASE(elf_machine_ppc, "PPC")
    CASE(elf_machine_ppc64, "PPC64")
    CASE(elf_machine_s390, "S390")
    CASE(elf_machine_v800, "V800")
    CASE(elf_machine_fr20, "FR20")
    CASE(elf_machine_rh32, "RH32")
    CASE(elf_machine_rce, "RCE")
    CASE(elf_machine_arm, "ARM")
    CASE(elf_machine_fake_alpha, "FAKE_ALPHA")
    CASE(elf_machine_sh, "SH")
    CASE(elf_machine_sparcv9, "SPARCV9")
    CASE(elf_machine_tricore, "TRICORE")
    CASE(elf_machine_arc, "ARC")
    CASE(elf_machine_h8_300, "H8_300")
    CASE(elf_machine_h8_300h, "H8_300H")
    CASE(elf_machine_h8s, "H8S")
    CASE(elf_machine_h8_500, "H8_500")
    CASE(elf_machine_ia_64, "IA_64")
    CASE(elf_machine_mips_x, "MIPS_X")
    CASE(elf_machine_coldfire, "COLDFIRE")
    CASE(elf_machine_68hc12, "68HC12")
    CASE(elf_machine_mma, "MMA")
    CASE(elf_machine_pcp, "PCP")
    CASE(elf_machine_ncpu, "NCPU")
    CASE(elf_machine_ndr1, "NDR1")
    CASE(elf_machine_starcore, "STARCORE")
    CASE(elf_machine_me16, "ME16")
    CASE(elf_machine_st100, "ST100")
    CASE(elf_machine_tinyj, "TINYJ")
    CASE(elf_machine_x86_64, "X86_64")
    CASE(elf_machine_pdsp, "PDSP")
    CASE(elf_machine_fx66, "FX66")
    CASE(elf_machine_st9plus, "ST9PLUS")
    CASE(elf_machine_st7, "ST7")
    CASE(elf_machine_68hc16, "68HC16")
    CASE(elf_machine_68hc11, "68HC11")
    CASE(elf_machine_68hc08, "68HC08")
    CASE(elf_machine_68hc05, "68HC05")
    CASE(elf_machine_svx, "SVX")
    CASE(elf_machine_st19, "ST19")
    CASE(elf_machine_vax, "VAX")
    CASE(elf_machine_cris, "CRIS")
    CASE(elf_machine_javelin, "JAVELIN")
    CASE(elf_machine_firepath, "FIREPATH")
    CASE(elf_machine_zsp, "ZSP")
    CASE(elf_machine_mmix, "MMIX")
    CASE(elf_machine_huany, "HUANY")
    CASE(elf_machine_prism, "PRISM")
    CASE(elf_machine_avr, "AVR")
    CASE(elf_machine_fr30, "FR30")
    CASE(elf_machine_d10v, "D10V")
    CASE(elf_machine_d30v, "D30V")
    CASE(elf_machine_v850, "V850")
    CASE(elf_machine_m32r, "M32R")
    CASE(elf_machine_mn10300, "MN10300")
    CASE(elf_machine_mn10200, "MN10200")
    CASE(elf_machine_pj, "PJ")
    CASE(elf_machine_or1k, "OR1K")
    //CASE(elf_machine_openrisc, "OPENRISC")
    CASE(elf_machine_arc_a5, "ARC_A5")
    //CASE(elf_machine_arc_compact, "ARC_COMPACT")
    CASE(elf_machine_xtensa, "XTENSA")
    CASE(elf_machine_videocore, "VIDEOCORE")
    CASE(elf_machine_tmm_gpp, "TMM_GPP")
    CASE(elf_machine_ns32k, "NS32K")
    CASE(elf_machine_tpc, "TPC")
    CASE(elf_machine_snp1k, "SNP1K")
    CASE(elf_machine_st200, "ST200")
    CASE(elf_machine_ip2k, "IP2K")
    CASE(elf_machine_max, "MAX")
    CASE(elf_machine_cr, "CR")
    CASE(elf_machine_f2mc16, "F2MC16")
    CASE(elf_machine_msp430, "MSP430")
    CASE(elf_machine_blackfin, "BLACKFIN")
    CASE(elf_machine_se_c33, "SE_C33")
    CASE(elf_machine_sep, "SEP")
    CASE(elf_machine_arca, "ARCA")
    CASE(elf_machine_unicore, "UNICORE")
    CASE(elf_machine_excess, "EXCESS")
    CASE(elf_machine_dxp, "DXP")
    CASE(elf_machine_altera_nios2, "ALTERA_NIOS2")
    CASE(elf_machine_crx, "CRX")
    CASE(elf_machine_xgate, "XGATE")
    CASE(elf_machine_c166, "C166")
    CASE(elf_machine_m16c, "M16C")
    CASE(elf_machine_dspic30f, "DSPIC30F")
    CASE(elf_machine_ce, "CE")
    CASE(elf_machine_m32c, "M32C")
    CASE(elf_machine_tsk3000, "TSK3000")
    CASE(elf_machine_rs08, "RS08")
    CASE(elf_machine_sharc, "SHARC")
    CASE(elf_machine_ecog2, "ECOG2")
    CASE(elf_machine_score7, "SCORE7")
    CASE(elf_machine_dsp24, "DSP24")
    CASE(elf_machine_videocore3, "VIDEOCORE3")
    CASE(elf_machine_latticemico32, "LATTICEMICO32")
    CASE(elf_machine_se_c17, "SE_C17")
    CASE(elf_machine_ti_c6000, "TI_C6000")
    CASE(elf_machine_ti_c2000, "TI_C2000")
    CASE(elf_machine_ti_c5500, "TI_C5500")
    CASE(elf_machine_ti_arp32, "TI_ARP32")
    CASE(elf_machine_ti_pru, "TI_PRU")
    CASE(elf_machine_mmdsp_plus, "MMDSP_PLUS")
    CASE(elf_machine_cypress_m8c, "CYPRESS_M8C")
    CASE(elf_machine_r32c, "R32C")
    CASE(elf_machine_trimedia, "TRIMEDIA")
    CASE(elf_machine_qdsp6, "QDSP6")
    CASE(elf_machine_8051, "8051")
    CASE(elf_machine_stxp7x, "STXP7X")
    CASE(elf_machine_nds32, "NDS32")
    CASE(elf_machine_ecog1x, "ECOG1X")
    CASE(elf_machine_maxq30, "MAXQ30")
    CASE(elf_machine_ximo16, "XIMO16")
    CASE(elf_machine_manik, "MANIK")
    CASE(elf_machine_craynv2, "CRAYNV2")
    CASE(elf_machine_rx, "RX")
    CASE(elf_machine_metag, "METAG")
    CASE(elf_machine_mcst_elbrus, "MCST_ELBRUS")
    CASE(elf_machine_ecog16, "ECOG16")
    CASE(elf_machine_cr16, "CR16")
    CASE(elf_machine_etpu, "ETPU")
    CASE(elf_machine_sle9x, "SLE9X")
    CASE(elf_machine_l10m, "L10M")
    CASE(elf_machine_k10m, "K10M")
    CASE(elf_machine_aarch64, "AARCH64")
    CASE(elf_machine_avr32, "AVR32")
    CASE(elf_machine_stm8, "STM8")
    CASE(elf_machine_tile64, "TILE64")
    CASE(elf_machine_tilepro, "TILEPRO")
    CASE(elf_machine_microblaze, "MICROBLAZE")
    CASE(elf_machine_cuda, "CUDA")
    CASE(elf_machine_tilegx, "TILEGX")
    CASE(elf_machine_cloudshield, "CLOUDSHIELD")
    CASE(elf_machine_corea_1st, "COREA_1ST")
    CASE(elf_machine_corea_2nd, "COREA_2ND")
    CASE(elf_machine_arc_compact2, "ARC_COMPACT2")
    CASE(elf_machine_open8, "OPEN8")
    CASE(elf_machine_rl78, "RL78")
    CASE(elf_machine_videocore5, "VIDEOCORE5")
    CASE(elf_machine_78kor, "78KOR")
    CASE(elf_machine_56800ex, "56800EX")
    CASE(elf_machine_ba1, "BA1")
    CASE(elf_machine_ba2, "BA2")
    CASE(elf_machine_xcore, "XCORE")
    CASE(elf_machine_mchp_pic, "MCHP_PIC")
    CASE(elf_machine_km32, "KM32")
    CASE(elf_machine_kmx32, "KMX32")
    CASE(elf_machine_emx16, "EMX16")
    CASE(elf_machine_emx8, "EMX8")
    CASE(elf_machine_kvarc, "KVARC")
    CASE(elf_machine_cdp, "CDP")
    CASE(elf_machine_coge, "COGE")
    CASE(elf_machine_cool, "COOL")
    CASE(elf_machine_norc, "NORC")
    CASE(elf_machine_csr_kalimba, "CSR_KALIMBA")
    CASE(elf_machine_z80, "Z80")
    CASE(elf_machine_visium, "VISIUM")
    CASE(elf_machine_ft32, "FT32")
    CASE(elf_machine_moxie, "MOXIE")
    CASE(elf_machine_amdgpu, "AMDGPU")
    CASE(elf_machine_riscv, "RISCV")
    CASE(elf_machine_bpf, "BPF")
    CASE(elf_machine_num, "NUM")
    CASE(elf_machine_alpha, "ALPHA")
  }
return "unknown";
}

const gchar* _s_elfphtype(elf_phtype_t type) {
  switch(type)
  {
    CASE(elf_phtype_null, "PT_NULL")
    CASE(elf_phtype_load, "PT_LOAD")
    CASE(elf_phtype_dynamic, "PT_DYNAMIC")
    CASE(elf_phtype_interp, "PT_INTERP")
    CASE(elf_phtype_note, "PT_NOTE")
    CASE(elf_phtype_shlib, "PT_SHLIB")
    CASE(elf_phtype_phdr, "PT_PHDR")
    CASE(elf_phtype_tls, "PT_TLS")
    default:
      if(type >= elf_phtype_loos && elf_phtype_hios >= type)
      {
        return "PT OS specific";
      } else
      if(type >= elf_phtype_loproc && elf_phtype_hiproc >= type)
      {
        return "PT processor specific";
      }
      break;
  }
return "unknown";
}

const gchar* _s_elfshtype(elf_shtype_t type) {
  switch(type)
  {
    CASE(elf_shtype_null, "SHT_NULL")
    CASE(elf_shtype_progbits, "SHT_PROGBITS")
    CASE(elf_shtype_symtab, "SHT_SYMTAB")
    CASE(elf_shtype_strtab, "SHT_STRTAB")
    CASE(elf_shtype_rela, "SHT_RELA")
    CASE(elf_shtype_hash, "SHT_HASH")
    CASE(elf_shtype_dynamic, "SHT_DYNAMIC")
    CASE(elf_shtype_note, "SHT_NOTE")
    CASE(elf_shtype_nobits, "SHT_NOBITS")
    CASE(elf_shtype_rel, "SHT_REL")
    CASE(elf_shtype_shlib, "SHT_SHLIB")
    CASE(elf_shtype_dynsym, "SHT_DYNSYM")
    CASE(elf_shtype_init_array, "SHT_INIT_ARRAY")
    CASE(elf_shtype_fini_array, "SHT_FINI_ARRAY")
    CASE(elf_shtype_preinit_array, "SHT_PREINIT_ARRAY")
    CASE(elf_shtype_group, "SHT_GROUP")
    CASE(elf_shtype_symtab_shndx, "SHT_SYMTAB_SHNDX")
    CASE(elf_shtype_num, "SHT_NUM")
    CASE(elf_shtype_gnu_attributes, "SHT_GNU_ATTRIBUTES")
    CASE(elf_shtype_gnu_hash, "SHT_GNU_HASH")
    CASE(elf_shtype_gnu_liblist, "SHT_GNU_LIBLIST")
    CASE(elf_shtype_checksum, "SHT_CHECKSUM")
    CASE(elf_shtype_sunw_move, "SHT_SUNW_move")
    CASE(elf_shtype_sunw_comdat, "SHT_SUNW_COMDAT")
    CASE(elf_shtype_sunw_syminfo, "SHT_SUNW_syminfo")
    CASE(elf_shtype_gnu_verdef, "SHT_GNU_verdef")
    CASE(elf_shtype_gnu_verneed, "SHT_GNU_verneed")
    CASE(elf_shtype_gnu_versym, "SHT_GNU_versym")
    default:
      if(type >= elf_shtype_loos && elf_shtype_hios >= type)
      {
        return "SHT OS specific";
      } else
      if(type >= elf_shtype_losunw && elf_shtype_hisunw >= type)
      {
        return "SHT_Sun specific";
      } else
      if(type >= elf_shtype_loproc && elf_shtype_hiproc >= type)
      {
        return "SHT processor specific";
      }
      if(type >= elf_shtype_louser && elf_shtype_hiuser >= type)
      {
        return "SHT User specific";
      }
      break;
  }
return "SHT_NULL";
}

const gchar* _s_elfsbind(elf_bind_type_t type) {
  switch(type)
  {
    CASE(elf_bind_local, "LOCAL")
    CASE(elf_bind_global, "GLOBAL")
    CASE(elf_bind_weak, "WEAK")
    CASE(elf_bind_num, "NUM")
    CASE(elf_bind_gnu_unic, "GNU_UNIC")
    default:
      if(type >= elf_bind_loos && elf_bind_hios >= type)
      {
        return "OS specific";
      } else
      if(type >= elf_bind_loproc && elf_bind_hiproc >= type)
      {
        return "processor specific";
      }
      break;
  }
return "";
}

const gchar* _s_elfstype(elf_sym_type_t type) {
  switch(type)
  {
    CASE(elf_sym_notype, "NOTYPE")
    CASE(elf_sym_object, "OBJECT")
    CASE(elf_sym_func, "FUNC")
    CASE(elf_sym_section, "SECTION")
    CASE(elf_sym_file, "FILE")
    CASE(elf_sym_common, "COMMON")
    CASE(elf_sym_tls, "TLS")
    CASE(elf_sym_num, "NUM")
    CASE(elf_sym_gnu_ifunc, "GNU_IFUNC")
    default:
      if(type >= elf_sym_loos && elf_sym_hios >= type)
      {
        return "OS specific";
      } else
      if(type >= elf_sym_loproc && elf_sym_hiproc >= type)
      {
        return "processor specific";
      }
      break;
  }
return "";
}

const gchar* _s_elfsvis(elf_sym_vis_t type) {
  switch(type)
  {
    CASE(elf_sym_vis_default, "DEFAULT")
    CASE(elf_sym_vis_internal, "INTERNAL")
    CASE(elf_sym_vis_hidden, "HIDDEN")
    CASE(elf_sym_vis_protected, "PROTECTED")
  }
return "unknown";
}

const gchar* _s_elfdynt(elf_dyn_type_t type) {
  switch(type)
  {
    CASE(elf_dyn_null, "DT_NULL")
    CASE(elf_dyn_needed, "DT_NEEDED")
    CASE(elf_dyn_pltrelsz, "DT_PLTRELSZ")
    CASE(elf_dyn_pltgot, "DT_PLTGOT")
    CASE(elf_dyn_hash, "DT_HASH")
    CASE(elf_dyn_strtab, "DT_STRTAB")
    CASE(elf_dyn_symtab, "DT_SYMTAB")
    CASE(elf_dyn_rela, "DT_RELA")
    CASE(elf_dyn_relasz, "DT_RELASZ")
    CASE(elf_dyn_relaent, "DT_RELAENT")
    CASE(elf_dyn_strsz, "DT_STRSZ")
    CASE(elf_dyn_syment, "DT_SYMENT")
    CASE(elf_dyn_init, "DT_INIT")
    CASE(elf_dyn_fini, "DT_FINI")
    CASE(elf_dyn_soname, "DT_SONAME")
    CASE(elf_dyn_rpath, "DT_RPATH")
    CASE(elf_dyn_symbolic, "DT_SYMBOLIC")
    CASE(elf_dyn_rel, "DT_REL")
    CASE(elf_dyn_relsz, "DT_RELSZ")
    CASE(elf_dyn_relent, "DT_RELENT")
    CASE(elf_dyn_pltrel, "DT_PLTREL")
    CASE(elf_dyn_debug, "DT_DEBUG")
    CASE(elf_dyn_textrel, "DT_TEXTREL")
    CASE(elf_dyn_jmprel, "DT_JMPREL")
  }
return "unknown";
}
