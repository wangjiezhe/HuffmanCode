/*
 * Main programming to compute the Huffman code practically.
 *
 * This file is placed in the public domain.
 *
 * Maintainer: Jiezhe Wang
 */

#include "huffman.h"

void test1 ()
{
  guint n = 100;
  gdouble P[n];
  for (guint i = 0; i < n; ++i) {
    P[i] = (i+1) / 100.;
  }

  gchar **res1 = huffman(n, P, 2);
  g_printf("\nCase 1, r = 2:\n");
  for (guint i = 0; i < n; ++i) {
    g_printf("x_%d: %s\n", i+1, res1[i]);
  }
  gchar **res2 = huffman(n, P, 3);
  g_printf("\nCase 1, r = 3:\n");
  for (guint i = 0; i < n; ++i) {
    g_printf("x_%d: %s\n", i+1, res2[i]);
  }
}

void test2 ()
{
  guint n = 5;
  gdouble P[] = {.01, .05, .39, .46, .09};

  gchar **res1 = huffman(n, P, 2);
  g_printf("\nCase 2, r = 2:\n");
  for (guint i = 0; i < n; ++i) {
    g_printf("x_%d: %s\n", i+1, res1[i]);
  }

  gchar **res2 = huffman(n, P, 3);
  g_printf("\nCase 2, r = 3:\n");
  for (guint i = 0; i < n; ++i) {
    g_printf("x_%d: %s\n", i+1, res2[i]);
  }
}

int main(void)
{
  test1();
  test2();
}
