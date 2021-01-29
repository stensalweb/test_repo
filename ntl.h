#ifndef NTL_H
#define NTL_H
#include <stddef.h> // for size_t
#include <stdbool.h>

/*
 * a null terminated list implementation
 *
 * a null terminated list of n elements of type struct E is defined as
 *
 * struct ntl {
 *    void * indices[n+1];  // indices[n] = NULL
 *    struct E e[n];  // sizeof (struct E) == size
 * };
 *
 * the list can be allocated as
 * p = ntl_calloc(n, size);
 *
 *
 *       +-----------------+
 *       |                 |
 * p -> [ | | | | | | |0][e_0]...............[e_(n-1)]
 *      \--indices[n+1]-/ \--------- e[n]-----------/
 *
 * p points to the begin of the memory block which overlaps with indices.
 *
 * p[0] points to e_0
 * p[1] points to e_1
 * ...
 * p[n-1] points to e_(n-1)
 * p[n] points to NULL
 *
 *
 *  for (int i = 0; p[i]; i++)
 *    // do something here for each element
 *
 */
#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/*
 * this is the preferred method to allocate a ntl
 */
void ** ntl_calloc (size_t nelems,  size_t elem_size);

/*
 * unless you have a very good reason, don't use this
 */
void ** ntl_malloc (size_t nelems,  size_t elem_size);

/*
 * the duplicated ntl of elements of elem_size is
 * zero initialized.
 */
void ** ntl_dup (void ** p, size_t elem_size);

/*
 * for each element e, calls free_elem(e)
 * free(p);
 */
void ntl_free(void **p, void (*free_elem)(void *));

size_t ntl_length (void **p);

/*
 * for each element e, calls f(e)
 */
void ntl_apply(void **p, void (*f)(void *p));

typedef void (ntl_converter)(void * from, void * to);
void ** ntl_fmap(void ** from_list, size_t to_elem_size, ntl_converter * f);


/*
 * sn2str(NULL, 0, p) will calculate the size needed to print *p
 * sn2str(buf, n, p) will print to buffer
 */
typedef int (sn2str)(char * str, size_t size, void *p);

struct ntl_str_delimiter {
  char start_delimiter;
  char * element_delimiter;
  char * last_element_delimiter;
  char end_delimiter;
};

int ntl_sn2str(char *buf, size_t buf_size, void **p,
               struct ntl_str_delimiter  * d,
               sn2str * x);

int ntl_as2str(char **buf_ptr, void **p,
               struct ntl_str_delimiter  * d,
               sn2str * x);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //NTL_H
