# Lab 02 (toolchain)

## Отчёт

### 1. Установка docker, сборка контейнера, настройка окружения
    
Копируем образ и стартуем контейнер, запускаем внутреннее окружение контейнера командной (в *Makefile* команда make run-docker):
```
sudo docker exec -it cpp sh -c "/bin/bash"
```
Далее настраиваем переменные окружения, до этого просмотрим, какие значения существующих переменных командой `printenv`:
```
HOSTNAME=667614b00de2
SCDT_INSTALLATION_ROOT=/opt/sc-dt
PWD=/root
HOME=/root
LESSCLOSE=/usr/bin/lesspipe %s %s
TERM=xterm-256color
LESSOPEN=| /usr/bin/lesspipe %s
SHLVL=1
VCPKG_INSTALLATION_ROOT=/usr/local/vcpkg
PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
```
В переменной **PATH** указаны пути, в которых между всего прочего происходит поиск нужных терминальных программ. Теперь добавим своих переменных окружения и отредактируем текущие для удобной работы с утилитами запуском `source /opt/sc-dt/env.sh`

```
SC_CLANG_PATH=/opt/sc-dt/llvm
HOSTNAME=667614b00de2
SC_QEMU_PATH=/opt/sc-dt/tools/bin
SCDT_INSTALLATION_ROOT=/opt/sc-dt
SC_QEMU_SYSTEM32=/opt/sc-dt/tools/bin/qemu-system-riscv32
SC_QEMU_SYSTEM=/opt/sc-dt/tools/bin/qemu-system-riscv64
SC_DT_PATH=/opt/sc-dt
SC_GDB_PATH=/opt/sc-dt/riscv-gcc
PWD=/root
SC_BSP_PATH=/opt/sc-dt/workspace/bsp
SC_OPENOCD_PATH=/opt/sc-dt/tools
SC_QEMU_SYSTEM64=/opt/sc-dt/tools/bin/qemu-system-riscv64
HOME=/root
SC_PATHS=/opt/sc-dt/riscv-gcc/bin:/opt/sc-dt/llvm/bin:/opt/sc-dt/tools/bin:/opt/sc-dt/riscv-gcc/bin:/opt/sc-dt/tools/bin:/opt/sc-dt/tools/bin
LESSCLOSE=/usr/bin/lesspipe %s %s
TERM=xterm-256color
LESSOPEN=| /usr/bin/lesspipe %s
SHLVL=1
VCPKG_INSTALLATION_ROOT=/usr/local/vcpkg
SC_CMAKE_TOOLCHAINS=/opt/sc-dt/workspace/cmake-toolchains
PATH=/opt/sc-dt/riscv-gcc/bin:/opt/sc-dt/llvm/bin:/opt/sc-dt/tools/bin:/opt/sc-dt/riscv-gcc/bin:/opt/sc-dt/tools/bin:/opt/sc-dt/tools/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
SC_GCC_PATH=/opt/sc-dt/riscv-gcc
```
По выводу `printenv` видно, что были добавлены новые пути, например, для компилятора (SC_GCC_PATH), отладчика **gdb**(SC_GDB_PATH), **qemu** (SC_QEMU_SYSTEM), а также добавлены пути в **PATH**, где лежат все инструменты и программы.

### 2. Работа с инструментами тулчейна
[**Тулчейн**](https://ru.wikipedia.org/wiki/Toolchain) - организованный набор иинструментов для разработки приложений. В данной работе используются компоненты разных тулчейнов, таких как [**GNU toolchain**](https://www.gnu.org/) (ПО: [GCC](https://gcc.gnu.org/) (*gnu compiler collection*), [GDB](https://www.sourceware.org/gdb/) (*gnu debugger*)) и [**LLVM**](https://llvm.org/) (ПО: [clang](https://clang.llvm.org/) (*фронтенд llvm*))

Компилируем `hello.c` под **risc-v 64** с помощью *Makefile* командой `make build`:
```
cc  hello.c -o build/hello.elf
```
 Наименование нужного компилятора `riscv64-unknown-linux-gnu-gcc` не подставилось в переменную **СС**, для того, чтобы его подставить нужно либо указать значение переменной перед `make build`:
```
CC=/opt/sc-dt/riscv-gcc/bin/riscv64-unknown-linux-gnu-gcc make build
```
либо экспортировать это значение как исходное для переменной окружения с помощью `export`:
```
CC=riscv64-unknown-linux-gnu-gcc
export CC
```
Теперь с помощью `echo $CC` можно убедиться, что экспортируемая переменная добавилась к переменным оболочки и приняла значение *riscv64-unknown-linux-gnu-gcc*. Команда `make build` после добавления CC:
```
riscv64-unknown-linux-gnu-gcc hello.c -o build/hello.elf
```

Эмуляция на  **qemu**: `make run-qemu`
```
qemu-x86_64 build/hello.elf
make: qemu-x86_64: No such file or directory
```
В **QEMU_USER** по умолчанию был прописан *qemu-x86_64* (эмуляция на архитектуре *x86_64*, а скомпилирован файл под *risc-v 64*), его в инструментах нет, поэтому *make* его и не нашел. Проделаем ту же процедуру с `export`:
```
QEMU_USER=qemu-riscv64
export QEMU_USER
```
Теперь запустим `make run-qemu`:
```
qemu-riscv64 build/hello.elf
qemu-riscv64: Could not open '/lib/ld-linux-riscv64-lp64d.so.1': No such file or directory
```
Ошибка связана с тем, что qemu не смог найти библиотеку для динамической линковки в стандартных путях. Найдём, где она расположена командой `find /opt/sc-dt | grep  ld-linux-riscv64-lp64d.so.1` (поиск в склонированных инструментах): `/opt/sc-dt/riscv-gcc/sysroot/lib/`. Добавим флаг `-L /opt/sc-dt/riscv-gcc/sysroot/`, который предоставляет **qemu** еще один путь для поиска (добавление сразу */opt/sc-dt/riscv-gcc/sysroot/lib/* не работает, вопрос почему открыт для обсуждения). Теперь добавим в *Makefile* переменную `QEMU_FLAGS ?= -L` и `QEMU_PATH=/opt/sc-dt/riscv-gcc/sysroot/` и снова припишем `make run-qemu`
```
riscv64-unknown-linux-gnu-gcc  hello.c -o build/hello.elf
qemu-riscv64 -L /opt/sc-dt/riscv-gcc/sysroot/ build/hello.elf
Hello, RISC-V!
```

### 3. Подключение gdb и отладка

Для того, чтобы проводить отладку с помощью **gdb** при компиляции нужно указывать флаг `-g`, который сохраняет дебаг-информацию, что, кстати, увеличивает размеры файла (8.3K без флага *-g*, 9.6К с флагом)
```
CFLAGS=-g make build
```
Рассмотрим вывод утилиты `objdump` (используем `riscv64-unknown-elf-objdump`), которая позволяет дизассемблировать файлы. Для дизассемблирования указывается флаг `-d`, для вывода информации для дебага укажем флаг `-g`. Результат сохраним в файл `build/hello.dis`
```
riscv64-unknown-elf-objdump -g build/hello.elf > build/hello.dis
```
В файлe *hello.dis* теперь есть информация о таких секциях как: .debug_aranges, .debug_info, .debug_abbrev, .debug_line, .debug_frame и т.д. Все эти секции составляются при компиляции и позволяют работать с дебагером *gdb*.

Теперь запустим **qemu** c флагом `-g 1234`:
```
qemu-riscv64 -g 1234 build/hello.elf
```
Теперь эмулятор будет ожидать, когда **gdb** подключится к виртуальному порту извне. Открываем второй терминал (проделываем те же процедуры настройки окружения), в терминале запускаем `riscv64-unknown-linux-gnu-gdb` и прописываем команду подключения к порту:
```
target remote localhost:1234
```
Далее можно работать с отладчиком, в котором уже будет подгружена наша команда. Хорошая вводная статья по gdb: https://azeria-labs.com/debugging-with-gdb-introduction/

### 4. Сборка своего приложение
В папке *square_solver* находятся файлы для сборки прилжения, решающего квадратного уравнения. 
Запуск и настройка **docker** окружения:
```
make run-docker
source /opt/sc-dt/env.sh
``` 

Сборка под **risc-v** и эмуляция на qemu (**gcc**):
```
CC=riscv64-unknown-linux-gnu-gcc QEMU_USER=qemu-riscv64 make run-qemu
```

Сборка под **risc-v** и эмуляция на qemu (**clang**):
```
CC=clang QEMU_USER=qemu-riscv64 make run-qemu
```
Это работает, так как при просмотре информации о **clang** коммандой `clang --version` видно, что таргет-система - riscv-v64:
```
Syntacore clang version 17.0.0 (SC git:/tools/llvm/llvm-project/ 8e902e1e697c9e34f6e4e91bf560a764435008a2)
Target: riscv64-unknown-linux-gnu
Thread model: posix
InstalledDir: /opt/sc-dt/llvm/bin
```
Чтобы скомпилировать под *x86-64* нужно указать флаг компиляции с нужной архитектурой `--target=...`


#### Оптимизации

Сборка под **x86-64** с флагом `-O0` (отключение оптимизаций):
```
CC=gcc CFLAGS=-O0 make build
```

Сборка под **x86-64** с флагом `-O3` (самый высокий уровень оптимизаций):
```
CC=gcc CFLAGS=-O3 make build
```

Сравним ассемблеры (сохраним `objdump` дизассемблирование в файлы `build/solverO0.dis` и `build/solverO3.dis`).

Во-первых, размер ассмблера при *-O3* больше чем без оптимизаций.

Второе, что стоит отметить, так это сниженное количество работы с памятью при оптимизации, на помощь приходят расширенные **xmm** регистры. Например, по этой причине передача некоторых аргументов функции происходит через регистры, а не через стек. Это происходит потому, что операции с памятью очень дорогие. 

Ещё можно вспомнить и заметить такие простые оптимизации, как разворот циклов, или вынос независимых данных из цикла.


## Литература и материалы 
- Понимая Docker (https://habr.com/ru/articles/253877/)
- Docker (https://www.docker.com/)
- Переменная среды (https://ru.wikipedia.org/wiki/%D0%9F%D0%B5%D1%80%D0%B5%D0%BC%D0%B5%D0%BD%D0%BD%D0%B0%D1%8F_%D1%81%D1%80%D0%B5%D0%B4%D1%8B)
- printenv (https://www.opennet.ru/man.shtml?topic=printenv&category=1&russian=0)

- GNU wiki (https://ru.wikipedia.org/wiki/GNU_toolchain)
- GNU (https://www.gnu.org/)
- GCC wiki (https://ru.wikipedia.org/wiki/GNU_Compiler_Collection)
- GCC (https://gcc.gnu.org/)
- GDB wiki (https://ru.wikipedia.org/wiki/GNU_Debugger)
- GDB (https://www.sourceware.org/gdb/)

- LLVM wiki (https://ru.wikipedia.org/wiki/LLVM)
- LLVM (https://llvm.org/)
- clang wiki (https://ru.wikipedia.org/wiki/Clang)
- clang (https://clang.llvm.org/)

- Debugging with GDB (https://azeria-labs.com/debugging-with-gdb-introduction/)