# Sense

Standarized platform independent structures for game input. Usually used together with a platform-specific input driver library.

```c
typedef struct SenseNamedButtons {
    int vertical;
    int horizontal;
    int a;
    int b;
    int leftShoulder;
    int rightShoulder;
    int x;
    int y;
    int menu;
} SenseNamedButtons;
```
