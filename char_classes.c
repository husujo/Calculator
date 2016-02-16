/**********************************************************************
    Character classes, to simplify switch statement labels.

    NB: These classes do NOT suffice for the assignment;
        you will need to modify this table.
 **********************************************************************/

#include "char_classes.h"

// const just means this can't be modified after declaration
// *********************   this corresponds with the ACII table!!!   *********************************

// switch everything we don't use to OTHER
const char_class char_classes[128] = {
  END,        /* nul   ^@ */      /* returned by get_character at EOF (end of file) literally EOF?*/ 
  OTHER,      /* soh   ^A */
  OTHER,      /* stx   ^B */
  OTHER,      /* etx   ^C */
  OTHER,      /* eot   ^D */
  OTHER,      /* enq   ^E */
  OTHER,      /* ack   ^F */
  OTHER,      /* bel   ^G */
  OTHER,      /* bs    ^H */
  TAB,      /* TAB    /t or ^I */
  EOLN,       /* nl  end of line (newline)    \n  ^J */    /* 10 */
  OTHER,      /* vt    ^K */
  OTHER,      /* ff    ^L */
  EOLN,       /* cr  end of line (carriage return) \r  ^M */
  OTHER,      /* so    ^N */
  OTHER,      /* si    ^O */
  OTHER,      /* dle   ^P */
  OTHER,      /* dc1   ^Q */
  OTHER,      /* dc2   ^R */
  OTHER,      /* dc3   ^S */
  OTHER,      /* dc4   ^T */    /* 20 */
  OTHER,      /* nak   ^U */
  OTHER,      /* syn   ^V */
  OTHER,      /* etb   ^W */
  OTHER,      /* can   ^X */
  OTHER,      /* em    ^Y */
  OTHER,      /* sub   ^Z */
  OTHER,      /* esc   ^[ */
  OTHER,      /* fs    ^\ */
  OTHER,      /* gs    ^] */
  OTHER,      /* rs    ^^ */    /* 30 */
  OTHER,      /* us    ^_ */
  WHITE,      /* sp */          // whitespace?
  OTHER,       /* ! */
  OTHER,    /* " */
  OTHER,      /* # */
  OTHER,  /* $ */
  PCT,        /* % */
  OTHER,        /* & */
  OTHER,    /* ' */
  LPAREN,     /* ( */
  RPAREN,     /* ) */
  STAR,       /* * */
  PLUS,       /* + */
  OTHER,      /* , */
  MINUS,      /* - */
  DOT,        /* . */
  SLASH,      /* / */
  DIG_0,      /* 0 */
  DIG_1,   /* 1 */
  DIG_2,   /* 2 */
  DIG_3,   /* 3 */
  DIG_4,   /* 4 */
  DIG_5,   /* 5 */
  DIG_6,   /* 6 */
  DIG_7,   /* 7 */
  DIG_8,     /* 8 */
  DIG_9,     /* 9 */
  OTHER,      /* : */
  SEMIC,      /* ; */
  OTHER,     /* < */
  OTHER,     /* = */
  OTHER,     /* > */
  OTHER,      /* ? */
  OTHER,      /* @ */
  OTHER,    /* A */
  OTHER,    /* B */
  OTHER,    /* C */
  OTHER,     /* D */
  OTHER,      /* E */
  OTHER,     /* F */
  OTHER,  /* G */
  OTHER,  /* H */
  OTHER,  /* I */
  OTHER,  /* J */
  OTHER,  /* K */
  OTHER,      /* L */
  OTHER,  /* M */
  OTHER,  /* N */
  OTHER,  /* O */
  OTHER,  /* P */
  OTHER,  /* Q */
  OTHER,  /* R */
  OTHER,  /* S */
  OTHER,  /* T */
  OTHER,  /* U */
  OTHER,  /* V */
  OTHER,  /* W */
  OTHER,      /* X */
  OTHER,  /* Y */
  OTHER,  /* Z */
  OTHER,      /* [ */
  OTHER,    /* \ */
  OTHER,      /* ] */
  OTHER,      /* ^ */
  OTHER,  /* _ */
  OTHER,      /* ` */
  OTHER,    /* a */
  OTHER,      /* b */   /* lc only, for escape sequences */
  OTHER,    /* c */
  OTHER,     /* d */
  OTHER,      /* e */
  OTHER,      /* f */   /* lc only, for escape sequences */
  OTHER,  /* g */
  OTHER,  /* h */
  OTHER,  /* i */
  OTHER,  /* j */
  OTHER,  /* k */
  OTHER,      /* l */
  OTHER,  /* m */
  OTHER,      /* n */   /* lc only, for escape sequences */
  OTHER,  /* o */
  OTHER,  /* p */
  OTHER,  /* q */
  OTHER,      /* r */   /* lc only, for escape sequences */
  OTHER,  /* s */
  OTHER,      /* t */   /* lc only, for escape sequences */
  OTHER,      /* u */   /* lc only, for escape sequences */
  OTHER,  /* v */
  OTHER,  /* w */
  OTHER,      /* x */
  OTHER,  /* y */
  OTHER,  /* z */
  OTHER,     /* { */
  OTHER,        /* | */
  OTHER,     /* } */
  OTHER,      /* ~ */
  OTHER       /* del   ^? */
};




