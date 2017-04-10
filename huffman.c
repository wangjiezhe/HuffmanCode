/* HuffmanCode - Library to generate n-ary Huffman code
 * Copyright (C) 2017  Jiezhe Wang
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "huffman.h"

/**
 * Element:
 * @var: the label of the element, positive for `real` Element, zero for generated
 * @p: the weight of the element
 * @code: the Huffman code for the element
 *
 * Contain the fields of a #Element.
 */
struct _Element {
    guint var;
    gdouble p;
    GString *code;
};

/**
 * print_element:
 * @e: a #Element
 *
 * Print the content of element @e.
 */
void print_element(Element *e)
{
  g_printf("Element %d: %.2f, %s\n", e->var, e->p, e->code->str);
}

/**
 * print_node:
 * @n: a #GNode
 *
 * Print the data of node @n.
 */
void print_node(GNode *n)
{
  Element *e = n->data;
  g_printf("Node: ");
  print_element(e);
}

/**
 * print_queue:
 * @q: a #GQueue
 *
 * Print the length of queue @q, and all elements in it.
 */
void print_queue(GQueue *q)
{
  g_printf("\nQueue length: %d\n", q->length);
  GList *l = q->head;
  while (l != NULL) {
    GNode *n = l->data;
    print_node(n);
    l = l->next;
  }
}

/**
 * element_new:
 * @var: the label of the element
 * @p: the weight of the element
 *
 * Create a new #Element.
 *
 * Returns: a newly allocated #Element
 */
Element * element_new (guint var, gdouble p)
{
  Element * t = g_malloc(sizeof(Element));
  t->var = var;
  t->p = p;
  t->code = g_string_new(NULL);
  return t;
}

/**
 * compare_element_var:
 * @a: a #Element
 * @b: a #Element
 *
 * Compare two elements with Element->var.
 *
 * Returns: negative value if @a < @b ; zero if @a = @b ; positive value if @a > @b
 */
gint compare_element_var (Element *a, Element *b)
{
  if (a->var < b->var)
    return -1;
  else if (a->var == b->var)
    return 0;
  else
    return 1;
}

/**
 * compare_node_var:
 * @a: a #GNode
 * @b: a #GNode
 *
 * A #GCompareDataFunc.
 * Compare Node with Node->data->var.
 *
 * Returns: negative value if @a < @b ; zero if @a = @b ; positive value if @a > @b
 */
gint compare_node_var (const void *a, const void *b, void *args)
{
  (void) args;
  GNode * node_a = (GNode *) a;
  GNode * node_b = (GNode *) b;
  return compare_element_var(node_a->data, node_b->data);
}

/**
 * compare_element_p:
 * @a: a #Element
 * @b: a #Element
 *
 * Compare two elements with Element->p.
 *
 * Returns: negative value if @a < @b ; zero if @a = @b ; positive value if @a > @b
 */
gint compare_element_p (Element *a, Element *b)
{
  if (a->p < b->p)
    return -1;
  else if (a->p == b->p)
    return 0;
  else
    return 1;
}

/**
 * compare_element_p:
 * @a: a #GNode
 * @b: a #GNode
 *
 * A #GCompareDataFunc.
 * Compare Node with Element->p.
 *
 * Returns: negative value if @a < @b ; zero if @a = @b ; positive value if @a > @b
 */
gint compare_node_p (const void *a, const void *b, void *args)
{
  (void) args;
  GNode * node_a = (GNode *) a;
  GNode * node_b = (GNode *) b;
  return compare_element_p(node_a->data, node_b->data);
}

/**
 * convert:
 * @n: the number of source words
 * @P: an array of weight of corresponding word
 * @r: the size of alphabet
 *
 * Generate the queue to use in #huffmantree.
 *
 * Returns: A priority queue of #GNode
 */
GQueue * convert (guint n, gdouble P[], guint r)
{
  /*
   * Make sure the number of source words is congruent to 1 module r-1.
   * If not, complete it.
   */
  guint k;
  if (r > 2) {
    guint t = n % (r-1);
    k = (t > 1) ? (r - t) : (1 - t);
  } else {
    k = 0;
  }

  GQueue *newP = g_queue_new();
  for (guint i = 0; i < n; ++i) {
    g_queue_push_tail(newP, g_node_new(element_new(i+1, P[i])));
  }

  /* Use sort to get a priority queue */
  g_queue_sort(newP, compare_node_p, NULL);

  for (guint i = 0; i < k; ++i) {
    g_queue_push_head(newP, g_node_new(element_new(0, 0)));
  }

  return newP;
}

/**
 * huffmantree:
 * @n: the number of source words
 * @P: an array of weight of corresponding word
 * @r: the size of alphabet
 *
 * Generate an @r-ary Huffman tree.
 *
 * Returns: the root the Huffman tree
 */
GNode * huffmantree (guint n, gdouble P[], guint r)
{
  GQueue *newP = convert(n, P, r);

  while (newP->length != 1) {
    Element *newElement = element_new(0, 0);
    GNode *newNode = g_node_new(newElement);
    for (guint i = 0; i < r; ++i) {
      GNode *tempNode = g_queue_pop_head(newP);
      Element *tempElement =  tempNode->data;
      g_node_append(newNode, tempNode);
      newElement->p += tempElement->p;
    }
    g_queue_insert_sorted(newP, newNode, compare_node_p, NULL);
  }

  return g_queue_pop_head(newP);
}

/**
 * code_node:
 * @node: the #GNode to add code information
 * @queue: a queue to store `real` elements
 *
 * Add the Huffman code to the node, and save it in @queue if it
 * contains a `real` element. Return false to continue the traverse.
 *
 * Returns: FALSE
 */
gboolean code_node (GNode *node, gpointer queue)
{
  GQueue *realLeafQueue = (GQueue *) queue;
  Element *data = node->data;
  if (!G_NODE_IS_ROOT(node)) {
    Element *parentData = node->parent->data;
    g_string_assign(data->code, parentData->code->str);
    gint posi = g_node_child_position(node->parent, node);
    gchar pos = (gchar)('0' + posi);
    g_string_append_c(data->code, pos);
    if (data->var > 0) {
      g_queue_push_tail(realLeafQueue, node);
    }
  }
  return FALSE;
}

/**
 * code_tree:
 * @root: the root #Gnode of the Huffman tree to add code
 *
 * Add code information to all nodes of the Huffman tree using bread-first traversal.
 *
 * Returns: A queue of leaves which contain `real` element
 */
GQueue * code_tree (GNode *root)
{
  GQueue *realLeafQueue = g_queue_new();
  g_node_traverse(root, G_LEVEL_ORDER, G_TRAVERSE_ALL, -1, code_node, realLeafQueue);
  g_queue_sort(realLeafQueue, compare_node_var, NULL);
  return realLeafQueue;
}

/**
 * huffman:
 * @n: the number of source words
 * @P: an array of weight of corresponding word
 * @r: the size of alphabet
 *
 * Compute the r-ary Huffman code with given number of words with weights
 *
 * Returns: A corresponding list of Huffman codes
 */
gchar ** huffman (guint n, gdouble P[], guint r)
{
  GNode *root = huffmantree(n, P, r);
  guint d = g_node_depth(root);
  GQueue *leaves = code_tree(root);
  gchar **codes = g_malloc(sizeof(gchar*)*n + sizeof(char)*d*n);
  for (guint i = 0; i < n; ++i) {
    GNode *tempNode = g_queue_pop_head(leaves);
    Element *tempElement = tempNode->data;
    codes[i] = tempElement->code->str;
  }
  /* g_node_destroy(root); */
  return codes;
}
