// Consistent Hashing

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include <sstream>
#include <algorithm>
#include <iterator>

// Utility function for generating hash
unsigned int hashFunction(const std::string &key) {
    std::hash<std::string> hasher;
    return hasher(key);
}

class ConsistentHashing {
private:
    std::map<unsigned int, std::string> circle; // A sorted map representing the hash circle
    int numberOfReplicas;

public:
    ConsistentHashing(int replicas) : numberOfReplicas(replicas) {}

    // Add node to the circle
    void addNode(const std::string &node) {
        for (int i = 0; i < numberOfReplicas; ++i) {
            std::stringstream ss;
            ss << node << ":" << i;
            unsigned int hash = hashFunction(ss.str());
            circle[hash] = node;
        }
    }

    // Remove node from the circle
    void removeNode(const std::string &node) {
        for (int i = 0; i < numberOfReplicas; ++i) {
            std::stringstream ss;
            ss << node << ":" << i;
            unsigned int hash = hashFunction(ss.str());
            circle.erase(hash);
        }
    }

    // Get the node that should contain the given key
    std::string getNode(const std::string &key) const {
        if (circle.empty()) {
            return "";
        }

        unsigned int hash = hashFunction(key);
        auto it = circle.lower_bound(hash);

        if (it == circle.end()) {
            return circle.begin()->second;  // If not found, wrap around to the first node
        }

        return it->second;
    }

    // Display the nodes in the hash circle
    void displayNodes() const {
        for (const auto &entry : circle) {
            std::cout << "Hash: " << entry.first << " -> Node: " << entry.second << std::endl;
        }
    }
};

int main() {
    ConsistentHashing ch(3);  // Number of virtual nodes/replicas per physical node

    ch.addNode("Node1");
    ch.addNode("Node2");
    ch.addNode("Node3");

    std::cout << "Initial Circle: " << std::endl;
    ch.displayNodes();

    std::vector<std::string> keys = {"Key1", "Key2", "Key3", "Key4", "Key5"};

    std::cout << "\nKey Distribution: " << std::endl;
    for (const auto &key : keys) {
        std::cout << "Key: " << key << " is mapped to Node: " << ch.getNode(key) << std::endl;
    }

    // Removing a node
    std::cout << "\nRemoving Node2 from the circle...\n";
    ch.removeNode("Node2");
    ch.displayNodes();

    std::cout << "\nKey Distribution after removing Node2: " << std::endl;
    for (const auto &key : keys) {
        std::cout << "Key: " << key << " is mapped to Node: " << ch.getNode(key) << std::endl;
    }

    return 0;
}
