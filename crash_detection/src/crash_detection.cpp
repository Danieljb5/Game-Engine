#include <base/library_loader.hpp>
#include <base/platform.hpp>

#ifdef LINUX
#include <execinfo.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <unistd.h>
typedef struct _sig_ucontext
{
    unsigned long uc_flags;
    ucontext_t* uc_link;
    stack_t uc_stack;
    sigcontext uc_mcontext;
    sigset_t ic_sigmask;
} sig_ucontext_t;

void* get_addr(const char* message)
{
    long addr = 0;
    char line[256];
    strcpy(line, message);
    char* addr_str = strtok(line, "+");
    addr_str = strtok(NULL, ")");
    addr = strtol(addr_str, NULL, 0);

    char* func_name = strtok(line, "(");
    func_name = strtok(NULL, "+");
    if(func_name != 0)
    {
        int p = 0;
        while(message[p] != '(' && message[p] != ' ' && message[p] != 0) ++p;
        char syscom[512] = {0};

        FILE* fp;
        int status;
        char output[256];
        sprintf(syscom, "nm -C %.*s | grep %s", p, message, func_name);
        fp = popen(syscom, "r");
        if(fp == NULL)
        {
            printf("error opening pipe\n");
            exit(1);
        }
        while(fgets(output, 256, fp) != NULL)
        {
            if(output != 0)
            {
                int i = 0;
                while(output[i] != ' ') ++i;
                char offset_addr_str[256] = {0};
                sprintf(offset_addr_str, "%.*s", i, output);
                addr += strtol(offset_addr_str, NULL, 16);
            }
        }

        pclose(fp);
    }

    return (void*)addr;
}

void crit_err_hdlr(int sig_num, siginfo_t* info, void* ucontext)
{
    void* array[128];
    void* caller_address;
    char** messages;
    int size, i;
    sig_ucontext_t* uc;

    uc = (sig_ucontext_t*)ucontext;

#if defined(__i386__)
    caller_address = (void*)uc->uc_mcontext.eip;
#elif defined(__x86_64__)
    caller_address = (void*)uc->uc_mcontext.rip;
#else
#error Unsupported architecture
#endif

    fprintf(stderr, "signal %d (%s), address is %p from %p\n", sig_num, strsignal(sig_num), info->si_addr, (void*)caller_address);

    size = backtrace(array, 128);

    array[1] = caller_address;

    messages = backtrace_symbols(array, size);

    for(i = 1; i < size && messages != NULL; ++i)
    {
        fprintf(stderr, "[bt]: (%d) %s\n", i, messages[i]);
        int p = 0;
        while(messages[i][p] != '(' && messages[i][p] != ' ' && messages[i][p] != 0) ++p;
        char syscom[512] = {0};
        sprintf(syscom, "addr2line -C -f -p -e %.*s %p\n", p, messages[i], get_addr(messages[i]));
        int ret = system(syscom);
        fprintf(stderr, "\n");
    }

    free(messages);
    
    exit(1);
}
#endif

extern "C"
{
    void init(LibraryManager* libManager)
    {
#ifdef LINUX
        struct sigaction sigact;
        sigact.sa_sigaction = crit_err_hdlr;
        sigact.sa_flags = SA_RESTART | SA_SIGINFO;

        if(sigaction(SIGSEGV, &sigact, (struct sigaction*)NULL) != 0)
        {
            fprintf(stderr, "error setting signal handler for %d (%s)\n", SIGSEGV, strsignal(SIGSEGV));
            exit(1);
        }
#endif
    }

#ifdef DEBUG
    void create_sigsegv() // intentionally creates a segfault to test the signal handler
    {
        int* foo = (int*)-1;
        std::cout << *foo << "\n";
    }
#endif
}