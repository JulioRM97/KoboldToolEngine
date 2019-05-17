// Only use this header for external libraries warnings.
// Native engine code MUST NOT generate any kind of warning.

#ifndef IGNORE_WARNINGS

#define IGNORE_WARNINGS 1

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4305)
#pragma warning(disable: 4305)
#pragma warning(disable: 4127)
#pragma warning(disable: 4201)
#pragma warning(disable: 4706)
#pragma warning(disable: 4702)
#pragma warning(disable: 4244)
#endif // _MSC_VER

#else // IGNORE_WARNINGS

#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER

#undef IGNORE_WARNINGS

#endif // IGNORE_WARNINGS