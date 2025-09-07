#pragma once

#include "Utils/global_defs.h"
#include "Utils/QLinkedHash.hpp"

#include <functional>

#include <QStringList>

template <typename T> class QLinkedTrie;

template <typename T>
class QLinkedTrieNode 
{
    using Node  = QLinkedTrieNode;

public:
    explicit QLinkedTrieNode(
        const QString& name, Node* parent = nullptr)
		: m_name(name), m_parent(parent) {}
    ~QLinkedTrieNode()
    {
        for (auto& child : m_children)
        {
            delete child;
			child = nullptr;
		}
		m_children.clear();
    }

    size_t size() const 
    { 
        return m_children.size(); 
	}
    bool empty() const
    { 
        return m_children.empty(); 
    }

    bool isEnd() const
    {
        return empty();
    }

    Node* insert(const QString& name)
    {
        if (!m_children.contains(name))
        {
            m_children.append(name, new Node(name, this));
        }
        return m_children[name];
    }
    void remove(const QString& name) 
    {
        m_children.remove(name); 
    }
    bool contains(const QString& name) const 
    { 
        return m_children.contains(name); 
    }

    Node* getChild(int idx) const
    {
        return idx >= 0 && idx < size() ? m_children[idx] : nullptr;
    }

    Node* getChild(const QString& name) const
    {
		return m_children.contains(name) ? m_children[name] : nullptr;
    }

    QList<Node*> getChildren() const { return m_children.toList(); }

    QString prefix() const
    {
        if (m_parent)
        {
            return m_parent->prefix() + m_name;
        }
		return m_name;
    }

    Node* parent() const
    {
        return m_parent; 
    }

    int getRow() const
    {
        if (m_parent)
        {
            return m_parent->m_children.indexOf(m_name);
        }
        return 0;
    }

    void printInfo(QString& info) const
    {
        std::function<QStringList(const Node*, int)>
            doPrintInfos = [&](const Node* node, int depth)
        {
            QStringList infos;
            for (const auto& child : node->m_children.toList())
            {
                infos.append(QString(depth * 2, ' ') + child->m_name);
                infos.append(doPrintInfos(child, depth + 1));
			}
			return infos;
        };
        info = doPrintInfos(this, 0).join('\n');
    }

	T& data() { return m_data; }

private:
    template <typename T> friend class QLinkedTrie;

    QString m_name;

    Node* m_parent;
    QLinkedHash<QString, Node*> m_children;

	T m_data;
};

template <typename T>
class QLinkedTrie
{
    using Node = QLinkedTrieNode<T>;

public:
    QLinkedTrie(QChar spiliter = '.')
        : m_spiliter(spiliter), m_root(new Node("")) {}
    ~QLinkedTrie() 
    { 
        delete m_root;
        m_root = nullptr;
    }

    void insert(const QString& word, const T& data)
    {
		const auto& parts = word.split(m_spiliter);

        Node* cur = m_root;
        for (const auto& part : parts)
        {
            cur = cur->insert(part);
        }
		cur->data() = data;
    }
    void remove(const QString& prefix)
    {
        // Reord the path of nodes
        const auto& path = findPath(prefix);

        // delete nodes
        for (auto it = path.rbegin(); it != path.rend(); ++it)
        {
			auto node = *it;
            if (node->empty())
            {
                if (auto parent = node->parent())
                {
                    parent->remove(node->m_name);
                }
            }
			// Stop if the node is not empty
            else
            {
                break;
            }
		}
    }

	// search for a whole word in the trie
    bool search(const QString& word) const
    { 
        return [node = findNode(word)] { return node && node->isEnd(); }(); 
    }
	// find if there is any word in the trie that starts with the given prefix
    bool startsWith(const QString& prefix) const 
    { 
        return findNode(prefix) != nullptr; 
    }
   
	// find the node corresponding to the given prefix
    Node* findNode(const QString& prefix) const
    {
        Node* cur = m_root;
        const auto& parts = prefix.split(m_spiliter);

        for (const auto& part : parts)
        {
            cur = cur->getChild(part);
            // return if the prefix does not exist
            if (!cur)
            {
                return nullptr;
            }
        }
        return cur;
    }
	// find the path of nodes corresponding to the given prefix
    QList<Node*> findPath(const QString& prefix) const
    {
        Node* cur = m_root;
        const auto& parts = prefix.split(m_spiliter);

        QList<Node*> path;
        for (const auto& part : parts)
        {
            cur = cur->getChild(part);
            // return if the prefix does not exist
            if (!cur)
            {
                return {};
            }

            path.append(cur);
        }
        return path;
    }

	// get the root node
    Node* getRoot() const { return m_root; }

	// traverse all words with the given prefix
    QStringList traverse(const QString& prefix = "") const
    {
		auto startNode = findNode(prefix);

        QStringList words;
        std::function<void(const Node*, const QString&)> dfs =
            [&](const Node* node, const QString& cur)
        {
            if (node->isEnd() && !cur.isEmpty())
            {
                words.append(cur);
            }
            for (const auto& child : node->getChildren())
            {
                dfs(child, cur.isEmpty() ? child->m_name : (cur + m_spiliter + child->m_name));
            }
        };

        dfs(startNode, prefix);

		return words;
    }

	// for debug
    void printInfo(QString& info) const
    {
        return m_root->printInfo(info);
	}

private:
    QChar m_spiliter;

    Node* m_root;
};
