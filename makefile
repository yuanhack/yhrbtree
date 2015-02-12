CC = gcc 
HEAD = yhrbtree.h
SRC = yhrbtree.c
DST = rbtest


# # export use
SO = libyhrbtree.so
LSO = -lyhrbtree
SO_PATH = lib 

MACRO = -Wall -g

all: ctags $(SO) test_str test_struct test_2key

# # export librarry
$(SO): $(HEAD) $(SRC)
	$(CC) $(MACRO) $(SRC) -shared -fpic -o $@ && \
		\mkdir -p $(SO_PATH) &&  \
		\cp $(SO) $(SO_PATH) -rf

# export use
test_str: test_str.c 
	$(CC) $(MACRO) -Wl,-rpath,$(SO_PATH) -L$(SO_PATH) $(LSO) $< -o $@

test_struct: test_struct.c 
	$(CC) $(MACRO) -Wl,-rpath,$(SO_PATH) -L$(SO_PATH) $(LSO) $< -o $@

test_2key: test_2key.c
	$(CC) $(MACRO) -Wl,-rpath,$(SO_PATH) -L$(SO_PATH) $(LSO) $< -o $@

ctags:
tags:
tag: *.c *.h
	ctags *
clean:
	rm -rf *.o test_str test_struct test_2key $(SO)  $(SO_PATH) $(export_directory) 

export_directory = export_rbtree
out: $(SO)
	\mkdir -p $(export_directory) && \
		\cp -rf  yhrbtree.h test_str.c test_struct.c test_2key.c \
			$(SO) demotest_makefile $(export_directory)

