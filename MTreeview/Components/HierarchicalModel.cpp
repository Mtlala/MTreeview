#include "HierarchicalModel.h"

HierarchicalModel::HierarchicalModel(QObject* parent)
    : QAbstractItemModel(parent),
      m_trie(new QLinkedTrie<PData>())
{
}

HierarchicalModel::~HierarchicalModel()
{
    delete m_trie;
    m_trie = nullptr;
}

void HierarchicalModel::setHeader(const QStringList& headers)
{
    beginResetModel();
    m_headers = headers;
	m_colCount = headers.size();
	endResetModel();
}
void HierarchicalModel::setColumnCount(int count)
{
    beginResetModel();
    m_colCount = count;
    endResetModel();
}

QModelIndex HierarchicalModel::index(int row, int column, const QModelIndex& parent) const
{ 
    Node* pNode = getParent(parent);
    if (!pNode || row >= pNode->size())
    {
        return QModelIndex();
    }

    Node* cNode = pNode->getChild(row);
    return cNode ? createIndex(row, column, cNode) : QModelIndex();
}

QModelIndex HierarchicalModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }
        
    Node* cur = static_cast<Node*>(index.internalPointer());
    if (!cur)
    {
        return QModelIndex();
    }

	Node* pNode = cur->parent();
    if (!pNode || pNode == m_trie->getRoot())
    {
        return QModelIndex();
    }
        
    return createIndex(pNode->getRow(), 0, cur->parent());
}

int HierarchicalModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
    {
		return 0;
    }

	auto pNode = getParent(parent);

    return pNode ? pNode->size() : 0;
}

int HierarchicalModel::columnCount(const QModelIndex&) const
{
    return m_colCount;
}

QVariant HierarchicalModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    
    auto ptr = index.internalPointer();
    auto id = index.internalId();

    Node* node = static_cast<Node*>(index.internalPointer());
    if (!node)
    {
		return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return node->name();
    case Qt::DecorationRole:
        //if (node->isEndOfWord()) {
        //    return QIcon(":/icons/word-end.png"); // 可以设置结束标志图标
        //}
        break;
    case Qt::ToolTipRole:
        //if (node->isEndOfWord()) {
        //    return "单词结束节点";
        //}
        break;
    }

    return QVariant();
}

QVariant HierarchicalModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) 
    {
        return section < m_headers.size() ? m_headers[section] : "";
    }
    return QVariant();
}

bool HierarchicalModel::hasChildren(const QModelIndex& parent) const
{
    auto pNode = getParent(parent);

    return pNode && !pNode->empty();
}

void HierarchicalModel::insertWord(const QString& word)
{
    beginResetModel();
    m_trie->insert(word, nullptr);
    endResetModel();
}
//
//void HierarchicalModel::clear()
//{
//    //beginResetModel();
//    //delete m_trie;
//    //m_trie = new Trie();
//    //endResetModel();
//}
//
//QStringList HierarchicalModel::getAllWords() const
//{
//    //auto words = m_trie->getAllWords();
//    return {};// QStringList(words.begin(), words.end());
//}