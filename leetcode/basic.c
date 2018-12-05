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
typedef struct {
    int digit;
    struct list_head list;
}digit_t;

int print_digit(int num, int *len, struct list_head *hdr) {
    int count = 0;
    digit_t *d;

    while(num > 0) {
        printf("%d ", num % 10);
        d = (digit_t *)malloc(sizeof(digit_t));
        d->digit = (num % 10);
        add_like_queue(&d->list, hdr);

        num = num / 10;
        count++;
    }
    printf("\n");
    *len = count;

    return count;
}

int do_digit(int num) {
    struct list_head hdr;
    hdr.prev = hdr.next = &hdr;
    struct list_head *pos;

    int return_len = 0;
    int i = 0;

    while(print_digit(num, &return_len, &hdr) > 1) {
        i = 0;
        for(pos = hdr.next; pos != &hdr; pos = pos->next) {
            digit_t *dg = list_entry(pos, digit_t, list);
            i += dg->digit;
        }

        num = i; // do it again!
        hdr.prev = hdr.next = &hdr;
    }

    return num;
}
int addDigits(int num) {
    return do_digit(num);
}
//==========================================================================================//
/**
  * Definition for singly-linked list.
  * struct ListNode {
  *     int val;
  *     struct ListNode *next;
  * };
  */
// note - the struct already at above code!
struct ListNode* middleNode(struct ListNode* head) {
    int count = 0;
    int middle;

    struct ListNode *pos;
    for(pos = head; pos != NULL; pos = pos->next)
    {
        count++;
    }

    printf("list node totally %d\n", count);

    middle = count/2;
    printf("middle=%d\n", middle);

    count = 0;
    for(pos = head; pos != NULL; pos = pos->next)
    {
        if(count++ == middle) {
            break;
        }
    }

    return pos;
        
}

void test_middleNode()
{
    struct ListNode hdr;

    struct ListNode nodes[6];
    nodes[0].val = 1;
    hdr.next = &nodes[0];

    nodes[1].val = 2;
    nodes[0].next = &nodes[1];

    nodes[2].val = 3;
    nodes[1].next = &nodes[2];

    nodes[3].val = 4;
    nodes[2].next = &nodes[3];

    nodes[4].val = 5;
    nodes[3].next = &nodes[4];

    nodes[5].val = 6;
    nodes[4].next = &nodes[5];

    nodes[5].next = NULL;

    struct ListNode *middle = middleNode(hdr.next);

    struct ListNode *pos;
    for(pos = middle; pos != NULL; pos = pos->next)
    {
        printf("The middle data:%d\n", pos->val);
    }

}
//==========================================================================================//
int numUniqueEmails(char** emails, int emailsSize) {
    return 0;
}
//==========================================================================================//
int hammingDistance(int x, int y) {
    int b = x ^ y;
    printf("the value:%d %#x\n", b, b);
    int count = 0;  
        while ( b )  
                {  
                            if( b& 1 ){  
                                            ++count;  
                                                    }  
                                    b>>= 1;  
                                        }  
        printf("now, the 1 has %d\n", count);
    return b;
}
typedef int bool;
//==========================================================================================//
bool judgeCircle(char* moves) {
    int a = 0;
    int b = 0;
    int c = 0;
    int d = 0;

    int i = 0;
    while(moves[i] != '\0') {
        switch(moves[i]) {
            case 'U':
                a++;
                break;
            case 'D':
                b++;
                break;
            case 'L':
                c++;
                break;
            case 'R':
                d++;
                break;
        }
        i++;
    }

    printf("the abcd is %d %d %d %d\n\n", a,b,c,d);

    return ((a == b) && (c == d));
}
//==========================================================================================//
bool checkRecord(char* s) {
    int i = 0;
    int prev = 0;
    int a=0,l=0;
    bool good = 1;

    while(s[i] != '\0'){
        printf("prev = %d, si=%d, a=%d, l=%d\n", prev, s[i], a, l);
        if (a > 1 || l > 1) {
            good = 0;
            break;
        }

        if(s[i] == 'A'){
            a++;
        } else if (s[i] == 'L') {
            printf("now, prev = %d\n", prev);
                if(prev == 0 || prev == 'L'){
                    l++;
                } else {
                    l++;
                }
        } else {
            ;
        }

        prev = s[i];
        i++;
    }

    return good;
        
}
//==========================================================================================//
int peakIndexInMountainArray(int* A, int ASize) {
    int i;
    int fast_i;
    for(i = 0; i <ASize; i++) {
        fast_i = i;
        fast_i ++;

        if (A[i] > A[fast_i]) {
            break;
        }
    }

    return i;
}
//==========================================================================================//

int main(int argc, char **argv)
{
    int b;
    char *a[] = {"abc", "hello"};
//    findWords(a, 2, &b);
    int i = checkRecord("PPAPLLL");
    printf("%d\n", i);

    return 0;
}
