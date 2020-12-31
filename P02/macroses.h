// macroses
#define ERR_MEM(call, err) do { \
                            if ((call) == NULL) { \
                                char buf_err[] = {"error memory allocation\n"}; \
                                write(2, buf_err, sizeof buf_err); \
                                return err; \
                            } \
                        } while (0)

#define ERR(call, val, err) do { \
                    if ((call) != val) return err; \
                } while (0)


// graph errors processing
#define SAFE(call) do { \
                    if ((call) != 0) { \
                        if ((call) == BAD_FD) { \
                            fprintf(stderr, "Bad file descriptor\n"); \
                        } else if ((call) == WRONG_INPUT) { \
                            fprintf(stderr, "Wrong graph input\n"); \
                        } else if ((call) == WRONG_WEIGHT) { \
                            fprintf(stderr, "Wrong graph weight\n"); \
                        } else if ((call) == WRONG_KEY) { \
                            fprintf(stderr, "Wrong graph key value\n"); \
                        } else if ((call) == NO_SUCH_NODE) { \
                            fprintf(stderr, "No such node in graph\n"); \
                        } else if ((call) == NO_PATH) { \
                            fprintf(stderr, "No such shortest path in graph\n"); \
                        } else if ((call) == NO_MEDIAN) { \
                            fprintf(stderr, "No median in graph\n"); \
                        } \
                        return call; \
                    } \
                } while (0)
