/* puttyalt_modaldialog.c - Modal dialog state (focus trap, buttons, result). */
#include <string.h>
#include <stdio.h>
#define MD_BUTTONS 4
#define MD_LABEL 24
enum md_result { MD_NONE=-1, MD_OK=0, MD_CANCEL=1, MD_YES=2, MD_NO=3 };
typedef struct { char title[64]; char message[256]; char buttons[MD_BUTTONS][MD_LABEL]; int button_ids[MD_BUTTONS]; int nbuttons; int focused; int result; int visible; } ModalDialog;
void modaldialog_init(ModalDialog *m) { if(m){ memset(m,0,sizeof(*m)); m->result=MD_NONE; } }
int modaldialog_show(ModalDialog *m, const char *title, const char *message) {
    if(!m) return -1;
    snprintf(m->title,64,"%s",title?title:""); snprintf(m->message,256,"%s",message?message:"");
    m->visible=1; m->result=MD_NONE; m->focused=0; m->nbuttons=0; return 0;
}
int modaldialog_add_button(ModalDialog *m, const char *label, int id) {
    if(!m||m->nbuttons>=MD_BUTTONS) return -1;
    snprintf(m->buttons[m->nbuttons],MD_LABEL,"%s",label); m->button_ids[m->nbuttons]=id; return m->nbuttons++;
}
void modaldialog_focus_next(ModalDialog *m) { if(m&&m->nbuttons>0) m->focused=(m->focused+1)%m->nbuttons; }
int modaldialog_activate(ModalDialog *m) {
    if(!m||m->focused<0||m->focused>=m->nbuttons) return MD_NONE;
    m->result=m->button_ids[m->focused]; m->visible=0; return m->result;
}
int modaldialog_is_visible(const ModalDialog *m) { return m?m->visible:-1; }
int modaldialog_result(const ModalDialog *m) { return m?m->result:MD_NONE; }
