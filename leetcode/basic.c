/**
 * C binding
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//==========================================================================================//

#define list_entry(p, type, mem) \
    (type *) ((unsigned long)p - (unsigned long)(&((type *)0)->mem))

struct list_head{
    struct list_head *prev;
    struct list_head *next;
};

/* insert @p between @a and @b */
void _add_list(struct list_head *p, struct list_head *a, struct list_head *b)
{
    a->next = p;
    b->prev = p;
    p->prev = a;
    p->next = b;
}

void add_like_queue(struct list_head *p, struct list_head *cur)
{
    _add_list(p, cur->prev, cur);
}

void add_like_stack(struct list_head *p, struct list_head *cur)
{
    _add_list(p, cur, cur->next);
}


/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* selfDividingNumbers(int left, int right, int* returnSize)
{
    int i;
    int num;
    int dirty = 0;
    int count = 0;

    typedef struct {
        int              dg_val;
        struct list_head list;
    }my_digit_t;

    struct list_head header; // store final dynamic array!
    struct list_head tmphdr;
    struct list_head *pos;

    header.prev = header.next = &header;

    my_digit_t *ptr = NULL;
    my_digit_t *ptr2 = NULL;

    for(i = left; i <= right; i++)
    {
        num = i;

        tmphdr.prev = tmphdr.next = &tmphdr;

        while(num > 0) {
            ptr = (my_digit_t *)malloc(sizeof(my_digit_t));
            ptr->dg_val = num % 10;

            add_like_stack(&ptr->list, &tmphdr);
            
            num /= 10;
        }

        dirty = 0;

        for(pos = tmphdr.next; pos != &tmphdr; pos = pos->next)
        {
            ptr = list_entry(pos, my_digit_t, list);

            if (ptr->dg_val == 0 || i % ptr->dg_val != 0)
            {
                dirty = 1;
                break;
            }
        }

        if(dirty == 0)
        {
            ptr = (my_digit_t *)malloc(sizeof(my_digit_t));
            ptr->dg_val = i;
            add_like_queue(&ptr->list, &header);
            count++;
        }
    }

    *returnSize = count;

    i = 0;
    int *final = (int *)malloc(count*sizeof(int));

    for(pos = header.next; pos != &header; pos = pos->next)
    {
        ptr = list_entry(pos, my_digit_t, list);
        final[i++] = ptr->dg_val;
    }

    return final;        
}

//==========================================================================================//

struct ListNode {
        int val;
        struct ListNode *next;
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
void deleteNode(struct ListNode* node) {

    // special, actually free the next-node, not the @node
    // specify in the parameter!
    node->val = node->next->val;
    node->next = node->next->next;
    free(node->next); 
}
//==========================================================================================//
/**
  * Return an array of arrays of size *returnSize.
  * The sizes of the arrays are returned as *columnSizes array.
  * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
  */
int** flipAndInvertImage(int** A, int ARowSize, int *AColSizes, int** columnSizes, int* returnSize) {
        
}

//==========================================================================================//

/**
  * Return an array of size *returnSize.
  * Note: The returned array must be malloced, assume caller calls free().
  */
char** findWords(char** words, int wordsSize, int* returnSize) {
    int i, j;

    char *mapping[] = {
        "qwertyuiop",
        "asdfghjkl",
        "zxcvbnm"
    };

    int k, map_idx;
    int meet;

    char *pos;
    struct my_keyboard{
        int              words_line_no;
        struct list_head list;
    };

    struct list_head hdr;
    hdr.prev = hdr.next = &hdr;

    int line1, line2, line3;
    int map_line = -1;
    int map_ok = 1;
    int count_mapped = 0;

    for(i = 0; i < wordsSize; i++)
    {
        printf("%s\n", words[i]);
        pos = words[i];

        j = 0;
        map_ok = 1;

        while(pos[j++] != '\0')
        {
            printf("%c ", pos[j - 1]);

            line1 = line2 = line3 = 0;

            for(k = 0; k < 3; k++)
            {
                map_idx = 0;
                meet = 0;
                while(mapping[k][map_idx] != '\0')
                {
                    if(pos[j - 1] == mapping[k][map_idx])
                    {
                        meet = 1;
                        break;
                    }
                    map_idx++;
                }

                if(meet == 1)
                {
                    if(map_line == -1){
                        map_line = k;
                    } else {
                        if (map_line != k) {
                            map_ok = 0;
                            break;
                        }
                    }
                }
            }

            if(map_ok == 0)
            {
                break;
            }

        }

        if(map_ok == 1)
        {
            // add it!
            struct my_keyboard *key = (struct my_keyboard *)malloc(sizeof(struct my_keyboard));

            key->words_line_no = i;
            add_like_queue(&key->list, &hdr);
            count_mapped++;
        }

        printf("\n");
    }

    char **ret = (char **)malloc(count_mapped * sizeof(char *));

    struct my_keyboard *p;
    i = 0;

    struct list_head *ppos;
    for(ppos = hdr.next; ppos != &hdr; ppos = ppos->next)
    {
        p = list_entry(ppos, struct my_keyboard, list);
        ret[i] = words[p->words_line_no];
        i++;
    }

    *returnSize = count_mapped;

    return ret;
}

//==========================================================================================//

int addDigits(int num) {
    return 0;
}

int main(int argc, char **argv)
{
    int b;
    char *a[] = {"abc", "hello"};
    findWords(a, 2, &b);
    printf("the return value = %d\n", b);

    return 0;
}
