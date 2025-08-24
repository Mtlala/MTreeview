#pragma once

#include <QObject>

#include "ui_MTreeview.h"

class MTreeviewPrivate;
class MTreeviewPrivate : public QObject
{
public:
	MTreeviewPrivate(QObject* parent = nullptr);
	~MTreeviewPrivate();

private:
	Ui::MTreeview ui;
};
