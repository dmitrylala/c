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
                fprintf(stderr, "Error: wrong matrix input\n"); \
            } else if ((call) == BAD_FD) { \
                fprintf(stderr, "Error: bad file descriptor\n"); \
            } \
            return (call); \
        } \
    } while (0)
