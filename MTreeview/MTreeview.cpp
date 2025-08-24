#include "MTreeview.h"
#include "MTreeviewPrivate.h"

MTreeview::MTreeview(QWidget* parent)
	: QWidget(parent),
	  d(new MTreeviewPrivate(this))
{
}

MTreeview::~MTreeview()
{
}