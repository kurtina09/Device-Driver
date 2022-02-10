#define LINK_LIST_MAGIC   'X'
#define DEL_NODE     _IOW(LINK_LIST_MAGIC, 0, char)
#define PRINT_LIST   _IO(LINK_LIST_MAGIC, 1)
#define ADD_NODE     _IOW(LINK_LIST_MAGIC, 2, char)
