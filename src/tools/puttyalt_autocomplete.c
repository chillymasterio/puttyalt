/*
 * puttyalt_autocomplete.c: Command auto-completion implementation.
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "puttyalt_autocomplete.h"

static const char *default_commands[] = {
    "ls", "cd", "pwd", "cat", "grep", "find", "awk", "sed",
    "ssh", "scp", "rsync", "wget", "curl", "tar", "gzip",
    "mkdir", "rmdir", "rm", "cp", "mv", "chmod", "chown",
    "ps", "kill", "top", "htop", "df", "du", "free",
    "systemctl", "journalctl", "service", "apt", "yum", "dnf",
    "docker", "kubectl", "git", "vim", "nano", "less", "head",
    "tail", "wc", "sort", "uniq", "cut", "tr", "xargs",
    "netstat", "ss", "ip", "iptables", "nft", "tcpdump",
    "mount", "umount", "fdisk", "lsblk", "blkid",
    NULL
};

void ac_init(AutoComplete *ac)
{
    memset(ac, 0, sizeof(*ac));
    ac->selected = -1;
}

void ac_load_defaults(AutoComplete *ac)
{
    for (int i = 0; default_commands[i] && ac->dict_count < AC_DICT_SIZE; i++) {
        strncpy(ac->dictionary[ac->dict_count], default_commands[i],
                AC_MAX_WORD_LEN - 1);
        ac->dict_count++;
    }
}

static int prefix_match(const char *word, const char *prefix)
{
    int plen = strlen(prefix);
    if (plen == 0)
        return 0;
    return strncmp(word, prefix, plen) == 0 && strlen(word) > (size_t)plen;
}

int ac_update(AutoComplete *ac, const char *current_word)
{
    ac->num_suggestions = 0;
    ac->selected = -1;

    if (!current_word || strlen(current_word) == 0) {
        ac->active = 0;
        return 0;
    }

    strncpy(ac->prefix, current_word, AC_MAX_WORD_LEN - 1);

    for (int i = 0; i < ac->dict_count && ac->num_suggestions < AC_MAX_SUGGESTIONS; i++) {
        if (prefix_match(ac->dictionary[i], current_word)) {
            AutoCompleteSuggestion *s = &ac->suggestions[ac->num_suggestions];
            strncpy(s->text, ac->dictionary[i], AC_MAX_WORD_LEN - 1);
            s->frequency = 0;
            s->from_history = 0;
            ac->num_suggestions++;
        }
    }

    ac->active = ac->num_suggestions > 0;
    if (ac->active)
        ac->selected = 0;
    return ac->num_suggestions;
}

const char *ac_get_selected(const AutoComplete *ac)
{
    if (!ac->active || ac->selected < 0 || ac->selected >= ac->num_suggestions)
        return NULL;
    return ac->suggestions[ac->selected].text;
}

int ac_next(AutoComplete *ac)
{
    if (!ac->active || ac->num_suggestions == 0)
        return -1;
    ac->selected = (ac->selected + 1) % ac->num_suggestions;
    return ac->selected;
}

int ac_prev(AutoComplete *ac)
{
    if (!ac->active || ac->num_suggestions == 0)
        return -1;
    ac->selected = (ac->selected - 1 + ac->num_suggestions) % ac->num_suggestions;
    return ac->selected;
}

void ac_dismiss(AutoComplete *ac)
{
    ac->active = 0;
    ac->num_suggestions = 0;
    ac->selected = -1;
}

int ac_add_to_dict(AutoComplete *ac, const char *word)
{
    if (!word || strlen(word) == 0)
        return -1;
    if (ac->dict_count >= AC_DICT_SIZE)
        return -1;

    /* Check for duplicates */
    for (int i = 0; i < ac->dict_count; i++) {
        if (strcmp(ac->dictionary[i], word) == 0)
            return 0; /* already exists */
    }

    strncpy(ac->dictionary[ac->dict_count], word, AC_MAX_WORD_LEN - 1);
    ac->dict_count++;
    return 1;
}
