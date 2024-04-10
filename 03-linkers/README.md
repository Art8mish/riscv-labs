# Lab 02 (toolchain)

## Отчёт

### 1. Исследование объектных файлов

В проекте есть два связанных файла `main.c` и `fact.c` (в *main* используются функции из *fact*). Соберём объектный файл только из `main.c` и исследуем его (`make main.o`):
```
riscv64-unknown-linux-gnu-gcc -O0 src/main.c -c -o obj/main.o
```

Причем выдаётся *warning*:
```
main.c:5:16: warning: implicit declaration of function 'fact' [-Wimplicit-function-declaration]
    5 |   unsigned f = fact(5);
      |                ^~~~
```
Что значит неявное объявление функции `fact`, справить это можно добавив строку c явным объявлением:
```C
extern unsigned fact(unsigned x); 
```

Исследуем `main.o` с помощью `readelf` (в файле *main.readelf*):
```
riscv64-unknown-linux-gnu-readelf -a main.o
```

Информация о файле:
- Тип: REL (Relocatable file) 
- Секции (-S): .text (исполняемый код), .rela.text (-r, таблица релокации), .data (данные), .bss, .rodata (неизменяемые данные), .comment, .note.GNU-stack, .riscv.attributes, .symtab (таблица символов), .strtab, .shstrtab (всего 10)
- Записи секции **.symtab**:
```
Symbol table '.symtab' contains 14 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS main.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1 .text
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3 .data
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4 .bss
     5: 0000000000000000     0 SECTION LOCAL  DEFAULT    5 .rodata
     6: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    1 $x
     7: 0000000000000000     0 SECTION LOCAL  DEFAULT    7 .note.GNU-stack
     8: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT    5 .LC0
     9: 0000000000000000     0 SECTION LOCAL  DEFAULT    6 .comment
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    8 .riscv.attributes
    11: 0000000000000000    58 FUNC    GLOBAL DEFAULT    1 main
    12: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND fact
    13: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
```
Из всего представленного интересуют символы: **main**, **fact**, **printf**. Первый имеет тип **FUNC** (функция), так как явно определяется в файле *main.с*. Что касается **fact** и **printf**, в исходном файле они определены неявно: через декларацию или через подключение библиотеки, поэтому компилятору в рамках одной единицы трансляции неясно что это за символы (**NOTYPE**).

### 2. Статическая линковка

Для начала нужно с помощью `ar` (опция `r` - вставить файлы в архив с замещением, модификатор `c` - создать заданный архив прии отсутствии) создать архив (статическую библиотеку) из файла *fact.c*:
```
ar rc ./obj/libfact.a obj/fact.o
``` 
С помощью `nm` можно посмотреть доступные в архиве символы:
```
riscv64-unknown-linux-gnu-nm ./obj/libfact.a

fact.o:
0000000000000000 T fact
```

Cлинкуем её с *main.o* в *fact* (`make fact-static-lib`):
```
riscv64-unknown-linux-gnu-gcc lib/fact.c -c -o obj/fact.o
ar rc ./obj/libfact.a obj/fact.o
riscv64-unknown-linux-gnu-gcc src/main.c -c -o obj/main.o
riscv64-unknown-linux-gnu-gcc ./obj/main.o -lfact -o ./build/fact.elf

/opt/sc-dt/riscv-gcc/bin/../lib/gcc/riscv64-unknown-linux-gnu/12.2.1/../../../..riscv64-unknown-linux-gnu/bin/ld: cannot find -lfact: No such file or directory
collect2: error: ld returned 1 exit status
```

`сollect2` (плагин для вызова линковщика `ld`) не нашёл `libfact` в текущей папке, для того, чтобы библиотека нашлась нужно явно указать компилятору, где её искать `-L./obj`:
```
riscv64-unknown-linux-gnu-gcc lib/fact.c -c -o obj/fact.o
ar rc ./obj/libfact.a obj/fact.o
riscv64-unknown-linux-gnu-gcc src/main.c -c -o obj/main.o
riscv64-unknown-linux-gnu-gcc ./obj/main.o -L./obj -lfact -o ./build/fact
```

`nm` для fact:
```
riscv64-unknown-linux-gnu-nm ./build/fact

0000000000011e10 d _DYNAMIC
0000000000012028 d _GLOBAL_OFFSET_TABLE_
0000000000012030 D _IO_stdin_used
                 w _ITM_deregisterTMCloneTable
                 w _ITM_registerTMCloneTable
00000000000104d0 t _PROCEDURE_LINKAGE_TABLE_
0000000000012048 B __BSS_END__
0000000000012000 D __DATA_BEGIN__
0000000000010690 r __FRAME_END__
0000000000010654 r __GNU_EH_FRAME_HDR
0000000000012030 D __SDATA_BEGIN__
0000000000012000 D __TMC_END__
0000000000010294 r __abi_tag
0000000000012040 B __bss_start
0000000000012000 D __data_start
00000000000105a0 t __do_global_dtors_aux
0000000000011e08 d __do_global_dtors_aux_fini_array_entry
0000000000012038 D __dso_handle
0000000000011e00 d __frame_dummy_init_array_entry
0000000000012800 A __global_pointer$
0000000000011e08 d __init_array_end
0000000000011e00 d __init_array_start
000000000001064c T __libc_csu_fini
00000000000105f4 T __libc_csu_init
                 U __libc_start_main@GLIBC_2.27
0000000000012040 D _edata
0000000000012048 B _end
0000000000010520 T _start
0000000000012040 b completed.0
0000000000012000 W data_start
000000000001055a t deregister_tm_clones
                 U fact
00000000000105c2 t frame_dummy
000000000001054e t load_gp
00000000000105c4 T main
                 U printf@GLIBC_2.27
0000000000010578 t register_tm_clones
```

Размер полученного исполняемого файла - 4.0K. Проверить работоспособность собранной программы можно в `qemu` (`make qemu-run`).

### 3. Динамическая линковка

Динамическая библиотека создаётся с помощью флага `-shared`:
```
riscv64-unknown-linux-gnu-gcc -shared obj/fact.o -o ./obj/libfact.so
```
Но теперь при компиляции нужно указывать путь до библиотеки не только для линковщика, но и для исполняемого файла. Это можно сделать с помощью `-rpath` (`-Wl,-rpath,<directory>` - это команда вставляется при линковке файлов)
```
riscv64-unknown-linux-gnu-gcc ./obj/main.o -fPIC -L./obj -lfact -Wl,-rpath,./obj -o ./build/fact
```

Полный цикл компиляции (`make fact-dynamic-lib`):
```
riscv64-unknown-linux-gnu-gcc lib/fact.c -c -o obj/fact.o
riscv64-unknown-linux-gnu-gcc -shared obj/fact.o -o ./obj/libfact.so
riscv64-unknown-linux-gnu-gcc src/main.c -c -o obj/main.o
riscv64-unknown-linux-gnu-gcc ./obj/main.o -fPIC -L./obj -lfact -Wl,-rpath,./obj -o ./build/fact
```

Тестируем приложение в `qemu` (`make run-qemu`), не забывая указывать, где искать стандартные библиотеки:
```
qemu-riscv64 -L /opt/sc-dt/riscv-gcc/sysroot/ ./build/fact
120
```

### 4. Создание своей динамической библиотеки

В папках *tasks/custom_dynamic_lib* и *tasks/custom_static_lib* находятся файлы для создания собственных динамической и статической библиотек соответственно (зайти в папку и прописать `make`). От текущего значения переменной `CC` будет зависит архитектура, для которой собирается приложение. В папке *tasks* находятся инструкции по сборке, как в связке со статической библиотекой (`make app-static`), так и с динамической (`make app-dyn`) (предворительно библиотеки нужно собрать для той же архитектуры). Командой `make run-qemu` можно запустить собранное для `riscv-64` приложение.

#### Инструкция для сборки (отн. папки *tasks*)

Статическая линковка, **x86-64** архитектура:
```
export CC=gcc 
cd custom_static_lib
make clean
make
cd ../
make clean
make app-static
./build/app
```

Динамическая линковка, **riscv-64** архитектура:
```
export CC=riscv64-unknown-linux-gnu-gcc
cd custom_dynamic_lib
make clean
make
cd ../
make clean
make app-dyn
make run-qemu
```

Ожидаемый вывод:
```
Hallo, welt! (strlen - 12)
```

## Литература и материалы 
- создание библиотек (https://younglinux.info/c/library)
- ar                 (https://www.opennet.ru/man.shtml?topic=ar&category=1)
- fPIC               (https://habr.com/ru/companies/badoo/articles/324616/)
- make               (https://www.opennet.ru/docs/RUS/gnumake/)

