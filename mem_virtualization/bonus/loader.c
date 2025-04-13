#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <elf.h>
#include <limits.h>

void *safe_map(uint64_t size) {
		void *addr = mmap(NULL, size, PROT_NONE,
						MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (addr == MAP_FAILED) {
				perror("failed to reserve base");
				exit(1);
		}
		return addr;
}

int main(int argc, char **argv) {
		if (argc < 2) { fprintf(stderr, "Usage: %s <pie-elf>\n", argv[0]); return 1; }

		// Load ELF file
		int fd = open(argv[1], O_RDONLY);
		struct stat st;
		fstat(fd, &st);
		void *file = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
		close(fd);

		Elf64_Ehdr *eh = file;
		Elf64_Phdr *ph = file + eh->e_phoff;

		// Find min and max vaddr for PT_LOAD
		uint64_t min_vaddr = UINT64_MAX, max_vaddr = 0;
		for (int i = 0; i < eh->e_phnum; i++) {
				if (ph[i].p_type != PT_LOAD) continue;
				if (ph[i].p_vaddr < min_vaddr) min_vaddr = ph[i].p_vaddr;
				if (ph[i].p_vaddr + ph[i].p_memsz > max_vaddr)
						max_vaddr = ph[i].p_vaddr + ph[i].p_memsz;
		}

		uint64_t image_size = max_vaddr - min_vaddr;
		void *base = safe_map(image_size);

		// Map each PT_LOAD segment into memory
		for (int i = 0; i < eh->e_phnum; i++) {
				if (ph[i].p_type != PT_LOAD) continue;


				size_t page_size = 0x1000;
				uint64_t vaddr   = ph[i].p_vaddr;
				uint64_t memsz   = ph[i].p_memsz;
				uint64_t filesz  = ph[i].p_filesz;
				uint64_t offset  = ph[i].p_offset;

				uint64_t vpage   = vaddr & ~(page_size - 1);
				uint64_t offpage = offset & ~(page_size - 1);
				uint64_t adjust  = vaddr - vpage;
				uint64_t mapsz   = memsz + adjust;

				void *dest = base + (vpage - min_vaddr);

				void *mapped = mmap(dest, mapsz,
								PROT_READ | PROT_WRITE | PROT_EXEC,
								MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED, -1, 0);

				if (mapped == MAP_FAILED) {
						perror("segment mmap");
						exit(1);
				}

				memcpy((uint8_t *)dest + adjust, (uint8_t *)file + offset, filesz);
		}

		// Find 'main' symbol
		Elf64_Shdr *sh = file + eh->e_shoff;
		Elf64_Sym *symtab = NULL;
		const char *strtab = NULL;
		size_t nsym = 0;

		for (int i = 0; i < eh->e_shnum; i++) {
				if (sh[i].sh_type == SHT_SYMTAB) {
						symtab = file + sh[i].sh_offset;
						nsym = sh[i].sh_size / sizeof(Elf64_Sym);
						strtab = file + sh[sh[i].sh_link].sh_offset;
						break;
				}
		}

		int (*main_func)() = NULL;
		for (size_t i = 0; i < nsym; i++) {
				if (strcmp(&strtab[symtab[i].st_name], "main") == 0) {
						uintptr_t offset = symtab[i].st_value - min_vaddr;
						main_func = (int (*)())((uint8_t *)base + offset);
						break;
				}
		}

		if (!main_func) {
				fprintf(stderr, "main() not found\n");
				return 1;
		}

		printf("Jumping to PIE main() at %p\n", main_func);
		int ret = main_func();
		printf("Return Value of your main: %d\n", ret);

		return 0;
}

