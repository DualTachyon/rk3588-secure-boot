ENTRY(_start)
OUTPUT_FORMAT(elf64-littleaarch64)

MEMORY
{
	SRAM (rwx): ORIGIN = 0xFF001000, LENGTH = 124K
}

SECTIONS
{
	. = ORIGIN(SRAM);

	.text : {
		. = ALIGN(4096);
		rx_section_start = .;

		*(.text.start)
		*(.text)
		*(.text.*)

		. = ALIGN(4096);
		rx_section_end = .;
	}

	.rodata : {
		. = ALIGN(4096);
		ro_section_start = .;

		*(.rodata)
		*(.rodata.*)

		. = ALIGN(4096);
		ro_section_end = .;
	}

	.bss : {
		. = ALIGN(4096);
		rw_section_start = .;

		bss_start = .;
		*(.bss)
		*(COMMON)
		bss_end = .;

		. = ALIGN(4096);
		rw_section_end = .;
	}
}

