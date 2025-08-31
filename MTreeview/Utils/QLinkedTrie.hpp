#pragma once

#include "Utils/global_defs.h"
#include "Utils/QLinkedHash.hpp"

#include <functional>
#include <memory>

#include <QStringList>

template<typename T>
class QLinkedTrieNode 
    : std::enable_shared_from_this<QLinkedTrieNode>
{
    using Node = QLinkedTrieNode;

    using NodePtr      = std::shared_ptr<Node>;
    using NodePtrConst = std::shared_ptr<const Node>;

	using NodeWPtr     = std::weak_ptr<Node>;

public:
    explicit QLinkedTrieNode(
        const QString& name, const NodePtr& parent = nullptr)
		: m_name(name), m_parent(parent) 
    {
        shared_from_this();
    }
    ~QLinkedTrieNode() = default;

    bool empty() const
    { 
        return m_children.empty(); 
    }
    size_t size() const 
    { 
        return m_children.size(); 
    }

    bool isEnd() const
    {
        return empty();
    }

    NodePtr insert(const QString& name)
    {
        if (!m_children.contains(name))
        {
            m_children.append(name, std::make_shared<Node>(name, shared_from_this()));
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

    NodePtr getChild(const QString& name) const
    {
		return m_children.contains(name) ? m_children[name] : nullptr;
    }

    QList<NodePtr> getChildren() const { return m_children.toList(); }

    QString prefix() const
    {
        if (const auto& parent = m_parent.lock())
        {
            return parent->prefix() + m_name;
        }
		return m_name;
    }

    NodePtr parent() const 
    {
        return m_parent.lock(); 
    }

    int getRow() const
    {
        if (const auto& parent = m_parent.lock())
        {
            return parent->m_children.indexOf(m_name);
        }
        return 0;
    }

    void printInfo(QString& info) const
    {
        std::function<QStringList(const NodePtrConst&, int)>
            doPrintInfos = [&](const NodePtrConst& node, int depth)
        {
            QStringList infos;
            for (const auto& child : node->m_children.toList())
            {
                infos.append(QString(depth * 2, ' ') + child->m_name);
                infos.append(doPrintInfos(child, depth + 1));
			}
			return infos;
        };
        info = doPrintInfos(shared_from_this(), 0).join('\n');
    }

	T& data() 
    { 
        return m_data; 
    }

private:
    friend class QLinkedTrie;

    QString m_name;

    NodeWPtr m_parent;
    QLinkedHash<QString, NodePtr> m_children;

    T m_data;
};

template<typename T>
class QLinkedTrie
{
    using Node    = QLinkedTrieNode<T>;
    using NodePtr = QLinkedTrieNode<T>::NodePtr;

public:
    QLinkedTrie(QChar spiliter = '.')
        : m_spiliter(spiliter), m_root(std::make_shared<Node>("")) {}
	~QLinkedTrie() = default;

    void insert(const QString& word, const T& data)
    {
		const auto& parts = word.split(m_spiliter);

        NodePtr cur = m_root;
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
    NodePtr findNode(const QString& prefix) const
    {
        NodePtr curNode = m_root;
        const auto& parts = prefix.split(m_spiliter);

        for (const auto& part : parts)
        {
            curNode = curNode->getChild(part);
            // return if the prefix does not exist
            if (!curNode)
            {
                return nullptr;
            }
        }
        return curNode;
    }
	// find the path of nodes corresponding to the given prefix
    QList<NodePtr> findPath(const QString& prefix) const
    {
        NodePtr curNode = m_root;
        const auto& parts = prefix.split(m_spiliter);

        QList<NodePtr> path;
        for (const auto& part : parts)
        {
            curNode = curNode->getChild(part);
            // return if the prefix does not exist
            if (!curNode)
            {
                return {};
            }

            path.append(curNode);
        }
        return path;
    }

	// get the root node
    NodePtr getRoot() const { return m_root; }

	// traverse all words with the given prefix
    QStringList traverse(const QString& prefix = "") const
    {
		auto startNode = findNode(prefix);

        QStringList words;
        std::function<void(const NodePtr&, const QString&)> dfs =
            [&](const NodePtr& node, const QString& cur)
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

    NodePtr m_root;
};
