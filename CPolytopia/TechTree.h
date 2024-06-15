#pragma once
#include <vector>
#include <string>

// Stores the technologies and dependence between them as a technologie tree
struct TechTree {
	struct TechNode {
		std::string m_name;
		bool m_available;
		std::vector<TechNode*> m_children;
		TechNode(std::string name): m_name(name), m_available(false), m_children() {}
		TechNode* addChild(std::string name) {
			m_children.push_back(new TechNode(name));
			return m_children.back();
		}
	};
	TechTree(): m_parentNode("") {}

	TechNode m_parentNode;
};
