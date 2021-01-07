// errors
#define ERR_MEM_ALLOC 1
#define SYS_ERR 2
#define CRIT_ERR 3
#define WRONG_INPUT 4
#define BAD_FD 5
#define WRONG_SIZE 6
#define NO_SUCH_INDEX 7
#define WRONG_INDEX 8
#define NO_METHOD 9
#define WRONG_METHOD 10


// macroses
#define ERR_MEM(call, err) \
    do { \
        if ((call) == NULL) { \
            char buf_err[] = {"error memory allocation\n"}; \
            write(2, buf_err, sizeof buf_err); \
            return err; \
        } \
    } while (0)

#define ERR(call, val, err) \
    do { \
        if ((call) != val) return err; \
    } while (0)

#define ERR_ACT(call, val, err, action) \
    do { \
        if ((call) != val) { \
            action; \
            return err; \
        } \
    } while (0)

#define ERR_ACTS(call, val, err, action1, action2) \
    do { \
        if ((call) != val) { \
            action1; \
            action2; \
            return err; \
        } \
    } while (0)

#define ERR_ACT_EQ(call, val, err, action) \
    do { \
        if ((call) == val) { \
            action; \
            return err; \
        } \
    } while (0)

#define ERR_SYS(call) \
    do { \
        if ((call) == -1) { \
            char buf_err[] = {"error in system call\n"}; \
            write(2, buf_err, sizeof buf_err); \
            return SYS_ERR; \
        } \
    } while (0)

#define ERR_CRIT(call) \
    do { \
        if ((call) == -1) return CRIT_ERR; \
    } while (0)

// errors processing in main
#define RAISE(call) \
    do { \
        if ((call) != 0) { \
            if ((call) == ERR_MEM_ALLOC) { \
                fprintf(stderr, "Error memory allocation\n"); \
            } else if (((call) == -1) || ((call) == SYS_ERR)) { \
                fprintf(stderr, "Error in system call\n"); \
            } else if ((call) == WRONG_INPUT) { \
                fprintf(stderr, "Error: wrong input\n"); \
            } else if ((call) == BAD_FD) { \
                fprintf(stderr, "Error: bad file descriptor\n"); \
            } else if ((call) == WRONG_SIZE) { \
                fprintf(stderr, "Error: bad sizes\n"); \
            } else if ((call) == NO_SUCH_INDEX) { \
                fprintf(stderr, "Error: no such index\n"); \
            } else if ((call) == WRONG_INDEX) { \
                fprintf(stderr, "Error: wrong index\n"); \
            } else if ((call) == NO_METHOD) { \
                fprintf(stderr, "Error: no method entered\n"); \
            } else if ((call) == WRONG_METHOD) { \
                fprintf(stderr, "Error: wrong method\n"); \
            } \
            return (call); \
        } \
    } while (0)
