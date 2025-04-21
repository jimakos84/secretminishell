# minishell
Minishell with Thiwnaka

---

### Proper Implementation of a Shell
[Build Your Own Shell - Codecrafters](https://github.com/codecrafters-io/build-your-own-x?tab=readme-ov-file#build-your-own-shell)

---

### Minishell Example Projects
- [cclaude42/minishell](https://github.com/cclaude42/minishell)
- [mcombeau/minishell](https://github.com/mcombeau/minishell)

---

### Bash Reference Manual
[Bash Manual - GNU](https://www.gnu.org/software/bash/manual/bash.html)

---

### Bash Reference Manual
[Minishell Architecture](https://whimsical.com/minishell-architecture-big-picture-7b9N8PL3qHrddbs977mQ2J)

---

### Allowed Functions

#### 01). `readline`
```c
char *readline (char *prompt);
```

So, exapmle of using the function
```c
char *line = readline("Enter a line: ");
```
`readline` returns the text of the line read.  A blank line returns the empty string.  If `EOF` is encountered while reading a line, and the line is empty, `NULL` is returned.  If an `EOF` is read with a non-empty line, it is treated as a newline.

#### 02). `add_history`
```c
void add_history(const char *line);
```
The `add_history` function is also part of the GNU Readline library and is used to add a line of input to the history list.

#### 03). `rl_clear_history`
```c
void rl_clear_history(void);
```
The `rl_clear_history` function in C is part of the GNU Readline library. It's used to clear the entire input history that Readline maintains — for example, the lines you've entered during an interactive session

#### 04). `rl_on_new_line`
```c
int rl_on_new_line(void);
```
Tells Readline that the cursor is on a new line, so it's ready to redraw the prompt and the current input line correctly. Returns `0` on success, `-1` if the internal line state is invalid (e.g. `rl_line_buffer` is `NULL`).

#### 05). `rl_replace_line`
```c
void rl_replace_line(const char *text, int clear_undo);
```
The `rl_replace_line` function from the GNU Readline library is used to replace the current input line with a new string — without affecting the command history.
* `text`: The new string to use as the input line
* `clear_undo`: If non-zero, clears the undo list

It changes what the user sees as the current line in the terminal.
#### 06). `rl_redisplay`
```c
void rl_redisplay(void);
```
The `rl_redisplay` function is part of the GNU Readline library, and it’s used to redraw the prompt and current input line on the screen. Forces Readline to refresh the current input line and prompt display.
After making changes to the input buffer (e.g. with `rl_replace_line`), or when terminal output messes up the current input display.

#### 07). `getcwd`
```c
char *getcwd(char *buf, size_t size);
```
The `getcwd` function in C is used to get the current working directory of the calling process.
* buf: A pointer to a buffer where the path will be stored.
* size: The size (in bytes) of that buffer.
Returns: A pointer to `buf` on success, or `NULL` on failure (e.g. if the buffer is too small). If you want `getcwd` to allocate memory for you (POSIX extension), pass `NULL` for `buf` and `0` for `size`.

#### 08). `chdir`
```c
int chdir(const char *path);
```
The chdir() function in C is used to change the current working directory of the running process.
* `path`: A string representing the new directory path.
Returns `0` on success `-1` on failure (and sets errno).

#### 09). `stat`
```c
int stat(const char *pathname, struct stat *statbuf);
```
The stat() function in C is used to get information about a file or directory — like its size, permissions, timestamps, and more.
* `pathname`: Path to the file or directory.
* `statbuf`: Pointer to a struct stat where the info will be stored.
Returns, `0` on success, `-1` on failure, and sets errno.

```c
struct stat
```
– Key Fields

* `st_mode`	File type and permissions
* `st_size`	File size in bytes
* `st_uid`	User ID of owner
* `st_gid`	Group ID of owner
* `st_mtime`	Time of last modification
* `st_ctime`	Time of last status change
* `st_atime`	Time of last access

#### 10). `lstat`
```c
int lstat(const char *pathname, struct stat *statbuf);
```
The `lstat` function in C is very similar to stat(), but with one key difference: `lstat` does not follow symbolic links, while `stat` does. In writing a tool like `ls -l` or `find`, and want to list symlinks without resolving them, use `lstat`.

#### 11). `fstat`
```c
int fstat(int fd, struct stat *statbuf);
```
The `fstat` function in C is used to get file information from an open file descriptor, instead of a file path.
* `fd`: A file descriptor (from open(), fileno(), pipe(), etc.).
* `statbuf`: A pointer to a struct stat that will be filled with file info.
Returns `0` on success. `-1` on failure (check errno).

#### 12). `unlink`
```c
int unlink(const char *pathname);
```
The `unlink` function in C is used to delete (remove) a file from the filesystem.`unlink` only removes the name from the directory. The file's contents are deleted only when no process has it open and there are no other links to it. Works like `rm` command in the shell.

#### 13). `execve`
```c
int execve(const char *pathname, char *const argv[], char *const envp[]);
```
The `execve` function in C is a low-level system call that replaces the current process image with a new program — meaning it starts another program and never returns (unless there’s an error).
* `pathname`	Full path to the executable file (e.g., /bin/ls)
* `argv[]`	Null-terminated array of argument strings. argv[0] should be the program name.
* `envp[]`	Null-terminated array of environment variables (like PATH, HOME, etc.)

#### 14). `dup`
```c
int dup(int oldfd);
```
The dup() function in C is used to duplicate a file descriptor. It creates a copy of an existing file descriptor that refers to the same open file or resource.
* `oldfd` : An existing (open) file descriptor (e.g., for a file, pipe, or socket).
Returns, A new file descriptor with the lowest unused number on success or `-1` on failure (and sets errno).

#### 15). `dup2`
```c
int dup2(int oldfd, int newfd);
```
The dup2() function in C is used to duplicate a file descriptor into a specific file descriptor number, often for I/O redirection (like redirecting stdout to a file).
* `oldfd`	The original file descriptor you want to duplicate
* `newfd`	The desired file descriptor number to copy into (e.g., STDOUT_FILENO)
If newfd is already open, it is closed automatically before being reused. `dup2()` is perfect for redirecting standard input/output/error (STDIN_FILENO, STDOUT_FILENO, STDERR_FILENO).
If `oldfd == newfd`, `dup2()` does nothing and returns newfd. If newfd was already open, it will be safely closed first (to avoid leaks).
Returns the new file descriptor (newfd) on success, or `-1` on error.

#### 16). `pipe`
```c
int pipe(int pipefd[2]);
```
The pipe() function in C is used to create a unidirectional communication channel between processes — usually between a parent and child process.
It provides inter-process communication (IPC) via file descriptors: one for reading, one for writing.
pipefd: An array of two integers:
* `pipefd[0]`: Read end
* `pipefd[1]`: Write end
Returns: `0` on success `-1` on failure (and sets errno).


#### 17). `opendir`
```c
DIR *opendir(const char *name);
```
* `name` : Path to the directory (e.g., "." for the current directory).
Returns, A pointer to a `DIR structure` (directory stream) on success.`NULL` on failure (check errno for details).

```c
struct dirent
```
- Key Fields
* `d_name`	Name of the file (string)
* `d_type`	Type of file (if available, e.g. regular file, dir)

#### 18). `readdir`
```c
struct dirent *readdir(DIR *dirp);
```
The `readdir` function in C is used to read entries from an open directory stream, one at a time. It's commonly used after `opendir` to list files and subdirectories.
* `dirp` : A pointer to a DIR object returned by `opendir`.
Returns, A pointer to a `struct dirent` representing the next directory entry. `NULL` when there are no more entries (or on error).

#### 19). `closedir`
```c
int closedir(DIR *dirp);
```
The `closedir` function in C is used to close a directory stream that was opened with `opendir`.
* `dirp` : A pointer to a DIR object (returned by `opendir`).
Returns `0` on success `-1` on failure (and sets errno).

#### 20). `strerror`
```c
char *strerror(int errnum);
```
The `strerror` function in C is used to convert an error number (errno) into a human-readable error message — like "Permission denied" or "No such file or directory".
errnum: An error number, usually obtained from the global variable errno (set by functions like open(), read(), execve(), etc.).
`strerror` returns a pointer to a static string, so don’t modify or free it.

#### 21). `perror`
```c
void perror(const char *s);
```
The `perror` function in C is used to print a descriptive error message to stderr, based on the current value of the global variable errno.
* `s`: A custom message you want to prefix the error with (like the function name or context).

#### 22). `isatty`
```c
int isatty(int fd);
```
The `isatty` function in C is used to check if a file descriptor refers to a terminal (TTY).
* `fd`: A file descriptor (e.g. STDIN_FILENO, STDOUT_FILENO, or any open file descriptor).
Returns, `1` (true) if the file descriptor is connected to a terminal device. `0` (false) if not (and sets errno).

#### 23). `ttyname`
```c
char *ttyname(int fd);
```
The `ttyname` function in C is used to get the name of the terminal (TTY) device associated with a given file descriptor.
* `fd` : A file descriptor (e.g., `STDIN_FILENO`, `STDOUT_FILENO`, etc.).
Returns, A pointer to a string containing the terminal device name (like /dev/tty1 or /dev/pts/0) on success.`NULL` on
failure (e.g. if fd is not a terminal), and sets errno.

#### 24). `ttyslot`
```c
int ttyslot(void);
```
The `ttyslot` function in C is used to get the slot number of the terminal in the system's terminal database (usually /etc/ttys or /etc/utmp).
It helps identify which terminal device a user is connected to.

#### 25). `ioctl`
```c
int ioctl(int fd, unsigned long request, ...);
```
The ioctl() function in C is a powerful, low-level system call used to perform device-specific input/output operations on file descriptors — often terminals,
sockets, or special devices.
* `fd`	File descriptor (e.g., terminal, socket, file)
* `request`	Device-specific control code (macro like TIOCGWINSZ, FIONREAD, etc.)
* `...`	Optional third argument — a pointer to data to send/receive. Returns `0` on success, `-1` on failure (sets errno).

#### 26). `getenv`
```c
```
#### 27). `tcsetattr`
```c
```
#### 28). `tcgetattr`
```c
```
#### 29). `tgetent`
```c
```
#### 30). `tgetflag`
```c
```
#### 31). `tgetnum`
```c
```
#### 32). `tgetstr`
```c
```
#### 33). `tgoto`
```c
```
#### 34). `tputs`
```c
```
#### 35). `access`
```c
```
#### 36). `open`
```c
int open(const char *pathname, int flags, mode_t mode);
```
Parameters:
  * pathname: The path to the file you want to open.
  * flags: These are options that define the behavior of the open operation (e.g., ```O_RDONLY```, ```O_WRONLY```, ```O_RDWR```, ```O_CREAT```).
  * mode: The file permission settings, required when creating a new file (typically used with ```O_CREAT```). It's given in octal format (e.g., 0644).
Return value:
  On success, it returns a file descriptor (a non-negative integer).
  On failure, it returns -1, and errno is set to indicate the error.

#### 37). `read`
```c
```
#### 38). `close`
```c
```
#### 39). `fork`
```c
```
#### 40). `wait`
```c
```
#### 41). `waitpid`
```c
```
#### 42). `wait3`
```c
```
#### 43). `wait4`
```c
```
#### 44). `signal`
```c
```
#### 45). `sigaction`
```c
```
#### 46). `sigemptyset`
```c
```
#### 47). `sigaddset`
```c
```
#### 48). `kill`
```c
```
#### 49). `exit`
```c
```
#### 50). `printf`
```c
int printf(const char *format, ...);
```
`printf` prints formatted output to the standard output (usually the terminal).

#### 51). `malloc`
```c
void *malloc(size_t size);
```
`malloc` in C is used to dynamically allocate memory at runtime from the heap.

#### 52). `free`
```c
void free(void *ptr);
```
The `free` function in C is used to deallocate memory that was previously allocated with `malloc`, `calloc`, or `realloc`. It's essential for preventing memory leaks.

#### 53). `write`
```c
```


