// errors
#define TOO_FEW_PARAMETERS 1
#define ERR_OPENING_FILE 2
#define ERR_CREATING_FILE 3
#define ERR_DELETING_FILE 4
#define ILLEGAL_OPTION 5
#define CRITICAL_ERROR 6
#define IS_A_DIRECTORY 7
#define ERR_MEMORY_ALLOCATION 8


// macroses
#define ERR_MSG(call, msg, err) \
    do { \
        if (call == -1) { \
            char buf_err[] = msg; \
            ERR(write(2, buf_err, sizeof buf_err)); \
            return err; \
        } \
    } while (0)

#define ERR(call) if (call == -1) return CRITICAL_ERROR

#define ERR_MEM(call) \
    do { \
        if (call == NULL) { \
            char buf_err[] = {"error memory allocation\n"}; \
            ERR(write(2, buf_err, sizeof buf_err)); \
            return ERR_MEMORY_ALLOCATION; \
        } \
    } while (0)
