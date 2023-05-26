#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

// Node structure for the tree
struct Node
{
  int data;
  std::vector<Node *> children;
};

// Breadth-First Search
void BFS(Node *root)
{
  if (root == nullptr)
    return;

  std::queue<Node *> queue;
  queue.push(root);

  while (!queue.empty())
  {
    Node *current = queue.front();
    queue.pop();

    std::cout << current->data << "\n";

    for (Node *child : current->children)
    {
      queue.push(child);
    }
  }
}

// Parallel Breadth-First Search
void BFS_Parallel(Node *root)
{
  if (root == nullptr)
    return;

  std::queue<Node *> queue;
  queue.push(root);

  #pragma omp parallel
  {
    while (!queue.empty())
    {
      Node *current = nullptr;

      #pragma omp critical
      {
        if (!queue.empty())
        {
          current = queue.front();
          queue.pop();
        }
      }

      if (current == nullptr)
        break;

      std::cout << current->data << "\n";

      #pragma omp for
      for (int i = 0; i < current->children.size(); ++i)
      {
        #pragma omp critical
        {
          queue.push(current->children[i]);
        }
      }
    }
  }
}

// Function to build the tree dynamically
Node *buildTree()
{
  int value;
  std::cout << "Enter the value of the node (or -1 to stop): ";
  std::cin >> value;

  if (value == -1)
    return nullptr;

  int numChildren;
  std::cout << "Enter the number of children for the node: ";
  std::cin >> numChildren;

  Node *node = new Node;
  node->data = value;

  for (int i = 0; i < numChildren; ++i)
  {
    std::cout << "Enter the child " << i + 1 << " of " << value << ":\n";
    node->children.push_back(buildTree());
  }

  return node;
}

int main()
{

  omp_set_num_threads(4);

  // Build the tree dynamically
  std::cout << "Enter the tree structure:\n";
  Node *root = buildTree();

  // Perform BFS
  std::cout << "\nBFS Traversal:\n";
  BFS(root);

  // Perform Parallel BFS
  std::cout << "\nParallel BFS Traversal:\n";
  BFS_Parallel(root);

  return 0;
}
