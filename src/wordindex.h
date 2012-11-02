/* every match will be stored in the selected data structure as liked list
   entry of type sturct match: */

struct match {
        char *filename;
        unsigned int line;
        unsigned int coumn;
        struct match *next;
};

