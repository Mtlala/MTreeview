#include "HierarchicalTreeView.h"
#include "HierarchicalModel.h"

HierarchicalTreeView::HierarchicalTreeView(QWidget* parent)
	: QTreeView(parent)
{
	auto mdl = new HierarchicalModel(this);
	this->setModel(mdl);

	mdl->setHeader({ tr("Name"), tr("Type"), tr("Value")});

	mdl->insertWord("Vars.PointArray");
	mdl->insertWord("Vars.PointArray2");
}

HierarchicalTreeView::~HierarchicalTreeView()
{
}