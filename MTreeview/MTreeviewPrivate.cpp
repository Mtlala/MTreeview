#include "MTreeviewPrivate.h"

MTreeviewPrivate::MTreeviewPrivate(QObject* parent)
	: QObject(parent)
{
	ui.setupUi(dynamic_cast<QWidget*>(parent));
}

MTreeviewPrivate::~MTreeviewPrivate()
{
}