#pragma once


#include "Utils/global_defs.h"

#include <QVector>
#include <QChar>
#include <QSharedPointer>

class TrieNode
{
public:
    explicit TrieNode(QChar value = QChar(), TrieNode* parent = nullptr);
    ~TrieNode();

    // 添加子节点
    TrieNode* addChild(QChar character);

    // 获取子节点
    TrieNode* getChild(QChar character) const;

    // 获取所有子节点
    QVector<TrieNode*> getChildren() const;

    // 检查是否有子节点
    bool hasChildren() const;

    // 获取节点值
    QChar getValue() const;

    // 设置/获取结束标志
    void setEndOfWord(bool isEnd);
    bool isEndOfWord() const;

    // 获取父节点
    TrieNode* getParent() const;

    // 获取节点在父节点中的位置
    int getRow() const;

private:
    QChar m_value;
    bool m_isEndOfWord;
    TrieNode* m_parent;
    QVector<TrieNode*> m_children;
};

class Trie
{
public:
    Trie();
    ~Trie();

    // 插入单词
    void insert(const QString& word);

    // 搜索单词
    bool search(const QString& word) const;

    // 获取根节点
    TrieNode* getRoot() const;

    // 获取所有单词
    QVector<QString> getAllWords() const;

    // 获取以指定前缀开头的所有单词
    QVector<QString> getWordsWithPrefix(const QString& prefix) const;

private:
    void collectWords(TrieNode* node, QString currentWord, QVector<QString>& words) const;

    TrieNode* m_root;
};
