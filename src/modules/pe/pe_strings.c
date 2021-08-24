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
#include <pe_parser.h>
#include <stdio.h>

#define CASE(n,s) case(n):return(s);break;

const gchar* _s_pever(uint16_t optmagic) {
  switch(optmagic)
  {
    CASE(0x10b,  "PE")
    CASE(0x20b, "PE+")
  }
return "";
}

const gchar* _s_pearch(pe_arch_t arch) {
  switch(arch)
  {
    CASE(pe_arch_unknown, "unknown")
    CASE(pe_arch_i386, "i386")
    CASE(pe_arch_r3000, "r3000")
    CASE(pe_arch_r4000, "r4000")
    CASE(pe_arch_r10000, "r10000")
    CASE(pe_arch_wcemipsv2, "wcemipsv2")
    CASE(pe_arch_alpha, "alpha")
    CASE(pe_arch_sh3, "sh3")
    CASE(pe_arch_sh3dsp, "sh3dsp")
    CASE(pe_arch_sh3e, "sh3e")
    CASE(pe_arch_sh4, "sh4")
    CASE(pe_arch_sh5, "sh5")
    CASE(pe_arch_arm, "arm")
    //CASE(pe_arch_armv7, "armv7")
    CASE(pe_arch_armnt, "armnt")
    CASE(pe_arch_arm64, "arm64")
    CASE(pe_arch_thumb, "thumb")
    CASE(pe_arch_am33, "am33")
    CASE(pe_arch_powerpc, "powerpc")
    CASE(pe_arch_powerpcfp, "powerpcfp")
    CASE(pe_arch_ia64, "ia64")
    CASE(pe_arch_mips16, "mips16")
    CASE(pe_arch_alpha64, "alpha64")
    CASE(pe_arch_mipsfpu, "mipsfpu")
    CASE(pe_arch_mipsfpu16, "mipsfpu16")
    //CASE(pe_arch_axp64, "axp64")
    CASE(pe_arch_tricore, "tricore")
    CASE(pe_arch_cef, "cef")
    CASE(pe_arch_ebc, "EFI byte code")
    CASE(pe_arch_amd64, "amd64")
    CASE(pe_arch_m32r, "m32r")
    CASE(pe_arch_cee, "cee")
  }
return "unknown";
}

const gchar* _s_petimestamp(uint32_t timestamp) {
  time_t time = timestamp;
  struct tm stm;

  static
  char _s_petimebuff[64];
#ifdef __STDC_LIB_EXT1__
# if _WIN32
    localtime_s(&stm, &time);
# else
    localtime_s(&time, &stm);
# endif // _WIN32
#else
  localtime_r(&time, &stm);
#endif // __STDC_LIB_EXT1__

  strftime(_s_petimebuff, 64, "%a %b %d %H:%M:%S %Y", &stm);
return _s_petimebuff;
}

#define IFSEPA "\r\n"

#define IF(c,s) \
if((c)) \
{ \
  g_string_append(return_, (s)); \
  g_string_append(return_, IFSEPA); \
}
#define IFAND(n,s) IF(flags&n,(s))

gchar* _s_pechars(pe_flag_t flags) {
  GString* return_ =
  g_string_new(NULL);

  IFAND(        pe_flag_relocs_stripped, "RELOCS_STRIPPED");
  IFAND(       pe_flag_executable_image, "EXECUTABLE_IMAGE");
  IFAND(     pe_flag_line_nums_stripped, "LINE_NUMS_STRIPPED");
  IFAND(    pe_flag_local_syms_stripped, "LOCAL_SYMS_STRIPPED");
  IFAND(      pe_flag_aggresive_ws_trim, "AGGRESIVE_WS_TRIM");
  IFAND(    pe_flag_large_address_aware, "LARGE_ADDRESS_AWARE");
  IFAND(      pe_flag_bytes_reversed_lo, "BYTES_REVERSED_LO");
  IFAND(          pe_flag_32bit_machine, "32BIT_MACHINE");
  IFAND(         pe_flag_debug_stripped, "DEBUG_STRIPPED");
  IFAND(pe_flag_removable_run_from_swap, "REMOVABLE_RUN_FROM_SWAP");
  IFAND(      pe_flag_net_run_from_swap, "NET_RUN_FROM_SWAP");
  IFAND(                 pe_flag_system, "SYSTEM");
  IFAND(                    pe_flag_dll, "DLL");
  IFAND(         pe_flag_up_system_only, "UP_SYSTEM_ONLY");
  IFAND(      pe_flag_bytes_reversed_hi, "BYTES_REVERSED_HI");
return g_string_free(return_, FALSE);
}

const gchar* _s_pesubsystem(pe_subsystem_t subsystem) {
  switch(subsystem)
  {
    CASE(pe_subsystem_unknown,"unknown")
    CASE(pe_subsystem_native,"Native")
    CASE(pe_subsystem_windows_gui,"Windows GUI")
    CASE(pe_subsystem_windows_cui,"Windows CUI")
    CASE(pe_subsystem_os2_cui,"OS2 CUI")
    CASE(pe_subsystem_posix_cui,"Posix CUI")
    CASE(pe_subsystem_native_windows,"Native windows")
    CASE(pe_subsystem_windows_ce_gui,"WindowsCE GUI")
    CASE(pe_subsystem_efi_application,"EFI application")
    CASE(pe_subsystem_efi_boot_service_driver,"EFI service driver")
    CASE(pe_subsystem_efi_runtime_driver,"EFI runtime driver")
    CASE(pe_subsystem_efi_rom,"EFI ROM")
    CASE(pe_subsystem_xbox,"Xbox")
    CASE(pe_subsystem_windows_boot_application,"Windows boot")
  }
return "";
}

gchar* _s_pedllchars(pe_dllflags_t flags) {
  GString* return_ =
  g_string_new(NULL);

  IFAND(      pe_dllflags_high_entropy_va, "High entropy base randomization");
  IFAND(         pe_dllflags_dynamic_base, "Dynamic base");
  IFAND(      pe_dllflags_force_integrity, "Forced integrity checks");
  IFAND(            pe_dllflags_nx_compat, "NX compatible");
  IFAND(         pe_dllflags_no_isolation, "Do not isolate the image");
  IFAND(               pe_dllflags_no_seh, "Does not use SEH");
  IFAND(              pe_dllflags_no_bind, "Do not bind");
  IFAND(         pe_dllflags_appcontainer, "Must execute in app container");
  IFAND(           pe_dllflags_wdm_driver, "WDM driver");
  IFAND(             pe_dllflags_guard_cf, "Control Flow Guard compatible");
  IFAND(pe_dllflags_terminal_server_aware, "Terminal server aware");
return g_string_free(return_, FALSE);
}

const gchar* _s_peddirname(int n) {
  const gchar* names[] =
  {
    "IMAGE_DIRECTORY_ENTRY_EXPORT",
    "IMAGE_DIRECTORY_ENTRY_IMPORT",
    "IMAGE_DIRECTORY_ENTRY_RESOURCE",
    "IMAGE_DIRECTORY_ENTRY_EXCEPTION",
    "IMAGE_DIRECTORY_ENTRY_SECURITY",
    "IMAGE_DIRECTORY_ENTRY_BASERELOC",
    "IMAGE_DIRECTORY_ENTRY_DEBUG",
    "IMAGE_DIRECTORY_ENTRY_ARCHITECTURE",
    "IMAGE_DIRECTORY_ENTRY_GLOBALPTR",
    "IMAGE_DIRECTORY_ENTRY_TLS",
    "IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG0",
    "IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT",
    "IMAGE_DIRECTORY_ENTRY_IAT",
    "IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT",
    "IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR",
    "IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR",
  };
return names[n];
}

#define ICASE(n,s) \
case (n): \
  g_string_append(return_, (s)); \
  g_string_append(return_, IFSEPA); \
  break;

const gchar* _s_pesectchars(pe_section_flags_t flags) {
  GString* return_ =
  g_string_new(NULL);

  IFAND(pe_section_cnt_code, "Contains code");
  IFAND(pe_section_cnt_initialized_data, "Contains initialized data");
  IFAND(pe_section_cnt_uninitialized_data, "Contains uninitialized data");
  //IFAND(pe_section_lnk_other, "reserved");
  IFAND(pe_section_lnk_info, "Contains link information");
  IFAND(pe_section_lnk_remove, "Discarded at link");
  IFAND(pe_section_lnk_comdat, "Contains CAMDAT information");
  IFAND(pe_section_no_defer_spec_exc, "Reset speculative exceptions handling bits in the TLB entries");
  IFAND(pe_section_gprel, "Contains data referenced through global pointer");
  //IFAND(pe_section_mem_fardata, "reserved");
  //IFAND(pe_section_mem_purgeable, "reserved");
  //IFAND(pe_section_mem_16bit, "reserved");
  //IFAND(pe_section_mem_locked, "reserved");
  //IFAND(pe_section_mem_preload, "reserved");

  switch(flags & pe_section_align_mask)
  {
    ICASE(pe_section_align_1bytes, "Alignment is 1 bytes");
    ICASE(pe_section_align_2bytes, "Alignment is 2 bytes");
    ICASE(pe_section_align_4bytes, "Alignment is 4 bytes");
    ICASE(pe_section_align_8bytes, "Alignment is 8 bytes");
    ICASE(pe_section_align_16bytes, "Alignment is 16 bytes");
    ICASE(pe_section_align_32bytes, "Alignment is 32 bytes");
    ICASE(pe_section_align_64bytes, "Alignment is 64 bytes");
    ICASE(pe_section_align_128bytes, "Alignment is 128 bytes");
    ICASE(pe_section_align_256bytes, "Alignment is 256 bytes");
    ICASE(pe_section_align_512bytes, "Alignment is 512 bytes");
    ICASE(pe_section_align_1024bytes, "Alignment is 1024 bytes");
    ICASE(pe_section_align_2048bytes, "Alignment is 2048 bytes");
    ICASE(pe_section_align_4096bytes, "Alignment is 4096 bytes");
    ICASE(pe_section_align_8192bytes, "Alignment is 8192 bytes");
  }

  IFAND(pe_section_lnk_nreloc_ovfl, "Contains extended relocations");
  IFAND(pe_section_mem_discardable, "Contains discard-able data");
  IFAND(pe_section_mem_not_cached, "Can not be cached");
  IFAND(pe_section_mem_not_paged, "Can not be paged");
  IFAND(pe_section_mem_shared, "Contains shared data");
  IFAND(pe_section_mem_execute, "Is executable");
  IFAND(pe_section_mem_read, "Is read-able");
  IFAND(pe_section_mem_write, "Is write-able");
return g_string_free(return_, FALSE);
}

const gchar* _s_peresid(pe_resource_id_t id) {
  switch(id)
  {
    CASE(pe_resource_id_cursor, "CURSOR")
    CASE(pe_resource_id_bitmap, "BITMAP")
    CASE(pe_resource_id_icon, "ICON")
    CASE(pe_resource_id_menu, "MENU")
    CASE(pe_resource_id_dialog, "DIALOG")
    CASE(pe_resource_id_string, "STRING")
    CASE(pe_resource_id_fontdir, "FONTDIR")
    CASE(pe_resource_id_font, "FONT")
    CASE(pe_resource_id_accelerator, "ACCELERATOR")
    CASE(pe_resource_id_rcdata, "RCDATA")
    CASE(pe_resource_id_messagetable, "MESSAGETABLE")
    CASE(pe_resource_id_group_cursor, "GROUP_CURSOR")
    CASE(pe_resource_id_group_icon, "GROUP_ICON")
    CASE(pe_resource_id_version, "VERSION")
    CASE(pe_resource_id_dlginclude, "DLGINCLUDE")
    CASE(pe_resource_id_plugplay, "PLUGPLAY")
    CASE(pe_resource_id_vxd, "VXD")
    CASE(pe_resource_id_anicursor, "ANICURSOR")
    CASE(pe_resource_id_aniicon, "ANIICON")
    CASE(pe_resource_id_html, "HTML")
    CASE(pe_resource_id_manifest, "MANIFEST")
  }
return NULL;
}

#undef IFSEPA
#define IFSEPA ","

gchar* _s_pefileveros(pe_filever_os_t flags) {
  GString* return_ =
  g_string_new(NULL);

  IFAND(pe_filever_os_dos, "VOS_DOS")
  IFAND(pe_filever_os_nt, "VOS_NT")
  IFAND(pe_filever_os__windows16, "VOS__WINDOWS16")
  IFAND(pe_filever_os__windows32, "VOS__WINDOWS32")
  IFAND(pe_filever_os_os216, "VOS_OS216")
  IFAND(pe_filever_os_os232, "VOS_OS232")
  IFAND(pe_filever_os__pm16, "VOS__PM16")
  IFAND(pe_filever_os__pm32, "VOS__PM32")

  if(flags == pe_filever_os_unknown)
  {
    g_string_append(return_, "VOS_UNKNOWN");
  }
  if(return_->len == 0)
  {
    g_string_printf(return_, "0x%x", flags);
  }
return g_string_free(return_, FALSE);
}

#undef IFSEPA
#define IFSEPA "\n\t"

const gchar* _s_pefilevertype(pe_filever_type_t type) {
  static
  gchar _s_pefileverbuf[64];

  switch(type)
  {
    CASE(pe_filever_type_unknown, "VFT_UNKNOWN")
    CASE(pe_filever_type_app, "VFT_APP")
    CASE(pe_filever_type_dll, "VFT_DLL")
    CASE(pe_filever_type_drv, "VFT_DRV")
    CASE(pe_filever_type_font, "VFT_FONT")
    CASE(pe_filever_type_vxd, "VFT_VXD")
    CASE(pe_filever_type_static_lib, "VFT_STATIC_LIB")
  }

  sprintf(_s_pefileverbuf, "0x%x", type);
return _s_pefileverbuf;
}
