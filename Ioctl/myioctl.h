#define MAGIC 'x'
#define CMD_1 _IO(MAGIC,1)
#define CMD_2 _IOW(MAGIC,2,int)

// these are the header files for command