# Исследование безопасности RISC-V приложений. LD_PRELOAD.

Исследование проводится с [Syntacore Development Toolkit](https://syntacore.com/). 
*sc-dt релиз

## Суть использования LD_PRELOAD

**LD_PRELOAD** - это переменная окружения, которая используется компоновщиком во время динамической линковки, чтобы подгрузить некоторые определения символов раньше стандартных (до libc.so например). В этом ключе **LD_PRELOAD** может быть использован для переопределения функций стандартных библиотек прямо во время исполнения, при чем не требуется вмешательство в исходный код программы. Далее разберём пример использования.

## Подобие хука через LD_PRELOAD

Напишем простейшее приложение на С с использованием функции из стандартной библиотеки, например `memcpy`:
```C
#include <stdlib.h>

void *memcpy (void *destination, const void *source, size_t n)

int main() {
    char source[] = "Hello, world!";
    char destination[20];

    memcpy(destination, source, strlen(source) + 1);

    printf("Copied string: %s\n", destination);

    return 0;
}
```

Результат исполнения (`make run-qemu`):
```
riscv64-unknown-linux-gnu-gcc ./obj/app.o -o ./build/app
qemu-riscv64 -L /opt/sc-dt/riscv-gcc/sysroot/ ./build/app

Copied string: Hello, world!
```


Теперь напишем свою реализацию `memcpy` в файле `hook1.c`, которая будет копировать в обратном порядке:
```C
void *memcpy (void *destination, const void *source, size_t n)
{
  for (size_t i=0; i < n - 1; i++)
    *((char *)destination + i) = *((const char *)source + (n - 2) - i);

  *((char *)destination + n - 1) = '\0';
  return destination;
}
```

Скомпилируем `hook1.c` в динамическую библиотеку (`make hook1`):
```
riscv64-unknown-linux-gnu-gcc -fPIC -c lib/hook1.c -o obj/hook1.o
riscv64-unknown-linux-gnu-gcc -shared obj/hook1.o -o ./obj/hook.so
```

Теперь при запуске будем указывать переменную `LD_PRELOAD=$PWD/obj/hook.so`. Результат исполнения (`make qemu-run-preload`):
```
qemu-riscv64 -L /opt/sc-dt/riscv-gcc/sysroot/ -E LD_PRELOAD="$PWD/./obj/hook.so" ./build/app

Copied string: !dlrow ,olleH
```

Исходное приложение **app** после первой компиляции никак не менялось, но результат другой. Это происходит потому, что при линковке в ячейку таблицы `PLT` записывается `memcpy` из библиотеки `hook.so`

## Скрытный LD_PRELOAD

Использование **LD_PRELOAD** можно сделать незаметный для исполняемой программы путем выполнения настоящей вызываемой функции. Это можно сделать с помощью `dlsym` (библиотека `stdlib.h`). Функция используется для получения адреса экспортируемого библиотекой символа (в нашем случае это `memcpy`). Напишем доработанный файл `hook2.c`:

```C

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdio.h>

const char *HOOK_FILE_PATH = "logs/theft.txt";

typedef void *(*memcpy_t)(void *, const void *, size_t);

void *memcpy (void *destination, const void *source, size_t n)
{

    FILE *fd = fopen(HOOK_FILE_PATH, "a");
    if (fd != NULL) {
        char dest[n]; 

        for (size_t i=0; i < n; i++)
            *(dest + i) = *((const char *)source + i);

        fwrite(dest, n, sizeof(char), fd);
        fclose(fd);
    }
    
    memcpy_t orig_memcpy;
    orig_memcpy = (memcpy_t) dlsym(RTLD_NEXT, "memcpy");
    return orig_memcpy(destination, source, n);;
}
```

Включаем символ `_GNU_SOURCE` для получения доступа к нестандартным GNU/Linux функциям и некоторым расширениям. Перед исполнением исходного кода *memcpy*, перекопируем информацию из *source* в свой файл. В качестве первого аргумента *dlsym* примнимает псевдо-указатель, есть возможность передать два значения: `RTLD_DEFAULT` (*dlsym* будет искать символ в библиотеке по умолчанию) и `RTLD_NEXT` (поиск в следующей по порядку библиотеке). Соответственно, чтобы получить указатель на оригинальный *memcpy*, нужен `RTLD_NEXT`.


```
make hook2
riscv64-unknown-linux-gnu-gcc -shared -fPIC -ldl lib/hook2.c -o ./obj/hook.so 

make run-qemu-preload
riscv64-unknown-linux-gnu-gcc  -c src/app.c -o obj/app.o
riscv64-unknown-linux-gnu-gcc   ./obj/app.o -o ./build/app

qemu-riscv64 -L /opt/sc-dt/riscv-gcc/sysroot/ -E LD_PRELOAD="$(PWD)/obj/hook.so" ./build/app

Copied string: Hello, world
```
При компиляции *hook.c* из *hook2.c* нужно добавить флаг `-ldl`, чтобы подключить библиотеку для *dl-функций*.

Для исходного приложения *app* ничего не меняется, но передаваемая информация была перехвачена в файл `logs/theft.txt`. Таким образом можно сать оболочки над функциям и производить отладку приложений.

## Заключение

**LD_PRELOAD** - это очень мощный инструмент. Самое распространенное и главное его предназначение - это отладка и тестирвание функций в динамически подключаемых библиотеках.

Нужно не забывать и об опасностях. Использовать его возможности стоит с осторожностью. Например, если экспортировать символ в систему с указанием на библиотеку переопределяющую некоторую стандартную функцию, то новое определение будет использоваться во всех исполняемых программах, что может привести к поломке системы. В добавок LD_PRELOAD очень сложно отследить, так как он предоставляет возможность переопределения любой реализованной функции, что открывает большой потенциал для атаки. Предупреждён - значит вооружён.


## Литература и материалы
- dynamic linking and loading difference (https://stackoverflow.com/questions/10052464/difference-between-dynamic-loading-and-dynamic-linking)
- LD_PRELOAD trick (https://habr.com/ru/companies/otus/articles/748494/,
                    https://habr.com/ru/articles/479858/,
                    https://stackoverflow.com/questions/426230/what-is-the-ld-preload-trick)
- PLT/GOT (https://rus-linux.net/MyLDP/algol/Shared-Libraries/Shared-Libraries-1.5.5.html)
- using LD_PRELOAD in QEMU (https://reverseengineering.stackexchange.com/questions/14927/ld-preload-in-qemu-user-space-emulation)
- _GNU_SOURCE (https://stackoverflow.com/questions/5582211/what-does-define-gnu-source-imply)
- dlsym (https://www.opennet.ru/man.shtml?topic=dlsym&category=3&russian=0)

- пример использования LD_PRELOAD (https://xakep.ru/2020/12/29/ld_preload-rootkit/)
