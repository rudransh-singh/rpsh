# rpsh- a shell created using C

the directory structure is as follows

```
.
├── a.out
├── colors.h
├── commands
│   ├── cd.c
│   ├── cd.h
│   ├── echo.c
│   ├── echo.h
│   ├── fgbgprocess.c
│   ├── fgbgprocess.h
│   ├── history.c
│   ├── history.h
│   ├── ls.c
│   ├── ls.h
│   ├── pinfo.c
│   ├── pinfo.h
│   ├── pwd.c
│   └── pwd.h
├── display.c
├── display.h
├── history.txt
├── makefile
├── OS.pdf
├── readme.md
├── shell.c
├── shell.h
├── utils.c
└── utils.h

1 directory, 26 files
```


## Usage

To use the shell just perform the following commands

- `make`
- `./a.out`

## Files and their Workings

```
cd.c - for the cd function
cd.h- header file for cd function
pwd.c- for the pwd function
pwd.h- header file for the pwd function
pinfo.c- for the pinfo function
pinfo.h - header file for the pinfo function
ls.c - for the ls function
ls.h- header file for the ls function
echo.c - for the echo function
echo.h - header file for the echo function
fgbgprocess.c - for execution of foreground and background processes
fgbgprocess.h - header file for the foreground and background processes
history.c - for the history function
history.h - header file for the history function
shellwithuparrow.c/shell.h - main function corresponding to the actual shell, contains code for displaying
					prompt, and taking input and performing approprate action
colors.h - header file which contains codes for colors as well as BRIGHT, DIM 
utils.c/utils.h- contains utility functions
display.c/display.h - contains functions to display the prompt

```
