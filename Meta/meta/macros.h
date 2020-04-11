// Not using include guard on purpose
// ReSharper disable CppMissingIncludeGuard

#ifdef META_DEFINE_MACROS

#define META_EXPAND_16(prefix) \
    prefix##0, prefix##1, prefix##2, prefix##3, prefix##4, prefix##5, prefix##6, prefix##7, \
    prefix##8, prefix##9, prefix##a, prefix##b, prefix##c, prefix##d, prefix##e, prefix##f

#define META_EXPAND_256(prefix) \
    META_EXPAND_16(prefix##0), META_EXPAND_16(prefix##1), META_EXPAND_16(prefix##2), META_EXPAND_16(prefix##3), \
    META_EXPAND_16(prefix##4), META_EXPAND_16(prefix##5), META_EXPAND_16(prefix##6), META_EXPAND_16(prefix##7), \
    META_EXPAND_16(prefix##8), META_EXPAND_16(prefix##9), META_EXPAND_16(prefix##a), META_EXPAND_16(prefix##b), \
    META_EXPAND_16(prefix##c), META_EXPAND_16(prefix##d), META_EXPAND_16(prefix##e), META_EXPAND_16(prefix##f)

#define META_TYPE_PARAMS_256 META_EXPAND_256(typename T)
#define META_TYPES_256 META_EXPAND_256(T)

#else

#undef META_EXPAND_16
#undef META_EXPAND_256
#undef META_TYPE_PARAMS_256
#undef META_TYPES_256

#endif
