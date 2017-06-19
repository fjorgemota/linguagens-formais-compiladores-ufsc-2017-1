#ifndef NODE_H
#define NODE_H

#include "all.h"

using namespace std;

/*!
 * Enum values corresponding to the direction of a routine during the
 * computation of a de Simone operation
 */
enum Direction {
    up, /*!< Direction corresponding to the ascend routine. */
    down /*!< Direction corresponding to the descend routine */
};

/*!
 * Defines the types of the nodes in a tree associated to a regular expression
 */
enum NodeType {
    DOT, /*!< Node of the type dot (.) */
    LAMBDA,/*!< Node of the type lambda */
    LEAF, /*!< Node of the type leaf */
    PLUS, /*!< Node of the type plus (+)*/
    QUESTION, /*!< Node of the type question mark (?) */
    STAR, /*!< Node of the type star (*) */
    UNION /*!< Node of the type union (|) */
};

// There is a symbol called Node, just name it here..
class Node;

/*!
 * Class related to the actions from ascend and descend routines of a tree
 * associated to a regular expression
 */
class NodeAction {
  public:
    /*!
     * Constructs a NodeAction object with specific node and direction
     *
     * @param n The node related to the action
     * @param d The direction that the node n will follow
     */
    NodeAction(Node* n, Direction d);

    /*!
     * Get the node related to this action
     */
    Node *getNode();

    /*!
     * Get the direction related to this action
     */
    Direction getDirection();

    /*!
     * Operator for the comparison between two NodeAction objects,
     * this is used for equivalence between two NodeActions objects
     * in a set.
     *
     * @param act The NodeAction to compare
     */
    bool operator<(const NodeAction &act) const;

  private:
    Direction direction; //!< The direction related to ascend or descend routine
    Node *node; //!< The node related to this action
};

/*!
 * Class that represents a node of a tree related to a regular expression. That
 * tree is used to compute the De Simone method
 */
class Node {
  public:
    /*!
     * Constructs a node with your specified value.
     *
     * @param v     The value associated to the node
     * @param root  The father of the node (by a direct relationship)
     */
    Node(char v, Node *root);

    /*!
     * Get the left son of the node
     */
    Node* getLeft();

    /*!
     * Get the right son of the node
     */
    Node* getRight();

    /*!
     * Get the father of the node (by a thread)
     */
    Node* getParent();

    /*!
     * Get the value associated with the node
     */
    char getValue();

    /*!
     * Set the right son of the node
     *
     * @param n Node to be the rightmost son
     */
    void setRight(Node* n);

    /*!
     * Set the left son of the node
     *
     * @param n Node to be the leftmost son
     */
    void setLeft(Node* n);

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node
     */
    virtual list<NodeAction> descend() = 0;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node
     */
    virtual list<NodeAction> ascend() = 0;

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    virtual NodeType getType() = 0;

  private:
    Node *left; //!< The leftmost son of the node
    Node *right; //!< The rightmost son of the node
    Node *root; //!< The father of the node (by a thread)
    char value; //!< The value associated with the node
};

/*!
 * Class related to the dot type node
 */
class DotNode : public Node {
  public:
    using Node::Node;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node.
     * The ascend routine of the dot type node is descend for the rightmost son
     */
    list<NodeAction> ascend();

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node.
     * The descend routine of the dot type node is descend for the leftmost son
     */
    list<NodeAction> descend();

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    NodeType getType();
};

/*!
 * Class related to the lambda type node
 */
class LambdaNode : public Node {
  public:
      using Node::Node;

      /*!
       * Get the list of NodeAction resulted by the ascend routine of the node.
       * Lambda type node do not have an ascend routine
       */
      list<NodeAction> ascend();

      /*!
       * Get the list of NodeAction resulted by the descent routine of the node.
       * Lambda type node do not have a descent routine
       */
      list<NodeAction> descend();

      /*!
       * Get the type of the node, defined by the enum NodeType
       */
      NodeType getType();
};

/*!
 * Class related to the leaf type node
 */
class LeafNode : public Node {
  public:
    using Node::Node;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node.
     * The ascend routine of a leaf type node is move up by the thread.
     */
    list<NodeAction> ascend();

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node.
     * Leaf type node do not have a descent routine
     */
    list<NodeAction> descend();

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    NodeType getType();
};

/*!
 * Class related to the plus type node
 */
class PlusNode : public Node {
  public:
    using Node::Node;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node.
     * The ascend routine of a plus type node is move up by the thread and and
     * descend by the leftmost son
     */
    list<NodeAction> ascend();

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node.
     * The descent routine of a plus type node is descend by the leftmost son
     */
    list<NodeAction> descend();

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    NodeType getType();
};

/*!
 * Class related to the question mark type node
 */
class QuestionMarkNode : public Node {
  public:
    using Node::Node;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node.
     * The ascend routine of a question mark type node is move up by the thread
     */
    list<NodeAction> ascend();

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node.
     * The descent routine of a question mark type node is move up by the
     * thread and and descend by the leftmost son
     */
    list<NodeAction> descend();

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    NodeType getType();
};

/*!
 * Class related to the star type node
 */
class StarNode : public Node {
  public:
    using Node::Node;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node.
     * The ascend routine of a star type node is move up by the thread and and
     * descend by the leftmost son
     */
    list<NodeAction> ascend();

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node.
     * The descent routine of a star type node is move up by the thread and and
     * descend by the leftmost son
     */
    list<NodeAction> descend();

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    NodeType getType();
};

/*!
 * Class related to the union type node
 */
class UnionNode : public Node {
  public:
    using Node::Node;

    /*!
     * Get the list of NodeAction resulted by the ascend routine of the node.
     * The ascend routine of a union type node is descend by the rightmost sons
     * until the end (where the node do not have rightmost son) and move up by
     * the thread
     */
    list<NodeAction> ascend();

    /*!
     * Get the list of NodeAction resulted by the descent routine of the node.
     * The descent routine of a union type node is descend by the leftmost son
     * and descend by the rightmost son
     */
    list<NodeAction> descend();

    /*!
     * Get the type of the node, defined by the enum NodeType
     */
    NodeType getType();
};
#endif
