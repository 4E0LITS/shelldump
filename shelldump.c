/*
A tiny utility that dumps files as escaped byte sequences
*/

#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const char usageinfo[] =
"Usage:\n"
"  shelldump [options] <file>\n"
"Dump files as escaped byte sequences\n"
""
"  -h  --help          :: this\n"
"  -r  --rows <width>  :: dump as rows of <width> escaped bytes\n"
"  -v  --verbose       :: be verbose (print each filename)\n";

int main(int argc, char* argv[]) {
    int help_flag = 0;    // holds help_flag for optparsing, later recycled to hold exit status
    int verbose_flag = 0;
    int row_width = 0;    // holds desired printed row width
    char* fail = NULL;    // points to invalid entry if invalid entry was received

    int fd;

    // general purpose counters
    int idx;              
    int c;

    char* buf;
    size_t bufsize;
    size_t chksize;

    /*
    parse opts
    */

    const struct option long_options[] = {
        {"help", 0, &help_flag, 1},
        {"rows", 1, NULL, 'r'},
        {"verbose", 0, &verbose_flag, 1},
        {NULL, 0, NULL, 0}
    };

    while ((c = getopt_long(argc, argv, "hr:v", long_options, NULL)) != -1) switch (c) {
        case 'h':
            help_flag = 1;
            break;
        case 'r':
            if ((row_width = atoi(optarg)) <= 0)
                fail = optarg;

            break;
        case 'v':
            verbose_flag = 1;
            break;
    }

    if (help_flag) {
        puts(usageinfo);
        return -1;
    }

    help_flag = 0;

    if (NULL != fail) {
        printf("option -r requires an unsigned int but received '%s'\n", fail);
        return -1;
    }

    /*
    allocate buffer for reading
    */

    if (row_width) {
        bufsize = (size_t) row_width;
    } else {
        bufsize = 32;
    }

    // accomodate NULL terminators
    if ((buf = malloc(bufsize + 1)) == NULL) {
        puts("[!] allocation failure\n");
        return 1;
    };

    for (c = optind; c < argc; c++) {
        if ((fd = open(argv[c], O_RDONLY)) == -1) {
            printf("[!] skipped %s: %s\n\n", argv[c], strerror(errno));
            help_flag = 1;
            continue;
        }

        if (verbose_flag)
            printf("\n%s:\n", argv[c]);

        // write escaped sequences
        putchar('\"');

        while ((chksize = read(fd, buf, bufsize)) > 0) {
            for (idx = 0; idx < chksize; idx++)
                printf("\\x%02x", (unsigned char) buf[idx]);
            if (row_width && chksize == bufsize)
                fputs("\"\n\"", stdout);
        }

        puts("\"");

        if (-1 == chksize) {
            printf("[!] error reading %s: %s\n\n", argv[c], strerror(errno));
            help_flag = 1;
        }
    }

    free(buf);
    
    if (help_flag)
        puts("One or more errors occurred.");
    
    return help_flag;
}