#pragma once

#include <QAbstractItemModel>

#include "Utils/QLinkedTrie.hpp"
#include "roaring/roaring.hh"

class TrieModel : public QAbstractItemModel
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
    explicit TrieModel(QObject* parent = nullptr);
    ~TrieModel();

    // QAbstractItemModel 接口实现
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& index) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;

    // 自定义方法
    void insertWord(const QString& word);
    //void clear();
    //QStringList getAllWords() const;

private:
    QLinkedTrie<PData> m_trie;
};