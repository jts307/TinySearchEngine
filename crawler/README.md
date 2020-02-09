# CS50 Lab 3
## Jacob Werzinsky, CS50 Winter 2020

### hashtable

A *hashtable* is an unordered collection of (key,item) pairs where any given key can only occur once in the hashtable. It 
starts out as a series of empty slots until (key,item) pairs are inserted into it one by one. The determination of where a 
pair goes depends upon a hash function which gives a numerical indicator of which slot a pair should go in. This hash function 
inserts pairs in such a way that all slots have a close to equal amount of items. Pairs in the set cannot be removed or 
updated, but items can be retrieved by asking for its related key. The only distinguishing factor of items are their keys. 

Note: This *hashtable* module makes use of the *set* module defined in [set.h](../set/set.h) and implemented in [set.c]
(../set/set.c). Take a look at its [README](../set/README.md) for more info.

### Usage

The *hashtable* module, defined in `hashtable.h` and implemented in `hashtable.c`, implements a hashtable of `set_t` sets 
where each set contains (`char*` key,`void*` item) pairs, and exports the following functions through `hashtable.h`:

```c
/**************** hashtable_new ****************/
/* Create a new (empty) hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
hashtable_t *hashtable_new(const int num_slots);

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true iff new item was inserted.
 * Notes:
 *   The key string is copied for use by the hashtable; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item);

/**************** hashtable_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the hashtable is unchanged by this operation.
 */
void *hashtable_find(hashtable_t *ht, const char *key);

/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to hashtable, 
 *   FILE open for writing,
 *   itemprint that can print a single (key, item) pair.
 * We print:
 *   nothing, if NULL fp.
 *   "(null)" if NULL ht.
 *   one line per hash slot, with no items, if NULL itemprint.
 *   otherwise, one line per hash slot, listing (key,item) pairs in that slot.
 * Note:
 *   the hashtable and its contents are not changed by this function,
 */
void hashtable_print(hashtable_t *ht, FILE *fp, 
                     void (*itemprint)(FILE *fp, const char *key, void *item));

/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to hashtable, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which hashtable items are handled is undefined.
 *   the hashtable and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void hashtable_iterate(hashtable_t *ht, void *arg,
                       void (*itemfunc)(void *arg, const char *key, void *item) );

/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item.
 *
 * Caller provides:
 *   valid hashtable pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if hashtable==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the hashtable itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in hashtable_insert.
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) );
```

### Implementation

The hashtable uses chaining and is implemented as an array of sets, where each set is a linked list.
The *hashtable* itself is represented as a `struct hashtable` containing a pointer to an array of pointers to sets. Also 
another pointer to the length of that array.

Each set is a `struct set` from the *set* module. The set stores unique keys in a linked list where each `char *key` has a 
`void *item` associated with it. To see more about the set's implementation go the README linked in the note right before the 
Usage header in this file.

The `hashtable_new` method works by creating and allocating space for a `struct hashtable`, then allocating space for an array 
of `struct set` pointers, where the length of said array is the integer passed by the caller. Each pointer in the array is 
initialized to a new set using `set_new()`. Lastly, the int pointer for the array length is allocated space and assigned the 
integer passed by the caller.

The `hashtable_insert` uses the hash function in [jhash.h](jhash.h) to calculate which set the key passed by the caller should 
go into. Then the (key,item) pair is inserted into the set, at the head of the linked list, using `set_insert`. It returns 
NULL if there was an error.

The `hashtable_find` uses the hash function in [jhash.h](jhash.h) to calculate which set the key passed by the caller should 
be in. Then the (key,item) pair is searched for in that set using `set_find` and returned if found. If the key is not found 
it returns NULL.

The `hashtable_print` method works by looping through the array of sets in the hashtable. Then for each set it passes 
`itemprint` along with the set to `set_print`. `Set_print` prints each (key,item) pair using `itemprint` seperated by 
commas and surrounded by curly brackets. When the hashtable or any of its parts are null then it prints "(null)\n".

The `hashtable_iterate` method loops through the array of set pointers and passes the `itemfunc` along with each set to 
`set_iterate`. `Set_iterate` then calls the `itemfunc` function on each item by looping through each set's linked list.

The `hashtable_delete` method works by looping through the array of set pointers and freeing each set through 
`set_delete`.`Hashtable_delete` passes the `itemdelete` function to `set_delete` which along with freeing the set also frees 
the items through `itemdelete` and the keys in each set. It then frees the array of set pointers and the int pointer that contains the length of the array. Lastly, it frees the `struct hashtable`.

### Assumptions

No non-obvious assumptions.

The `item` inserted cannot be NULL, and thus a NULL return from `hashtable_find` must indicate either error or the key was not 
found.

### Compilation

To compile, do: `make hashtable.o`.

To test, do: `make test`.
See [TESTING](TESTING.md) for details of testing and an example test run.
