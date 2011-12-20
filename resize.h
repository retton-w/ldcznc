
#ifndef _RESIZE_H_
#define	_RESIZE_H_


#include "ce_buffer.h"


typedef struct Resize_Object {
        int   fd;

} Resize_Object;

typedef struct Resize_Object *Resize_Handle;



class Resize {

public:
	static Resize_Handle ResizeCreate();
	static int ResizeDelete(Resize_Handle handle);
	static int ResizeConfig(Resize_Handle handle,
				struct cmemBuffer *source_buf,
				struct cmemBuffer *destination_buf,
				struct cmemBuffer *destination_buf2);
	static int ResizeExecute(Resize_Handle handle,
				struct cmemBuffer *source_buf,
				struct cmemBuffer *destination_buf,
				struct cmemBuffer *destination_buf2);

private:
	
	Resize() { }
	~Resize() { }

};


#endif

