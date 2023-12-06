MEMORY
{
  UNPLACED_SECTIONS (wx) : ORIGIN = 0x100000000, LENGTH = 0
  RAM1 (wx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
  FLASH1 (wx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
}

SECTIONS
{
  __RAM1_segment_start__ = 0x20000000;
  __RAM1_segment_end__ = 0x20008000;
  __RAM1_segment_size__ = 0x00008000;
  __FLASH1_segment_start__ = 0x00000000;
  __FLASH1_segment_end__ = 0x00040000;
  __FLASH1_segment_size__ = 0x00040000;

  __HEAPSIZE__ = 8192;
  __SEGGER_RTL_vfprintf = __SEGGER_RTL_vfprintf_long;
  __SEGGER_RTL_vfscanf = __SEGGER_RTL_vfscanf_long;
  __STACKSIZE_PROCESS__ = 0;
  __STACKSIZE__ = 8192;

  __reserved_ram_load_start__ = 0x20000000;
  .reserved_ram 0x20000000 (NOLOAD) : AT(0x20000000)
  {
    __reserved_ram_start__ = .;
    *(.reserved_ram .reserved_ram.*)
    . = MAX(__reserved_ram_start__ + 0x20000000-0x20000000 , .);
  }
  __reserved_ram_end__ = __reserved_ram_start__ + SIZEOF(.reserved_ram);
  __reserved_ram_size__ = SIZEOF(.reserved_ram);
  __reserved_ram_load_end__ = __reserved_ram_end__;

  . = ASSERT(__reserved_ram_start__ == __reserved_ram_end__ || (__reserved_ram_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .reserved_ram is too large to fit in RAM1 memory segment");

  . = ASSERT(__reserved_ram_size__ <= 0x20000000-0x20000000 , "error: .reserved_ram section is larger than specified size");

  __vectors_ram_load_start__ = 0x20000000;
  .vectors_ram 0x20000000 (NOLOAD) : AT(0x20000000)
  {
    __vectors_ram_start__ = .;
    __app_ram_start__ =   __vectors_ram_start__;
    *(.vectors_ram .vectors_ram.*)
  }
  __vectors_ram_end__ = __vectors_ram_start__ + SIZEOF(.vectors_ram);
  __vectors_ram_size__ = SIZEOF(.vectors_ram);
  __vectors_ram_load_end__ = __vectors_ram_end__;

  . = ASSERT(__vectors_ram_start__ == __vectors_ram_end__ || (__vectors_ram_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .vectors_ram is too large to fit in RAM1 memory segment");
  . = ASSERT(__reserved_ram_end__ <= __vectors_ram_start__ , "error: section .reserved_ram overlaps absolute placed section .vectors_ram");

  __nrf_sections_run_load_start__ = ALIGN(__vectors_ram_end__ , 4);
  .nrf_sections_run ALIGN(__vectors_ram_end__ , 4) (NOLOAD) : AT(ALIGN(__vectors_ram_end__ , 4))
  {
    __nrf_sections_run_start__ = .;
    __start_nrf_sections_run =   __nrf_sections_run_start__;
    KEEP(*(.nrf_sections_run .nrf_sections_run.*))
  }
  __nrf_sections_run_end__ = __nrf_sections_run_start__ + SIZEOF(.nrf_sections_run);
  __nrf_sections_run_size__ = SIZEOF(.nrf_sections_run);
  __nrf_sections_run_load_end__ = __nrf_sections_run_end__;

  . = ASSERT(__nrf_sections_run_start__ == __nrf_sections_run_end__ || (__nrf_sections_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .nrf_sections_run is too large to fit in RAM1 memory segment");

  __cli_sorted_cmd_ptrs_run_load_start__ = ALIGN(__nrf_sections_run_end__ , 4);
  .cli_sorted_cmd_ptrs_run ALIGN(__nrf_sections_run_end__ , 4) (NOLOAD) : AT(ALIGN(__nrf_sections_run_end__ , 4))
  {
    __cli_sorted_cmd_ptrs_run_start__ = .;
    __start_cli_sorted_cmd_ptrs =   __cli_sorted_cmd_ptrs_run_start__;
    KEEP(*(.cli_sorted_cmd_ptrs_run .cli_sorted_cmd_ptrs_run.*))
  }
  __cli_sorted_cmd_ptrs_run_end__ = __cli_sorted_cmd_ptrs_run_start__ + SIZEOF(.cli_sorted_cmd_ptrs_run);
  __cli_sorted_cmd_ptrs_run_size__ = SIZEOF(.cli_sorted_cmd_ptrs_run);
  __stop_cli_sorted_cmd_ptrs =   __cli_sorted_cmd_ptrs_run_end__;
  __cli_sorted_cmd_ptrs_run_load_end__ = __cli_sorted_cmd_ptrs_run_end__;

  . = ASSERT(__cli_sorted_cmd_ptrs_run_start__ == __cli_sorted_cmd_ptrs_run_end__ || (__cli_sorted_cmd_ptrs_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .cli_sorted_cmd_ptrs_run is too large to fit in RAM1 memory segment");

  __fs_data_run_load_start__ = ALIGN(__cli_sorted_cmd_ptrs_run_end__ , 4);
  .fs_data_run ALIGN(__cli_sorted_cmd_ptrs_run_end__ , 4) (NOLOAD) : AT(ALIGN(__cli_sorted_cmd_ptrs_run_end__ , 4))
  {
    __fs_data_run_start__ = .;
    __start_fs_data =   __fs_data_run_start__;
    KEEP(*(.fs_data_run .fs_data_run.*))
  }
  __fs_data_run_end__ = __fs_data_run_start__ + SIZEOF(.fs_data_run);
  __fs_data_run_size__ = SIZEOF(.fs_data_run);
  __stop_fs_data =   __fs_data_run_end__;
  __fs_data_run_load_end__ = __fs_data_run_end__;

  . = ASSERT(__fs_data_run_start__ == __fs_data_run_end__ || (__fs_data_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .fs_data_run is too large to fit in RAM1 memory segment");

  __log_dynamic_data_run_load_start__ = ALIGN(__fs_data_run_end__ , 4);
  .log_dynamic_data_run ALIGN(__fs_data_run_end__ , 4) (NOLOAD) : AT(ALIGN(__fs_data_run_end__ , 4))
  {
    __log_dynamic_data_run_start__ = .;
    __start_log_dynamic_data =   __log_dynamic_data_run_start__;
    KEEP(*(.log_dynamic_data_run .log_dynamic_data_run.*))
  }
  __log_dynamic_data_run_end__ = __log_dynamic_data_run_start__ + SIZEOF(.log_dynamic_data_run);
  __log_dynamic_data_run_size__ = SIZEOF(.log_dynamic_data_run);
  __stop_log_dynamic_data =   __log_dynamic_data_run_end__;
  __log_dynamic_data_run_load_end__ = __log_dynamic_data_run_end__;

  . = ASSERT(__log_dynamic_data_run_start__ == __log_dynamic_data_run_end__ || (__log_dynamic_data_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .log_dynamic_data_run is too large to fit in RAM1 memory segment");

  __log_filter_data_run_load_start__ = ALIGN(__log_dynamic_data_run_end__ , 4);
  .log_filter_data_run ALIGN(__log_dynamic_data_run_end__ , 4) (NOLOAD) : AT(ALIGN(__log_dynamic_data_run_end__ , 4))
  {
    __log_filter_data_run_start__ = .;
    __start_log_filter_data =   __log_filter_data_run_start__;
    KEEP(*(.log_filter_data_run .log_filter_data_run.*))
  }
  __log_filter_data_run_end__ = __log_filter_data_run_start__ + SIZEOF(.log_filter_data_run);
  __log_filter_data_run_size__ = SIZEOF(.log_filter_data_run);
  __stop_log_filter_data =   __log_filter_data_run_end__;
  __log_filter_data_run_load_end__ = __log_filter_data_run_end__;

  . = ASSERT(__log_filter_data_run_start__ == __log_filter_data_run_end__ || (__log_filter_data_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .log_filter_data_run is too large to fit in RAM1 memory segment");

  __nrf_sections_run_end_load_start__ = ALIGN(__log_filter_data_run_end__ , 4);
  .nrf_sections_run_end ALIGN(__log_filter_data_run_end__ , 4) (NOLOAD) : AT(ALIGN(__log_filter_data_run_end__ , 4))
  {
    __nrf_sections_run_end_start__ = .;
    __end_nrf_sections_run =   __nrf_sections_run_end_start__;
    KEEP(*(.nrf_sections_run_end .nrf_sections_run_end.*))
  }
  __nrf_sections_run_end_end__ = __nrf_sections_run_end_start__ + SIZEOF(.nrf_sections_run_end);
  __nrf_sections_run_end_size__ = SIZEOF(.nrf_sections_run_end);
  __nrf_sections_run_end_load_end__ = __nrf_sections_run_end_end__;

  . = ASSERT(__nrf_sections_run_end_start__ == __nrf_sections_run_end_end__ || (__nrf_sections_run_end_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .nrf_sections_run_end is too large to fit in RAM1 memory segment");

  __fast_run_load_start__ = ALIGN(__nrf_sections_run_end_end__ , 4);
  .fast_run ALIGN(__nrf_sections_run_end_end__ , 4) (NOLOAD) : AT(ALIGN(__nrf_sections_run_end_end__ , 4))
  {
    __fast_run_start__ = .;
    *(.fast_run .fast_run.*)
  }
  __fast_run_end__ = __fast_run_start__ + SIZEOF(.fast_run);
  __fast_run_size__ = SIZEOF(.fast_run);
  __fast_run_load_end__ = __fast_run_end__;

  . = ASSERT(__fast_run_start__ == __fast_run_end__ || (__fast_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .fast_run is too large to fit in RAM1 memory segment");

  __data_run_load_start__ = ALIGN(__fast_run_end__ , 4);
  .data_run ALIGN(__fast_run_end__ , 4) (NOLOAD) : AT(ALIGN(__fast_run_end__ , 4))
  {
    __data_run_start__ = .;
    *(.data_run .data_run.*)
  }
  __data_run_end__ = __data_run_start__ + SIZEOF(.data_run);
  __data_run_size__ = SIZEOF(.data_run);
  __data_run_load_end__ = __data_run_end__;

  . = ASSERT(__data_run_start__ == __data_run_end__ || (__data_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .data_run is too large to fit in RAM1 memory segment");

  __tdata_run_load_start__ = ALIGN(__data_run_end__ , 4);
  .tdata_run ALIGN(__data_run_end__ , 4) (NOLOAD) : AT(ALIGN(__data_run_end__ , 4))
  {
    __tdata_run_start__ = .;
    *(.tdata_run .tdata_run.*)
  }
  __tdata_run_end__ = __tdata_run_start__ + SIZEOF(.tdata_run);
  __tdata_run_size__ = SIZEOF(.tdata_run);
  __tdata_run_load_end__ = __tdata_run_end__;

  . = ASSERT(__tdata_run_start__ == __tdata_run_end__ || (__tdata_run_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .tdata_run is too large to fit in RAM1 memory segment");

  __bss_load_start__ = ALIGN(__tdata_run_end__ , 4);
  .bss ALIGN(__tdata_run_end__ , 4) (NOLOAD) : AT(ALIGN(__tdata_run_end__ , 4))
  {
    __bss_start__ = .;
    *(.bss .bss.* .gnu.linkonce.b.*) *(COMMON)
  }
  __bss_end__ = __bss_start__ + SIZEOF(.bss);
  __bss_size__ = SIZEOF(.bss);
  __bss_load_end__ = __bss_end__;

  . = ASSERT(__bss_start__ == __bss_end__ || (__bss_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .bss is too large to fit in RAM1 memory segment");

  __tbss_load_start__ = ALIGN(__bss_end__ , 4);
  .tbss ALIGN(__bss_end__ , 4) (NOLOAD) : AT(ALIGN(__bss_end__ , 4))
  {
    __tbss_start__ = .;
    *(.tbss .tbss.*)
  }
  __tbss_end__ = __tbss_start__ + SIZEOF(.tbss);
  __tbss_size__ = SIZEOF(.tbss);
  __tbss_load_end__ = __tbss_end__;

  . = ASSERT(__tbss_start__ == __tbss_end__ || (__tbss_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .tbss is too large to fit in RAM1 memory segment");

  __non_init_load_start__ = ALIGN(__tbss_end__ , 4);
  .non_init ALIGN(__tbss_end__ , 4) (NOLOAD) : AT(ALIGN(__tbss_end__ , 4))
  {
    __non_init_start__ = .;
    *(.non_init .non_init.*)
  }
  __non_init_end__ = __non_init_start__ + SIZEOF(.non_init);
  __non_init_size__ = SIZEOF(.non_init);
  __non_init_load_end__ = __non_init_end__;

  . = ASSERT(__non_init_start__ == __non_init_end__ || (__non_init_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .non_init is too large to fit in RAM1 memory segment");

  __heap_load_start__ = ALIGN(__non_init_end__ , 4);
  .heap ALIGN(__non_init_end__ , 4) (NOLOAD) : AT(ALIGN(__non_init_end__ , 4))
  {
    __heap_start__ = .;
    *(.heap .heap.*)
    . = ALIGN(MAX(__heap_start__ + __HEAPSIZE__ , .), 4);
  }
  __heap_end__ = __heap_start__ + SIZEOF(.heap);
  __heap_size__ = SIZEOF(.heap);
  __heap_load_end__ = __heap_end__;

  . = ASSERT(__heap_start__ == __heap_end__ || (__heap_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .heap is too large to fit in RAM1 memory segment");

  . = ASSERT(__HEAPSIZE__  == ALIGN(__HEAPSIZE__, 4), "error: .heap section size does not match alignment");

  . = ASSERT(__heap_size__ <= __HEAPSIZE__ , "error: .heap section is larger than specified size");

  __stack_load_start__ = __RAM1_segment_start__ + (__RAM1_segment_size__ - 8192);
  .stack __RAM1_segment_start__ + (__RAM1_segment_size__ - 8192) (NOLOAD) : AT(__RAM1_segment_start__ + (__RAM1_segment_size__ - 8192))
  {
    __stack_start__ = .;
    __StackLimit =   __stack_start__;
    *(.stack .stack.*)
    . = ALIGN(MAX(__stack_start__ + __STACKSIZE__ , .), 8);
  }
  __stack_end__ = __stack_start__ + SIZEOF(.stack);
  __stack_size__ = SIZEOF(.stack);
  __StackTop =   __stack_end__;
  __stack_load_end__ = __stack_end__;

  . = ASSERT(__stack_start__ == __stack_end__ || (__stack_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .stack is too large to fit in RAM1 memory segment");
  . = ASSERT(__heap_end__ <= __stack_start__ , "error: section .heap overlaps absolute placed section .stack");

  . = ASSERT(__STACKSIZE__  == ALIGN(__STACKSIZE__, 8), "error: .stack section size does not match alignment");

  . = ASSERT(__stack_size__ <= __STACKSIZE__ , "error: .stack section is larger than specified size");

  __stack_process_load_start__ = ALIGN(__stack_end__ , 8);
  .stack_process ALIGN(__stack_end__ , 8) (NOLOAD) : AT(ALIGN(__stack_end__ , 8))
  {
    __stack_process_start__ = .;
    *(.stack_process .stack_process.*)
    . = ALIGN(MAX(__stack_process_start__ + __STACKSIZE_PROCESS__ , .), 8);
  }
  __stack_process_end__ = __stack_process_start__ + SIZEOF(.stack_process);
  __stack_process_size__ = SIZEOF(.stack_process);
  __stack_process_load_end__ = __stack_process_end__;

  __RAM1_segment_used_end__ = ALIGN(__stack_end__ , 8) + SIZEOF(.stack_process);
  __RAM1_segment_used_size__ = __RAM1_segment_used_end__ - __RAM1_segment_start__;

  . = ASSERT(__stack_process_start__ == __stack_process_end__ || (__stack_process_end__ - __RAM1_segment_start__) <= __RAM1_segment_size__ , "error: .stack_process is too large to fit in RAM1 memory segment");

  . = ASSERT(__STACKSIZE_PROCESS__  == ALIGN(__STACKSIZE_PROCESS__, 8), "error: .stack_process section size does not match alignment");

  . = ASSERT(__stack_process_size__ <= __STACKSIZE_PROCESS__ , "error: .stack_process section is larger than specified size");

}

