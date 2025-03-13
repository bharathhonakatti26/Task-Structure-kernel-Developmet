#include <linux/init_task.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/cred.h>
#include <linux/mm.h>
#include <linux/sched/mm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Proxy_021");
MODULE_DESCRIPTION("A Kernel Module for Listing Tasks");
MODULE_VERSION("1.1");

const char* get_task_state_string(long state);
void print_memory_map(struct task_struct *task);
void traverse_process_tree(struct task_struct *parent_process);
int process_tree_init(void);
void process_tree_exit(void);

const char* get_task_state_string(long state){
    switch (state){
        case TASK_RUNNING: return "Running";
        case TASK_INTERRUPTIBLE: return "Interruptible";
        case TASK_UNINTERRUPTIBLE: return "Uninterruptible";
        case TASK_STOPPED: return "Stopped";
        case TASK_TRACED: return "Traced";
        case EXIT_DEAD: return "Dead";
        case EXIT_ZOMBIE: return "Zombie";
        case TASK_PARKED: return "Parked";
        case TASK_DEAD: return "Dead";
        case TASK_WAKEKILL: return "Wake Kill";
        case TASK_WAKING: return "Waking";
        default: return "Unknown";
    }
}

void print_memory_map(struct task_struct *task){
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    struct vma_iterator vmi;

    mm = get_task_mm(task);
    if (!mm){
        printk(KERN_INFO "Process ID: %d has no memory mapping (kernel thread or exited)\n", task->pid);
        return;
    }

    down_read(&mm->mmap_lock);

    printk(KERN_INFO "Memory Map for Process ID: %d\n", task->pid);

    vma_iter_init(&vmi, mm, 0);

    while ((vma = vma_next(&vmi)) != NULL){  
        printk(KERN_INFO "Start: %lx, End: %lx, Flags: %lx\n",
               vma->vm_start, vma->vm_end, vma->vm_flags);
    }

    up_read(&mm->mmap_lock);
    mmput(mm);
}

void traverse_process_tree(struct task_struct *parent_process){
    struct task_struct *child_process;
    struct list_head *list;

    list_for_each(list, &parent_process->children){
        child_process = list_entry(list, struct task_struct, sibling);

        const char *state_description = get_task_state_string(child_process->__state);
        unsigned int user_id = __kuid_val(child_process->cred->uid);
        unsigned int group_id = __kgid_val(child_process->cred->gid);
        const char *process_name = child_process->comm;

        printk(KERN_INFO "Process ID: %d\nUser ID: %u\nGroup ID: %u\nProcess Name: %s\nState: %s\n",
               child_process->pid, user_id, group_id, process_name, state_description);

        print_memory_map(child_process);

        traverse_process_tree(child_process);
    }
}

int process_tree_init(void){
    printk(KERN_INFO "Loading process tree module and listing processes...\n");
    traverse_process_tree(&init_task);
    printk(KERN_INFO "Process tree module loaded successfully.\n");
    return 0;
}

void process_tree_exit(void){
    printk(KERN_INFO "Unloading process tree module.\n");
}

module_init(process_tree_init);
module_exit(process_tree_exit);