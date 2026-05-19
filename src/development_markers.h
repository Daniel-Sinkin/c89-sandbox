/* dans/development_markers.h */
#ifndef DANS_DEVELOPMENT_MARKERS_H
#define DANS_DEVELOPMENT_MARKERS_H

#if defined(mut)
#    error "mut marker macro is already defined"
#endif
#if defined(mut_unchecked)
#    error "mut_unchecked marker macro is already defined"
#endif
#if defined(DANS_NODISCARD)
#    error "DANS_NODISCARD marker macro is already defined"
#endif

#if defined(__clang__)
#    define mut __attribute__((annotate("mut")))
#    define mut_unchecked __attribute__((annotate("mut_unchecked")))
#else
#    define mut
#    define mut_unchecked
#endif

#if defined(__clang__) || defined(__GNUC__)
#    define DANS_NODISCARD __attribute__((warn_unused_result))
#else
#    define DANS_NODISCARD
#endif

#endif /* DANS_DEVELOPMENT_MARKERS_H */
