#pragma once

#include <QMainWindow>
#include "ui_TestWindow.h"

class TestWindow : public QMainWindow
{
	Q_OBJECT

public:
	TestWindow(QWidget *parent = nullptr);
	~TestWindow();

private:
	Ui::TestWindow ui;
};

