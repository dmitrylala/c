#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <elf.h>

char *input_str(char *);
uint32_t find_password_address(FILE *, uint32_t);
void rewrite_password(FILE *, uint32_t, const char *);

// Input string of unknown length
// Returns NULL in case of EOF
char *input_str(char *str) {
    int c, i = 0;
    while((c = getchar()) != '\n') {
        if (c == EOF) {
            return NULL;
        }
        ++i;
        str = realloc(str, i * sizeof(*str));
        assert(str != NULL);
        str[i - 1] = c;
    }
    i += 2;
    str = realloc(str, i * sizeof(*str));
    assert(str != NULL);
    str[i - 2] = '\n';
    str[i - 1] = '\0';

    return str;
}

// Findind password's address in game (elf) file
uint32_t find_password_address(FILE *game, uint32_t address) {
    Elf32_Ehdr elf_start;
    fseek(game, 0L, SEEK_SET);
    fread(&elf_start, sizeof(Elf32_Ehdr), 1, game);
    fseek(game, elf_start.e_shoff, SEEK_SET);
    Elf32_Shdr section_start;
    do {
        fread(&section_start, sizeof(Elf32_Shdr), 1, game);
    } while (!((address >= section_start.sh_addr) &&
            (address < section_start.sh_addr + section_start.sh_size)));
    return address - section_start.sh_addr + section_start.sh_offset;
}

// Rewriting password to a new one (let's consider that previous
// password was not empty)
void rewrite_password(FILE *game, uint32_t password_address,
                        const char *password) {
    fseek(game, password_address, SEEK_SET);
    fwrite(password, sizeof(char), strlen(password) + 1, game);
    printf("Your new password is: %s\n", password);
}


int main(int argc, char *argv[]) {

    // New password
    const char *new_pass = "ahaha\0";

    // Constants
    const char *main_str = "<main>:\0";
    const char *strcmp_str = "<strcmp@plt>\0";
    const char *dollar = "$\0";

    // Variables
    char *str = NULL, *address_str;
    uint32_t address = 0;

    while(1) {
        if ((str = input_str(str)) == NULL) {
            break;
        }
        if (strstr(str, main_str) != NULL) {
            while((str = input_str(str)) != NULL) {
                if ((address_str = strstr(str, dollar)) != NULL) {
                    sscanf(&address_str[3], "%x", &address);
                    continue;
                }
                if (strstr(str, strcmp_str) != NULL) {
                    break;
                }
            }
        }
    }
    free(str);

    FILE *game = fopen(argv[1], "r+");
    if (game == NULL) {
        perror("File opening error: ");
        exit(1);
    }

    uint32_t pass_address = find_password_address(game, address);
    rewrite_password(game, pass_address, new_pass);
    fclose(game);
    return 0;
}
