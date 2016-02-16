/**********************************************************************
    Character classes, to simplify switch statement labels.
 **********************************************************************/

#ifndef CHAR_CLASSES_H // if CHAR_CLASSES undefined:
#define CHAR_CLASSES_H // define CHAR_CLASSES


typedef enum { // enumerated type, so char_class my_char_class = END;
    END,    /* NUL, returned by reader at end of file */

    DIG_0,      /* 0      */
    DIG_1,
    DIG_2,
    DIG_3,
    DIG_4,
    DIG_5,
    DIG_6,
    DIG_7,
    DIG_8,
    DIG_9,
    WHITE,      /* space, */
    TAB,        // tab
    EOLN,       /* newline or carriage return */

    PCT,        /* % */
    LPAREN,     /* ( */
    RPAREN,     /* ) */
    STAR,       /* * */
    PLUS,       /* + */
    MINUS,      /* - */
    DOT,        /* . */
    SLASH,      /* / */
    SEMIC,      /* ; */

    OTHER       /* catchall for everything else:
                   valid only inside comments and strings */
} char_class;

// extern means this will be used/declared somewhere else, idk why need, maybe for compiler warnings
extern const char_class char_classes[128];

/* definitions to make switch statements easier to write: */
// macro cases basically work to condense lots of cases into one case

#define CASE_DIGIT   \
    case DIG_0:     \
    case DIG_1:      \
    case DIG_2:      \
    case DIG_3:        \
    case DIG_4:         \
    case DIG_5:         \
    case DIG_6:         \
    case DIG_7:        \
    case DIG_8:         \
    case DIG_9

#define CASE_DIGIT_NONZERO \
    case DIG_1:      \
    case DIG_2:      \
    case DIG_3:        \
    case DIG_4:         \
    case DIG_5:         \
    case DIG_6:         \
    case DIG_7:        \
    case DIG_8:         \
    case DIG_9

#define CASE_SIGN       \
    case PLUS:          \
    case MINUS

#endif
