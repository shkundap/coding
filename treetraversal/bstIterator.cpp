/*
Problem description:
Implement an iterator over a binary search tree (BST). Your iterator will be initialized with the root node of a BST.
Calling next() will return the next smallest number in the BST.
*/
/*
Approach 1: Array based method
- In this method we will use the traditional inorder traversal to get all the BST node values and store it in a vector.
- Next and hasNext function will be then built on top of the populated vector instead of the BST.
- Time complexity of inorder traversal: O(n)
- Time complexity of next() and hasNext(): O(1)
- Space complexity: O(n) for n values stored in vector and O(h) stack space is used for inorder recursion,
here h is height of tree.
*/
class BSTIterator {
private:
    TreeNode *root;
    int index;
    vector<int> sorted_ele;

public:
    BSTIterator(TreeNode* root1):root(root1), index(-1) {
        /* Perform inorder traversal and store sorted values in sorted_ele */
        inorderTraversal(root1);
    }

    void inorderTraversal(TreeNode *node)
    {
        if(node == NULL) return;

        inorderTraversal(node->left);
        sorted_ele.push_back(node->val);
        inorderTraversal(node->right);
    }

    // @return the next smallest number
    // If there is a valid next value then returns the value else returns -1
    int next() {
        if(hasNext())
         return sorted_ele[++index];

        return -1;
    }

    // @return whether we have a next smallest number
    bool hasNext() {
        return (index + 1) < sorted_ele.size();
    }
}; */

/*
Approach 2: controlled recursion
We cannot stop a recursion abruptly inbetween and continue it later. Hence we will replicate
the recursion with the use of local stack. We make use of a function called push left which will push all
the left child of a node on to stack until left leaf node.
- To build next function pop the top of stack this element is the next element. But add the right node of the top element
if it exits to stack. And also add all its left childs with pushLeft function.
- Time complexity of next(): O(h) sometimes but O(1) otherwise
- Time complexity of hasNext: O(1)
- Space complexity: O(h) h is height of tree and utmost we will have these many nodes in stack at any point in time.
*/
class BSTIterator {
private:
    TreeNode *root;
    stack<TreeNode*> s;

public:
    BSTIterator(TreeNode* root1):root(root1) {
        pushLeft(root1);
    }

    // @return the next smallest number
    int next() {
        TreeNode *node = s.top();
        s.pop();

        if(node->right)
            pushLeft(node->right);

        return node->val;
    }

    // @return whether we have a next smallest number
    bool hasNext() {
        return !s.empty();
    }

    void pushLeft(TreeNode* node)
    {
        while(node != NULL)
        {
            s.push(node);
            node = node->left;
        }
    }
};
