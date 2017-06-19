#ifndef REGULAR_EXPRESSION_H
#define REGULAR_EXPRESSION_H
#include "all.h"
#include "node.h"
#include "finite_automata.h"

/*!
 * Class used to represent a Regular Expression
 */
class RegularExpression {
  public:
    /*!
     * Constructs a new Regular Expresion based on a string
     *
     * @param  re The regular expression to use in the creation of the string
     */
    RegularExpression(string re);

    /*!
     * Get the regular expression constructed
     *
     * @return The regular expression string defined in the constructor of
     * the class
     */
    string getRegularExpression();

    /*!
     * Computes the De Simone tree related to this regular expression
     *
     * @return The root node of the De Simone tree related to this regular
     * expression
     */
    Node* getTree();

    /*!
     * Returns a deterministic (and possibly minimal) finite automata, based
     * on the De Simone tree related to this regular expression
     *
     * @see RegularExpression::getTree
     *
     * @return The deterministic (and possibly minimal) finite automata related
     * to this regular expression
     */
    FiniteAutomata getAutomata();


    /*!
     * Check if a character is a terminal
     *
     * @param  c The character to check if it is a terminal
     * @return   true if the character is a terminal, false otherwise
     */
    static bool isTerminal(char c);

  private:
    /*!
     * Compute the position of the char with the less priority (according to De
     * Simone rules) in the regular expression passed
     *
     * @param  re The regular expression to analyze
     * @return    The position of the character with less priority in the
     * regular expression passed
     */
    int getLessPriority(string re);

    /*!
     * Normalize the regular expression so we can identify concatenations easily
     *
     * @return The normalized regular expression
     */
    string normalize();

    /*!
     * Constructs and return the appropriate Node object for a specific
     * character
     *
     * @param  c    The character to create a node for
     * @param  root The root of the node
     * @return      The appropriate Node object for that specific character
     */
    Node* getNode(char c, Node *root);

    /*!
     * Compute and return the appropriate tree for a regular expression
     *
     * @param  re   The regular expression to process
     * @param  root The root of the node of the tree
     * @return      The tree related to that regular expression
     */
    Node* getTree(string re, Node *root);

    /*!
     * Return a mapping from Node (a leaf) to a set of nodes that are reachable
     * ascending from that leaf based on a tree.
     *
     * @param  tree The tree to use to create the mapping
     * @return      A mapping from Node (a leaf) to a set of nodes that are
     * reachable ascending from that leaf
     */
    map<Node*, set<Node*>> getCompositionPerLeaf(Node* tree);

    /*!
     * Get the composition for the first state (in other words, the nodes that
     * are reachable descending from the root node).
     *
     * @param  tree The tree to use to compute the composition of the first node
     * @return      The composition of the first node
     */
    set<Node*> getFirstComposition(Node* tree);

    /*!
     * The alphabet of the Finite Automata based on the composition per leaf
     * table
     *
     * @param  leaves_comp The composition per leaf node
     * @return             The set of characters that represent the alphabet of
     *                     the Finite Automata
     */
    set<char> getAlphabet(map<Node*, set<Node*>> leaves_comp);

    /*!
     * Return the order (according to De Simone rules) for a specific character
     *
     * @param  c The character to compute the order
     * @return   The order for the specified character
     */
    int getOrder(char c);

    /*!
     * Check if a character is a multiplier
     *
     * @param  c The character to check if it is a multiplier
     * @return   true if the character is a multiplier, false otherwise
     */
    static bool isMultiplier(char c);

    /*!
     * Check if there should be a concatenation between a character and another
     *
     * @param  prev The character before the possible concatenation
     * @param  c    The character after the possible concatenation
     * @return      true if a concatenation should be between these two
     *              characters, false otherwise
     */
    bool shouldConcatenate(char prev, char c);

    /*!
     * Check if there is Lambda node in the composition specified
     *
     * @param  composition The composition to check
     * @return             true if there is lambda node in that composition,
     *                     false otherwise
     */
    bool hasLambda(set<Node*> composition);

    /*!
     * Given a string with multipliers, return the appropriate multiplier for
     * that string.
     *
     * @param  multipliers A string containing only multipliers
     * @return             A character with the appropriate multiplier for that
     *                     string
     */
    char fixMultipliers(string multipliers);

    /*!
     * Compute the leaves (may include lambda node, too) reachable in the tree
     * given a list of NodeAction to process, while avoiding loops in the
     * processing.
     *
     * @param  to_process The list of NodeAction to process
     * @return The list of leaves (or lambda, if reachable) reachables while
     *         walking on the tree
     */
    set<Node*> getLeaves(list<NodeAction> to_process);

    string regex; //!< The regular expression specified by the user
};

#endif  // REGULAR_EXPRESSION_H
