#include <glib.h>
#include <glib/gprintf.h>

typedef struct _Element Element;
struct _Element {
  guint var;
  gdouble p;
  GString *code;
};

Element * element_new (gint var, gdouble p);

gint compare_element_var (Element *a, Element *b);
gint compare_node_var (const void *a, const void *b, void *args);
gint compare_element_p (Element *a, Element *b);
gint compare_node_p (const void *a, const void *b, void *args);

GQueue * convert (guint n, gdouble P[], guint r);
GNode * huffmantree (guint n, gdouble P[], guint r);

int code_node (GNode *node, gpointer args);
GQueue * code_tree (GNode *root);

gchar ** huffman (guint n, gdouble P[], guint r);

void print_element (Element *e);
void print_node (GNode *n);
void print_queue (GQueue *q);
