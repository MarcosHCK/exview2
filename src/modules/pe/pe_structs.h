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
#ifndef __PE_STRUCTS_INCLUDED__
#define __PE_STRUCTS_INCLUDED__ 1
#include <stdint.h>

typedef uint64_t vaddr_t;

#pragma pack(push, 1)
#define PACKED \
__attribute__((packed, aligned(1)))

typedef struct __pe_dos_header pe_dos_header_t;
struct __pe_dos_header {
  uint16_t e_magic;
  uint16_t e_cblp;
  uint16_t e_cp;
  uint16_t e_crlc;
  uint16_t e_cparhdr;
  uint16_t e_minalloc;
  uint16_t e_maxalloc;
  uint16_t e_ss;
  uint16_t e_sp;
  uint16_t e_csum;
  uint16_t e_ip;
  uint16_t e_cs;
  uint16_t e_lfarlc;
  uint16_t e_ovno;
  uint16_t e_res[4];
  uint16_t e_oemid;
  uint16_t e_oeminfo;
  uint16_t e_res2[10];
  uint32_t e_lfanew;
} PACKED;

typedef struct __pe_sign pe_sign_t;
struct __pe_sign {
  uint8_t Magic[2]; /* PE */
  uint8_t zeroed[2];
} PACKED;

typedef struct __pe_file_header pe_file_header_t;
struct __pe_file_header {
  uint16_t Arch;
  uint16_t NumberOfSections;
  uint32_t TimeDateStamp;
  uint32_t PointerToSymbolTable;
  uint32_t NumberOfSymbols;
  uint16_t SizeOfOptionalHeader;
  uint16_t Characteristics;
} PACKED;

#define pe_file_header_t_sz 20

typedef enum {
  pe_flag_relocs_stripped = 0x0001,
  pe_flag_executable_image = 0x0002,
  pe_flag_line_nums_stripped = 0x0004,
  pe_flag_local_syms_stripped = 0x0008,
  pe_flag_aggresive_ws_trim = 0x0010,
  pe_flag_large_address_aware = 0x0020,
  pe_flag_bytes_reversed_lo = 0x0080,
  pe_flag_32bit_machine = 0x0100,
  pe_flag_debug_stripped = 0x0200,
  pe_flag_removable_run_from_swap = 0x0400,
  pe_flag_net_run_from_swap = 0x0800,
  pe_flag_system = 0x1000,
  pe_flag_dll = 0x2000,
  pe_flag_up_system_only = 0x4000,
  pe_flag_bytes_reversed_hi = 0x8000,
} pe_flag_t;

typedef enum {
  pe_arch_unknown = 0,
  pe_arch_i386 = 0x014c,
  pe_arch_r3000 = 0x0162,
  pe_arch_r4000 = 0x0166,
  pe_arch_r10000 = 0x0168,
  pe_arch_wcemipsv2 = 0x0169,
  pe_arch_alpha = 0x0184,
  pe_arch_sh3 = 0x01a2,
  pe_arch_sh3dsp = 0x01a3,
  pe_arch_sh3e = 0x01a4,
  pe_arch_sh4 = 0x01a6,
  pe_arch_sh5 = 0x01a8,
  pe_arch_arm = 0x01c0,
  pe_arch_armv7 = 0x01c4,
  pe_arch_armnt = 0x01c4,
  pe_arch_arm64 = 0xaa64,
  pe_arch_thumb = 0x01c2,
  pe_arch_am33 = 0x01d3,
  pe_arch_powerpc = 0x01f0,
  pe_arch_powerpcfp = 0x01f1,
  pe_arch_ia64 = 0x0200,
  pe_arch_mips16 = 0x0266,
  pe_arch_alpha64 = 0x0284,
  pe_arch_mipsfpu = 0x0366,
  pe_arch_mipsfpu16 = 0x0466,
  pe_arch_axp64 = pe_arch_alpha64,
  pe_arch_tricore = 0x0520,
  pe_arch_cef = 0x0cef,
  pe_arch_ebc = 0x0ebc,
  pe_arch_amd64 = 0x8664,
  pe_arch_m32r = 0x9041,
  pe_arch_cee = 0xc0ee,
} pe_arch_t;

typedef struct __pe_header pe_header_t;
struct __pe_header {
  struct __pe_sign sign;
  struct __pe_file_header header;
} PACKED;

typedef struct __pe_data_directory pe_data_directory_t;
struct __pe_data_directory {
  uint32_t VirtualAddress;
  uint32_t Size;
} PACKED;

typedef enum {
  pe_bitlen_32,
  pe_bitlen_64,
} pe_bitlen_t;

#define pe_numberof_data_directories 16

typedef struct __pe_optional_header32 pe_optional_header32_t;
struct __pe_optional_header32 {
  uint16_t Magic;
  uint8_t MajorLinkerVersion;
  uint8_t MinorLinkerVersion;
  uint32_t SizeOfCode;
  uint32_t SizeOfInitializedData;
  uint32_t SizeOfUninitializedData;
  uint32_t AddressOfEntryPoint;
  uint32_t BaseOfCode;
  uint32_t BaseOfData;
  uint32_t ImageBase;
  uint32_t SectionAlignment;
  uint32_t FileAlignment;
  uint16_t MajorOperatingSystemVersion;
  uint16_t MinorOperatingSystemVersion;
  uint16_t MajorImageVersion;
  uint16_t MinorImageVersion;
  uint16_t MajorSubsystemVersion;
  uint16_t MinorSubsystemVersion;
  uint32_t Win32VersionValue;
  uint32_t SizeOfImage;
  uint32_t SizeOfHeaders;
  uint32_t CheckSum;
  uint16_t Subsystem;
  uint16_t DllCharacteristics;
  uint32_t SizeOfStackReserve;
  uint32_t SizeOfStackCommit;
  uint32_t SizeOfHeapReserve;
  uint32_t SizeOfHeapCommit;
  uint32_t LoaderFlags;
  uint32_t NumberOfRvaAndSizes;
  struct __pe_data_directory DataDirectory[pe_numberof_data_directories];
} PACKED;

typedef struct __pe_optional_header64 pe_optional_header64_t;
struct __pe_optional_header64 {
  uint16_t Magic;
  uint8_t MajorLinkerVersion;
  uint8_t MinorLinkerVersion;
  uint32_t SizeOfCode;
  uint32_t SizeOfInitializedData;
  uint32_t SizeOfUninitializedData;
  uint32_t AddressOfEntryPoint;
  uint32_t BaseOfCode;
  uint64_t ImageBase;
  uint32_t SectionAlignment;
  uint32_t FileAlignment;
  uint16_t MajorOperatingSystemVersion;
  uint16_t MinorOperatingSystemVersion;
  uint16_t MajorImageVersion;
  uint16_t MinorImageVersion;
  uint16_t MajorSubsystemVersion;
  uint16_t MinorSubsystemVersion;
  uint32_t Win32VersionValue;
  uint32_t SizeOfImage;
  uint32_t SizeOfHeaders;
  uint32_t CheckSum;
  uint16_t Subsystem;
  uint16_t DllCharacteristics;
  uint64_t SizeOfStackReserve;
  uint64_t SizeOfStackCommit;
  uint64_t SizeOfHeapReserve;
  uint64_t SizeOfHeapCommit;
  uint32_t LoaderFlags;
  uint32_t NumberOfRvaAndSizes;
  struct __pe_data_directory DataDirectory[pe_numberof_data_directories];
} PACKED;

typedef enum {
  pe_subsystem_unknown = 0,
  pe_subsystem_native,
  pe_subsystem_windows_gui,
  pe_subsystem_windows_cui,
  pe_subsystem_os2_cui = 5,
  pe_subsystem_posix_cui = 7,
  pe_subsystem_native_windows,
  pe_subsystem_windows_ce_gui,
  pe_subsystem_efi_application,
  pe_subsystem_efi_boot_service_driver,
  pe_subsystem_efi_runtime_driver,
  pe_subsystem_efi_rom,
  pe_subsystem_xbox,
  pe_subsystem_windows_boot_application = 16,
} pe_subsystem_t;

typedef enum {
  pe_dllflags_high_entropy_va = 0x0020,
  pe_dllflags_dynamic_base = 0x0040,
  pe_dllflags_force_integrity = 0x0080,
  pe_dllflags_nx_compat = 0x0100,
  pe_dllflags_no_isolation = 0x0200,
  pe_dllflags_no_seh = 0x0400,
  pe_dllflags_no_bind = 0x0800,
  pe_dllflags_appcontainer = 0x1000,
  pe_dllflags_wdm_driver = 0x2000,
  pe_dllflags_guard_cf = 0x4000,
  pe_dllflags_terminal_server_aware = 0x8000,
} pe_dllflags_t;

typedef enum {
  pe_directory_export = 0,
  pe_directory_import,
  pe_directory_resource,
  pe_directory_exception,
  pe_directory_security,
  pe_directory_basereloc,
  pe_directory_debug,
  pe_directory_architecture,
  pe_directory_globalptr,
  pe_directory_tls,
  pe_directory_load_config,
  pe_directory_bound_import,
  pe_directory_iat,
  pe_directory_delay_import,
  pe_directory_com_descriptor,
} pe_directory_type_t;

#define pe_sizeof_short_name 8

typedef struct __pe_section_header pe_section_header_t;
struct __pe_section_header {
  uint8_t Name[pe_sizeof_short_name];
  union {
    uint32_t PhysicalAddress;
    uint32_t VirtualSize;
  } Misc;

  uint32_t VirtualAddress;
  uint32_t SizeOfRawData;
  uint32_t PointerToRawData;
  uint32_t PointerToRelocations;
  uint32_t PointerToLinenumbers;
  uint16_t NumberOfRelocations;
  uint16_t NumberOfLinenumbers;
  uint32_t Characteristics;
} PACKED;

typedef enum {
  pe_section_no_pad = 0x00000008,
  pe_section_cnt_code = 0x00000020,
  pe_section_cnt_initialized_data = 0x00000040,
  pe_section_cnt_uninitialized_data = 0x00000080,
  pe_section_lnk_other = 0x00000100,
  pe_section_lnk_info = 0x00000200,
  pe_section_lnk_remove = 0x00000800,
  pe_section_lnk_comdat = 0x00001000,
  pe_section_no_defer_spec_exc = 0x00004000,
  pe_section_gprel = 0x00008000,
  pe_section_mem_fardata = 0x00008000,
  pe_section_mem_purgeable = 0x00020000,
  pe_section_mem_16bit = 0x00020000,
  pe_section_mem_locked = 0x00040000,
  pe_section_mem_preload = 0x00080000,
  pe_section_align_1bytes = 0x00100000,
  pe_section_align_2bytes = 0x00200000,
  pe_section_align_4bytes = 0x00300000,
  pe_section_align_8bytes = 0x00400000,
  pe_section_align_16bytes = 0x00500000,
  pe_section_align_32bytes = 0x00600000,
  pe_section_align_64bytes = 0x00700000,
  pe_section_align_128bytes = 0x00800000,
  pe_section_align_256bytes = 0x00900000,
  pe_section_align_512bytes = 0x00a00000,
  pe_section_align_1024bytes = 0x00b00000,
  pe_section_align_2048bytes = 0x00c00000,
  pe_section_align_4096bytes = 0x00d00000,
  pe_section_align_8192bytes = 0x00e00000,
  pe_section_lnk_nreloc_ovfl = 0x01000000,
  pe_section_mem_discardable = 0x02000000,
  pe_section_mem_not_cached = 0x04000000,
  pe_section_mem_not_paged = 0x08000000,
  pe_section_mem_shared = 0x10000000,
  pe_section_mem_execute = 0x20000000,
  pe_section_mem_read = 0x40000000,
  pe_section_mem_write = 0x80000000,
} pe_section_flags_t;

#define pe_section_align_mask 0x00f00000

typedef struct __pe_import_descriptor pe_import_descriptor_t;
struct __pe_import_descriptor {
  union {
    uint32_t Characteristics;
    uint32_t OriginalFirstThunk;
  };

  uint32_t TimeDateStamp;

  uint32_t ForwarderChain;
  uint32_t Name;
  uint32_t FirstThunk;
} PACKED;

typedef struct __pe_export_descriptor pe_export_descriptor_t;
struct __pe_export_descriptor {
  uint32_t Characteristics;
  uint32_t TimeDateStamp;
  uint16_t MajorVersion;
  uint16_t MinorVersion;
  uint32_t Name;
  uint32_t Base;
  uint32_t NumberOfFunctions;
  uint32_t NumberOfNames;
  uint32_t AddressOfFunctions;
  uint32_t AddressOfNames;
  uint32_t AddressOfNameOrdinals;
};

typedef struct __pe_resource_directory pe_resource_directory_t;
struct __pe_resource_directory {
  uint32_t Characteristics;
  uint32_t TimeDateStamp;
  uint16_t MajorVersion;
  uint16_t MinorVersion;
  uint16_t NumberOfNamedEntries;
  uint16_t NumberOfIdEntries;
} PACKED;

typedef struct __pe_resource_data_entry pe_resource_data_entry_t;
struct __pe_resource_data_entry {
  uint32_t OffsetToData;
  uint32_t Size;
  uint32_t CodePage;
  uint32_t Reserved;
} PACKED;

typedef struct __pe_resource_folder pe_resource_folder_t;
struct __pe_resource_folder {
  union {
    struct {
      uint32_t NameOffset:31;
      uint32_t NameIsString:1;
    };

    uint32_t Name;
    uint16_t Id;
  };
  union {
    uint32_t OffsetToData;
    struct {
      uint32_t OffsetToDirectory:31;
      uint32_t DataIsDirectory:1;
    };
  };
} PACKED;

typedef struct __pe_resource_a_string pe_resource_a_string_t;
struct __pe_resource_a_string {
  uint16_t Length;
  char NameString[1];
} PACKED;

typedef struct __pe_resource_u_string pe_resource_u_string_t;
struct __pe_resource_u_string {
  uint16_t Length;
  wchar_t NameString[1];
} PACKED;

typedef enum {
  pe_resource_id_cursor = 1,
  pe_resource_id_bitmap,
  pe_resource_id_icon,
  pe_resource_id_menu,
  pe_resource_id_dialog,
  pe_resource_id_string,
  pe_resource_id_fontdir,
  pe_resource_id_font,
  pe_resource_id_accelerator,
  pe_resource_id_rcdata,
  pe_resource_id_messagetable,
  pe_resource_id_group_cursor,
  pe_resource_id_group_icon = 14,
  pe_resource_id_version = 16,
  pe_resource_id_dlginclude,
  pe_resource_id_plugplay = 19,
  pe_resource_id_vxd,
  pe_resource_id_anicursor,
  pe_resource_id_aniicon,
  pe_resource_id_html,
  pe_resource_id_manifest,
} pe_resource_id_t;

typedef struct __pe_fixed_version_info pe_fixed_version_info_t;
struct __pe_fixed_version_info {
  uint32_t dwSignature;
  uint32_t dwStrucVersion;
  uint32_t dwFileVersionMS;
  uint32_t dwFileVersionLS;
  uint32_t dwProductVersionMS;
  uint32_t dwProductVersionLS;
  uint32_t dwFileFlagsMask;
  uint32_t dwFileFlags;
  uint32_t dwFileOS;
  uint32_t dwFileType;
  uint32_t dwFileSubtype;
  uint32_t dwFileDateMS;
  uint32_t dwFileDateLS;
};

typedef enum {
  pe_filever_os_dos = 0x00010000,
  pe_filever_os_nt = 0x00040000,
  pe_filever_os__windows16 = 0x00000001,
  pe_filever_os__windows32 = 0x00000004,
  pe_filever_os_os216 = 0x00020000,
  pe_filever_os_os232 = 0x00030000,
  pe_filever_os__pm16 = 0x00000002,
  pe_filever_os__pm32 = 0x00000003,
  pe_filever_os_unknown = 0x00000000,
} pe_filever_os_t;

typedef enum {
  pe_filever_type_unknown,
  pe_filever_type_app,
  pe_filever_type_dll,
  pe_filever_type_drv,
  pe_filever_type_font,
  pe_filever_type_vxd,
  pe_filever_type_static_lib = 7,
} pe_filever_type_t;

#pragma pack(pop)

#endif // __PE_STRUCTS_INCLUDED__
