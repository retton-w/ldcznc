
#ifndef		_BUF_LIST_H_
#define   	_BUF_LIST_H_

#include <stdlib.h>
#include <stdio.h>
#include "ce_buffer.h"

/* buffer list base class */
class BufList {

public:
        BufList(int length, int count);
        virtual ~BufList(void);

        int get_len(void);
        virtual int PutBuffer(void *buf) = 0;
        virtual void *GetBuffer(void) = 0;

protected :

        struct head {
                struct head *next;
        };
        struct head head;

	int buf_length;
};


//堆栈式buffer_list, head.next指向栈顶
inline BufList::BufList(int length, int count)
{
        struct head *item;
	void *tmp = NULL;
	buf_length = length;
        this->head.next = NULL;
        while (--count >= 0) {
#if 0
                if ((item = (struct head *)malloc(length + sizeof(*item))) == NULL) {

                        break;
                }
#else
		//printf("before buffer_create, buf length is %d\n", buf_length);
		if (buffer_createEx(&tmp, buf_length) < 0) {

			printf("create Ex\n");
			break;
		}
		item = (struct head *)tmp;
		tmp = NULL;
		//printf("count is %d\n", count);
#endif
                item->next = head.next;
                head.next = item;
        }
}


inline BufList::~BufList(void)
{
        struct head *item = head.next;
        while (item != NULL) {

                this->head.next = item->next;
#if 0
                free(item);
#else
		buffer_deleteEx((void *)item, buf_length); 
#endif
                item = head.next;
        }
}


inline int BufList::get_len(void)
{
        int i = 0;
        struct head *temp = &(this->head);
        while (temp->next != NULL) {
                temp = temp->next;
                ++i;
        }
        return i;
}


/* buffer filo : first in, last out */
class BufFilo : public BufList {

public :
        BufFilo(int length, int count) : BufList(length, count) {
	
		//printf("len is %d, num is %d\n", length, count);
	}
        ~BufFilo(void) {}
        void *GetBuffer(void);
        int PutBuffer(void *buf);
};


inline void *BufFilo::GetBuffer(void)
{
        void *buf = NULL;
        struct head *item = this->head.next;
        if (this->head.next == NULL) {  //buffer_list为空
                return buf;
        }
        buf = (char *)item + sizeof(*item);
        this->head.next = item->next;
        return (void *)buf;
}


inline int BufFilo::PutBuffer(void *buf)
{
        struct head *item = (struct head *)((char *)buf - sizeof(*item));
        item->next = this->head.next;
        this->head.next = item;
        return 0;
}


/* buffer fifo : first in, first out */
class BufFifo : public BufList {

public :
        
	BufFifo(int length, int count) : BufList(length, count) {}
        ~BufFifo(void) {}
        void *GetBuffer(void);
        int PutBuffer(void *buf);
};


inline void *BufFifo::GetBuffer(void)
{
        void *buf = NULL;
        struct head *item = this->head.next;
        if (this->head.next == NULL) {
                return buf;
        }
        buf = (char *)item + sizeof(*item);
        this->head.next = item->next;
        return (void *)buf;
}


inline int BufFifo::PutBuffer(void *buf)
{
        struct head *item = (struct head *)((char *)buf - sizeof(*item));
        struct head *temp = &(this->head);

        while (temp->next != NULL) {
                temp = temp->next;
        }
        temp->next = item;
        item->next = NULL;

        return 0;
}

#endif 	    /* !BUF_LIST_H_ */


