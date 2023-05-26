#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>
using namespace std;

// Node structure for the tree
struct Node
{
  int data;
  vector<Node *> children;
};


void DFS(Node *root)
{
  if (root == nullptr)
    return;

  std::cout << root->data << std::endl;
  
  for (Node *child : root->children)
  {
    DFS(child);
  }
}


// Depth-First Search using OpenMP
void DFS_Parallel(Node *root)
{
  // Check if the root is NULL
  if (root == nullptr)
    return;

  // Create a stack for DFS traversal
  stack<Node *> stack;
  stack.push(root);

// Perform DFS using OpenMP
#pragma omp parallel
  {
    // Perform DFS until the shared stack is empty
    while (!stack.empty())
    {
      // Get the top node from the shared stack
      Node *current = nullptr;

      #pragma omp critical
      {
        if (!stack.empty())
        {
          current = stack.top();
          stack.pop();
        }
      }

      // Exit the loop if the shared stack is empty
      if (current == nullptr)
        break;

      // Process the current node
      cout << current->data << "\n";

      // Add the child nodes to the shared stack
      #pragma omp critical
      {
        for (Node *child : current->children)
        {
          stack.push(child);
        }
      }
    }
  }
}






// Function to build the tree dynamically
Node *buildTree()
{
  int value;
  cout << "Enter the value of the node (or -1 to stop): ";
  cin >> value;

  if (value == -1)
    return nullptr;

  int numChildren;
  cout << "Enter the number of children for the node: ";
  cin >> numChildren;

  Node *node = new Node;
  node->data = value;

  for (int i = 0; i < numChildren; ++i)
  {
    cout << "Enter the child " << i + 1 << " of " << value << ":\n";
    node->children.push_back(buildTree());
  }

  return node;
}

int main()
{
  omp_set_num_threads(4);

  // Build the tree dynamically
  cout << "Enter the tree structure:\n";
  Node *root = buildTree();

  // Perform DFS using OpenMP
  cout<<"\n\n\nDFS w/o Parallelization: "<<endl;
  DFS(root);

  cout<<"\n\n\nDFS w Parallelization: "<<endl;
  DFS_Parallel(root);

  return 0;
}
