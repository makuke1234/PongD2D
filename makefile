SHELL=cmd
CC=gcc
CXX=g++
ASMC=nasm
CDEFFLAGS=-std=c2x -Wall -Wextra -Wpedantic -Wconversion -m64 -D UNICODE -D _UNICODE
CXXDEFFLAGS=-std=c++20 -Wall -Wextra -Wpedantic -Wconversion -m64 -D UNICODE -D _UNICODE
CDEBFLAGS=-g -O0 -D _DEBUG
CFLAGS=-O3 -Wl,--strip-all,--build-id=none,--gc-sections -fno-ident -D NDEBUG
LIB=-municode -lgdi32 -ld2d1 -ldwrite -luuid -mwindows

SRC=src
SRC_SINGLE=srcSingle
TARGET=PongD2D
OBJD=objd
OBJ=obj

OBJD_SINGLE=$(OBJD)_s
OBJ_SINGLE=$(OBJ)_s

default: debug


$(OBJD):
	mkdir $(OBJD)
$(OBJ):
	mkdir $(OBJ)

$(OBJD_SINGLE):
	mkdir $(OBJD_SINGLE)
$(OBJ_SINGLE):
	mkdir $(OBJ_SINGLE)

ASMSUFFIX=asm

C_SRCS=$(wildcard $(SRC)/*.c)
ASM_SRCS=$(wildcard $(SRC)/*.$(ASMSUFFIX))
CXX_SRCS=$(wildcard $(SRC)/*.cpp)
RSC_OBJ=$(wildcard $(SRC)/*.rc)
RSC_OBJ:=$(RSC_OBJ:%.rc=%.rc.o)

RSC_OBJ_R=$(RSC_OBJ:$(SRC)/%=$(OBJ)/%)
RSC_OBJ_D=$(RSC_OBJ:$(SRC)/%=$(OBJD)/%)

C_SRCS_SINGLE=$(wildcard $(SRC_SINGLE)/*.c)
ASM_SRCS_SINGLE=$(wildcard $(SRC_SINGLE)/*.$(ASMSUFFIX))
CXX_SRCS_SINGLE=$(wildcard $(SRC_SINGLE)/*.cpp)


debug_obj=$(C_SRCS:%.c=%.c.o)
debug_obj+=$(ASM_SRCS:%.$(ASMSUFFIX)=%.$(ASMSUFFIX).o)
debug_obj+=$(CXX_SRCS:%.cpp=%.cpp.o)
debug_obj+=$(RSC_OBJ_D)

debug_obj_single=$(C_SRCS_SINGLE:%.c=%.c.o)
debug_obj_single+=$(ASM_SRCS_SINGLE:%.$(ASMSUFFIX)=%.$(ASMSUFFIX).o)
debug_obj_single+=$(CXX_SRCS_SINGLE:%.cpp=%.cpp.o)
debug_obj_single+=$(RSC_OBJ_D)



release_obj=$(C_SRCS:%.c=%.c.o)
release_obj+=$(ASM_SRCS:%.$(ASMSUFFIX)=%.$(ASMSUFFIX).o)
release_obj+=$(CXX_SRCS:%.cpp=%.cpp.o)
release_obj+=$(RSC_OBJ_R)


release_obj_single=$(C_SRCS_SINGLE:%.c=%.c.o)
release_obj_single+=$(ASM_SRCS_SINGLE:%.$(ASMSUFFIX)=%.$(ASMSUFFIX).o)
release_obj_single+=$(CXX_SRCS_SINGLE:%.cpp=%.cpp.o)
release_obj_single+=$(RSC_OBJ_R)



debug_obj:=$(debug_obj:$(SRC)/%=$(OBJD)/%)
release_obj:=$(release_obj:$(SRC)/%=$(OBJ)/%)


debug_obj_single:=$(debug_obj_single:$(SRC_SINGLE)/%=$(OBJD_SINGLE)/%)
release_obj_single:=$(release_obj_single:$(SRC_SINGLE)/%=$(OBJ_SINGLE)/%)


$(OBJ)/%.rc.o: $(SRC)/%.rc $(OBJ)
	windres -i $< -o $@ -D NDEBUG

$(OBJD)/%.rc.o: $(SRC)/%.rc $(OBJD)
	windres -i $< -o $@ -D _DEBUG

$(OBJD)/%.c.o: $(SRC)/%.c $(OBJD)
	$(CC) $< -c -o $@ $(CDEFFLAGS) $(CDEBFLAGS)
$(OBJD)/%.$(ASMSUFFIX).o: $(SRC)/%.$(ASMSUFFIX) $(OBJD)
	$(ASMC) $< -f win64 -o $@ -O0
$(OBJD)/%.cpp.o: $(SRC)/%.cpp $(OBJD)
	$(CXX) $< -c -o $@ $(CXXDEFFLAGS) $(CDEBFLAGS)

$(OBJD_SINGLE)/%.c.o: $(SRC_SINGLE)/%.c $(OBJD_SINGLE)
	$(CC) $< -c -o $@ $(CDEFFLAGS) $(CDEBFLAGS)
$(OBJD_SINGLE)/%.$(ASMSUFFIX).o: $(SRC_SINGLE)/%.$(ASMSUFFIX) $(OBJD_SINGLE)
	$(ASMC) $< -f win64 -o $@ -O0
$(OBJD_SINGLE)/%.cpp.o: $(SRC_SINGLE)/%.cpp $(OBJD_SINGLE)
	$(CXX) $< -c -o $@ $(CXXDEFFLAGS) $(CDEBFLAGS)



$(OBJ)/%.c.o: $(SRC)/%.c $(OBJ)
	$(CC) $< -c -o $@ $(CDEFFLAGS) $(CFLAGS)
$(OBJ)/%.$(ASMSUFFIX).o: $(SRC)/%.$(ASMSUFFIX) $(OBJ)
	$(ASMC) $< -f win64 -o $@ -Ox
$(OBJ)/%.cpp.o: $(SRC)/%.cpp $(OBJ)
	$(CXX) $< -c -o $@ $(CXXDEFFLAGS) $(CFLAGS)

$(OBJ_SINGLE)/%.c.o: $(SRC_SINGLE)/%.c $(OBJ_SINGLE)
	$(CC) $< -c -o $@ $(CDEFFLAGS) $(CFLAGS)
$(OBJ_SINGLE)/%.$(ASMSUFFIX).o: $(SRC_SINGLE)/%.$(ASMSUFFIX) $(OBJ_SINGLE)
	$(ASMC) $< -f win64 -o $@ -Ox
$(OBJ_SINGLE)/%.cpp.o: $(SRC_SINGLE)/%.cpp $(OBJ_SINGLE)
	$(CXX) $< -c -o $@ $(CXXDEFFLAGS) $(CFLAGS)


debug: $(debug_obj)
	$(CXX) $^ -o deb$(TARGET) $(CDEBFLAGS) $(LIB)

debugsingle: $(debug_obj_single)
	$(CXX) $^ -o deb$(TARGET) $(CDEBFLAGS) $(LIB)


release: $(release_obj)
	$(CXX) $^ -o $(TARGET) $(CFLAGS) $(LIB)

releasesingle: $(release_obj_single)
	$(CXX) $^ -o $(TARGET) $(CFLAGS) $(LIB)



clean.o:
	IF EXIST $(OBJD) rd /s /q $(OBJD)
	IF EXIST $(OBJ) rd /s /q $(OBJ)
	IF EXIST $(OBJD_SINGLE) rd /s /q $(OBJD_SINGLE)
	IF EXIST $(OBJ_SINGLE) rd /s /q $(OBJ_SINGLE)

clean: clean.o
	del deb$(TARGET).exe
	del $(TARGET).exe
