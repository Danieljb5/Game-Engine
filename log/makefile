# Template variables #
FLAGS = -std=c++17
INCLUDES = -Isrc -Iinclude
LINKS = 
LINKDIRS = -Llib -Wl,-rpath,.
DEFPLAT = linux
DEFMODE = debug
CMDS = 

# Pre defined variables #
SILENT = @
CXX = g++
.PHONY: all clean cmds deps run

# User configurable variables #
OBJDIR = obj
SRCDIR = src
SRCS := $(wildcard $(SRCDIR)/*.cpp)
DEPS = 		# Path to directories containing other makefile projects this depends on #
LIBGEN = 1 	# Defines whether the output is a library file #
STATICLIB = 0	# Only active if LIBGEN = 1, creates a static library instead of a dynamic one #
PROJECT = cl_log

# Debug level flags #
DEBUG_FLAGS = -Og -g
DEBUG_INCLUDES = 
DEBUG_LINKS = 
DEBUG_LINKDIRS = 
DEBUG_CMDS = 

RELEASE_FLAGS = -O3 -fopenmp -D_GLIBCXX_PARALLEL
RELEASE_INCLUDES = 
RELEASE_LINKS = 
RELEASE_LINKDIRS = 
RELEASE_CMDS = 

DIST_FLAGS = -O3 -fopenmp -D_GLIBCXX_PARALLEL
DIST_INCLUDES = 
DIST_LINKS = 
DIST_LINKDIRS = 
DIST_CMDS = 

# Platform flags #
COMMON_FLAGS = 
COMMON_INCLUDES = -I ../base -I ../thread/include
COMMON_LINKS = -pthread
COMMON_LINKDIRS = 
COMMON_CMDS = 

LINUX_FLAGS = 
LINUX_INCLUDES = 
LINUX_LINKS = 
LINUX_LINKDIRS = 
LINUX_CMDS = 

WINDOWS_FLAGS = 
WINDOWS_INCLUDES = 
WINDOWS_LINKS = 
WINDOWS_LINKDIRS = 
WINDOWS_CMDS = 

# Command parameters #
ifndef platform
platform = $(DEFPLAT)
endif
ifndef mode
mode = $(DEFMODE)
endif
ifdef verbose
SILENT = 
endif

# Auto generated variables #
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
BUILD_DIR = $(mode)-$(platform)-x64
OBJDIR := $(OBJDIR)/$(BUILD_DIR)
BUILD_DIR := bin/$(BUILD_DIR)

# Combine variables into common variables #
ifeq ($(strip $(platform)), linux)
CXX = g++
FLAGS := $(FLAGS) $(LINUX_FLAGS) -DLINUX
INCLUDES := $(INCLUDES) $(LINUX_INCLUDES)
LINKS := $(LINKS) $(LINUX_LINKS)
LINKDIRS := $(LINKDIRS) $(LINUX_LINKDIRS)
RUN_CMD = cd $(BUILD_DIR) && ./$(TARGET)
ifneq ($(strip $(LINUX_CMDS)), )
ifeq ($(strip $(CMDS)), )
CMDS := $(LINUX_CMDS)
else
CMDS := $(CMDS) ; $(LINUX_CMDS)
endif
endif
ifeq ($(strip $(LIBGEN)), 1)
ifeq ($(strip $(STATICLIB)), 1)
TARGET = lib$(PROJECT).a
else
TARGET = lib$(PROJECT).so
endif
else
TARGET = $(PROJECT)
endif
endif

ifeq ($(strip $(platform)), windows)
CXX = x86_64-w64-mingw32-g++-posix
FLAGS := $(FLAGS) $(WINDOWS_FLAGS) -DWINDOWS
INCLUDES := $(INCLUDES) $(WINDOWS_INCLUDES)
LINKS := $(LINKS) $(WINDOWS_LINKS)
LINKDIRS := $(LINKDIRS) $(WINDOWS_LINKDIRS)
RUN_CMD = cd $(BUILD_DIR) && wine $(TARGET)
ifneq ($(strip $(WINDOWS_CMDS)), )
ifeq ($(strip $(CMDS)), )
CMDS := $(WINDOWS_CMDS)
else
CMDS := $(CMDS) ; $(WINDOWS_CMDS)
endif
endif
ifeq ($(strip $(LIBGEN)), 1)
ifeq ($(strip $(STATICLIB)), 1)
TARGET = $(PROJECT).lib
else
TARGET = $(PROJECT).dll
endif
else
TARGET = $(PROJECT).exe
endif
endif

FLAGS := $(FLAGS) $(COMMON_FLAGS)
INCLUDES := $(INCLUDES) $(COMMON_INCLUDES)
LINKS := $(LINKS) $(COMMON_LINKS)
LINKDIRS := $(LINKDIRS) $(COMMON_LINKDIRS)
ifneq ($(strip $(COMMON_CMDS)), )
ifeq ($(strip $(CMDS)), )
CMDS := $(COMMON_CMDS)
else
CMDS := $(CMDS) ; $(COMMON_CMDS)
endif
endif

ifeq ($(strip $(mode)), debug)
FLAGS := $(FLAGS) $(DEBUG_FLAGS) -DDEBUG
INCLUDES := $(INCLUDES) $(DEBUG_INCLUDES)
LINKS := $(LINKS) $(DEBUG_LINKS)
LINKDIRS := $(LINKDIRS) $(DEBUG_LINKDIRS)
ifeq ($(strip $(DEBUG_CMDS)), )
CMDS := $(CMDS)
else
ifeq ($(strip $(CMDS)), )
CMDS := $(DEBUG_CMDS)
else
CMDS := $(CMDS) ; $(DEBUG_CMDS)
endif
endif
endif
ifeq ($(strip $(mode)), release)
FLAGS := $(FLAGS) $(RELEASE_FLAGS) -DRELEASE
INCLUDES := $(INCLUDES) $(RELEASE_INCLUDES)
LINKS := $(LINKS) $(RELEASE_LINKS)
LINKDIRS := $(LINKDIRS) $(RELEASE_LINKDIRS)
ifneq ($(strip $(RELEASE_CMDS)), )
ifeq ($(strip $(CMDS)), )
CMDS := $(RELEASE_CMDS)
else
CMDS := $(CMDS) ; $(RELEASE_CMDS)
endif
endif
endif
ifeq ($(strip $(mode)), dist)
FLAGS := $(FLAGS) $(DIST_FLAGS) -DDIST
INCLUDES := $(INCLUDES) $(DIST_INCLUDES)
LINKS := $(LINKS) $(DIST_LINKS)
LINKDIRS := $(LINKDIRS) $(DIST_LINKDIRS)
ifneq ($(strip $(DIST_CMDS)), )
ifeq ($(strip $(CMDS)), )
CMDS := $(DIST_CMDS)
else
CMDS := $(CMDS) ; $(DIST_CMDS)
endif
endif
endif

ifeq ($(strip $(LIBGEN)), 1)
ifeq ($(strip $(STATICLIB)), 1)
COMPILER = $(SILENT) $(CXX) -c $(FLAGS) $(INCLUDES)
LINKER = $(SILENT) ar rcs $(BUILD_DIR)/$(TARGET) $(OBJS) $(LINKS)
else
COMPILER = $(SILENT) $(CXX) -c $(FLAGS) $(INCLUDES) -fPIC
LINKER = $(SILENT) $(CXX) -shared $(OBJS) -o $(BUILD_DIR)/$(TARGET) $(FLAGS) $(LINKS) $(LINKDIRS)
endif
else
COMPILER = $(SILENT) $(CXX) -c $(FLAGS) $(INCLUDES)
LINKER = $(SILENT) $(CXX) $(OBJS) $(LINKS) $(LINKDIRS) -o $(BUILD_DIR)/$(TARGET) $(FLAGS)
endif

# Targets #
all : $(BUILD_DIR)/$(TARGET)
	

clean :
	$(SILENT) rm bin -r || true
	$(SILENT) rm obj -r || true
	
cmds:
	$(CMDS)
	
deps: $(BUILD_DIR)
	$(SILENT) $(foreach dep,$(DEPS), cd $(dep) && $(MAKE);)
	
run : $(BUILD_DIR)/$(TARGET)
	$(SILENT) $(RUN_CMD)
	
$(BUILD_DIR):
	$(SILENT) mkdir -p $(BUILD_DIR)

$(OBJDIR)/%.o : $(SRCDIR)/%.cpp deps cmds
	$(SILENT) mkdir -p $(dir $@)
	$(COMPILER) $<
	$(SILENT) mv $(notdir $@) $(dir $@)

$(BUILD_DIR)/$(TARGET) : $(OBJS)
	$(LINKER)
