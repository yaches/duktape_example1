RM = rm -f
MKDIR = mkdir -p

CXX = g++ 
BUILDDIR = build
TARGET = exmpl
LIB = duk
SOURCES = $(LIB)/*.c *.cpp

FLAGS = -lm

all: 
		$(MKDIR) $(BUILDDIR)
		$(CXX) $(FLAGS) -I$(LIB) $(SOURCES) -o $(BUILDDIR)/$(TARGET)

clean:
		$(RM) $(TARGET)
