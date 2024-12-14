#include<linux/kernel.h>
#include <linux/module.h> /* Needed by all modules */
#include <linux/printk.h> /* Needed for pr_info() */
#include<linux/proc_fs.h> /* Necessary because we use the proc fs */
#include <linux/version.h>
#include <linux/time.h>
#include <linux/timekeeping.h>
#include <linux/string.h>

#define procfs_name "tsulab"
static struct proc_dir_entry *our_proc_file = NULL;

MODULE_LICENSE("GPL");

static ssize_t procfile_read(
    struct file *file_pointer, char __user *buffer, 
    size_t buffer_length, loff_t* offset) 
{
    struct timespec64 now;
    ktime_get_real_ts64(&now);
    struct tm tm_now;
    time64_to_tm(now.tv_sec, 0, &tm_now);
    int current_day = tm_now.tm_mday;

    char s[240] = "       Декабрь 2024\n\nПн  Вт  Ср  Чт  Пт  Сб  Вс\n";
    int week_pos = 6;
    for (int i = 0; i < week_pos; ++i) {
        strcat(s, " ");
        strcat(s, " ");
        strcat(s, " ");
        strcat(s, " ");
    }

    char buf[3];
    for (long i = 1; i <= 31; ++i, ++week_pos) {
        if (week_pos == 0) {
                strcat(s, "\n");
        }

        if (i < 10) {
            strcat(s, "0");
        }
        
        sprintf(buf, "%lu", i);
        strcat(s, buf);
        
        if (i == current_day) {
            strcat(s, "*");
        }
        else {
            strcat(s, " ");
        }
        strcat(s, " ");

        if (week_pos == 6) {
                week_pos = -1;
        }
    }
    if(week_pos != -1) {
        strcat(s, "\n");
    }

    int len = sizeof(s); 
    ssize_t ret = len; 

    if (*offset >= len || copy_to_user(buffer, s, len)) { 
        pr_info("copy_to_user failed\n"); 
        ret = 0; 
    } else { 
        pr_info("procfile read %s\n", 
        file_pointer->f_path.dentry->d_name.name); 
        *offset += len; 
    } 

    return ret;
}

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
    static const struct proc_ops proc_file_fops = {
    .proc_read = procfile_read,
};
#else
    static const struct file_operations proc_file_fops = {
    .read = procfile_read,
};
#endif


static int __init procfs1_init(void)
{
    our_proc_file = proc_create(
    procfs_name, 0644, NULL, &proc_file_fops);
    return 0;
}

static void __exit procfs1_exit(void)
{
    proc_remove(our_proc_file);
    pr_info("/proc/%s removed\n", procfs_name);
}

module_init(procfs1_init);
module_exit(procfs1_exit);
