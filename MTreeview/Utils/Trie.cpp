#include "Trie.h"

TrieNode::TrieNode(QChar value, TrieNode* parent)
    : m_value(value), m_isEndOfWord(false), m_parent(parent)
{
}

TrieNode::~TrieNode()
{
    qDeleteAll(m_children);
}

TrieNode* TrieNode::addChild(QChar character)
{
    // 检查是否已存在该字符的子节点
    for (TrieNode* child : m_children) {
        if (child->getValue() == character) {
            return child;
        }
    }

    // 创建新节点
    TrieNode* newNode = new TrieNode(character, this);
    m_children.append(newNode);
    return newNode;
}

TrieNode* TrieNode::getChild(QChar character) const
{
    for (TrieNode* child : m_children) {
        if (child->getValue() == character) {
            return child;
        }
    }
    return nullptr;
}

QVector<TrieNode*> TrieNode::getChildren() const
{
    return m_children;
}

bool TrieNode::hasChildren() const
{
    return !m_children.isEmpty();
}

QChar TrieNode::getValue() const
{
    return m_value;
}

void TrieNode::setEndOfWord(bool isEnd)
{
    m_isEndOfWord = isEnd;
}

bool TrieNode::isEndOfWord() const
{
    return m_isEndOfWord;
}

TrieNode* TrieNode::getParent() const
{
    return m_parent;
}

int TrieNode::getRow() const
{
    if (m_parent) {
        return m_parent->getChildren().indexOf(const_cast<TrieNode*>(this));
    }
    return 0;
}

Trie::Trie()
{
    m_root = new TrieNode(); // 根节点值为空
}

Trie::~Trie()
{
    delete m_root;
}

void Trie::insert(const QString& word)
{
    if (word.isEmpty()) return;

    TrieNode* currentNode = m_root;
    for (const QChar& ch : word) {
        currentNode = currentNode->addChild(ch);
    }
    currentNode->setEndOfWord(true);
}

bool Trie::search(const QString& word) const
{
    if (word.isEmpty()) return false;

    TrieNode* currentNode = m_root;
    for (const QChar& ch : word) {
        currentNode = currentNode->getChild(ch);
        if (!currentNode) return false;
    }
    return currentNode->isEndOfWord();
}

TrieNode* Trie::getRoot() const
{
    return m_root;
}

QVector<QString> Trie::getAllWords() const
{
    QVector<QString> words;
    collectWords(m_root, "", words);
    return words;
}

QVector<QString> Trie::getWordsWithPrefix(const QString& prefix) const
{
    QVector<QString> words;

    // 先找到前缀节点
    TrieNode* currentNode = m_root;
    for (const QChar& ch : prefix) {
        currentNode = currentNode->getChild(ch);
        if (!currentNode) return words; // 前缀不存在
    }

    // 收集所有以该前缀开头的单词
    collectWords(currentNode, prefix, words);
    return words;
}

void Trie::collectWords(TrieNode* node, QString currentWord, QVector<QString>& words) const
{
    if (!node) return;

    if (node->isEndOfWord() && !currentWord.isEmpty()) {
        words.append(currentWord);
    }

    for (TrieNode* child : node->getChildren()) {
        collectWords(child, currentWord + child->getValue(), words);
    }
}