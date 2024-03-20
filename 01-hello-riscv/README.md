# Lab 01 (hello riscv)

## Отчёт

#### 1. Установка нужных модулей и тулчейна для riscv64
#### 2. Разбор <span style="color: orange">  Makefile </span>:
    - `.PHONY` нужен для объявления *фальшивых* целей, которые будут выполнятся в любом случае (вне зависимости от состояния каких-либо файлов)
    - `_build_dir: @mkdir -p ${BUILD_DIR}` для создания директории ./build (при указании опции -p перед созданием каталога предварительно создаются все несуществующие вышележащие каталоги.)
    - `hello: hello.c _build_dir` вызывает указанный в переменной `СС` компилятор c флагами `CCFLAGS`
    - `show` вызывает команду `file`
    - `run` - основная цель: компиляция и запуск программы
    - `run-qemu` запускает программу в эмуляторе `qemu`, указанном в `QEMU_USER`

#### 3. После компиляции файла `gcc` без флагов исследуем его командой `file` (`make show`). Вывод:
```
build/hello.elf: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=e7798427e5a52989ba13f4dffa989cbb6f1a0530, for GNU/Linux 3.2.0, not stripped
```
Из полученной информации видно, что файл формата - **ELF**, интерпретация битов - **LSB** (Least Significant Bit, т.е. младший значимый бит, *little-endian*), использована динамическая линковка библиотек, скомпилирован для архитектуры **x86-64**

#### 4. Исследование командой `ldd build/hello.elf`:
```
build/hello.elf: ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, BuildID[sha1]=e7798427e5a52989ba13f4dffa989cbb6f1a0530, for GNU/Linux 3.2.0, not stripped
```
    - vDSO (virtual dynamic shared object) - библиотека, которую ядро ​​автоматически включает в каждое пользовательское приложение. Она связана с оптимизацией системных вызовов.
    - libc.so.6 - стандартная C библиотека, также автоматически включаемая во все приложения
    - /lib64/ld-linux-x86-64.so.2 - библиотека линкера **ld**

#### 5. Эмуляция на `qemu`:
```
CC=riscv64-linux-gnu-gcc QEMU_USER=qemu-riscv64 CCFLAGS=-static make run-qemu
```
или же
```
riscv64-linux-gnu-gcc -static hello.c -o build/hello.elf
qemu-riscv64 build/hello.elf
```
В данном случае файл скомпилирован под архетиктуру riscv64, это можно пронаблюдать с помощью `file`
```
build/hello.elf: ELF 64-bit LSB executable, UCB RISC-V, RVC, double-float ABI, version 1 (SYSV), statically linked, BuildID[sha1]=d0e3daa97648f4bbf207a4e523d3c263d015f3ae, for GNU/Linux 4.15.0, not stripped
```
При поиске информации с помощью `ldd` выдаёт `not a dynamic executable`, так как файл был скомпилирован с флагом `static`, который включает в сам ELF-файл все подключаемые библиотек. Кстати, размер файла из-за этого сильно разрастается (посмотрим размеры с и без -static с помощью команды `ls -lh build/hello.elf`)"
<table>
    <tr>
        <th>динамическая линковка (без флагов)</th>
        <th>статическая линковка (с флагом -static)</th>
    </tr>
    <tr>
        <td>12K</td>
        <td>492K</td>
    </tr>
</table>
Размер увеличился в 41 раз, что крайне существенно, это является большим минусом статической линковки при создании проектов, состоящих из нескольких файлов (библиотеки в таком случае будут каждый раз включаться в каждый из файлов, а не переиспользоваться, как при динамической линковке)

## Литература и материалы 
- .PHONY (https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html)
- mkdir (https://www.opennet.ru/man.shtml?topic=mkdir&category=1&russian=4)
- ELF format (https://habr.com/ru/articles/480642/)
- vDSO (https://man7.org/linux/man-pages/man7/vdso.7.html)
- опции GCC (https://www.opennet.ru/docs/RUS/gcc/gcc1-2.html)


