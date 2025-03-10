#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/cred.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Proxy_021");
MODULE_DESCRIPTION("A Kernel Module for Listing Tasks");
MODULE_VERSION("1.1");

const char* task_state_to_string(long state);
void dfs(struct task_struct *task);
int tasks_lister_dfs_init(void);
void tasks_lister_dfs_exit(void);

const char* task_state_to_string(long state)
{
    switch (state) {
        case TASK_RUNNING: return "TASK_RUNNING";
        case TASK_INTERRUPTIBLE: return "TASK_INTERRUPTIBLE";
        case TASK_UNINTERRUPTIBLE: return "TASK_UNINTERRUPTIBLE";
        case TASK_STOPPED: return "TASK_STOPPED";
        case TASK_TRACED: return "TASK_TRACED";
        case TASK_DEAD: return "TASK_DEAD";
        case TASK_WAKEKILL: return "TASK_WAKEKILL";
        case TASK_WAKING: return "TASK_WAKING";
        case TASK_PARKED: return "TASK_PARKED";
        case TASK_NOLOAD: return "TASK_NOLOAD";
        case TASK_NEW: return "TASK_NEW";
        default: return "UNKNOWN";
    }
}

void dfs(struct task_struct *task)
{
    struct task_struct *task_next;
    struct list_head *list;

    list_for_each(list, &task->children) {
        task_next = list_entry(list, struct task_struct, sibling);

        const char *state_str = task_state_to_char(task_next);
        
        unsigned int uid = task_next->cred->uid.val;
        unsigned int gid = task_next->cred->gid.val;
        const char *command = task_next->comm;

        printk(KERN_INFO "Process ID: %d\nUser ID : %u\nGroup ID: %u\nCommand Path: %s\nState: %s\n\n",
               task_next->pid, uid, gid, command, state_str);

        dfs(task_next);
    }
}

int tasks_lister_dfs_init(void)
{
    printk(KERN_INFO "Loading module nd getting all tasks ..\n");

    dfs(&init_task);

    printk(KERN_INFO "Module loaded.\n");
    return 0;
}

void tasks_lister_dfs_exit(void)
{
    printk(KERN_INFO "Module removed.\n");
}

module_init(tasks_lister_dfs_init);
module_exit(tasks_lister_dfs_exit);

