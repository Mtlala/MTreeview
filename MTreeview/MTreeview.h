#pragma once

#include <QtCore/qglobal.h>
#include <QWidget>

#ifndef BUILD_STATIC
# if defined(MTREEVIEW_LIB)
#  define MTREEVIEW_EXPORT Q_DECL_EXPORT
# else
#  define MTREEVIEW_EXPORT Q_DECL_IMPORT
# endif
#else
# define MTREEVIEW_EXPORT
#endif

class MTreeviewPrivate;
class MTREEVIEW_EXPORT MTreeview : public QWidget
{
public:
    MTreeview(QWidget* parent = nullptr);
	~MTreeview();

private:
	MTreeviewPrivate* d;
};
