# Kernel Module for Listing Tasks Using DFS

This project is a Linux kernel module that lists tasks (processes) using a Depth-First Search (DFS) approach. The module traverses the task list starting from the initial task (`init_task`) and prints information about each task, including its PID, UID, GID, command path, and state.

## Features

- Lists all tasks in the system using DFS.
- Prints task information including PID, UID, GID, command path, and state.
- Supports various task states such as `TASK_RUNNING`, `TASK_INTERRUPTIBLE`, `TASK_UNINTERRUPTIBLE`, etc.

## Installation

1. Clone the repository:
    ```sh
    git clone https://github.com/yourusername/your-repo-name.git
    cd your-repo-name
    ```

2. Build the module:
    ```sh
    make
    ```

3. Load the module:
    ```sh
    sudo insmod tasks_lister_dfs.ko
    ```

4. Check the kernel log to see the output:
    ```sh
    dmesg
    ```

## Usage

Once the module is loaded, it will automatically start listing tasks using DFS and print the information to the kernel log. You can view the output using the `dmesg` command.

## Uninstallation

1. Remove the module:
    ```sh
    sudo rmmod tasks_lister_dfs
    ```

2. Clean the build files:
    ```sh
    make clean
    ```

## Files

- `tasks_lister_dfs.c`: The main source code for the kernel module.
- `Makefile`: The makefile for building the kernel module.

## License

This project is licensed under the GPL License - see the [LICENSE](LICENSE) file for details.

## Author

- Proxy_021

## Acknowledgments

- Linux Kernel Module Programming Guide
- Linux Kernel Documentation