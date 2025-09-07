#pragma once

#include <QTreeView>

class HierarchicalTreeView : public QTreeView
{
public:
	HierarchicalTreeView(QWidget* parent = nullptr);
	~HierarchicalTreeView();
};
