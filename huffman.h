/* HuffmanCode - Library to generate n-ary Huffman code
 * Copyright (C) 2017  Jiezhe Wang
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <glib.h>
#include <glib/gprintf.h>

typedef struct _Element Element;

Element * element_new (guint var, gdouble p);

gint compare_element_var (Element *a, Element *b);
gint compare_node_var (const void *a, const void *b, void *args);
gint compare_element_p (Element *a, Element *b);
gint compare_node_p (const void *a, const void *b, void *args);

GQueue * convert (guint n, gdouble P[], guint r);
GNode * huffmantree (guint n, gdouble P[], guint r);

int code_node (GNode *node, gpointer queue);
GQueue * code_tree (GNode *root);

gchar ** huffman (guint n, gdouble P[], guint r);

void print_element (Element *e);
void print_node (GNode *n);
void print_queue (GQueue *q);

#endif  /* __HUFFMAN_H__ */
