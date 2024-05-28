# Поверхностное исследование LD_PRELOAD

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

Скомпилируем `hook1.c` в динамическую библиотеку:
```
riscv64-unknown-linux-gnu-gcc lib/hook1.c -c -o obj/hook1.o
riscv64-unknown-linux-gnu-gcc -shared obj/hook1.o -o ./obj/hook.so
```

Теперь при запуске будем указывать переменную `LD_PRELOAD=$PWD/obj/hook.so`. Результат исполнения (`make qemu-run-preload`):
```
qemu-riscv64 -L /opt/sc-dt/riscv-gcc/sysroot/ LD_PRELOAD=$PWD/obj/hook.so ./build/app
Copied string: !dlrow ,olleH
```

Исходное приложение `app` после первой компиляции никак не менялось, но результат другой. Это происходит потому, что при линковке в ячейку таблицы `PLT` записывается `memcpy` из библиотеки `hook.so`

### 1. Исследование объектных файлов


## Литература и материалы 
- dynamic linking and loading difference (https://stackoverflow.com/questions/10052464/difference-between-dynamic-loading-and-dynamic-linking)
- LD_PRELOAD trick (https://habr.com/ru/companies/otus/articles/748494/,
                    https://habr.com/ru/articles/479858/,
                    https://stackoverflow.com/questions/426230/what-is-the-ld-preload-trick)
- PLT/GOT (https://rus-linux.net/MyLDP/algol/Shared-Libraries/Shared-Libraries-1.5.5.html)

