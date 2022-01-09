# makefile for c3vsRamLoader
# This makefile is for QNX 4.25

# Define -LANG_RUS for output in Russian

OBJDIR=release
INCDIR=inc
OUTFILE=c3vsRamLoader

CC=cc 

LFLAGS= 
OS = QNX 4.25
CFLAGS= -c -T1
OBJ=  main.o obrazldr.o tmk_kk.o potok.o vme.o tmk_driver.o 
#CFLAGS= -c -T1 -D PTK
#OBJ=  main.o obrazldr.o tmk_kk.o potok.o vme.o tmk_driver.o 

target: $(OBJDIR)/$(OUTFILE)
	@echo
	@echo ======== COMPLETED
	@echo $(OUTFILE) for $(OS) processor created.
	@rm -f release/*.o
	@chmod a-x release/$(OUTFILE)
	@usemsg release/$(OUTFILE) inc/usage_inf 
	@chmod a+x release/$(OUTFILE)
 
$(OBJDIR)/$(OUTFILE): $(addprefix $(OBJDIR)/,$(OBJ))
	@echo
	@echo ======== LINKING $(OUTFILE) 
	$(CC) $(addprefix $(OBJDIR)/,$(OBJ))  $(LFLAGS) -o $@ 

$(OBJDIR)/%.o : %.c 
	@echo
	@echo ======== COMPILE $< ...
	$(CC) $(CFLAGS) $< -o $(OBJDIR)/
