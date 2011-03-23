GTEST_DIR = /home/matt/lib/gtest-1.5.0
SRC_DIR = /home/matt/Documents/cse335/honors_option

TEST_SRCS = test/TestRunner.cpp test/CellTest.cpp test/PuzzleTest.cpp \
	test/SingleCandidateMethodTest.cpp
LIB_SRCS = Puzzle.cpp Cell.cpp SingleCandidateMethod.cpp

DEPDIR = .deps
df = $(DEPDIR)/$(@F)

# preprocessor
CPPFLAGS += -I$(GTEST_DIR)/include -I$(SRC_DIR)
# C++ compiler
CXXFLAGS = -Wall -std=c++0x

# gtest headers, don't need to change
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h

MAKEDEPEND = $(CXX) $(CPPFLAGS) -MM -o $(df).d $<
MAKEDEPEND_TEST = $(CXX) $(CPPFLAGS) -MM -o $(df).d -MT $(basename $<).o $<

SRCS := main.cpp $(LIB_SRCS)
OBJS := $(SRCS:%.cpp=%.o)
LIB_OBJS := $(LIB_SRCS:%.cpp=%.o)
TEST_OBJS := $(TEST_SRCS:%.cpp=%.o)

# targets:
all : sudoku run_tests

sudoku : $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $@

run_tests : $(LIB_OBJS) $(TEST_OBJS) gtest.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

#TestRunner.o : test/TestRunner.cpp 
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

# dependency stuff
.D_TARGET:
	mkdir -p $(DEPDIR)
	touch $@

.PRECIOUS: .D_TARGET

# GTEST building stuff don't touch me
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
           $(GTEST_DIR)/src/gtest-all.cc

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
           $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# tests
%Test.o : %Test.cpp .D_TARGET $(GTEST_HEADERS)
	$(MAKEDEPEND_TEST);
	@cp $(df).d $(df).P;
#	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
#		-e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; 
	@rm -f $(df).d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

# objects from sources
%.o : %.cpp .D_TARGET
	$(MAKEDEPEND);
	@cp $(df).d $(df).P; \
#	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
#		-e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; 
	@rm -f $(df).d
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -o $@ -c $<

-include $(TEST_SRCS:test/%.cpp=$(DEPDIR)/%.o.P)
-include $(SRCS:%.cpp=$(DEPDIR)/%.o.P)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) \
		gtest.a gtest_main.a gtest-all.o gtest_main.o \
		.D_TARGET sudoku run_tests
	rm -rf $(DEPDIR)