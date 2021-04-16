// zestaw makr pozwalajacych na prosta obsluge portow procosora.
// przyklad z dokumentacji avr-libc

#define GLUE(a, b)     a##b

/* single-bit macros, used for control bits */
#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define TGL_(what, p, m) GLUE(what, p) ^= (1 << (m))
#define GET_(/* PIN, */ p, m) (GLUE(PIN, p) & (1 << (m)))

#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)
#define TGL(what, x) TGL_(what, x)
#define GET(/* PIN, */ x) GET_(x)


