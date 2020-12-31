// macroses
#define ERR_MEM(call, err) do { \
                            if ((call) == NULL) { \
                                char buf_err[] = {"error memory allocation\n"}; \
                                write(2, buf_err, sizeof buf_err); \
                                return err; \
                            } \
                        } while (0)

#define SAFE(call) do { \
                        flag = call; \
                        if (flag != 0) { \
                            goto FINALLY; \
                        } \
                    } \
                    while (0)

// errors processing
#define RAISE_1(call) do { \
                        err = call; \
                        if (err != 0) { \
                            if (err == 11) { \
                                fprintf(stderr, "Error: database overflow\n"); \
                            } else if (err == 12) { \
                                fprintf(stderr, "Error: empty database\n"); \
                            } else if (err == 13) { \
                                fprintf(stderr, "Error: bad variable initialization\n"); \
                            } else if (err == 18) { \
                                fprintf(stderr, "Error: memory allocation\n"); \
                            } \
                            goto FINALLY_BASE; \
                        } \
                    } \
                    while (0)


#define RAISE_2(call) do { \
                        err = call; \
                        if (err != 0) { \
                            if (err == 1) { \
                                printf("Syntax error\n"); \
                            } else if (err == 2) { \
                                printf("Error: brackets disbalance - missing closing parenthesis\n"); \
                            } else if (err == 3) { \
                                printf("Error: brackets disbalance - missing opening parenthesis\n"); \
                            } else if (err == 4) { \
                                printf("Error: invalid operation\n"); \
                            } else if (err == 5) { \
                                printf("Error: invalid operand\n"); \
                            } else if (err == 6) { \
                                printf("Error: missing operand\n"); \
                            } else if (err == 7) { \
                                printf("Error: division by zero\n"); \
                            } else if (err == 8) { \
                                printf("Error: polis overflow\n"); \
                            } else if (err == 9) { \
                                printf("Error: missing operator\n"); \
                            } else if (err == 10) { \
                                printf("Error: uninitialized variable\n"); \
                            } else if (err == 15) { \
                                printf("Error: stack overflow\n"); \
                            } else if (err == 15) { \
                                printf("Error: stack underflow\n"); \
                            } else if (err == 17) { \
                                printf("Error: incorrect value in polis\n"); \
                            } else if (err == 18) { \
                                fprintf(stderr, "Error: memory allocation\n"); \
                            } \
                            goto FINALLY; \
                        } \
                    } \
                    while (0)