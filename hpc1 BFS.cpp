#include <iostream>
#include <queue>
#include <omp.h>

using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    Node(int value) : data(value), left(nullptr), right(nullptr) {}
};

Node* insertNode(Node* root, int value) {
    if (!root) return new Node(value);
    if (value < root->data) root->left = insertNode(root->left, value);
    else root->right = insertNode(root->right, value);
    return root;
}

void parallelBFS(Node* root) {
    if (!root) return;
    queue<Node*> q;
    q.push(root);
    #pragma omp parallel
    {
        #pragma omp single
        {
            while (!q.empty()) {
                Node* node = q.front(); q.pop();
                cout << node->data << " ";
                #pragma omp task firstprivate(node)
                { if (node->left) q.push(node->left); }
                #pragma omp task firstprivate(node)
                { if (node->right) q.push(node->right); }
            }
        }
    }
}

void parallelDFS(Node* root) {
    if (!root) return;
    #pragma omp parallel sections
    {
        #pragma omp section
        { parallelDFS(root->left); }
        #pragma omp section
        { parallelDFS(root->right); }
    }
    cout << root->data << " ";
}

int main() {
    Node* root = nullptr;
    int n, value;
    cout << "Enter number of nodes and their values: ";
    cin >> n;
    while (n-- > 0 && cin >> value) root = insertNode(root, value);
    cout << "Parallel BFS: ";
    parallelBFS(root);
    cout << "\nParallel DFS: ";
    parallelDFS(root);
    cout << endl;
    return 0;
}


