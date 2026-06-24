/* puttyalt_keyboardlayout.c - Map keyboard layout key codes.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* Translate a US-layout key to its shifted symbol. */
char kl2_shift(char c) {
    switch (c) {
        case '1': return '!'; case '2': return '@'; case '3': return '#';
        case '4': return '$'; case '5': return '%'; case '6': return '^';
        case '7': return '&'; case '8': return '*'; case '9': return '(';
        case '0': return ')'; case '-': return '_'; case '=': return '+';
        case '[': return '{'; case ']': return '}'; case '\\': return '|';
        case ';': return ':'; case '\'': return '"'; case ',': return '<';
        case '.': return '>'; case '/': return '?'; case '`': return '~';
        default: return (c >= 'a' && c <= 'z') ? c - 32 : c;
    }
}
char kl2_unshift(char c) {
    if (c >= 'A' && c <= 'Z') return c + 32;
    switch (c) {
        case '!': return '1'; case '@': return '2'; case '#': return '3';
        case '$': return '4'; case '%': return '5'; case '^': return '6';
        case '&': return '7'; case '*': return '8'; case '(': return '9';
        case ')': return '0';
        default: return c;
    }
}
