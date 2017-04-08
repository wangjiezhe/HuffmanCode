#include "huffman.h"

void print_element(Element *e)
{
  g_printf("Element %d: %.2f, %s\n", e->var, e->p, e->code->str);
}

void print_node(GNode *n)
{
  Element *e = n->data;
  print_element(e);
}

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

Element * element_new (gint var, gdouble p)
{
  Element * t = g_malloc(sizeof(Element));
  t->var = var;
  t->p = p;
  t->code = g_string_new(NULL);
  return t;
}

gint compare_element_var (Element *a, Element *b)
{
  if (a->var < b->var)
    return -1;
  else if (a->var == b->var)
    return 0;
  else
    return 1;
}

gint compare_node_var (const void *a, const void *b, void *args)
{
  (void) args;
  GNode * node_a = (GNode *) a;
  GNode * node_b = (GNode *) b;
  return compare_element_var(node_a->data, node_b->data);
}

gint compare_element_p (Element *a, Element *b)
{
  if (a->p < b->p)
    return -1;
  else if (a->p == b->p)
    return 0;
  else
    return 1;
}

gint compare_node_p (const void *a, const void *b, void *args)
{
  (void) args;
  GNode * node_a = (GNode *) a;
  GNode * node_b = (GNode *) b;
  return compare_element_p(node_a->data, node_b->data);
}

GQueue * convert (guint n, gdouble P[], guint r)
{
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
  for (guint i = 0; i < k; ++i) {
    g_queue_push_head(newP, g_node_new(element_new(n+i+1, 0)));
  }

  g_queue_sort(newP, compare_node_p, NULL);

  return newP;
}

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

gboolean code_node (GNode *node, gpointer args)
{
  GQueue *leafQueue = (GQueue *) args;
  Element *data = node->data;
  if (!G_NODE_IS_ROOT(node)) {
    Element *parentData = node->parent->data;
    g_string_assign(data->code, parentData->code->str);
    gint posi = g_node_child_position(node->parent, node);
    gchar pos = (gchar)('0' + posi);
    g_string_append_c(data->code, pos);
    if (G_NODE_IS_LEAF(node)) {
      g_queue_push_tail(leafQueue, node);
    }
  }
  return FALSE;
}

GQueue * code_tree (GNode *root)
{
  GQueue *leafQueue = g_queue_new();
  g_node_traverse(root, G_LEVEL_ORDER, G_TRAVERSE_ALL, -1, code_node, leafQueue);
  g_queue_sort(leafQueue, compare_node_var, NULL);
  return leafQueue;
}

gchar ** huffman (guint n, gdouble P[], guint r)
{
  GNode *root = huffmantree(n, P, r);
  guint d = g_node_depth(root);
  GQueue *leaves = code_tree(root);
  /* gchar **codes = g_malloc(sizeof(gchar*)*n); */
  gchar **codes = g_malloc(sizeof(gchar*)*n + sizeof(char)*d*n);
  for (guint i = 0; i < n; ++i) {
    GNode *tempNode = g_queue_pop_head(leaves);
    Element *tempElement = tempNode->data;
    /* codes[i] = g_malloc(sizeof(char)*d); */
    codes[i] = tempElement->code->str;
  }
  /* g_node_destroy(root); */
  return codes;
}
