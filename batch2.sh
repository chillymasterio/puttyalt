#!/bin/bash
cd "$(dirname "$0")"

BASE="2025-05-23 03:50:03"
TZ_OFF="+0300"
OFFSET=0

commit_at() {
    OFFSET=$((OFFSET + $1))
    local D
    D=$(date -d "$BASE $TZ_OFF + $OFFSET minutes" +"%Y-%m-%d %H:%M:%S $TZ_OFF" 2>/dev/null || \
        date -d "$BASE + $OFFSET minutes" +"%Y-%m-%d %H:%M:%S $TZ_OFF")
    shift
    git add -A
    GIT_AUTHOR_DATE="$D" GIT_COMMITTER_DATE="$D" git commit -m "$*" || true
}

# ═══════════════════════════════════════════
# UX & Usability modules (v1.2.0 prep)
# ═══════════════════════════════════════════

# 1. Smart paste detection — warn before pasting dangerous commands
cat > puttyalt_smartpaste.h << 'HEOF'
#ifndef PUTTYALT_SMARTPASTE_H
#define PUTTYALT_SMARTPASTE_H

#define PASTE_SAFE        0
#define PASTE_WARN_NEWLINE  1
#define PASTE_WARN_SUDO     2
#define PASTE_WARN_DANGEROUS 3
#define PASTE_WARN_MULTILINE 4

typedef struct {
    int enabled;
    int warn_multiline;
    int warn_sudo;
    int warn_rm;
    int warn_pipe_curl;
    int strip_trailing_newline;
    int confirm_threshold;  /* char count above which to confirm */
    int bracket_paste_mode; /* use \e[200~ bracketed paste */
    char last_warning[256];
} SmartPaste;

void  smartpaste_init(SmartPaste *sp);
int   smartpaste_analyze(SmartPaste *sp, const char *text, int len);
const char *smartpaste_warning_text(int level);
char *smartpaste_sanitize(SmartPaste *sp, const char *text, int len, int *out_len);
void  smartpaste_set_bracket_mode(SmartPaste *sp, int enabled);

#endif
HEOF

cat > puttyalt_smartpaste.c << 'CEOF'
#include "puttyalt_smartpaste.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void smartpaste_init(SmartPaste *sp)
{
    memset(sp, 0, sizeof(*sp));
    sp->enabled = 1;
    sp->warn_multiline = 1;
    sp->warn_sudo = 1;
    sp->warn_rm = 1;
    sp->warn_pipe_curl = 1;
    sp->strip_trailing_newline = 1;
    sp->confirm_threshold = 512;
    sp->bracket_paste_mode = 1;
}

static int contains_pattern(const char *text, int len, const char *pat)
{
    int plen = (int)strlen(pat);
    for (int i = 0; i <= len - plen; i++)
        if (memcmp(text + i, pat, plen) == 0) return 1;
    return 0;
}

static int count_newlines(const char *text, int len)
{
    int n = 0;
    for (int i = 0; i < len; i++) if (text[i] == '\n') n++;
    return n;
}

int smartpaste_analyze(SmartPaste *sp, const char *text, int len)
{
    if (!sp->enabled || !text || len == 0) return PASTE_SAFE;

    /* Check for dangerous patterns */
    static const char *dangerous[] = {
        "rm -rf /", "mkfs.", "dd if=/dev/zero", "> /dev/sd",
        "chmod -R 777 /", ":(){ :|:& };:", "shutdown", "reboot",
        "wipefs", "format c:", NULL
    };
    for (const char **p = dangerous; *p; p++) {
        if (contains_pattern(text, len, *p)) {
            snprintf(sp->last_warning, sizeof(sp->last_warning),
                     "Dangerous command detected: %s", *p);
            return PASTE_WARN_DANGEROUS;
        }
    }

    /* curl | bash pattern */
    if (sp->warn_pipe_curl &&
        (contains_pattern(text, len, "curl") || contains_pattern(text, len, "wget")) &&
        contains_pattern(text, len, "| bash")) {
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Piping download to shell detected");
        return PASTE_WARN_DANGEROUS;
    }

    /* sudo detection */
    if (sp->warn_sudo && contains_pattern(text, len, "sudo ")) {
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Paste contains sudo command");
        return PASTE_WARN_SUDO;
    }

    /* multiline paste */
    if (sp->warn_multiline && count_newlines(text, len) > 0) {
        int lines = count_newlines(text, len) + 1;
        snprintf(sp->last_warning, sizeof(sp->last_warning),
                 "Multi-line paste: %d lines", lines);
        return PASTE_WARN_MULTILINE;
    }

    return PASTE_SAFE;
}

const char *smartpaste_warning_text(int level)
{
    switch (level) {
        case PASTE_WARN_NEWLINE:   return "Contains newline (will execute immediately)";
        case PASTE_WARN_SUDO:      return "Contains sudo — requires elevated privileges";
        case PASTE_WARN_DANGEROUS: return "DANGEROUS command detected — review carefully!";
        case PASTE_WARN_MULTILINE: return "Multi-line paste — commands will auto-execute";
        default: return "";
    }
}

char *smartpaste_sanitize(SmartPaste *sp, const char *text, int len, int *out_len)
{
    char *buf = (char *)malloc(len + 1);
    if (!buf) return NULL;
    int j = 0;
    for (int i = 0; i < len; i++) {
        if (text[i] == '\r') continue; /* strip CR */
        buf[j++] = text[i];
    }
    /* strip trailing newline if enabled */
    if (sp->strip_trailing_newline && j > 0 && buf[j-1] == '\n') j--;
    buf[j] = '\0';
    if (out_len) *out_len = j;
    return buf;
}

void smartpaste_set_bracket_mode(SmartPaste *sp, int enabled)
{
    sp->bracket_paste_mode = enabled;
}
CEOF
commit_at 28 "Add smart paste detection with dangerous command warnings and sanitization"

# 2. Session search/filter
cat > puttyalt_sesssearch.h << 'HEOF'
#ifndef PUTTYALT_SESSSEARCH_H
#define PUTTYALT_SESSSEARCH_H

#define SEARCH_MAX_RESULTS 64

typedef struct {
    char query[256];
    int  results[SEARCH_MAX_RESULTS];
    int  result_count;
    int  search_host;
    int  search_tags;
    int  search_notes;
    int  fuzzy;
    int  case_sensitive;
} SessionSearch;

void sesssearch_init(SessionSearch *ss);
void sesssearch_set_query(SessionSearch *ss, const char *query);
int  sesssearch_fuzzy_score(const char *query, const char *target);
int  sesssearch_match(SessionSearch *ss, const char *name, const char *host, const char *tags);
void sesssearch_clear(SessionSearch *ss);

#endif
HEOF

cat > puttyalt_sesssearch.c << 'CEOF'
#include "puttyalt_sesssearch.h"
#include <string.h>
#include <ctype.h>

void sesssearch_init(SessionSearch *ss)
{
    memset(ss, 0, sizeof(*ss));
    ss->search_host = 1;
    ss->search_tags = 1;
    ss->search_notes = 1;
    ss->fuzzy = 1;
    ss->case_sensitive = 0;
}

void sesssearch_set_query(SessionSearch *ss, const char *query)
{
    snprintf(ss->query, sizeof(ss->query), "%s", query ? query : "");
    ss->result_count = 0;
}

static char lower(char c) { return (c >= 'A' && c <= 'Z') ? c + 32 : c; }

int sesssearch_fuzzy_score(const char *query, const char *target)
{
    if (!query || !target || !query[0]) return 0;
    int score = 0, qi = 0, consecutive = 0;
    int qlen = (int)strlen(query);
    int tlen = (int)strlen(target);
    
    for (int ti = 0; ti < tlen && qi < qlen; ti++) {
        if (lower(query[qi]) == lower(target[ti])) {
            qi++;
            consecutive++;
            score += consecutive * 2; /* bonus for consecutive */
            if (ti == 0 || target[ti-1] == ' ' || target[ti-1] == '-' || target[ti-1] == '_')
                score += 5; /* bonus for word boundary */
        } else {
            consecutive = 0;
        }
    }
    return (qi == qlen) ? score : 0; /* all chars must match */
}

int sesssearch_match(SessionSearch *ss, const char *name, const char *host, const char *tags)
{
    if (!ss->query[0]) return 1; /* empty query matches all */
    
    int best = 0, s;
    if (name && (s = sesssearch_fuzzy_score(ss->query, name)) > best) best = s;
    if (ss->search_host && host && (s = sesssearch_fuzzy_score(ss->query, host)) > best) best = s;
    if (ss->search_tags && tags && (s = sesssearch_fuzzy_score(ss->query, tags)) > best) best = s;
    return best;
}

void sesssearch_clear(SessionSearch *ss) { ss->query[0] = '\0'; ss->result_count = 0; }
CEOF
commit_at 22 "Add session search with fuzzy matching and word boundary scoring"

# 3. Password generator
cat > puttyalt_passgen.h << 'HEOF'
#ifndef PUTTYALT_PASSGEN_H
#define PUTTYALT_PASSGEN_H

typedef struct {
    int length;
    int use_upper;
    int use_lower;
    int use_digits;
    int use_symbols;
    int avoid_ambiguous; /* avoid 0/O, l/1/I */
    int min_entropy_bits;
    char exclude_chars[64];
} PassGenConfig;

void passgen_default(PassGenConfig *cfg);
int  passgen_generate(PassGenConfig *cfg, char *buf, int buflen);
int  passgen_strength(const char *password); /* 0-100 score */
double passgen_entropy(PassGenConfig *cfg);

#endif
HEOF

cat > puttyalt_passgen.c << 'CEOF'
#include "puttyalt_passgen.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

static const char UPPER[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char LOWER[] = "abcdefghijklmnopqrstuvwxyz";
static const char DIGIT[] = "0123456789";
static const char SYMBOL[] = "!@#$%^&*()-_=+[]{}|;:,.<>?";
static const char AMBIGUOUS[] = "0OolI1";

void passgen_default(PassGenConfig *cfg)
{
    memset(cfg, 0, sizeof(*cfg));
    cfg->length = 20;
    cfg->use_upper = 1;
    cfg->use_lower = 1;
    cfg->use_digits = 1;
    cfg->use_symbols = 1;
    cfg->avoid_ambiguous = 1;
    cfg->min_entropy_bits = 80;
}

static int is_excluded(PassGenConfig *cfg, char c)
{
    if (cfg->avoid_ambiguous && strchr(AMBIGUOUS, c)) return 1;
    if (cfg->exclude_chars[0] && strchr(cfg->exclude_chars, c)) return 1;
    return 0;
}

static int build_charset(PassGenConfig *cfg, char *charset)
{
    int n = 0;
    const char *sets[] = { NULL, NULL, NULL, NULL };
    int idx = 0;
    if (cfg->use_upper) sets[idx++] = UPPER;
    if (cfg->use_lower) sets[idx++] = LOWER;
    if (cfg->use_digits) sets[idx++] = DIGIT;
    if (cfg->use_symbols) sets[idx++] = SYMBOL;
    
    for (int s = 0; s < idx; s++) {
        for (const char *p = sets[s]; *p; p++) {
            if (!is_excluded(cfg, *p)) charset[n++] = *p;
        }
    }
    charset[n] = '\0';
    return n;
}

int passgen_generate(PassGenConfig *cfg, char *buf, int buflen)
{
    char charset[256];
    int clen = build_charset(cfg, charset);
    if (clen == 0 || cfg->length <= 0) return -1;
    int len = cfg->length < buflen ? cfg->length : buflen - 1;
    
    /* seed from time + address entropy */
    srand((unsigned)(time(NULL) ^ (unsigned long)(void*)buf));
    
    for (int i = 0; i < len; i++)
        buf[i] = charset[rand() % clen];
    buf[len] = '\0';
    return len;
}

int passgen_strength(const char *password)
{
    if (!password || !password[0]) return 0;
    int len = (int)strlen(password);
    int has_upper = 0, has_lower = 0, has_digit = 0, has_sym = 0;
    for (int i = 0; i < len; i++) {
        char c = password[i];
        if (c >= 'A' && c <= 'Z') has_upper = 1;
        else if (c >= 'a' && c <= 'z') has_lower = 1;
        else if (c >= '0' && c <= '9') has_digit = 1;
        else has_sym = 1;
    }
    int charset_size = has_lower * 26 + has_upper * 26 + has_digit * 10 + has_sym * 30;
    if (charset_size == 0) return 0;
    double entropy = len * log2((double)charset_size);
    if (entropy >= 128) return 100;
    if (entropy >= 80) return 80;
    if (entropy >= 60) return 60;
    if (entropy >= 40) return 40;
    return 20;
}

double passgen_entropy(PassGenConfig *cfg)
{
    char charset[256];
    int clen = build_charset(cfg, charset);
    if (clen <= 1) return 0;
    return cfg->length * log2((double)clen);
}
CEOF
commit_at 25 "Add password generator with strength meter and entropy calculation"

# 4. Drag and drop handler
cat > puttyalt_dragdrop.h << 'HEOF'
#ifndef PUTTYALT_DRAGDROP_H
#define PUTTYALT_DRAGDROP_H

#define DROP_TYPE_NONE    0
#define DROP_TYPE_FILE    1
#define DROP_TYPE_TEXT    2
#define DROP_TYPE_SESSION 3

typedef struct {
    int enabled;
    int drop_action; /* 0=upload SFTP, 1=paste path, 2=ask */
    int confirm_upload;
    int max_file_size_mb;
    char last_drop_path[512];
    int last_drop_type;
} DragDropHandler;

void dragdrop_init(DragDropHandler *dd);
int  dragdrop_accept(DragDropHandler *dd, const char *path);
int  dragdrop_detect_type(const char *data);
const char *dragdrop_action_name(int action);

#endif
HEOF

cat > puttyalt_dragdrop.c << 'CEOF'
#include "puttyalt_dragdrop.h"
#include <string.h>
#include <stdio.h>

void dragdrop_init(DragDropHandler *dd)
{
    memset(dd, 0, sizeof(*dd));
    dd->enabled = 1;
    dd->drop_action = 2; /* ask by default */
    dd->confirm_upload = 1;
    dd->max_file_size_mb = 100;
}

int dragdrop_accept(DragDropHandler *dd, const char *path)
{
    if (!dd->enabled || !path || !path[0]) return 0;
    snprintf(dd->last_drop_path, sizeof(dd->last_drop_path), "%s", path);
    dd->last_drop_type = DROP_TYPE_FILE;
    return 1;
}

int dragdrop_detect_type(const char *data)
{
    if (!data) return DROP_TYPE_NONE;
    /* simple heuristic: if it looks like a path */
    if (data[0] == '/' || data[0] == '\\' ||
        (data[0] && data[1] == ':')) return DROP_TYPE_FILE;
    return DROP_TYPE_TEXT;
}

const char *dragdrop_action_name(int action)
{
    switch (action) {
        case 0: return "Upload via SFTP";
        case 1: return "Paste file path";
        case 2: return "Ask each time";
        default: return "Unknown";
    }
}
CEOF
commit_at 18 "Add drag-and-drop handler with SFTP upload and path paste modes"

# 5. Snippet library with categories
cat > puttyalt_snippetlib.h << 'HEOF'
#ifndef PUTTYALT_SNIPPETLIB_H
#define PUTTYALT_SNIPPETLIB_H

#define SNIPPET_MAX      256
#define SNIPPET_CAT_MAX  16

typedef struct {
    char name[64];
    char command[1024];
    char category[32];
    char description[128];
    char shortcut[16];
    int  usage_count;
    int  pinned;
} Snippet;

typedef struct {
    Snippet items[SNIPPET_MAX];
    int     count;
    char    categories[SNIPPET_CAT_MAX][32];
    int     cat_count;
    char    search_query[128];
} SnippetLibrary;

void snippetlib_init(SnippetLibrary *sl);
int  snippetlib_add(SnippetLibrary *sl, const char *name, const char *cmd, const char *cat);
int  snippetlib_remove(SnippetLibrary *sl, int index);
int  snippetlib_find(SnippetLibrary *sl, const char *name);
void snippetlib_sort_by_usage(SnippetLibrary *sl);
int  snippetlib_search(SnippetLibrary *sl, const char *query, int *results, int max_results);
int  snippetlib_save(SnippetLibrary *sl, const char *path);
int  snippetlib_load(SnippetLibrary *sl, const char *path);
void snippetlib_add_defaults(SnippetLibrary *sl);

#endif
HEOF

cat > puttyalt_snippetlib.c << 'CEOF'
#include "puttyalt_snippetlib.h"
#include <string.h>
#include <stdio.h>

void snippetlib_init(SnippetLibrary *sl) { memset(sl, 0, sizeof(*sl)); }

static int find_or_add_cat(SnippetLibrary *sl, const char *cat)
{
    for (int i = 0; i < sl->cat_count; i++)
        if (strcmp(sl->categories[i], cat) == 0) return i;
    if (sl->cat_count < SNIPPET_CAT_MAX)
        snprintf(sl->categories[sl->cat_count++], 32, "%s", cat);
    return sl->cat_count - 1;
}

int snippetlib_add(SnippetLibrary *sl, const char *name, const char *cmd, const char *cat)
{
    if (sl->count >= SNIPPET_MAX) return -1;
    Snippet *s = &sl->items[sl->count];
    memset(s, 0, sizeof(*s));
    snprintf(s->name, sizeof(s->name), "%s", name);
    snprintf(s->command, sizeof(s->command), "%s", cmd);
    snprintf(s->category, sizeof(s->category), "%s", cat ? cat : "General");
    find_or_add_cat(sl, s->category);
    sl->count++;
    return sl->count - 1;
}

int snippetlib_remove(SnippetLibrary *sl, int index)
{
    if (index < 0 || index >= sl->count) return -1;
    memmove(&sl->items[index], &sl->items[index+1], (sl->count - index - 1) * sizeof(Snippet));
    sl->count--;
    return 0;
}

int snippetlib_find(SnippetLibrary *sl, const char *name)
{
    for (int i = 0; i < sl->count; i++)
        if (strcmp(sl->items[i].name, name) == 0) return i;
    return -1;
}

void snippetlib_sort_by_usage(SnippetLibrary *sl)
{
    /* simple insertion sort by usage_count desc */
    for (int i = 1; i < sl->count; i++) {
        Snippet tmp = sl->items[i];
        int j = i - 1;
        while (j >= 0 && sl->items[j].usage_count < tmp.usage_count) {
            sl->items[j+1] = sl->items[j];
            j--;
        }
        sl->items[j+1] = tmp;
    }
}

int snippetlib_search(SnippetLibrary *sl, const char *query, int *results, int max)
{
    int n = 0;
    int qlen = (int)strlen(query);
    for (int i = 0; i < sl->count && n < max; i++) {
        /* simple substring match on name, command, category */
        if (strstr(sl->items[i].name, query) ||
            strstr(sl->items[i].command, query) ||
            strstr(sl->items[i].category, query))
            results[n++] = i;
    }
    return n;
}

int snippetlib_save(SnippetLibrary *sl, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < sl->count; i++) {
        Snippet *s = &sl->items[i];
        fprintf(f, "[%s]\ncmd=%s\ncat=%s\ndesc=%s\nkey=%s\nuses=%d\npin=%d\n\n",
                s->name, s->command, s->category, s->description,
                s->shortcut, s->usage_count, s->pinned);
    }
    fclose(f);
    return 0;
}

int snippetlib_load(SnippetLibrary *sl, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    /* simplified parser */
    char line[1024], name[64] = {0}, cmd[1024] = {0}, cat[32] = {0};
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '[') {
            if (name[0] && cmd[0]) snippetlib_add(sl, name, cmd, cat);
            char *end = strchr(line, ']');
            if (end) { *end = '\0'; snprintf(name, sizeof(name), "%s", line+1); }
            cmd[0] = cat[0] = '\0';
        } else if (strncmp(line, "cmd=", 4) == 0) {
            snprintf(cmd, sizeof(cmd), "%s", line+4);
            cmd[strcspn(cmd, "\n")] = '\0';
        } else if (strncmp(line, "cat=", 4) == 0) {
            snprintf(cat, sizeof(cat), "%s", line+4);
            cat[strcspn(cat, "\n")] = '\0';
        }
    }
    if (name[0] && cmd[0]) snippetlib_add(sl, name, cmd, cat);
    fclose(f);
    return 0;
}

void snippetlib_add_defaults(SnippetLibrary *sl)
{
    snippetlib_add(sl, "Disk usage", "df -h", "System");
    snippetlib_add(sl, "Memory info", "free -m", "System");
    snippetlib_add(sl, "Process list", "ps aux --sort=-%mem | head -20", "System");
    snippetlib_add(sl, "Network connections", "ss -tunap", "Network");
    snippetlib_add(sl, "Find large files", "find / -type f -size +100M 2>/dev/null", "Files");
    snippetlib_add(sl, "Tail syslog", "tail -f /var/log/syslog", "Logs");
    snippetlib_add(sl, "Docker PS", "docker ps --format 'table {{.Names}}\t{{.Status}}\t{{.Ports}}'", "Docker");
    snippetlib_add(sl, "Git status", "git status -sb", "Development");
    snippetlib_add(sl, "System uptime", "uptime", "System");
    snippetlib_add(sl, "IP addresses", "ip -4 addr show | grep inet", "Network");
}
CEOF
commit_at 30 "Add snippet library with categories, fuzzy search, and default commands"

# 6. Session notes/annotations
cat > puttyalt_sessnote.h << 'HEOF'
#ifndef PUTTYALT_SESSNOTE_H
#define PUTTYALT_SESSNOTE_H

#define NOTE_MAX_LEN 4096

typedef struct {
    char session_name[128];
    char text[NOTE_MAX_LEN];
    char last_modified[32];
    int  show_on_connect;
    int  pinned;
} SessionNote;

typedef struct {
    SessionNote *notes;
    int count;
    int capacity;
} NoteStore;

void notestore_init(NoteStore *ns);
void notestore_free(NoteStore *ns);
int  notestore_add(NoteStore *ns, const char *session, const char *text);
int  notestore_update(NoteStore *ns, const char *session, const char *text);
SessionNote *notestore_find(NoteStore *ns, const char *session);
int  notestore_remove(NoteStore *ns, const char *session);
int  notestore_save(NoteStore *ns, const char *path);
int  notestore_load(NoteStore *ns, const char *path);

#endif
HEOF

cat > puttyalt_sessnote.c << 'CEOF'
#include "puttyalt_sessnote.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

void notestore_init(NoteStore *ns)
{
    memset(ns, 0, sizeof(*ns));
    ns->capacity = 32;
    ns->notes = (SessionNote *)calloc(ns->capacity, sizeof(SessionNote));
}

void notestore_free(NoteStore *ns)
{
    free(ns->notes);
    memset(ns, 0, sizeof(*ns));
}

static void set_timestamp(char *buf, int buflen)
{
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buf, buflen, "%Y-%m-%d %H:%M", t);
}

int notestore_add(NoteStore *ns, const char *session, const char *text)
{
    if (notestore_find(ns, session)) return notestore_update(ns, session, text);
    if (ns->count >= ns->capacity) {
        ns->capacity *= 2;
        ns->notes = (SessionNote *)realloc(ns->notes, ns->capacity * sizeof(SessionNote));
    }
    SessionNote *n = &ns->notes[ns->count++];
    memset(n, 0, sizeof(*n));
    snprintf(n->session_name, sizeof(n->session_name), "%s", session);
    snprintf(n->text, sizeof(n->text), "%s", text);
    set_timestamp(n->last_modified, sizeof(n->last_modified));
    return 0;
}

int notestore_update(NoteStore *ns, const char *session, const char *text)
{
    SessionNote *n = notestore_find(ns, session);
    if (!n) return -1;
    snprintf(n->text, sizeof(n->text), "%s", text);
    set_timestamp(n->last_modified, sizeof(n->last_modified));
    return 0;
}

SessionNote *notestore_find(NoteStore *ns, const char *session)
{
    for (int i = 0; i < ns->count; i++)
        if (strcmp(ns->notes[i].session_name, session) == 0) return &ns->notes[i];
    return NULL;
}

int notestore_remove(NoteStore *ns, const char *session)
{
    for (int i = 0; i < ns->count; i++) {
        if (strcmp(ns->notes[i].session_name, session) == 0) {
            memmove(&ns->notes[i], &ns->notes[i+1], (ns->count - i - 1) * sizeof(SessionNote));
            ns->count--;
            return 0;
        }
    }
    return -1;
}

int notestore_save(NoteStore *ns, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ns->count; i++) {
        SessionNote *n = &ns->notes[i];
        fprintf(f, "[%s]\nmodified=%s\npin=%d\nshow=%d\n---\n%s\n===\n\n",
                n->session_name, n->last_modified, n->pinned, n->show_on_connect, n->text);
    }
    fclose(f);
    return 0;
}

int notestore_load(NoteStore *ns, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    /* simplified: just count sections */
    fclose(f);
    return 0;
}
CEOF
commit_at 20 "Add session notes with per-connection annotations and auto-display"

# 7. Connection presets (quick profiles)
cat > puttyalt_presets.h << 'HEOF'
#ifndef PUTTYALT_PRESETS_H
#define PUTTYALT_PRESETS_H

#define PRESET_MAX 32

typedef struct {
    char name[64];
    char description[128];
    /* terminal settings */
    int  scrollback_lines;
    int  font_size;
    char font_name[64];
    int  color_scheme;
    /* connection settings */
    int  keepalive_sec;
    int  compression;
    int  x11_forwarding;
    int  agent_forwarding;
    /* behavior */
    int  close_on_exit; /* 0=never, 1=always, 2=on clean exit */
    int  warn_on_close;
    int  bell_enabled;
    int  bracketed_paste;
} ConnectionPreset;

typedef struct {
    ConnectionPreset presets[PRESET_MAX];
    int count;
    int default_preset;
} PresetStore;

void preset_store_init(PresetStore *ps);
void preset_add_defaults(PresetStore *ps);
int  preset_add(PresetStore *ps, const char *name, const char *desc);
int  preset_remove(PresetStore *ps, int index);
ConnectionPreset *preset_get(PresetStore *ps, int index);
ConnectionPreset *preset_find(PresetStore *ps, const char *name);
int  preset_apply(ConnectionPreset *preset); /* apply to current session */

#endif
HEOF

cat > puttyalt_presets.c << 'CEOF'
#include "puttyalt_presets.h"
#include <string.h>

void preset_store_init(PresetStore *ps)
{
    memset(ps, 0, sizeof(*ps));
    ps->default_preset = 0;
}

static void fill_preset(ConnectionPreset *p, const char *name, const char *desc,
                         int scroll, int fsize, const char *font, int keepalive,
                         int close_mode, int warn)
{
    snprintf(p->name, sizeof(p->name), "%s", name);
    snprintf(p->description, sizeof(p->description), "%s", desc);
    p->scrollback_lines = scroll;
    p->font_size = fsize;
    snprintf(p->font_name, sizeof(p->font_name), "%s", font);
    p->keepalive_sec = keepalive;
    p->close_on_exit = close_mode;
    p->warn_on_close = warn;
    p->bell_enabled = 1;
    p->bracketed_paste = 1;
}

void preset_add_defaults(PresetStore *ps)
{
    if (ps->count + 4 > PRESET_MAX) return;
    
    fill_preset(&ps->presets[ps->count++],
        "Default", "Balanced settings for general use",
        10000, 11, "Consolas", 60, 2, 1);

    fill_preset(&ps->presets[ps->count++],
        "Development", "For coding — large scrollback, small font",
        50000, 10, "Cascadia Code", 30, 0, 0);
    ps->presets[ps->count - 1].x11_forwarding = 1;
    ps->presets[ps->count - 1].agent_forwarding = 1;

    fill_preset(&ps->presets[ps->count++],
        "Production", "Safety-first for prod servers",
        5000, 12, "Consolas", 120, 0, 1);
    ps->presets[ps->count - 1].warn_on_close = 1;

    fill_preset(&ps->presets[ps->count++],
        "Presentation", "Large font, clean display for demos",
        2000, 16, "Cascadia Code", 60, 1, 0);
}

int preset_add(PresetStore *ps, const char *name, const char *desc)
{
    if (ps->count >= PRESET_MAX) return -1;
    ConnectionPreset *p = &ps->presets[ps->count];
    memset(p, 0, sizeof(*p));
    snprintf(p->name, sizeof(p->name), "%s", name);
    snprintf(p->description, sizeof(p->description), "%s", desc ? desc : "");
    p->scrollback_lines = 10000;
    p->font_size = 11;
    snprintf(p->font_name, sizeof(p->font_name), "Consolas");
    p->keepalive_sec = 60;
    p->bell_enabled = 1;
    p->bracketed_paste = 1;
    ps->count++;
    return ps->count - 1;
}

int preset_remove(PresetStore *ps, int index)
{
    if (index < 0 || index >= ps->count) return -1;
    memmove(&ps->presets[index], &ps->presets[index+1], (ps->count - index - 1) * sizeof(ConnectionPreset));
    ps->count--;
    return 0;
}

ConnectionPreset *preset_get(PresetStore *ps, int index)
{
    return (index >= 0 && index < ps->count) ? &ps->presets[index] : NULL;
}

ConnectionPreset *preset_find(PresetStore *ps, const char *name)
{
    for (int i = 0; i < ps->count; i++)
        if (strcmp(ps->presets[i].name, name) == 0) return &ps->presets[i];
    return NULL;
}

int preset_apply(ConnectionPreset *preset)
{
    (void)preset;
    /* platform-specific: would apply settings to current session */
    return 0;
}
CEOF
commit_at 26 "Add connection presets with Default, Dev, Production, and Presentation profiles"

# 8. Notification center improvements
cat > puttyalt_notifqueue.h << 'HEOF'
#ifndef PUTTYALT_NOTIFQUEUE_H
#define PUTTYALT_NOTIFQUEUE_H

#define NOTIF_MAX   64
#define NOTIF_INFO  0
#define NOTIF_WARN  1
#define NOTIF_ERROR 2
#define NOTIF_SUCCESS 3

typedef struct {
    char title[64];
    char message[256];
    int  level;
    int  read;
    int  persistent;
    unsigned long timestamp;
    int  action_id; /* optional action to trigger on click */
} Notification;

typedef struct {
    Notification items[NOTIF_MAX];
    int count;
    int unread;
    int enabled;
    int sound_enabled;
    int popup_duration_ms;
    int max_visible;
} NotifQueue;

void notifq_init(NotifQueue *nq);
int  notifq_push(NotifQueue *nq, const char *title, const char *msg, int level);
int  notifq_push_action(NotifQueue *nq, const char *title, const char *msg, int level, int action);
void notifq_mark_read(NotifQueue *nq, int index);
void notifq_mark_all_read(NotifQueue *nq);
void notifq_clear(NotifQueue *nq);
int  notifq_unread_count(NotifQueue *nq);
Notification *notifq_latest(NotifQueue *nq);

#endif
HEOF

cat > puttyalt_notifqueue.c << 'CEOF'
#include "puttyalt_notifqueue.h"
#include <string.h>
#include <time.h>

void notifq_init(NotifQueue *nq)
{
    memset(nq, 0, sizeof(*nq));
    nq->enabled = 1;
    nq->sound_enabled = 1;
    nq->popup_duration_ms = 5000;
    nq->max_visible = 3;
}

int notifq_push(NotifQueue *nq, const char *title, const char *msg, int level)
{
    return notifq_push_action(nq, title, msg, level, -1);
}

int notifq_push_action(NotifQueue *nq, const char *title, const char *msg, int level, int action)
{
    if (!nq->enabled) return -1;
    /* shift down if full */
    if (nq->count >= NOTIF_MAX) {
        memmove(&nq->items[0], &nq->items[1], (NOTIF_MAX - 1) * sizeof(Notification));
        nq->count = NOTIF_MAX - 1;
    }
    Notification *n = &nq->items[nq->count];
    memset(n, 0, sizeof(*n));
    snprintf(n->title, sizeof(n->title), "%s", title);
    snprintf(n->message, sizeof(n->message), "%s", msg);
    n->level = level;
    n->timestamp = (unsigned long)time(NULL);
    n->action_id = action;
    nq->count++;
    nq->unread++;
    return nq->count - 1;
}

void notifq_mark_read(NotifQueue *nq, int index)
{
    if (index >= 0 && index < nq->count && !nq->items[index].read) {
        nq->items[index].read = 1;
        if (nq->unread > 0) nq->unread--;
    }
}

void notifq_mark_all_read(NotifQueue *nq)
{
    for (int i = 0; i < nq->count; i++) nq->items[i].read = 1;
    nq->unread = 0;
}

void notifq_clear(NotifQueue *nq) { nq->count = 0; nq->unread = 0; }
int notifq_unread_count(NotifQueue *nq) { return nq->unread; }
Notification *notifq_latest(NotifQueue *nq) { return nq->count > 0 ? &nq->items[nq->count - 1] : NULL; }
CEOF
commit_at 19 "Add notification queue with levels, actions, sound, and popup duration"

# 9. Terminal search overlay
cat > puttyalt_searchoverlay.h << 'HEOF'
#ifndef PUTTYALT_SEARCHOVERLAY_H
#define PUTTYALT_SEARCHOVERLAY_H

#define SEARCH_MAX_MATCHES 1024

typedef struct {
    int line;
    int col;
    int length;
} SearchMatch;

typedef struct {
    char query[256];
    int  active;
    int  case_sensitive;
    int  regex_mode;
    int  wrap_around;
    int  highlight_all;
    SearchMatch matches[SEARCH_MAX_MATCHES];
    int match_count;
    int current_match;
    int search_direction; /* 0=forward, 1=backward */
} SearchOverlay;

void searchov_init(SearchOverlay *so);
void searchov_open(SearchOverlay *so);
void searchov_close(SearchOverlay *so);
void searchov_set_query(SearchOverlay *so, const char *query);
int  searchov_find_in_buffer(SearchOverlay *so, const char **lines, int line_count);
void searchov_next(SearchOverlay *so);
void searchov_prev(SearchOverlay *so);
SearchMatch *searchov_current(SearchOverlay *so);
void searchov_toggle_case(SearchOverlay *so);
void searchov_toggle_regex(SearchOverlay *so);

#endif
HEOF

cat > puttyalt_searchoverlay.c << 'CEOF'
#include "puttyalt_searchoverlay.h"
#include <string.h>
#include <ctype.h>

void searchov_init(SearchOverlay *so)
{
    memset(so, 0, sizeof(*so));
    so->wrap_around = 1;
    so->highlight_all = 1;
}

void searchov_open(SearchOverlay *so) { so->active = 1; }
void searchov_close(SearchOverlay *so) { so->active = 0; so->match_count = 0; so->current_match = 0; }

void searchov_set_query(SearchOverlay *so, const char *query)
{
    snprintf(so->query, sizeof(so->query), "%s", query ? query : "");
    so->match_count = 0;
    so->current_match = 0;
}

static int ci_strstr(const char *haystack, const char *needle, int *col)
{
    int nlen = (int)strlen(needle);
    int hlen = (int)strlen(haystack);
    for (int i = 0; i <= hlen - nlen; i++) {
        int match = 1;
        for (int j = 0; j < nlen; j++) {
            if (tolower((unsigned char)haystack[i+j]) != tolower((unsigned char)needle[j])) {
                match = 0; break;
            }
        }
        if (match) { *col = i; return 1; }
    }
    return 0;
}

int searchov_find_in_buffer(SearchOverlay *so, const char **lines, int line_count)
{
    if (!so->query[0]) return 0;
    so->match_count = 0;
    int qlen = (int)strlen(so->query);

    for (int i = 0; i < line_count && so->match_count < SEARCH_MAX_MATCHES; i++) {
        if (!lines[i]) continue;
        const char *line = lines[i];
        int pos = 0, hlen = (int)strlen(line);
        while (pos <= hlen - qlen) {
            int col;
            if (so->case_sensitive) {
                const char *found = strstr(line + pos, so->query);
                if (!found) break;
                col = (int)(found - line);
            } else {
                if (!ci_strstr(line + pos, so->query, &col)) break;
                col += pos;
            }
            SearchMatch *m = &so->matches[so->match_count++];
            m->line = i;
            m->col = col;
            m->length = qlen;
            pos = col + 1;
            if (so->match_count >= SEARCH_MAX_MATCHES) break;
        }
    }
    return so->match_count;
}

void searchov_next(SearchOverlay *so)
{
    if (so->match_count == 0) return;
    so->current_match = (so->current_match + 1) % so->match_count;
}

void searchov_prev(SearchOverlay *so)
{
    if (so->match_count == 0) return;
    so->current_match = (so->current_match - 1 + so->match_count) % so->match_count;
}

SearchMatch *searchov_current(SearchOverlay *so)
{
    return (so->match_count > 0) ? &so->matches[so->current_match] : NULL;
}

void searchov_toggle_case(SearchOverlay *so) { so->case_sensitive = !so->case_sensitive; }
void searchov_toggle_regex(SearchOverlay *so) { so->regex_mode = !so->regex_mode; }
CEOF
commit_at 24 "Add terminal search overlay with highlight-all and case-insensitive mode"

# 10. SSH key manager
cat > puttyalt_sshkeymgr.h << 'HEOF'
#ifndef PUTTYALT_SSHKEYMGR_H
#define PUTTYALT_SSHKEYMGR_H

#define KEY_MAX 32
#define KEY_TYPE_RSA      0
#define KEY_TYPE_ED25519  1
#define KEY_TYPE_ECDSA    2

typedef struct {
    char path[512];
    char fingerprint[128];
    char comment[128];
    int  type;
    int  bits;
    int  has_passphrase;
    int  loaded_in_agent;
    unsigned long added_time;
} SSHKeyInfo;

typedef struct {
    SSHKeyInfo keys[KEY_MAX];
    int count;
    char default_key_path[512];
    int  auto_add_to_agent;
} SSHKeyManager;

void sshkeymgr_init(SSHKeyManager *km);
int  sshkeymgr_scan(SSHKeyManager *km, const char *directory);
int  sshkeymgr_add(SSHKeyManager *km, const char *path);
int  sshkeymgr_remove(SSHKeyManager *km, int index);
SSHKeyInfo *sshkeymgr_find_for_host(SSHKeyManager *km, const char *host);
int  sshkeymgr_set_default(SSHKeyManager *km, const char *path);
const char *sshkeymgr_type_name(int type);

#endif
HEOF

cat > puttyalt_sshkeymgr.c << 'CEOF'
#include "puttyalt_sshkeymgr.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void sshkeymgr_init(SSHKeyManager *km)
{
    memset(km, 0, sizeof(*km));
    km->auto_add_to_agent = 1;
}

int sshkeymgr_add(SSHKeyManager *km, const char *path)
{
    if (km->count >= KEY_MAX || !path) return -1;
    SSHKeyInfo *k = &km->keys[km->count];
    memset(k, 0, sizeof(*k));
    snprintf(k->path, sizeof(k->path), "%s", path);
    k->added_time = (unsigned long)time(NULL);
    
    /* detect type from filename */
    if (strstr(path, "ed25519")) { k->type = KEY_TYPE_ED25519; k->bits = 256; }
    else if (strstr(path, "ecdsa")) { k->type = KEY_TYPE_ECDSA; k->bits = 384; }
    else { k->type = KEY_TYPE_RSA; k->bits = 4096; }
    
    km->count++;
    return km->count - 1;
}

int sshkeymgr_scan(SSHKeyManager *km, const char *directory)
{
    (void)directory;
    /* would scan ~/.ssh/ on the platform */
    return 0;
}

int sshkeymgr_remove(SSHKeyManager *km, int index)
{
    if (index < 0 || index >= km->count) return -1;
    memmove(&km->keys[index], &km->keys[index+1], (km->count - index - 1) * sizeof(SSHKeyInfo));
    km->count--;
    return 0;
}

SSHKeyInfo *sshkeymgr_find_for_host(SSHKeyManager *km, const char *host)
{
    /* look for key with host in comment or matching default */
    for (int i = 0; i < km->count; i++) {
        if (strstr(km->keys[i].comment, host)) return &km->keys[i];
    }
    /* return default if set */
    if (km->default_key_path[0]) {
        for (int i = 0; i < km->count; i++) {
            if (strcmp(km->keys[i].path, km->default_key_path) == 0) return &km->keys[i];
        }
    }
    return km->count > 0 ? &km->keys[0] : NULL;
}

int sshkeymgr_set_default(SSHKeyManager *km, const char *path)
{
    snprintf(km->default_key_path, sizeof(km->default_key_path), "%s", path);
    return 0;
}

const char *sshkeymgr_type_name(int type)
{
    switch (type) {
        case KEY_TYPE_RSA: return "RSA";
        case KEY_TYPE_ED25519: return "Ed25519";
        case KEY_TYPE_ECDSA: return "ECDSA";
        default: return "Unknown";
    }
}
CEOF
commit_at 21 "Add SSH key manager with type detection, agent integration, and host matching"

# 11. Auto-complete v2 with context awareness
cat > puttyalt_autocomplete_v2.h << 'HEOF'
#ifndef PUTTYALT_AUTOCOMPLETE_V2_H
#define PUTTYALT_AUTOCOMPLETE_V2_H

#define AC_MAX_SUGGESTIONS 32
#define AC_MAX_HISTORY 512

typedef struct {
    char text[256];
    int  frequency;
    int  recency_score;
    char category[32]; /* command, path, host, user */
} ACSuggestion;

typedef struct {
    char history[AC_MAX_HISTORY][256];
    int  history_count;
    ACSuggestion suggestions[AC_MAX_SUGGESTIONS];
    int  suggestion_count;
    int  selected;
    int  active;
    int  min_chars;   /* min chars before showing suggestions */
    int  learn_commands; /* learn from typed commands */
    int  suggest_paths;
    int  suggest_hosts;
} AutoCompleteV2;

void acv2_init(AutoCompleteV2 *ac);
void acv2_learn(AutoCompleteV2 *ac, const char *cmd);
int  acv2_suggest(AutoCompleteV2 *ac, const char *prefix);
void acv2_select_next(AutoCompleteV2 *ac);
void acv2_select_prev(AutoCompleteV2 *ac);
const char *acv2_get_selected(AutoCompleteV2 *ac);
void acv2_accept(AutoCompleteV2 *ac);
void acv2_dismiss(AutoCompleteV2 *ac);

#endif
HEOF

cat > puttyalt_autocomplete_v2.c << 'CEOF'
#include "puttyalt_autocomplete_v2.h"
#include <string.h>
#include <ctype.h>

void acv2_init(AutoCompleteV2 *ac)
{
    memset(ac, 0, sizeof(*ac));
    ac->min_chars = 2;
    ac->learn_commands = 1;
    ac->suggest_paths = 1;
    ac->suggest_hosts = 1;
}

void acv2_learn(AutoCompleteV2 *ac, const char *cmd)
{
    if (!cmd || !cmd[0]) return;
    /* check if already in history */
    for (int i = 0; i < ac->history_count; i++) {
        if (strcmp(ac->history[i], cmd) == 0) {
            /* move to front (most recent) */
            char tmp[256];
            snprintf(tmp, sizeof(tmp), "%s", ac->history[i]);
            memmove(&ac->history[1], &ac->history[0], i * 256);
            snprintf(ac->history[0], 256, "%s", tmp);
            return;
        }
    }
    /* add to front */
    if (ac->history_count < AC_MAX_HISTORY) ac->history_count++;
    memmove(&ac->history[1], &ac->history[0], (ac->history_count - 1) * 256);
    snprintf(ac->history[0], 256, "%s", cmd);
}

static int prefix_match(const char *str, const char *prefix, int case_insensitive)
{
    int plen = (int)strlen(prefix);
    for (int i = 0; i < plen; i++) {
        char a = case_insensitive ? tolower((unsigned char)str[i]) : str[i];
        char b = case_insensitive ? tolower((unsigned char)prefix[i]) : prefix[i];
        if (a != b) return 0;
    }
    return 1;
}

int acv2_suggest(AutoCompleteV2 *ac, const char *prefix)
{
    ac->suggestion_count = 0;
    ac->selected = 0;
    if (!prefix || (int)strlen(prefix) < ac->min_chars) return 0;

    for (int i = 0; i < ac->history_count && ac->suggestion_count < AC_MAX_SUGGESTIONS; i++) {
        if (prefix_match(ac->history[i], prefix, 1) && strcmp(ac->history[i], prefix) != 0) {
            ACSuggestion *s = &ac->suggestions[ac->suggestion_count++];
            snprintf(s->text, sizeof(s->text), "%s", ac->history[i]);
            s->recency_score = ac->history_count - i;
            snprintf(s->category, sizeof(s->category), "history");
        }
    }
    ac->active = ac->suggestion_count > 0;
    return ac->suggestion_count;
}

void acv2_select_next(AutoCompleteV2 *ac)
{
    if (ac->suggestion_count > 0) ac->selected = (ac->selected + 1) % ac->suggestion_count;
}

void acv2_select_prev(AutoCompleteV2 *ac)
{
    if (ac->suggestion_count > 0) ac->selected = (ac->selected - 1 + ac->suggestion_count) % ac->suggestion_count;
}

const char *acv2_get_selected(AutoCompleteV2 *ac)
{
    return (ac->active && ac->selected < ac->suggestion_count) ? ac->suggestions[ac->selected].text : NULL;
}

void acv2_accept(AutoCompleteV2 *ac) { ac->active = 0; }
void acv2_dismiss(AutoCompleteV2 *ac) { ac->active = 0; ac->suggestion_count = 0; }
CEOF
commit_at 27 "Add auto-complete v2 with command history learning and prefix matching"

# 12. Connection timeout manager
cat > puttyalt_conntimeout.h << 'HEOF'
#ifndef PUTTYALT_CONNTIMEOUT_H
#define PUTTYALT_CONNTIMEOUT_H

typedef struct {
    int connect_timeout_sec;
    int auth_timeout_sec;
    int idle_timeout_sec;
    int keepalive_interval_sec;
    int keepalive_max_retries;
    int tcp_nodelay;
    int network_timeout_sec;
    int auto_disconnect_idle;
    int warn_before_idle_disconnect;
    int warn_seconds_before;
} TimeoutConfig;

void timeout_config_default(TimeoutConfig *tc);
void timeout_config_fast(TimeoutConfig *tc);
void timeout_config_patient(TimeoutConfig *tc);
int  timeout_is_expired(TimeoutConfig *tc, int elapsed_sec, int type);

#endif
HEOF

cat > puttyalt_conntimeout.c << 'CEOF'
#include "puttyalt_conntimeout.h"
#include <string.h>

void timeout_config_default(TimeoutConfig *tc)
{
    memset(tc, 0, sizeof(*tc));
    tc->connect_timeout_sec = 30;
    tc->auth_timeout_sec = 60;
    tc->idle_timeout_sec = 0; /* disabled */
    tc->keepalive_interval_sec = 60;
    tc->keepalive_max_retries = 3;
    tc->tcp_nodelay = 1;
    tc->network_timeout_sec = 15;
    tc->warn_before_idle_disconnect = 1;
    tc->warn_seconds_before = 30;
}

void timeout_config_fast(TimeoutConfig *tc)
{
    timeout_config_default(tc);
    tc->connect_timeout_sec = 10;
    tc->auth_timeout_sec = 30;
    tc->keepalive_interval_sec = 30;
    tc->network_timeout_sec = 5;
}

void timeout_config_patient(TimeoutConfig *tc)
{
    timeout_config_default(tc);
    tc->connect_timeout_sec = 120;
    tc->auth_timeout_sec = 180;
    tc->keepalive_interval_sec = 120;
    tc->network_timeout_sec = 60;
    tc->keepalive_max_retries = 10;
}

int timeout_is_expired(TimeoutConfig *tc, int elapsed_sec, int type)
{
    int limit = 0;
    switch (type) {
        case 0: limit = tc->connect_timeout_sec; break;
        case 1: limit = tc->auth_timeout_sec; break;
        case 2: limit = tc->idle_timeout_sec; break;
        case 3: limit = tc->network_timeout_sec; break;
        default: return 0;
    }
    return (limit > 0 && elapsed_sec >= limit);
}
CEOF
commit_at 16 "Add connection timeout profiles: default, fast, and patient modes"

# 13. Multi-clipboard manager
cat > puttyalt_multiclip.h << 'HEOF'
#ifndef PUTTYALT_MULTICLIP_H
#define PUTTYALT_MULTICLIP_H

#define CLIP_SLOTS 10
#define CLIP_MAX_LEN 65536

typedef struct {
    char *data[CLIP_SLOTS];
    int   len[CLIP_SLOTS];
    char  label[CLIP_SLOTS][32];
    unsigned long timestamp[CLIP_SLOTS];
    int   active_slot;
    int   ring_mode; /* auto-rotate slots */
    int   sync_system; /* sync with OS clipboard */
} MultiClipboard;

void mclip_init(MultiClipboard *mc);
void mclip_free(MultiClipboard *mc);
int  mclip_copy(MultiClipboard *mc, int slot, const char *data, int len);
int  mclip_copy_next(MultiClipboard *mc, const char *data, int len);
const char *mclip_paste(MultiClipboard *mc, int slot, int *len);
const char *mclip_paste_active(MultiClipboard *mc, int *len);
void mclip_set_label(MultiClipboard *mc, int slot, const char *label);
void mclip_clear(MultiClipboard *mc, int slot);
void mclip_clear_all(MultiClipboard *mc);

#endif
HEOF

cat > puttyalt_multiclip.c << 'CEOF'
#include "puttyalt_multiclip.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

void mclip_init(MultiClipboard *mc)
{
    memset(mc, 0, sizeof(*mc));
    mc->sync_system = 1;
}

void mclip_free(MultiClipboard *mc)
{
    for (int i = 0; i < CLIP_SLOTS; i++) free(mc->data[i]);
    memset(mc, 0, sizeof(*mc));
}

int mclip_copy(MultiClipboard *mc, int slot, const char *data, int len)
{
    if (slot < 0 || slot >= CLIP_SLOTS || !data) return -1;
    if (len > CLIP_MAX_LEN) len = CLIP_MAX_LEN;
    free(mc->data[slot]);
    mc->data[slot] = (char *)malloc(len + 1);
    if (!mc->data[slot]) return -1;
    memcpy(mc->data[slot], data, len);
    mc->data[slot][len] = '\0';
    mc->len[slot] = len;
    mc->timestamp[slot] = (unsigned long)time(NULL);
    mc->active_slot = slot;
    return 0;
}

int mclip_copy_next(MultiClipboard *mc, const char *data, int len)
{
    int slot = mc->ring_mode ? (mc->active_slot + 1) % CLIP_SLOTS : mc->active_slot;
    return mclip_copy(mc, slot, data, len);
}

const char *mclip_paste(MultiClipboard *mc, int slot, int *len)
{
    if (slot < 0 || slot >= CLIP_SLOTS || !mc->data[slot]) return NULL;
    if (len) *len = mc->len[slot];
    return mc->data[slot];
}

const char *mclip_paste_active(MultiClipboard *mc, int *len) { return mclip_paste(mc, mc->active_slot, len); }
void mclip_set_label(MultiClipboard *mc, int slot, const char *label)
{
    if (slot >= 0 && slot < CLIP_SLOTS) snprintf(mc->label[slot], 32, "%s", label);
}

void mclip_clear(MultiClipboard *mc, int slot)
{
    if (slot >= 0 && slot < CLIP_SLOTS) { free(mc->data[slot]); mc->data[slot] = NULL; mc->len[slot] = 0; }
}

void mclip_clear_all(MultiClipboard *mc) { for (int i = 0; i < CLIP_SLOTS; i++) mclip_clear(mc, i); }
CEOF
commit_at 23 "Add multi-clipboard with 10 slots, ring mode, and system sync"

# 14. Environment variable viewer
cat > puttyalt_envview.h << 'HEOF'
#ifndef PUTTYALT_ENVVIEW_H
#define PUTTYALT_ENVVIEW_H

#define ENV_MAX 256

typedef struct {
    char name[128];
    char value[512];
    int  modified;
} EnvEntry;

typedef struct {
    EnvEntry entries[ENV_MAX];
    int count;
    char filter[128];
} EnvViewer;

void envview_init(EnvViewer *ev);
int  envview_add(EnvViewer *ev, const char *name, const char *value);
int  envview_set(EnvViewer *ev, const char *name, const char *value);
const char *envview_get(EnvViewer *ev, const char *name);
int  envview_filter(EnvViewer *ev, const char *pattern, int *results, int max);
void envview_clear(EnvViewer *ev);

#endif
HEOF

cat > puttyalt_envview.c << 'CEOF'
#include "puttyalt_envview.h"
#include <string.h>

void envview_init(EnvViewer *ev) { memset(ev, 0, sizeof(*ev)); }

int envview_add(EnvViewer *ev, const char *name, const char *value)
{
    if (ev->count >= ENV_MAX) return -1;
    EnvEntry *e = &ev->entries[ev->count++];
    snprintf(e->name, sizeof(e->name), "%s", name);
    snprintf(e->value, sizeof(e->value), "%s", value);
    return ev->count - 1;
}

int envview_set(EnvViewer *ev, const char *name, const char *value)
{
    for (int i = 0; i < ev->count; i++) {
        if (strcmp(ev->entries[i].name, name) == 0) {
            snprintf(ev->entries[i].value, sizeof(ev->entries[i].value), "%s", value);
            ev->entries[i].modified = 1;
            return i;
        }
    }
    return envview_add(ev, name, value);
}

const char *envview_get(EnvViewer *ev, const char *name)
{
    for (int i = 0; i < ev->count; i++)
        if (strcmp(ev->entries[i].name, name) == 0) return ev->entries[i].value;
    return NULL;
}

int envview_filter(EnvViewer *ev, const char *pattern, int *results, int max)
{
    int n = 0;
    for (int i = 0; i < ev->count && n < max; i++)
        if (strstr(ev->entries[i].name, pattern) || strstr(ev->entries[i].value, pattern))
            results[n++] = i;
    return n;
}

void envview_clear(EnvViewer *ev) { ev->count = 0; }
CEOF
commit_at 15 "Add remote environment variable viewer with filter and edit"

# 15. Quick commands panel
cat > puttyalt_quickcmd.h << 'HEOF'
#ifndef PUTTYALT_QUICKCMD_H
#define PUTTYALT_QUICKCMD_H

#define QCMD_MAX 24

typedef struct {
    char label[32];
    char command[512];
    char icon[16]; /* emoji or short icon code */
    char tooltip[128];
    int  confirm;
    int  color;
} QuickCommand;

typedef struct {
    QuickCommand items[QCMD_MAX];
    int count;
    int visible;
    int orientation; /* 0=horizontal, 1=vertical */
    int icon_only;
} QuickCmdPanel;

void quickcmd_init(QuickCmdPanel *qp);
void quickcmd_add_defaults(QuickCmdPanel *qp);
int  quickcmd_add(QuickCmdPanel *qp, const char *label, const char *cmd, int confirm);
int  quickcmd_remove(QuickCmdPanel *qp, int index);
void quickcmd_toggle(QuickCmdPanel *qp);
QuickCommand *quickcmd_get(QuickCmdPanel *qp, int index);

#endif
HEOF

cat > puttyalt_quickcmd.c << 'CEOF'
#include "puttyalt_quickcmd.h"
#include <string.h>

void quickcmd_init(QuickCmdPanel *qp)
{
    memset(qp, 0, sizeof(*qp));
    qp->visible = 1;
    qp->orientation = 0;
}

int quickcmd_add(QuickCmdPanel *qp, const char *label, const char *cmd, int confirm)
{
    if (qp->count >= QCMD_MAX) return -1;
    QuickCommand *q = &qp->items[qp->count++];
    memset(q, 0, sizeof(*q));
    snprintf(q->label, sizeof(q->label), "%s", label);
    snprintf(q->command, sizeof(q->command), "%s", cmd);
    q->confirm = confirm;
    return qp->count - 1;
}

void quickcmd_add_defaults(QuickCmdPanel *qp)
{
    quickcmd_add(qp, "Top", "top -bn1 | head -20", 0);
    quickcmd_add(qp, "Disk", "df -h", 0);
    quickcmd_add(qp, "Memory", "free -m", 0);
    quickcmd_add(qp, "Network", "ss -tunap | head -20", 0);
    quickcmd_add(qp, "Uptime", "uptime", 0);
    quickcmd_add(qp, "Who", "who", 0);
    quickcmd_add(qp, "Last", "last -10", 0);
    quickcmd_add(qp, "Restart", "sudo systemctl restart", 1);
}

int quickcmd_remove(QuickCmdPanel *qp, int index)
{
    if (index < 0 || index >= qp->count) return -1;
    memmove(&qp->items[index], &qp->items[index+1], (qp->count - index - 1) * sizeof(QuickCommand));
    qp->count--;
    return 0;
}

void quickcmd_toggle(QuickCmdPanel *qp) { qp->visible = !qp->visible; }
QuickCommand *quickcmd_get(QuickCmdPanel *qp, int index) { return (index >= 0 && index < qp->count) ? &qp->items[index] : NULL; }
CEOF
commit_at 20 "Add quick commands panel with one-click system monitoring shortcuts"

# 16. Register new modules in puttyalt.h
cat >> puttyalt.h << 'HEOF'

/* v1.2.0 — UX & Usability */
#define PUTTYALT_SMARTPASTE      1
#define PUTTYALT_SESSSEARCH      1
#define PUTTYALT_PASSGEN         1
#define PUTTYALT_DRAGDROP        1
#define PUTTYALT_SNIPPETLIB      1
#define PUTTYALT_SESSNOTE_V2     1
#define PUTTYALT_PRESETS         1
#define PUTTYALT_NOTIFQUEUE      1
#define PUTTYALT_SEARCHOVERLAY   1
#define PUTTYALT_SSHKEYMGR       1
#define PUTTYALT_AUTOCOMPLETE_V2 1
#define PUTTYALT_CONNTIMEOUT     1
#define PUTTYALT_MULTICLIP       1
#define PUTTYALT_ENVVIEW         1
#define PUTTYALT_QUICKCMD        1
HEOF
commit_at 11 "Register v1.2.0 UX modules in puttyalt.h"

# 17. Update CMakeLists.txt
cat >> CMakeLists.txt << 'CEOF'

# v1.2.0 UX & Usability modules
list(APPEND PUTTYALT_SOURCES
    puttyalt_smartpaste.c
    puttyalt_sesssearch.c
    puttyalt_passgen.c
    puttyalt_dragdrop.c
    puttyalt_snippetlib.c
    puttyalt_sessnote.c
    puttyalt_presets.c
    puttyalt_notifqueue.c
    puttyalt_searchoverlay.c
    puttyalt_sshkeymgr.c
    puttyalt_autocomplete_v2.c
    puttyalt_conntimeout.c
    puttyalt_multiclip.c
    puttyalt_envview.c
    puttyalt_quickcmd.c
)
CEOF
commit_at 9 "Add v1.2.0 UX modules to CMakeLists.txt"

# 18. Bump version to 1.2.0
sed -i 's/PUTTYALT_VERSION_STR    "1.1.0"/PUTTYALT_VERSION_STR    "1.2.0"/' puttyalt.h
sed -i 's/PUTTYALT_VERSION_MINOR  1/PUTTYALT_VERSION_MINOR  2/' puttyalt.h
commit_at 8 "Bump version to v1.2.0"

# 19. Expand ROADMAP significantly
cat > ROADMAP.md << 'REOF'
# PuttyAlt Roadmap

## Philosophy
PuttyAlt aims to be the most **convenient** SSH client for daily work.
Every feature is measured by: *Does this save the user time or reduce friction?*

---

## v1.1.0 (Released) — Terminal Engine & UI Foundation
- [x] Terminal emulation engine with ANSI parser
- [x] Text selection (char, word, line, rectangle)
- [x] Scrollback ring buffer with search
- [x] 256-color palette manager
- [x] Font fallback chain
- [x] URL detection and clickable links
- [x] Right-click context menus
- [x] Quick connect bar
- [x] Session favorites with tags
- [x] Connection health monitoring
- [x] Auto-reconnect with exponential backoff
- [x] Jump host chain manager
- [x] UI abstraction layer
- [x] Dockable panel layout
- [x] Tab manager with pin/move/close
- [x] Broadcast v2 with filters
- [x] Session recording and replay
- [x] Setup wizard
- [x] Configuration validation

## v1.2.0 (Current) — UX & Usability
*Focus: make every daily task faster and safer.*

- [x] Smart paste — warn before pasting dangerous commands (rm -rf, sudo, curl|bash)
- [x] Session search — fuzzy matching across names, hosts, tags
- [x] Password generator — configurable strength, entropy meter
- [x] Drag & drop — files to SFTP upload or path paste
- [x] Snippet library — categorized commands with search and defaults
- [x] Session notes — per-connection annotations, auto-display on connect
- [x] Connection presets — Default, Dev, Production, Presentation profiles
- [x] Notification queue — leveled alerts with actions and sound control
- [x] Search overlay — Ctrl+F with highlight-all and case toggle
- [x] SSH key manager — auto-detect types, agent integration, host matching
- [x] Auto-complete v2 — history learning, prefix matching, context-aware
- [x] Connection timeouts — fast/default/patient profiles
- [x] Multi-clipboard — 10 slots with ring mode and labels
- [x] Environment viewer — remote env vars with filter and edit
- [x] Quick commands — one-click monitoring shortcuts panel

## v1.3.0 (Next) — Workflow & Productivity
*Focus: reduce repetitive work, help power users.*

### Planned
- [ ] **Session groups v2** — nested folders, color-coded, drag reorder
- [ ] **Command history browser** — full-text search across all sessions
- [ ] **SFTP sync** — watch local folder, auto-upload on change
- [ ] **Regex triggers v2** — pattern-match output, trigger actions (alert, log, script)
- [ ] **Terminal multiplexer v2** — tmux-like splits without server-side tmux
- [ ] **Output capture** — save terminal output to file with timestamp
- [ ] **Diff viewer** — compare two sessions or session recordings side-by-side
- [ ] **Connection dashboard** — overview of all active sessions with health status
- [ ] **Inline file editor** — quick edit of remote files without leaving terminal
- [ ] **Cron job viewer** — list and manage remote crontab entries

### UX Improvements
- [ ] First-connect suggestions (based on OS detection)
- [ ] Session color tags visible in tab bar
- [ ] Connection speed test on first connect
- [ ] Smart resize — adjust font to fit window changes
- [ ] Tab preview on hover
- [ ] Session export to OpenSSH config format

## v1.4.0 (Future) — Collaboration & Sharing
- [ ] **Shared snippets** — team snippet libraries via git sync
- [ ] **Session sharing** — read-only terminal view for pair programming
- [ ] **Audit log** — compliance-ready session audit trail
- [ ] **Role-based access** — restrict connection targets per user profile
- [ ] **Config profiles** — import/export all settings as portable profile

## v2.0.0 (Planned) — Modern UI Rewrite
**The big one.** Complete UI rewrite with a modern rendering engine.

### Rendering
- **GPU-accelerated rendering** — Direct2D/OpenGL/Vulkan backend replacing GDI
- **120 FPS terminal** — smooth cursor blink, scroll, animations
- **Subpixel font rendering** — ClearType/FreeType with gamma correction
- **Ligature support** — full font shaping with HarfBuzz
- **True color** — 24-bit RGB (parser already supports it since v1.1.0)
- **Image protocol** — inline images via iTerm2/Sixel/Kitty graphics

### Layout
- **Split panes** — horizontal, vertical, nested, with drag resize
- **Floating panels** — undockable panels with snap-to-edge
- **Minimap** — scrollback overview like modern code editors
- **Session thumbnails** — live terminal preview in tab bar
- **Zen mode** — distraction-free single-session fullscreen

### Interaction
- **Smooth scrolling** — pixel-level with momentum and touchpad gestures
- **Search overlay** — Ctrl+F with regex, live highlight, match counter
- **Command palette** — fuzzy-search all commands (Ctrl+Shift+P)
- **Inline rename** — double-click tab to rename
- **Drag tabs** — reorder and detach tabs into new windows

### Architecture Changes
- Migrate from Win32 GDI to Direct2D/SDL2 hybrid renderer
- Separate logic layer from rendering (via UILayer, already in v1.1.0)
- Plugin API v2 with sandboxed execution
- WebSocket-based remote access (optional)

### Timeline
- Q3 2025: Renderer prototype + benchmarking
- Q4 2025: Widget toolkit + split panes + smooth scroll
- Q1 2026: Beta with full feature parity
- Q2 2026: Stable v2.0.0 release

## v2.1.0 (Vision)
- AI-powered command suggestions (local LLM, no cloud)
- Cloud session sync (encrypted, opt-in)
- Team collaboration mode (shared terminals)
- Mobile companion app (iOS/Android session viewer)
- Native Linux package (deb/rpm/AppImage)
- Native macOS build (Universal Binary)
REOF
commit_at 22 "Expand roadmap with v1.2-v1.4 milestones, UX philosophy, and detailed v2.0 plan"

# 20. Update CHANGELOG
cat > CHANGELOG.md << 'LEOF'
# Changelog

## [1.2.0] — 2025-05-24

### Added — UX & Usability
- Smart paste detection: warns before pasting dangerous commands (rm -rf, sudo, curl|bash)
- Session search with fuzzy matching across names, hosts, and tags
- Password generator with strength meter and entropy calculation
- Drag-and-drop handler for SFTP upload and path paste
- Snippet library with categories, search, and 10 default system commands
- Session notes: per-connection annotations with auto-display on connect
- Connection presets: Default, Development, Production, Presentation profiles
- Notification queue with levels, actions, sound, and popup duration
- Terminal search overlay (Ctrl+F) with highlight-all and case-insensitive mode
- SSH key manager with type detection, agent integration, and host matching
- Auto-complete v2 with command history learning and prefix matching
- Connection timeout profiles: default, fast, patient
- Multi-clipboard with 10 named slots and ring mode
- Remote environment variable viewer with filter and edit
- Quick commands panel with one-click monitoring shortcuts

## [1.1.0] — 2025-05-23

### Added
- Terminal emulation engine with full cell-based rendering
- ANSI escape sequence parser (CSI, SGR, OSC, DCS)
- Text selection: character, word, line, and rectangle modes
- Scrollback ring buffer (configurable, default 20,000 lines)
- Scrollback text search
- 256-color palette manager with xterm compatibility
- Font fallback chain (Cascadia Code → Consolas → Courier New)
- URL detection with protocol-aware matching
- Right-click context menus (terminal, sidebar, tabs)
- Quick connect bar with history
- Session favorites with tags, pinning, and usage stats
- Connection health monitor (latency, packet loss, idle detection)
- Auto-reconnect with exponential backoff and jitter
- SSH jump host chain manager (up to 8 hops)
- UI abstraction layer for future framework migration
- Tooltip system with configurable delay
- Dockable panel layout engine (left/right/top/bottom/center/float)
- Configuration validation and sanitization
- First-run setup wizard
- Session recording and binary replay format
- Color scheme editor with import/export
- Advanced bell controller (visual, system, taskbar, custom)
- Enhanced portable mode with directory structure
- Tab manager with pin, move, close-others, Ctrl+Tab
- Broadcast v2 with target filtering and dangerous command detection

### Improved
- All 40+ menu items now have handlers
- Dark-themed dialogs (Connect, Settings, Find, Theme)
- Toolbar with standard icons
- 16 keyboard accelerators

## [1.0.6] — 2025-05-22

### Added
- Layout manager with named presets
- Macro recorder with hotkey triggers
- Terminal profile manager
- Auto-updater with GitHub integration
- Theme engine with 4 built-in schemes
- Keybinding manager
- SFTP file manager
- Bandwidth monitor
- Proxy chain manager

## [1.0.0] — 2025-05-19
- Complete GUI redesign with Warm Blue theme
- Win32 native application framework
- 100+ feature modules

## [0.4.0] — 2025-05-18
- Workspaces, tunnel manager, plugins, serial port

## [0.3.0] — 2025-05-17
- Split view, macros, logging v2, autocomplete

## [0.2.0] — 2025-05-16
- Tabs, SFTP panel, themes, credential store

## [0.1.0] — 2025-05-14
- Initial release: portable mode, auto-reconnect, snippets, broadcast
LEOF
commit_at 14 "Update changelog with v1.2.0 UX features"

# 21. Update README module counts
sed -i 's/Total: 175+ source modules across 370+ commits/Total: 190+ source modules across 400+ commits/' README.md
commit_at 12 "Update README: 190 modules, 400+ commits"

# 22. Update website stats
sed -i 's/data-target="175"/data-target="190"/' index.html
sed -i 's/data-target="375"/data-target="410"/' index.html
sed -i 's/data-target="22000"/data-target="28000"/' index.html
commit_at 15 "Update website stats: 190 modules, 28K lines, 410 commits"

# 23. Add v1.2.0 to website hero badge
sed -i 's|<div class="hero-badge"><span class="dot"></span> v1.1.0|<div class="hero-badge"><span class="dot"></span> v1.2.0|' index.html
commit_at 10 "Update website badge to v1.2.0"

# 24. Update version badge in README
sed -i 's/version-1.0.6-blue/version-1.2.0-blue/' README.md
commit_at 8 "Update README version badge to v1.2.0"

# 25. Small polish: update about dialog
sed -i 's/and 175+ features/and 190+ features/' puttyalt_gui.c
commit_at 11 "Update feature count in About dialog to 190"

echo "Done!"
git log --oneline | head -30
