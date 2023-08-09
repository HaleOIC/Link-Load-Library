# Record (Symbol)

By type into the command `nm SimpleSection.o` we can have

```
0000000000000000 T func1
0000000000000000 D global_init_var
0000000000000004 b global_static_var
0000000000000000 B global_uninit_var
000000000000002b T main
                 U printf
0000000000000004 d static_var.1
0000000000000008 b static_var2.0
```

these are the two important kinds of symbols which will attach importance to the final executable file.

- **global symbol**
  - `func1` `main`  `global_uninit_var`
- **External symbol**
  - `printf` 



Symbol table architecture

```c
/* Symbol table entry.  */

typedef struct
{
  Elf32_Word	st_name;		/* Symbol name (string tbl index) */
  Elf32_Addr	st_value;		/* Symbol value */
  Elf32_Word	st_size;		/* Symbol size */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char	st_other;		/* Symbol visibility */
  Elf32_Section	st_shndx;		/* Section index */
} Elf32_Sym;

typedef struct
{
  Elf64_Word	st_name;		/* Symbol name (string tbl index) */
  unsigned char	st_info;		/* Symbol type and binding */
  unsigned char st_other;		/* Symbol visibility */
  Elf64_Section	st_shndx;		/* Section index */
  Elf64_Addr	st_value;		/* Symbol value */
  Elf64_Xword	st_size;		/* Symbol size */
} Elf64_Sym;
```



Type into the command `readelf -s SimpleSection.o`

```
Symbol table '.symtab' contains 14 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS SimpleSection.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
     5: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    4 global_static_var
     6: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     7: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 static_var.1
     8: 0000000000000008     4 OBJECT  LOCAL  DEFAULT    4 static_var2.0
     9: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 global_init_var
    10: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    4 global_uninit_var
    11: 0000000000000000    43 FUNC    GLOBAL DEFAULT    1 func1
    12: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    13: 000000000000002b    55 FUNC    GLOBAL DEFAULT    1 main
```





### Special Symbol

Typing the following codes 

```shell
$gcc SpecialSymbol.c -o SpecialSymbol
$./SpecialSymbol
```

we can have 

```
Executable Start 0x55ade1298000
Text End 0x55ade1299205 0x55ade1299205 0x55ade1299205
Data End 0x55ade129c010 0x55ade129c010
Executable End 0x55ade129c018 0x55ade129c018
```



### Name Decoration and Name Mangling

```shell
$ c++filt _ZN1N1C4funcEi
N::C::func(int)
$ c++filt _ZZ4mainE3foo
main::foo
$ c++filt _ZZ4funcvE3foo
func()::foo
```



```shell
$ g++ ManualNameMangling.cpp -o ManualNameMangling
$ ./ManualNameMangling 
42
```

- From the view of compiler, `_ZN6myname3varE` is the same as `myname::var`
-  







