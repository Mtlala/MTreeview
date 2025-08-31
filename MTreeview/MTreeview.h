#pragma once

#include <QtCore/qglobal.h>
#include <QWidget>

#include "Utils/global_defs.h"

class MTreeviewPrivate;
class MTREEVIEW_EXPORT MTreeview : public QWidget
{
public:
    MTreeview(QWidget* parent = nullptr);
	~MTreeview();

private:
	MTreeviewPrivate* d;
};
