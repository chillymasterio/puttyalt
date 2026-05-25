/*
 * PuttyAlt Linux launcher — terminal SSH client.
 * Uses system ssh with PuttyAlt session management overlay.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#define VERSION "2.0.2"
#define CONFIG_DIR ".puttyalt"
#define SESSIONS_FILE "sessions.conf"
#define MAX_SESSIONS 256
#define MAX_LINE 512

typedef struct {
    char name[128];
    char host[256];
    int port;
    char user[64];
    char keyfile[256];
    char label[32];
    int rating;
} Session;

static Session g_sessions[MAX_SESSIONS];
static int g_session_count = 0;
static char g_config_dir[512];

static void init_config_dir(void)
{
    const char *home = getenv("HOME");
    if (!home) home = "/tmp";
    snprintf(g_config_dir, sizeof(g_config_dir), "%s/%s", home, CONFIG_DIR);
    mkdir(g_config_dir, 0700);
}

static void load_sessions(void)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", g_config_dir, SESSIONS_FILE);
    FILE *fp = fopen(path, "r");
    if (!fp) return;
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), fp) && g_session_count < MAX_SESSIONS) {
        Session *s = &g_sessions[g_session_count];
        if (sscanf(line, "%127[^|]|%255[^|]|%d|%63[^|]|%255[^|]|%31[^|]|%d",
                   s->name, s->host, &s->port, s->user, s->keyfile, s->label, &s->rating) >= 2) {
            if (s->port <= 0) s->port = 22;
            g_session_count++;
        }
    }
    fclose(fp);
}

static void save_sessions(void)
{
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", g_config_dir, SESSIONS_FILE);
    FILE *fp = fopen(path, "w");
    if (!fp) return;
    for (int i = 0; i < g_session_count; i++) {
        Session *s = &g_sessions[i];
        fprintf(fp, "%s|%s|%d|%s|%s|%s|%d\n",
                s->name, s->host, s->port, s->user, s->keyfile, s->label, s->rating);
    }
    fclose(fp);
}

static void print_banner(void)
{
    printf("\033[36m");
    printf("  ____        _   _          _    _ _   \n");
    printf(" |  _ \\ _   _| |_| |_ _   _/ \\  | | |_ \n");
    printf(" | |_) | | | | __| __| | | / _ \\ | | __|\n");
    printf(" |  __/| |_| | |_| |_| |_/ ___ \\| | |_ \n");
    printf(" |_|    \\__,_|\\__|\\__|\\__/_/   \\_\\_|\\__|\n");
    printf("\033[0m");
    printf("  v%s — Modern SSH Terminal\n\n", VERSION);
}

static void list_sessions(void)
{
    if (g_session_count == 0) {
        printf("  No saved sessions. Use 'add' to create one.\n\n");
        return;
    }
    printf("  \033[1m%-4s %-20s %-30s %-6s %-8s\033[0m\n", "#", "Name", "Host", "Port", "Label");
    printf("  %-4s %-20s %-30s %-6s %-8s\n", "---", "----", "----", "----", "-----");
    for (int i = 0; i < g_session_count; i++) {
        Session *s = &g_sessions[i];
        char hoststr[300];
        if (s->user[0])
            snprintf(hoststr, sizeof(hoststr), "%s@%s", s->user, s->host);
        else
            snprintf(hoststr, sizeof(hoststr), "%s", s->host);
        printf("  %-4d %-20s %-30s %-6d %-8s\n", i+1, s->name, hoststr, s->port, s->label);
    }
    printf("\n");
}

static void connect_session(Session *s)
{
    char cmd[1024];
    int pos = 0;
    pos += snprintf(cmd + pos, sizeof(cmd) - pos, "ssh");
    if (s->port != 22)
        pos += snprintf(cmd + pos, sizeof(cmd) - pos, " -p %d", s->port);
    if (s->keyfile[0])
        pos += snprintf(cmd + pos, sizeof(cmd) - pos, " -i %s", s->keyfile);
    if (s->user[0])
        pos += snprintf(cmd + pos, sizeof(cmd) - pos, " %s@%s", s->user, s->host);
    else
        pos += snprintf(cmd + pos, sizeof(cmd) - pos, " %s", s->host);

    printf("  \033[33m> %s\033[0m\n\n", cmd);
    system(cmd);
}

static void add_session(void)
{
    if (g_session_count >= MAX_SESSIONS) { printf("  Max sessions reached.\n"); return; }
    Session *s = &g_sessions[g_session_count];
    memset(s, 0, sizeof(*s));

    printf("  Name: "); fflush(stdout);
    if (!fgets(s->name, 128, stdin) || !s->name[0]) return;
    s->name[strcspn(s->name, "\n")] = 0;

    printf("  Host: "); fflush(stdout);
    if (!fgets(s->host, 256, stdin) || !s->host[0]) return;
    s->host[strcspn(s->host, "\n")] = 0;

    char buf[64];
    printf("  Port [22]: "); fflush(stdout);
    if (fgets(buf, sizeof(buf), stdin) && buf[0] != '\n')
        s->port = atoi(buf);
    else
        s->port = 22;

    printf("  User (optional): "); fflush(stdout);
    if (fgets(s->user, 64, stdin)) s->user[strcspn(s->user, "\n")] = 0;

    printf("  Key file (optional): "); fflush(stdout);
    if (fgets(s->keyfile, 256, stdin)) s->keyfile[strcspn(s->keyfile, "\n")] = 0;

    printf("  Label (optional): "); fflush(stdout);
    if (fgets(s->label, 32, stdin)) s->label[strcspn(s->label, "\n")] = 0;

    g_session_count++;
    save_sessions();
    printf("  \033[32mSession '%s' saved.\033[0m\n\n", s->name);
}

static void interactive_mode(void)
{
    char input[256];
    print_banner();
    printf("  Commands: list, add, connect <#>, delete <#>, quit\n\n");

    while (1) {
        printf("  \033[36mputtyalt>\033[0m ");
        fflush(stdout);
        if (!fgets(input, sizeof(input), stdin)) break;
        input[strcspn(input, "\n")] = 0;
        if (!input[0]) continue;

        if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0 || strcmp(input, "q") == 0)
            break;
        else if (strcmp(input, "list") == 0 || strcmp(input, "ls") == 0)
            list_sessions();
        else if (strcmp(input, "add") == 0 || strcmp(input, "new") == 0)
            add_session();
        else if (strncmp(input, "connect ", 8) == 0 || strncmp(input, "c ", 2) == 0) {
            int idx = atoi(input + (input[1] == ' ' ? 2 : 8)) - 1;
            if (idx >= 0 && idx < g_session_count)
                connect_session(&g_sessions[idx]);
            else
                printf("  Invalid session number.\n");
        }
        else if (strncmp(input, "delete ", 7) == 0 || strncmp(input, "rm ", 3) == 0) {
            int idx = atoi(input + (input[1] == 'm' ? 3 : 7)) - 1;
            if (idx >= 0 && idx < g_session_count) {
                printf("  Deleted '%s'\n", g_sessions[idx].name);
                memmove(&g_sessions[idx], &g_sessions[idx+1],
                        sizeof(Session) * (g_session_count - idx - 1));
                g_session_count--;
                save_sessions();
            }
        }
        else if (strcmp(input, "help") == 0 || strcmp(input, "?") == 0)
            printf("  Commands: list, add, connect <#>, delete <#>, quit\n\n");
        else
            printf("  Unknown command. Type 'help'.\n");
    }
}

int main(int argc, char **argv)
{
    init_config_dir();
    load_sessions();

    if (argc >= 2) {
        /* Direct connect: puttyalt user@host:port */
        if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
            printf("PuttyAlt %s\n", VERSION);
            return 0;
        }
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf("Usage: puttyalt [user@]host[:port]\n");
            printf("       puttyalt                  (interactive mode)\n");
            return 0;
        }
        /* Parse user@host:port */
        Session s;
        memset(&s, 0, sizeof(s));
        s.port = 22;
        char *at = strchr(argv[1], '@');
        char *colon = strrchr(argv[1], ':');
        if (at) {
            *at = 0;
            snprintf(s.user, sizeof(s.user), "%s", argv[1]);
            snprintf(s.host, sizeof(s.host), "%s", at + 1);
        } else {
            snprintf(s.host, sizeof(s.host), "%s", argv[1]);
        }
        if (colon && colon > s.host) {
            *colon = 0;
            s.port = atoi(colon + 1);
            if (s.port <= 0) s.port = 22;
        }
        /* Check for -p, -i, -l flags */
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-p") == 0 && i+1 < argc) s.port = atoi(argv[++i]);
            else if (strcmp(argv[i], "-l") == 0 && i+1 < argc) snprintf(s.user, 64, "%s", argv[++i]);
            else if (strcmp(argv[i], "-i") == 0 && i+1 < argc) snprintf(s.keyfile, 256, "%s", argv[++i]);
        }
        connect_session(&s);
        return 0;
    }

    interactive_mode();
    return 0;
}
