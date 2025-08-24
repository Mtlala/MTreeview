#include "TrieModel.h"

TrieModel::TrieModel(QObject* parent)
    : QAbstractItemModel(parent)
    , m_trie(new Trie())
{
}

TrieModel::~TrieModel()
{
    delete m_trie;
}

QModelIndex TrieModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    TrieNode* parentNode = nullptr;
    if (!parent.isValid()) {
        parentNode = m_trie->getRoot();
    }
    else {
        parentNode = static_cast<TrieNode*>(parent.internalPointer());
    }

    if (!parentNode || row >= parentNode->getChildren().size())
        return QModelIndex();

    TrieNode* childNode = parentNode->getChildren().at(row);
    return createIndex(row, column, childNode);
}

QModelIndex TrieModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
        return QModelIndex();

    TrieNode* childNode = static_cast<TrieNode*>(index.internalPointer());
    TrieNode* parentNode = childNode->getParent();

    if (parentNode == m_trie->getRoot() || !parentNode)
        return QModelIndex();

    return createIndex(parentNode->getRow(), 0, parentNode);
}

int TrieModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
        return 0;

    TrieNode* parentNode = nullptr;
    if (!parent.isValid()) {
        parentNode = m_trie->getRoot();
    }
    else {
        parentNode = static_cast<TrieNode*>(parent.internalPointer());
    }

    return parentNode ? parentNode->getChildren().size() : 0;
}

int TrieModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return 1; // 只有一列显示字符
}

QVariant TrieModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TrieNode* node = static_cast<TrieNode*>(index.internalPointer());
    if (!node)
        return QVariant();

    switch (role) {
    case Qt::DisplayRole:
        return node->getValue();
    case Qt::DecorationRole:
        if (node->isEndOfWord()) {
            return QIcon(":/icons/word-end.png"); // 可以设置结束标志图标
        }
        break;
    case Qt::ToolTipRole:
        if (node->isEndOfWord()) {
            return "单词结束节点";
        }
        break;
    }

    return QVariant();
}

QVariant TrieModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return section == 0 ? "字典树" : QVariant();
    }
    return QVariant();
}

bool TrieModel::hasChildren(const QModelIndex& parent) const
{
    TrieNode* parentNode = nullptr;
    if (!parent.isValid()) {
        parentNode = m_trie->getRoot();
    }
    else {
        parentNode = static_cast<TrieNode*>(parent.internalPointer());
    }

    return parentNode && parentNode->hasChildren();
}

void TrieModel::insertWord(const QString& word)
{
    beginResetModel();
    m_trie->insert(word);
    endResetModel();
}

void TrieModel::clear()
{
    beginResetModel();
    delete m_trie;
    m_trie = new Trie();
    endResetModel();
}

QStringList TrieModel::getAllWords() const
{
    auto words = m_trie->getAllWords();
    return QStringList(words.begin(), words.end());
}