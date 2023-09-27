#ifndef _LIBJCONTAINER_H
#define _LIBJCONTAINER_H

#define libjcontainer_of(ptr, type, member) ({                      				\
						const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
						(type *)( (char *)__mptr - offsetof(type,member) );})


# endif // _LIBJCONTAINER_H
