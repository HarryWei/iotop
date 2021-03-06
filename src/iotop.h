#ifndef __IOTOP_H__
#define __IOTOP_H__

#define _POSIX_C_SOURCE 1
#define _BSD_SOURCE 1

#include <sys/types.h>
#include <stdint.h>

#define VERSION "0.1"

typedef union
{
    struct _flags
    {
        int batch_mode;
        int only;
        int processes;
        int accumulated;
        int kilobytes;
        int timestamp;
        int quite;
    } f;
    int opts[7];
} config_t;

typedef struct
{
    int iter;
    int delay;
    int pid;
    int user_id;
} params_t;

extern config_t config;
extern params_t params;


struct xxxid_stats
{
	/* Some fields copy from kernel/include/uapi/linux/taskstats.h */
    pid_t tid;
	int nl_sock;
	int nl_fam_id;
    uint64_t swapin_delay_total;  // nanoseconds
    uint64_t blkio_delay_total;  // nanoseconds
    uint64_t read_bytes;
    uint64_t write_bytes;

    uint64_t ac_utime; //microseconds
    uint64_t ac_stime; //microseconds

    double blkio_val;
    double swapin_val;
    double read_val;
    double write_val;

    int io_prio;

    int euid;
    char *cmdline;
	int cmd_type;

    void *__next;
};

void nl_init(struct xxxid_stats *cs);
void nl_term(struct xxxid_stats *cs);

int nl_xxxid_info(pid_t xxxid, int isp, struct xxxid_stats *stats);
void dump_xxxid_stats(struct xxxid_stats *stats);

typedef int (*filter_callback)(struct xxxid_stats *);

struct xxxid_stats* fetch_data(int processes, filter_callback);
void free_stats_chain(struct xxxid_stats *chain);

typedef void (*view_callback)(struct xxxid_stats *current, struct xxxid_stats *prev);

void view_batch(struct xxxid_stats *, struct xxxid_stats *);
void view_curses(struct xxxid_stats *, struct xxxid_stats *);
void view_curses_finish();

typedef int (*how_to_sleep)(unsigned int seconds);
int curses_sleep(unsigned int seconds);

/* utils.c */

enum
{
    PIDGEN_FLAGS_PROC,
    PIDGEN_FLAGS_TASK
};

struct pidgen
{
    void *__proc;
    void *__task;
    int __flags;
};

const char *xprintf(const char *format, ...);
const char *read_cmdline2(int pid);

struct pidgen *openpidgen(int flags);
void closepidgen(struct pidgen *pg);
int pidgen_next(struct pidgen *pg);

/* ioprio.h */

int get_ioprio(pid_t pid);
const char *str_ioprio(int io_prio);

//For control program
int get_taskstats(int pid, struct xxxid_stats *cs);
void cal_io_percent(struct xxxid_stats *prev, struct xxxid_stats *cs, int window);

#endif // __IOTOP_H__

