/*
typedef struct ListItem ListItem;
struct ListItem
{
	void*		data;
	ListItem*	prev;
	ListItem*	next
};

typedef struct
{
	ListItem*	head;
} List;

List		list_newlist(void);
ListItem	list_newitem(void* data);
ListItem	list_hd(List);
List		list_tl(List);
List		list_cons(ListItem, List);
*/

typedef struct DoubleListItem DoubleListItem;
struct DoubleListItem
{
	double			value;
	DoubleListItem*		next;
};

typedef struct
{
	DoubleListItem*		hd;
	DoubleListItem*		tl;
	int			len;
} DoubleList;
