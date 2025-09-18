# UNIX-Customized-Virtual-File-System


This project is a command-line-based simulation of a virtual file system implemented in C++. It manages files entirely in memory, mimicking the core concepts of a traditional file system like inodes, file descriptors, and system calls for file manipulation.

---

## Features ✨

* **In-Memory Management:** All files and their metadata are stored in memory during runtime, with no actual disk I/O.
* **Inode-Based:** Uses a linked list of inodes to manage file metadata, including permissions, size, and type.
* **File Descriptor Table:** Manages open files using a User File Descriptor Table (UFDT), similar to Unix-like operating systems.
* **Rich Command Set:** Provides a shell interface with commands for common file operations.
* **Built-in Help:** Includes a `man` command to get details on specific commands and a `help` command to list all available options.

---

## How to Compile and Run 🚀

You will need a C++ compiler (like G++) to build and run this project.

1.  **Save the Code:** Save the code into a file named `cvfs.cpp`.

2.  **Compile the Code:** Open your terminal or command prompt and run the following command:
    ```bash
    g++ cvfs.cpp -o unix_cvfs
    ```

3.  **Run the Application:** Execute the compiled program to start the virtual file system shell:
    ```bash
    ./unix_cvfs
    ```
    You will see the "UNIX CVFS" prompt and can start using the commands.

---

## Available Commands 💻

Here is a list of all the commands supported by the Marvellous CVFS shell:

| Command | Usage Example | Description |
| :--- | :--- | :--- |
| `creat` | `creat demo.txt 3` | Creates a new regular file with a given name and permission (1: read, 2: write, 3: read+write). |
| `read` | `read 0 10` | Reads a specified number of bytes from a file using its file descriptor. |
| `write` | `write 0` | Writes data to a file using its file descriptor. The shell will prompt you to enter the data. |
| `ls` | `ls` | Lists the names of all files currently existing in the file system. |
| `stat` | `stat demo.txt` | Displays detailed statistical information about a file, such as its size, inode number, and permissions. |
| `unlink` | `unlink demo.txt` | Deletes a file from the file system. |
| `man` | `man creat` | Shows the manual page with a description and usage instructions for a specific command. |
| `help` | `help` | Displays a list of all available commands. |
| `clear` | `clear` | Clears the terminal screen. |
| `exit` | `exit` | Terminates the CVFS shell and ends the program. |
