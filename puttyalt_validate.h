#ifndef PUTTYALT_VALIDATE_H
#define PUTTYALT_VALIDATE_H

typedef enum {
    VAL_OK = 0,
    VAL_WARN,
    VAL_ERROR
} ValidationLevel;

typedef struct ValidationMessage {
    ValidationLevel level;
    char field[64];
    char message[256];
} ValidationMessage;

typedef struct ValidationResult {
    ValidationMessage messages[32];
    int count;
    int has_errors;
} ValidationResult;

void val_init(ValidationResult *vr);
void val_add(ValidationResult *vr, ValidationLevel level,
             const char *field, const char *msg);
int  val_port(int port);
int  val_hostname(const char *host);
int  val_username(const char *user);
int  val_path(const char *path);

#endif
