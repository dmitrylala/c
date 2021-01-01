#include <stdio.h>
#include <string.h>

int main(int argc, char **argv) {
    int temp_arr[256] = {0};
    int flag = 0;
    printf("%s%c", argv[0], ' ');
    for (int i = 1; i < argc; ++i) {
        if (flag == 1) {
            temp_arr[i] = 1;
        } else {
            if ((strcmp(argv[i], "-a") == 0) || (strcmp(argv[i], "-X") == 0) ||
                (strcmp(argv[i], "-C") == 0) || (strcmp(argv[i], "-h") == 0)) {
                printf("%s%c%s%c", argv[i], ' ', argv[i + 1], ' ');
                ++i;
            } else {
                if (strcmp(argv[i], "--") == 0) {
                    flag = 1;
                } else {
                    if ((argv[i][0] == '-') && (strlen(argv[i]) != 1)) {
                        printf("%s%c", argv[i], ' ');
                    } else {
                        temp_arr[i] = 1;
                    }
                }
            }
        }
    }
    printf("%s", "-- ");
    for (int i = 0; i < argc; ++i) {
        if (temp_arr[i] == 1) {
            printf("%s%c", argv[i], ' ');
        }
    }
    printf("\n");
    return 0;
}

