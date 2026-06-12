/* puttyalt_optransform.c - minimal single-document OT engine for the
 * shared command-line edit buffer (PuttyAlt). Self-contained C99. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define OT_DOC_CAP   256   /* max document text length (excl. NUL) */
#define OT_TEXT_CAP  64    /* max insert payload (excl. NUL)       */
#define OT_HIST_CAP  64    /* applied-op ring for transform-against */

enum ot_kind { OT_NOP = 0, OT_INSERT = 1, OT_DELETE = 2 };

typedef struct {
    int      kind;                  /* enum ot_kind                      */
    int      pos;                   /* offset the op acts at             */
    int      len;                   /* insert bytes / delete chars       */
    int      site;                  /* originating peer id (tie-break)   */
    int      rev;                   /* base revision the op was built on */
    char     text[OT_TEXT_CAP + 1]; /* inserted bytes (insert only)      */
} ot_op;

typedef struct {
    char     doc[OT_DOC_CAP + 1];   /* NUL-terminated document           */
    int      len;                   /* current document length           */
    int      rev;                   /* document revision counter         */
    int      site;                  /* this peer's id                    */
    ot_op    hist[OT_HIST_CAP];     /* ring of applied ops               */
    int      hcount;                /* total ops applied (monotonic)     */
} ot_buf;

void otbuf_init(ot_buf *b, int site)
{
    if (!b) return;
    memset(b, 0, sizeof(*b));
    b->site = site;
}

const char *otbuf_text(const ot_buf *b)
{
    return b ? b->doc : "";
}

int otbuf_revision(const ot_buf *b)
{
    return b ? b->rev : -1;
}

int otbuf_make_insert(const ot_buf *b, ot_op *op, int pos, const char *s)
{
    int n;
    if (!b || !op || !s) return -1;
    if (pos < 0 || pos > b->len) return -1;
    n = (int)strlen(s);
    if (n <= 0 || n > OT_TEXT_CAP) return -1;
    if (b->len + n > OT_DOC_CAP) return -1;
    memset(op, 0, sizeof(*op));
    op->kind = OT_INSERT;
    op->pos  = pos;
    op->len  = n;
    op->site = b->site;
    op->rev  = b->rev;
    snprintf(op->text, sizeof(op->text), "%s", s);
    return 0;
}

int otbuf_make_delete(const ot_buf *b, ot_op *op, int pos, int len)
{
    if (!b || !op) return -1;
    if (pos < 0 || len <= 0 || pos + len > b->len) return -1;
    memset(op, 0, sizeof(*op));
    op->kind = OT_DELETE;
    op->pos  = pos;
    op->len  = len;
    op->site = b->site;
    op->rev  = b->rev;
    return 0;
}

/* Transform 'a' so it can apply after concurrent op 'o' already was.
 * Equal-position ties broken deterministically by site id. */
int otbuf_transform(ot_op *a, const ot_op *o)
{
    if (!a || !o) return -1;
    if (a->kind == OT_NOP || o->kind == OT_NOP) return 0;
    if (o->kind == OT_INSERT) {
        if (o->pos < a->pos || (o->pos == a->pos && o->site < a->site))
            a->pos += o->len;
    } else { /* o is a delete */
        int o_end = o->pos + o->len;
        if (o_end <= a->pos) {
            a->pos -= o->len;
        } else if (o->pos <= a->pos) {        /* delete covers a->pos */
            if (a->kind == OT_DELETE) {
                int over = o_end - a->pos;
                a->len -= (over < a->len) ? over : a->len;
            }
            a->pos = o->pos;
        } else if (a->kind == OT_DELETE && o->pos < a->pos + a->len) {
            int over = (a->pos + a->len) - o->pos;
            a->len -= (over < a->len) ? over : a->len;
        }
    }
    if (a->len < 0) a->len = 0;
    if (a->pos < 0) a->pos = 0;
    if (a->pos > OT_DOC_CAP) a->pos = OT_DOC_CAP;
    return 0;
}

int otbuf_apply(ot_buf *b, const ot_op *op)
{
    if (!b || !op) return -1;
    if (op->kind == OT_INSERT) {
        if (op->pos < 0 || op->pos > b->len) return -1;
        if (op->len <= 0 || b->len + op->len > OT_DOC_CAP) return -1;
        memmove(b->doc + op->pos + op->len, b->doc + op->pos,
                (size_t)(b->len - op->pos));
        memcpy(b->doc + op->pos, op->text, (size_t)op->len);
        b->len += op->len;
    } else if (op->kind == OT_DELETE) {
        if (op->pos < 0 || op->len <= 0 || op->pos + op->len > b->len)
            return -1;
        memmove(b->doc + op->pos, b->doc + op->pos + op->len,
                (size_t)(b->len - op->pos - op->len));
        b->len -= op->len;
    } else {
        return 0;
    }
    b->doc[b->len] = '\0';
    b->hist[b->hcount % OT_HIST_CAP] = *op;
    b->hcount++;
    b->rev++;
    return 0;
}

/* Merge a remote op built against an older revision: transform it past
 * every local op applied since op.rev, then apply it locally. */
int otbuf_apply_remote(ot_buf *b, ot_op op)
{
    int behind, start, i;
    if (!b) return -1;
    if (op.rev < 0 || op.rev > b->rev) return -1;
    behind = b->rev - op.rev;
    if (behind > OT_HIST_CAP) return -1;
    /* history overrun */
    start = b->hcount - behind;
    if (start < 0) return -1;
    for (i = start; i < b->hcount; i++)
        if (otbuf_transform(&op, &b->hist[i % OT_HIST_CAP]) != 0)
            return -1;
    if (op.kind == OT_DELETE && op.len <= 0) {  /* cancelled by concurrent del */
        b->rev++;
        return 0;
    }
    op.rev = b->rev;
    return otbuf_apply(b, &op);
}
