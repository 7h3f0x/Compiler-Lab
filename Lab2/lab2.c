#include <stdio.h>
#include <regex.h>
#include <stdbool.h>
#include <string.h>

bool get_input(char *buf, size_t size) {
    char* res = fgets(buf, size, stdin);
    if (res == NULL) {
        return false;
    }
    char *ch = strchr(buf, '\n');
    if (ch != NULL) {
        *ch = '\0';
    }
    return true;
}

// 'a*', 'a*b+', 'abb'
char* patterns[] = {
    "^a*$",
    "^a*b+$",
    "^abb$"
};

int main(void) {
    regex_t regs[3];
    for (int idx = 0; idx < (sizeof(regs)/sizeof(regs[0])); ++idx) {
        regcomp(&regs[idx], patterns[idx], REG_EXTENDED);
    }

    char input[0x100];

    while (true) {
        bool cont = get_input(input, sizeof(input));
        if (cont == false) {
            break;
        }

        for (int idx = 0; idx < (sizeof(regs)/sizeof(regs[0])); ++idx) {
            int res = regexec(&regs[idx], input, 0, NULL, 0);
            switch (res) {
                case 0:
                    printf("%s is accepted under rule '%s'\n", input, patterns[idx]);
                    break;
                case REG_NOMATCH:
                    break;
                default:
                    fprintf(stderr, "something weird happened for %s, %s\n", input, patterns[idx]);
            }
        }
        
    }

    for (int idx = 0; idx < (sizeof(regs)/sizeof(regs[0])); ++idx) {
        regfree(&regs[idx]);
    }

    return 0;
}
