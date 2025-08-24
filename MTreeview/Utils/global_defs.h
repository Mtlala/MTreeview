#pragma once

#ifndef BUILD_STATIC
# if defined(MTREEVIEW_LIB)
#  define MTREEVIEW_EXPORT Q_DECL_EXPORT
# else
#  define MTREEVIEW_EXPORT Q_DECL_IMPORT
# endif
#else
# define MTREEVIEW_EXPORT
#endif

