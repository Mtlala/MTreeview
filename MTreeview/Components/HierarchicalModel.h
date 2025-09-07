#pragma once

#include <QAbstractItemModel>

#include "Utils/QLinkedTrie.hpp"
#include "roaring/roaring.hh"

class HierarchicalModel : public QAbstractItemModel
{
private:
    using RBitmap = roaring::Roaring;

    struct Data
    {
        int type;
        RBitmap bis;
    };
	using PData = std::shared_ptr<Data>;
    using Node  = QLinkedTrieNode<PData>;

public:
    explicit HierarchicalModel(QObject* parent = nullptr);
    ~HierarchicalModel();

    void setHeader(const QStringList& headers);
	void setColumnCount(int count);

    // QAbstractItemModel 接口实现
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    virtual QModelIndex parent(const QModelIndex& index) const override;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;

    // 自定义方法
    void insertWord(const QString& word);
    //void clear();
    //QStringList getAllWords() const;

private:
    inline Node* getParent(const QModelIndex& parent) const
    {
		return parent.isValid() ? static_cast<Node*>(parent.internalPointer()) : m_trie->getRoot();
    }

private:
    QLinkedTrie<PData>* m_trie;

	QStringList m_headers;
	int m_colCount;
};