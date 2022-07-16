#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <iostream>
#include <sys/unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fstream>
#include <sys/mount.h>
#include <sys/wait.h>
#include <list>
#include <experimental/filesystem>

#define STACK_SIZE 8192
#define FAILURE -1
#define SUCCESS 0
#define DIR_MODE 0755
#define MIN_ARG_NUM 5
#define NOTIFY_TRUE "1"

#define MOUNT_ERR "system error: could not mount proc.\n"
#define UNMOUNT_ERR "system error: could not unmount proc.\n"
#define CLONE_ERR "system error: clone system call failed.\n"
#define WAIT_ERR "system error: wait system call failed.\n"
#define ALLOC_ERR "system error: malloc failed to allocate memory.\n"
#define SETHOST_ERR "system error: sethostname system call failed.\n"
#define CHROOT_ERR "system error: chroot system call failed.\n"
#define MKDIR_ERR "system error: mkdir system call failed.\n"
#define CHDIR_ERR "system error: chdir system call failed.\n"
#define OPEN_PROCS_ERR "system error: could not open /sys/fs/cgroup/pids/cgroup.procs\n"
#define OPEN_MAX_ERR "system error: could not open /sys/fs/cgroup/pids/cgroup.max\n"
#define OPEN_NOTIFY_ERR "system error: could not open /sys/fs/cgroup/pids/cgroup.notify_on_release\n"
#define REMOVE_DIR_ERR "system error: could not remove directory.\n"

typedef struct Container {
    char* hostName;
    char* rootDir;
    char* numOfProcesses;
    char* processFileSystem;
    char** programArgs;

    Container(char** argv, int argc)
    {
        this->hostName = argv[1];
        this->rootDir = argv[2];
        this->numOfProcesses = argv[3];
        this->processFileSystem = argv[4];
        // create a new array for program arguments
        programArgs = new char*[argc - 3];
        for (int i = 0; i < argc - 3; i++)
        {
            programArgs[i] = *(argv + i + 4);
        }
        programArgs[argc - 4] = (char *) 0;
    }

    ~Container()
    {
        delete[] programArgs;
    }
} Container;

/**
 * called if a system call was resulted in an error. Prints a relevant error msg
 * from errno and exits with EXIT_FAILURE.
 * @param errorMessage the msg to print.
 */
void errorHandler(const char* errorMessage);

/**
 * the code for a new container. creates a file system, mounts proc and runs
 * the target program with included variables.
 * @param args should be converted to Container struct.
 * @return 0 on success, else exits with EXIT_FAILURE.
 */
int newContainer(void* args);

/**
 * creates a new process and child containers from paramaters given by user in
 * command line.
 * @param argc number of parameters user gave. argc - 3 is num of variables for
 *              the target program.
 * @param argv <program_name> <new_hostname> <new_filesystem_directory> <num_processes>
                <path_to_program_to_run_within_container> <[Optional]args_for_program>
 * @return 0 on success, else exits with EXIT_FAILURE.
 */
int newProcess(int argc, char** argv);

#endif //_CONTAINER_H_
