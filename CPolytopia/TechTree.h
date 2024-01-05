#pragma once
#include <vector>
#include <string>

class TechTree {
public:
	class TechNode {
		std::string m_name;
		bool m_available;
		std::vector<TechNode*> m_children;
	public:
		TechNode(std::string name): m_name(name), m_available(false), m_children() {}

		std::vector<TechNode*>& getChildren() {
			return m_children;
		}
		const std::vector<TechNode*>& getChildren() const {
			return m_children;
		}
		int nbChildren() const {
			return m_children.size();
		}
		void discover() {
			m_available = true;
		}
		TechNode* addChild(std::string name) {
			m_children.push_back(new TechNode(name));
			return m_children.back();
		}
	};
	TechTree(): m_parentNode("") {}

	size_t nbBranches() const {
		return m_parentNode.nbChildren();
	}
	TechNode * getBranch(int i) {
		return m_parentNode.getChildren()[i];
	}
	const TechNode * getBranch(int i) const {
		return m_parentNode.getChildren()[i];
	}
	const TechNode* getTree() const {
		return &m_parentNode;
	}
	TechNode* getTree() {
		return &m_parentNode;
	}
private:
	TechNode m_parentNode;
};
