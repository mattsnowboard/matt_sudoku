GTEST_DIR = /home/matt/lib/gtest-1.5.0
GMOCK_DIR = /home/matt/lib/gmock-1.5.0
SRC_DIR = /home/matt/Documents/cse335/honors_option

QTINC := -I/usr/share/qt4/mkspecs/linux-g++ -I/usr/include/qt4/QtCore \
	-I/usr/include/qt4/QtGui -I/usr/include/qt4

TEST_SRCS = test/TestRunner.cpp test/CellTest.cpp test/PuzzleTest.cpp \
	test/SingleCandidateMethodTest.cpp test/ExclusionMethodTest.cpp \
	test/BlockIntersectionMethodTest.cpp test/CoveringSetMethodTest.cpp \
	test/SimpleValidatorTest.cpp test/PuzzleMarkerTest.cpp \
	test/PlayerValidatorTest.cpp test/SolverHelperTest.cpp \
	test/GuessCommandTest.cpp test/MarkCommandTest.cpp \
	test/UnmarkCommandTest.cpp test/MethodSolverTest.cpp \
	test/SimplePuzzleImporterTest.cpp test/SolvedPuzzleImporterTest.cpp \
	test/AddHintMarksCommandTest.cpp test/CellControllerTest.cpp \
	test/PuzzleControllerTest.cpp test/SolveCommandTest.cpp
QT_SRCS = 
LIB_SRCS = Puzzle.cpp Cell.cpp SingleCandidateMethod.cpp ExclusionMethod.cpp \
	BlockIntersectionMethod.cpp CoveringSetMethod.cpp SimpleValidator.cpp \
	PuzzleMarker.cpp PlayerValidator.cpp SolverHelper.cpp GuessCommand.cpp \
	MarkCommand.cpp UnmarkCommand.cpp MethodSolver.cpp \
	SimplePuzzleImporter.cpp SolvedPuzzleImporter.cpp GameManager.cpp \
	CellController.cpp AddHintMarksCommand.cpp GameController.cpp \
	PuzzleController.cpp SolveCommand.cpp

DEPDIR = .deps
df = $(DEPDIR)/$(@F)

# preprocessor
CPPFLAGS += -I$(GTEST_DIR)/include -I$(GMOCK_DIR)/include -I$(SRC_DIR) $(QTINC)

# C++ compiler
CXXFLAGS = -Wall -std=c++0x
# qt defines
QTDEF = -D_REENTRANT -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
# stuff to link for Qt
QTFLAGS = -L/usr/lib -lQtCore -lQtGui -lpthread

# gtest headers, don't need to change
GTEST_HEADERS = $(GTEST_DIR)/include/gtest/*.h \
                $(GTEST_DIR)/include/gtest/internal/*.h
# gmock stuff, don't need to change
GMOCK_HEADERS = $(GMOCK_DIR)/include/gmock/*.h \
                $(GMOCK_DIR)/include/gmock/internal/*.h \
                $(GTEST_HEADERS)

MAKEDEPEND = $(CXX) $(CPPFLAGS) -MM -o $(df).d $<
MAKEDEPEND_TEST = $(CXX) $(CPPFLAGS) -MM -o $(df).d -MT $(basename $<).o $<
MAKEDEPEND_QT = $(CXX) $(CPPFLAGS) -MM -o $(df).d -MT $(basename $<).o $<

SRCS := main.cpp $(LIB_SRCS)
OBJS := $(SRCS:%.cpp=%.o)
LIB_OBJS := $(LIB_SRCS:%.cpp=%.o)
QT_OBJS := $(QT_SRCS:%.cpp=%.o)
TEST_OBJS := $(TEST_SRCS:%.cpp=%.o)

# targets:
debug : CXXFLAGS += -g -O0
# removed this warning because it sucks: -Wconversion (int to size_t!)
debug_warn : CXXFLAGS += -pedantic -Wextra 
debug_warn : debug
debug : all
release : CXXFLAGS += -O2
release : all

lib : CXXFLAGS += -fPIC
lib : debug libSudokuLib.so

libSudokuLib.so : $(LIB_OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared -o libSudokuLib.so $(LIB_OBJS)

all : sudoku run_tests

sudoku : $(OBJS) $(QT_OBJS)
	$(CXX) $(CPPFLAGS) $(QTDEF) $(CXXFLAGS) $(QTFLAGS) $^ -o $@

run_tests : $(LIB_OBJS) $(TEST_OBJS) gtest.a gmock.a
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@

# dependency stuff
.D_TARGET:
	mkdir -p $(DEPDIR)
	touch $@

.PRECIOUS: .D_TARGET

# GTEST building stuff don't touch me
GTEST_SRCS_ = $(GTEST_DIR)/src/*.cc $(GTEST_DIR)/src/*.h $(GTEST_HEADERS)
GMOCK_SRCS_ = $(GMOCK_DIR)/src/*.cc $(GMOCK_HEADERS)

gtest-all.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) -c \
           $(GTEST_DIR)/src/gtest-all.cc

gmock-all.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock-all.cc

gmock_main.o : $(GMOCK_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) -I$(GMOCK_DIR) $(CXXFLAGS) \
            -c $(GMOCK_DIR)/src/gmock_main.cc

gmock.a : gmock-all.o gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.o : $(GTEST_SRCS_)
	$(CXX) $(CPPFLAGS) -I$(GTEST_DIR) $(CXXFLAGS) -c \
           $(GTEST_DIR)/src/gtest_main.cc

gtest.a : gtest-all.o
	$(AR) $(ARFLAGS) $@ $^

gtest_main.a : gtest-all.o gtest_main.o
	$(AR) $(ARFLAGS) $@ $^

# QT stuff
%Qt.o : %Qt.o .D_TARGET
	$(MAKEDEPEND_QT);
	@cp $(df).d $(df).P;
#	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
#		-e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).P; 
	@rm -f $(df).d
	$(CXX) $(CPPFLAGS) $(QTDEF) $(CXXFLAGS) -o $@ -c $<

# tests
%Test.o : %Test.cpp .D_TARGET $(GMOCK_HEADERS)
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

-include $(QT_SRCS:%.cpp=$(DEPDIR)/%.o.P)
-include $(TEST_SRCS:test/%.cpp=$(DEPDIR)/%.o.P)
-include $(SRCS:%.cpp=$(DEPDIR)/%.o.P)

clean:
	$(RM) $(OBJS) $(TEST_OBJS) $(QT_OBJS) \
		gtest.a gtest_main.a gtest-all.o gtest_main.o \
		.D_TARGET sudoku run_tests
	rm -rf $(DEPDIR)