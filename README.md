# C-preprocessor

##### Preprocesses C files by parsing and replacing the preprocessing directives.

* Usage:

`so-cpp [-D <SYMBOL>[=<MAPPING>]] [-I <DIR>] [<INFILE>] [ [-o] <OUTFILE>]`

* Example

```c
#define VAR0 1 \
        + 2\
        + 3\
        + 4
#define VAR1 1 * 3
#define TEST_IF VAR0 + VAR1 + 5

int main() {
#if 0
    printf("No\n");
#elif 0
    printf("Maybe\n");
#elif VAR0
    printf("ABC\n");
#else
    printf("Yes\n");
#endif

#ifdef TEST_IF
    printf("#defines working in #ifs\n");
#endif

   return 0;
}
```

will be preprocessed to 
```c
int main() {
    printf("ABC\n");
    
    printf("#defines working in #ifs\n");
    
    return 0;
}
```
