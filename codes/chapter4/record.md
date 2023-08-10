# Learning Records

Type into the command `gcc -c a.c b.c` and we can have 

```shell
$ gcc -c a.c b.c
a.c: In function ‘main’:
a.c:9:9: warning: implicit declaration of function ‘swap’ [-Wimplicit-function-declaration]
    9 |         swap(&a, &shared);
      |         ^~~~
```

and then we type into `ld a.o b.o -e main -o ab`, we will have

```shell
$ ld a.o b.o -e main -o ab
ld: a.o: in function `main':
a.c:(.text+0x4d): undefined reference to `__stack_chk_fail'
```

the reasons why is this can be checked in this link [:sailboat:link](https://zhuanlan.zhihu.com/p/150793679)

so we can fix the `a.c` file to `a_fix.c` as follow:
```c
/**
 * a_fix.c
 */
extern int shared; 	// weak referrence


// we need to specify the exit function to get out of the segent
void exit() {
	asm("movq $60, %rax \n\t"
		"syscall \n\t");
}


int hale() {

	int a = 10;
	swap(&a, &shared);

	exit();
}
```

and then type into the command `gcc -c -fno-builtin -fno-stack-protector a_fix.c b.c` and `ld -e hale a_fix.o b.o -o ab`

```shell
$ gcc -c -fno-builtin -fno-stack-protector a_fix.c b.c
a_fix.c: In function ‘hale’:
a_fix.c:17:9: warning: implicit declaration of function ‘swap’ [-Wimplicit-function-declaration]
   17 |         swap(&a, &shared);
      |         ^~~~
$ ld -e hale a_fix.o b.o -o ab
$ ./ab
$
```

```shell
$ objdump -h a_fix.o 
a_fix.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         0000004f  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000000  0000000000000000  0000000000000000  0000008f  2**0
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  0000008f  2**0
                  ALLOC
  3 .comment      00000026  0000000000000000  0000000000000000  0000008f  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000b5  2**0
                  CONTENTS, READONLY
  5 .note.gnu.property 00000020  0000000000000000  0000000000000000  000000b8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .eh_frame     00000058  0000000000000000  0000000000000000  000000d8  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA

$ objdump -h b.o

b.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         0000004f  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  1 .data         00000004  0000000000000000  0000000000000000  00000090  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000000  0000000000000000  0000000000000000  00000094  2**0
                  ALLOC
  3 .comment      00000026  0000000000000000  0000000000000000  00000094  2**0
                  CONTENTS, READONLY
  4 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000ba  2**0
                  CONTENTS, READONLY
  5 .note.gnu.property 00000020  0000000000000000  0000000000000000  000000c0  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  6 .eh_frame     00000038  0000000000000000  0000000000000000  000000e0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
$ objdump -h ab

ab:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .note.gnu.property 00000020  00000000004001c8  00000000004001c8  000001c8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  1 .text         0000009e  0000000000401000  0000000000401000  00001000  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
  2 .eh_frame     00000078  0000000000402000  0000000000402000  00002000  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  3 .data         00000004  0000000000403000  0000000000403000  00003000  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  4 .comment      00000025  0000000000000000  0000000000000000  00003004  2**0
                  CONTENTS, READONLY

```

Have a look at the diverse assemble codes of `a.o`

```shell
$ objdump -d a.o

a.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:	f3 0f 1e fa          	endbr64
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	48 83 ec 10          	sub    $0x10,%rsp
   c:	c7 45 fc 0a 00 00 00 	movl   $0xa,-0x4(%rbp)
  13:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
  17:	48 8d 15 00 00 00 00 	lea    0x0(%rip),%rdx        # 1e <main+0x1e>
  1e:	48 89 d6             	mov    %rdx,%rsi
  21:	48 89 c7             	mov    %rax,%rdi
  24:	b8 00 00 00 00       	mov    $0x0,%eax
  29:	e8 00 00 00 00       	call   2e <main+0x2e>
  2e:	b8 00 00 00 00       	mov    $0x0,%eax
  33:	c9                   	leave
  34:	c3                   	ret

$ objdump -d a_fix.o

a_fix.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <exit>:
   0:	f3 0f 1e fa          	endbr64
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	48 c7 c0 3c 00 00 00 	mov    $0x3c,%rax
   f:	0f 05                	syscall
  11:	90                   	nop
  12:	5d                   	pop    %rbp
  13:	c3                   	ret

0000000000000014 <hale>:
  14:	f3 0f 1e fa          	endbr64
  18:	55                   	push   %rbp
  19:	48 89 e5             	mov    %rsp,%rbp
  1c:	48 83 ec 10          	sub    $0x10,%rsp
  20:	c7 45 fc 0a 00 00 00 	movl   $0xa,-0x4(%rbp)
  27:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
  2b:	48 8d 15 00 00 00 00 	lea    0x0(%rip),%rdx        # 32 <hale+0x1e>
  32:	48 89 d6             	mov    %rdx,%rsi
  35:	48 89 c7             	mov    %rax,%rdi
  38:	b8 00 00 00 00       	mov    $0x0,%eax
  3d:	e8 00 00 00 00       	call   42 <hale+0x2e>
  42:	b8 00 00 00 00       	mov    $0x0,%eax
  47:	e8 00 00 00 00       	call   4c <hale+0x38>
  4c:	90                   	nop
  4d:	c9                   	leave
  4e:	c3                   	ret

```

```shell
$ objdump -d ab

ab:     file format elf64-x86-64


Disassembly of section .text:

0000000000401000 <exit>:
  401000:	f3 0f 1e fa          	endbr64
  401004:	55                   	push   %rbp
  401005:	48 89 e5             	mov    %rsp,%rbp
  401008:	48 c7 c0 3c 00 00 00 	mov    $0x3c,%rax
  40100f:	0f 05                	syscall
  401011:	90                   	nop
  401012:	5d                   	pop    %rbp
  401013:	c3                   	ret

0000000000401014 <hale>:
  401014:	f3 0f 1e fa          	endbr64
  401018:	55                   	push   %rbp
  401019:	48 89 e5             	mov    %rsp,%rbp
  40101c:	48 83 ec 10          	sub    $0x10,%rsp
  401020:	c7 45 fc 0a 00 00 00 	movl   $0xa,-0x4(%rbp)
  401027:	48 8d 45 fc          	lea    -0x4(%rbp),%rax
  40102b:	48 8d 15 ce 1f 00 00 	lea    0x1fce(%rip),%rdx        # 403000 <shared>
  401032:	48 89 d6             	mov    %rdx,%rsi
  401035:	48 89 c7             	mov    %rax,%rdi
  401038:	b8 00 00 00 00       	mov    $0x0,%eax
  40103d:	e8 0d 00 00 00       	call   40104f <swap>
  401042:	b8 00 00 00 00       	mov    $0x0,%eax
  401047:	e8 b4 ff ff ff       	call   401000 <exit>
  40104c:	90                   	nop
  40104d:	c9                   	leave
  40104e:	c3                   	ret

000000000040104f <swap>:
  40104f:	f3 0f 1e fa          	endbr64
  401053:	55                   	push   %rbp
  401054:	48 89 e5             	mov    %rsp,%rbp
  401057:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  40105b:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  40105f:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401063:	8b 10                	mov    (%rax),%edx
  401065:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401069:	8b 00                	mov    (%rax),%eax
  40106b:	31 c2                	xor    %eax,%edx
  40106d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401071:	89 10                	mov    %edx,(%rax)
  401073:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401077:	8b 10                	mov    (%rax),%edx
  401079:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  40107d:	8b 00                	mov    (%rax),%eax
  40107f:	31 c2                	xor    %eax,%edx
  401081:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  401085:	89 10                	mov    %edx,(%rax)
  401087:	48 8b 45 f0          	mov    -0x10(%rbp),%rax
  40108b:	8b 10                	mov    (%rax),%edx
  40108d:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401091:	8b 00                	mov    (%rax),%eax
  401093:	31 c2                	xor    %eax,%edx
  401095:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  401099:	89 10                	mov    %edx,(%rax)
  40109b:	90                   	nop
  40109c:	5d                   	pop    %rbp
  40109d:	c3                   	ret

```

### Relocation Table

Have a look at the `a.o` relocation table 

```shell
$ objdump -r a.o

a.o:     file format elf64-x86-64

RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE              VALUE
000000000000001a R_X86_64_PC32     shared-0x0000000000000004
000000000000002a R_X86_64_PLT32    swap-0x0000000000000004


RELOCATION RECORDS FOR [.eh_frame]:
OFFSET           TYPE              VALUE
0000000000000020 R_X86_64_PC32     .text

```



### static library link

```shell
$ ar -t /usr/lib/gcc/x86_64-linux-gnu/12/libstdc++.a
compatibility.o
compatibility-debug_list.o
compatibility-debug_list-2.o
compatibility-c++0x.o
compatibility-atomic-c++0x.o
compatibility-thread-c++0x.o
compatibility-chrono.o
compatibility-condvar.o
array_type_info.o
atexit_arm.o
atexit_thread.o
atomicity.o
bad_alloc.o
bad_array_length.o
bad_array_new.o
bad_cast.o
bad_typeid.o
class_type_info.o
cp-demangle.o
del_op.o
del_opa.o
del_opant.o
del_opnt.o
del_ops.o
del_opsa.o
del_opv.o
del_opva.o
del_opvant.o
del_opvnt.o
del_opvs.o
del_opvsa.o
dyncast.o
eh_alloc.o
eh_arm.o
eh_aux_runtime.o
eh_call.o
eh_catch.o
eh_exception.o
eh_globals.o
eh_personality.o
eh_ptr.o
eh_term_handler.o
eh_terminate.o
eh_throw.o
eh_tm.o
eh_type.o
eh_unex_handler.o
enum_type_info.o
function_type_info.o
fundamental_type_info.o
guard.o
guard_error.o
hash_bytes.o
nested_exception.o
new_handler.o
new_op.o
new_opa.o
new_opant.o
new_opnt.o
new_opv.o
new_opva.o
new_opvant.o
new_opvnt.o
pbase_type_info.o
pmem_type_info.o
pointer_type_info.o
pure.o
si_class_type_info.o
tinfo.o
tinfo2.o
vec.o
vmi_class_type_info.o
vterminate.o
allocator-inst.o
basic_file.o
bitmap_allocator.o
c++locale.o
codecvt.o
codecvt_members.o
collate_members.o
collate_members_cow.o
complex_io.o
concept-inst.o
cow-istream-string.o
ext-inst.o
globals_io.o
hash_tr1.o
hashtable_tr1.o
ios_failure.o
ios_init.o
ios_locale.o
istream-string.o
istream.o
list-aux-2.o
list-aux.o
list.o
list_associated-2.o
list_associated.o
locale.o
locale_facets.o
locale_init.o
localename.o
math_stubs_float.o
math_stubs_long_double.o
messages_members.o
messages_members_cow.o
misc-inst.o
monetary_members.o
monetary_members_cow.o
mt_allocator.o
numeric_members.o
numeric_members_cow.o
parallel_settings.o
pool_allocator.o
stdexcept.o
streambuf.o
strstream.o
time_members.o
tree.o
valarray.o
chrono.o
lt1-codecvt.o
condition_variable.o
cow-fstream-inst.o
cow-locale_init.o
cow-shim_facets.o
cow-sstream-inst.o
cow-stdexcept.o
cow-string-inst.o
cow-string-io-inst.o
cow-wstring-inst.o
cow-wstring-io-inst.o
ctype.o
ctype_configure_char.o
ctype_members.o
cxx11-hash_tr1.o
cxx11-ios_failure.o
cxx11-locale-inst.o
cxx11-shim_facets.o
cxx11-stdexcept.o
cxx11-wlocale-inst.o
debug.o
ext11-inst.o
fstream-inst.o
functexcept.o
functional.o
futex.o
future.o
hash_c++0x.o
hashtable_c++0x.o
ios-inst.o
ios.o
iostream-inst.o
istream-inst.o
limits.o
locale-inst.o
mutex.o
ostream-inst.o
placeholders.o
random.o
regex.o
shared_ptr.o
snprintf_lite.o
sso_string.o
sstream-inst.o
streambuf-inst.o
string-inst.o
string-io-inst.o
system_error.o
thread.o
wlocale-inst.o
wstring-inst.o
wstring-io-inst.o
cow-fs_dir.o
cow-fs_ops.o
cow-fs_path.o
lt2-cow-string-inst.o
floating_from_chars.o
floating_to_chars.o
fs_dir.o
fs_ops.o
fs_path.o
memory_resource.o
lt3-ostream-inst.o
lt4-string-inst.o
lt5-sstream-inst.o
```

```shell
$ gcc -static --verbose -fno-builtin hello.c 
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/x86_64-linux-gnu/12/lto-wrapper
OFFLOAD_TARGET_NAMES=nvptx-none:amdgcn-amdhsa
OFFLOAD_TARGET_DEFAULT=1
Target: x86_64-linux-gnu
Configured with: ../src/configure -v --with-pkgversion='Ubuntu 12.2.0-3ubuntu1' --with-bugurl=file:///usr/share/doc/gcc-12/README.Bugs --enable-languages=c,ada,c++,go,d,fortran,objc,obj-c++,m2 --prefix=/usr --with-gcc-major-version-only --program-suffix=-12 --program-prefix=x86_64-linux-gnu- --enable-shared --enable-linker-build-id --libexecdir=/usr/lib --without-included-gettext --enable-threads=posix --libdir=/usr/lib --enable-nls --enable-clocale=gnu --enable-libstdcxx-debug --enable-libstdcxx-time=yes --with-default-libstdcxx-abi=new --enable-gnu-unique-object --disable-vtable-verify --enable-plugin --enable-default-pie --with-system-zlib --enable-libphobos-checking=release --with-target-system-zlib=auto --enable-objc-gc=auto --enable-multiarch --disable-werror --enable-cet --with-arch-32=i686 --with-abi=m64 --with-multilib-list=m32,m64,mx32 --enable-multilib --with-tune=generic --enable-offload-targets=nvptx-none=/build/gcc-12-U8K4Qv/gcc-12-12.2.0/debian/tmp-nvptx/usr,amdgcn-amdhsa=/build/gcc-12-U8K4Qv/gcc-12-12.2.0/debian/tmp-gcn/usr --enable-offload-defaulted --without-cuda-driver --enable-checking=release --build=x86_64-linux-gnu --host=x86_64-linux-gnu --target=x86_64-linux-gnu
Thread model: posix
Supported LTO compression algorithms: zlib zstd
gcc version 12.2.0 (Ubuntu 12.2.0-3ubuntu1) 
COLLECT_GCC_OPTIONS='-static' '-v' '-fno-builtin' '-mtune=generic' '-march=x86-64' '-dumpdir' 'a-'
 /usr/lib/gcc/x86_64-linux-gnu/12/cc1 -quiet -v -imultiarch x86_64-linux-gnu hello.c -quiet -dumpdir a- -dumpbase hello.c -dumpbase-ext .c -mtune=generic -march=x86-64 -version -fno-builtin -fasynchronous-unwind-tables -fstack-protector-strong -Wformat -Wformat-security -fstack-clash-protection -fcf-protection -o /tmp/ccI7pK1z.s
GNU C17 (Ubuntu 12.2.0-3ubuntu1) version 12.2.0 (x86_64-linux-gnu)
	compiled by GNU C version 12.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.25-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
ignoring nonexistent directory "/usr/local/include/x86_64-linux-gnu"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/12/include-fixed"
ignoring nonexistent directory "/usr/lib/gcc/x86_64-linux-gnu/12/../../../../x86_64-linux-gnu/include"
#include "..." search starts here:
#include <...> search starts here:
 /usr/lib/gcc/x86_64-linux-gnu/12/include
 /usr/local/include
 /usr/include/x86_64-linux-gnu
 /usr/include
End of search list.
GNU C17 (Ubuntu 12.2.0-3ubuntu1) version 12.2.0 (x86_64-linux-gnu)
	compiled by GNU C version 12.2.0, GMP version 6.2.1, MPFR version 4.1.0, MPC version 1.2.1, isl version isl-0.25-GMP

GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
Compiler executable checksum: d2b97d7359ed8c253c6a0abebad2ff2d
COLLECT_GCC_OPTIONS='-static' '-v' '-fno-builtin' '-mtune=generic' '-march=x86-64' '-dumpdir' 'a-'
 as -v --64 -o /tmp/ccYoCykx.o /tmp/ccI7pK1z.s
GNU assembler version 2.39 (x86_64-linux-gnu) using BFD version (GNU Binutils for Ubuntu) 2.39
COMPILER_PATH=/usr/lib/gcc/x86_64-linux-gnu/12/:/usr/lib/gcc/x86_64-linux-gnu/12/:/usr/lib/gcc/x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/12/:/usr/lib/gcc/x86_64-linux-gnu/
LIBRARY_PATH=/usr/lib/gcc/x86_64-linux-gnu/12/:/usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/:/usr/lib/gcc/x86_64-linux-gnu/12/../../../../lib/:/lib/x86_64-linux-gnu/:/lib/../lib/:/usr/lib/x86_64-linux-gnu/:/usr/lib/../lib/:/usr/lib/gcc/x86_64-linux-gnu/12/../../../:/lib/:/usr/lib/
COLLECT_GCC_OPTIONS='-static' '-v' '-fno-builtin' '-mtune=generic' '-march=x86-64' '-dumpdir' 'a.'
 /usr/lib/gcc/x86_64-linux-gnu/12/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/12/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/12/lto-wrapper -plugin-opt=-fresolution=/tmp/ccBatsfY.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_eh -plugin-opt=-pass-through=-lc --build-id -m elf_x86_64 --hash-style=gnu --as-needed -static -z relro /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/12/crtbeginT.o -L/usr/lib/gcc/x86_64-linux-gnu/12 -L/usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/12/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/12/../../.. /tmp/ccYoCykx.o --start-group -lgcc -lgcc_eh -lc --end-group /usr/lib/gcc/x86_64-linux-gnu/12/crtend.o /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/crtn.o
COLLECT_GCC_OPTIONS='-static' '-v' '-fno-builtin' '-mtune=generic' '-march=x86-64' '-dumpdir' 'a.'

```

- `/usr/lib/gcc/x86_64-linux-gnu/12/cc1 -quiet -v -imultiarch x86_64-linux-gnu hello.c -quiet -dumpdir a- -dumpbase hello.c -dumpbase-ext .c -mtune=generic -march=x86-64 -version -fno-builtin -fasynchronous-unwind-tables -fstack-protector-strong -Wformat -Wformat-security -fstack-clash-protection -fcf-protection -o /tmp/ccI7pK1z.s` 
  - **compile `hello.c` to a tempt assembly file `/tmp/ccI7pK1z.s` by using `cc1`**
- `as -v --64 -o /tmp/ccYoCykx.o /tmp/ccI7pK1z.s`
  - **assemble `/tmp/ccI7pK1z.s` to a tempt object file `/tmp/ccYoCykx.o` by using `as`**
- `/usr/lib/gcc/x86_64-linux-gnu/12/collect2 -plugin /usr/lib/gcc/x86_64-linux-gnu/12/liblto_plugin.so -plugin-opt=/usr/lib/gcc/x86_64-linux-gnu/12/lto-wrapper -plugin-opt=-fresolution=/tmp/ccBatsfY.res -plugin-opt=-pass-through=-lgcc -plugin-opt=-pass-through=-lgcc_eh -plugin-opt=-pass-through=-lc --build-id -m elf_x86_64 --hash-style=gnu --as-needed -static -z relro /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/crt1.o /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/crti.o /usr/lib/gcc/x86_64-linux-gnu/12/crtbeginT.o -L/usr/lib/gcc/x86_64-linux-gnu/12 -L/usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu -L/usr/lib/gcc/x86_64-linux-gnu/12/../../../../lib -L/lib/x86_64-linux-gnu -L/lib/../lib -L/usr/lib/x86_64-linux-gnu -L/usr/lib/../lib -L/usr/lib/gcc/x86_64-linux-gnu/12/../../.. /tmp/ccYoCykx.o --start-group -lgcc -lgcc_eh -lc --end-group /usr/lib/gcc/x86_64-linux-gnu/12/crtend.o /usr/lib/gcc/x86_64-linux-gnu/12/../../../x86_64-linux-gnu/crtn.o`
  - **link the all related object files to the executable files with `/tmp/ccYoCykx.o`**
    - `crt1.o`
    - `crti.o`
    - `crtbeginT.o`
    - `crtend.o `
    - `crtn.o`

### Control link process

```c
/**
 * use the file without the C library to show the minimum program
 * TinyHelloWorld.c
 */

char *str = "Hello world!\n";

void print() {
	asm("movl $13, %%edx \n\t"
		"movl %0, %%ecx \n\t"
		"movl $0, %%ebx \n\t"
		"movl $4, %%eax \n\t"
		::"r"(str):"edx", "ecx", "ebx");
}

void exit() {
	asm("movl $42, %ebx \n\t"
		"movl $1, %eax \n\t"
		"int $0x80 \n\t");
}

void nomain() {
	print();
	exit();
}
```

If we use the `gcc -c -fno-builtin TinyHelloWorld.c `, we will encounter the errors

```shell
$ gcc -c -fno-builtin TinyHelloWorld.c 
TinyHelloWorld.c: Assembler messages:
TinyHelloWorld.c:10: Error: unsupported instruction `mov'
```

we should add the property `-m32` to enforce the system compile the program to 32bits-class

```shell
$ gcc -c -fno-builtin -m32 TinyHelloWorld.c 
$ ld -static -e nomain -o TinyHelloWorld TinyHelloWorld.o
ld: i386 architecture of input file `TinyHelloWorld.o' is incompatible with i386:x86-64 output
ld: TinyHelloWorld.o: in function `print':
TinyHelloWorld.c:(.text+0xa): undefined reference to `_GLOBAL_OFFSET_TABLE_'
ld: TinyHelloWorld.o: in function `exit':
TinyHelloWorld.c:(.text+0x34): undefined reference to `_GLOBAL_OFFSET_TABLE_'
ld: TinyHelloWorld.o: in function `nomain':
TinyHelloWorld.c:(.text+0x50): undefined reference to `_GLOBAL_OFFSET_TABLE_'
$ ld -static -m elf_i386 -e nomain -o TinyHelloWorld TinyHelloWorld.o
$ ./TinyHelloWorld 
Hello world!
$ echo $?
42
```

that means we should specify the machine type by using `-m elf_i386`

```shell
$ readelf -S TinyHelloWorld
There are 10 section headers, starting at offset 0x31a4:

Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        08049000 001000 000065 00  AX  0   0  1
  [ 2] .rodata           PROGBITS        0804a000 002000 00000e 00   A  0   0  1
  [ 3] .eh_frame         PROGBITS        0804a010 002010 000090 00   A  0   0  4
  [ 4] .got.plt          PROGBITS        0804bff4 002ff4 00000c 04  WA  0   0  4
  [ 5] .data             PROGBITS        0804c000 003000 000004 00  WA  0   0  4
  [ 6] .comment          PROGBITS        00000000 003004 000025 01  MS  0   0  1
  [ 7] .symtab           SYMTAB          00000000 00302c 0000c0 10      8   4  4
  [ 8] .strtab           STRTAB          00000000 0030ec 00006c 00      0   0  1
  [ 9] .shstrtab         STRTAB          00000000 003158 00004b 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  D (mbind), p (processor specific)

```

### combine all the sections to a single one

```shell
$ gcc -c -fno-builtin -m32 TinyHelloWorld.c 
hale@Halevm:~/Desktop/Link-Load-Library/codes/chapter4/codes$ ld -static -T TinyHelloWorld.lds -m elf_i386 -o TinyHelloWorld TinyHelloWorld.o
ld: warning: TinyHelloWorld has a LOAD segment with RWX permissions
hale@Halevm:~/Desktop/Link-Load-Library/codes/chapter4/codes$ ./TinyHelloWorld 
Hello world!
```

```shell
$ objdump -s TinyHelloWorld

TinyHelloWorld:     file format elf32-i386

Contents of section tinytext:
 8048094 5589e553 e8680000 00050301 00008b80  U..S.h..........
 80480a4 fcffffff ba0d0000 0089c1bb 00000000  ................
 80480b4 b8040000 00cd8090 8b5dfcc9 c35589e5  .........]...U..
 80480c4 e83c0000 0005d700 0000bb2a 000000b8  .<.........*....
 80480d4 01000000 cd80905d c35589e5 e8200000  .......].U... ..
 80480e4 0005bb00 0000e8a5 ffffffe8 cdffffff  ................
 80480f4 905dc348 656c6c6f 20776f72 6c64210a  .].Hello world!.
 8048104 00                                   .               
Contents of section .text.__x86.get_pc_thunk.ax:
 8048105 8b0424c3                             ..$.            
Contents of section .eh_frame:
 804810c 14000000 00000000 017a5200 017c0801  .........zR..|..
 804811c 1b0c0404 88010000 20000000 1c000000  ........ .......
 804812c 68ffffff 2d000000 00410e08 8502420d  h...-....A....B.
 804813c 05418303 68c5c30c 04040000 1c000000  .A..h...........
 804814c 40000000 71ffffff 1c000000 00410e08  @...q........A..
 804815c 8502420d 0558c50c 04040000 1c000000  ..B..X..........
 804816c 60000000 6dffffff 1a000000 00410e08  `...m........A..
 804817c 8502420d 0556c50c 04040000 10000000  ..B..V..........
 804818c 80000000 75ffffff 04000000 00000000  ....u...........
Contents of section .data.rel.local:
 804819c f7800408                             ....            
Contents of section .got.plt:
 80481a0 00000000 00000000 00000000           ............    
```

### BFD library 

```shell
$ sudo apt install binutils-dev
$ gcc -o target target.c -lbfd
$ ./target 
elf64-x86-64
elf32-i386
elf32-iamcu
elf32-x86-64
pei-i386
pe-x86-64
pei-x86-64
elf64-little
elf64-big
elf32-little
elf32-big
pe-bigobj-x86-64
pe-i386
srec
symbolsrec
verilog
tekhex
binary
ihex
plugin
```

