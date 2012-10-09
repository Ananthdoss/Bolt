/***************************************************************************                                                                                     
*   Copyright 2012 Advanced Micro Devices, Inc.                                     
*                                                                                    
*   Licensed under the Apache License, Version 2.0 (the "License");   
*   you may not use this file except in compliance with the License.                 
*   You may obtain a copy of the License at                                          
*                                                                                    
*       http://www.apache.org/licenses/LICENSE-2.0                      
*                                                                                    
*   Unless required by applicable law or agreed to in writing, software              
*   distributed under the License is distributed on an "AS IS" BASIS,              
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.         
*   See the License for the specific language governing permissions and              
*   limitations under the License.                                                   

***************************************************************************/                                                                                     

#define TEST_DOUBLE 0
#define TEST_DEVICE_VECTOR 1
#define TEST_CPU_DEVICE 0
#define GOOGLE_TEST 1
#if (GOOGLE_TEST == 1)
#include "common/stdafx.h"
#include "common/myocl.h"

#include <bolt/cl/count.h>
#include <bolt/cl/functional.h>
#include <bolt/miniDump.h>
#include <bolt/unicode.h>

#include <gtest/gtest.h>
#include <boost/shared_array.hpp>
#include <array>
#include <algorithm>
class testCountIfFloatWithStdVector: public ::testing::TestWithParam<int>{
protected:
	int aSize;
public:
	testCountIfFloatWithStdVector():aSize(GetParam()){
	}
	
};


std::string InRange_CodeString = 
BOLT_CODE_STRING(
template<typename T>
// Functor for range checking.
struct InRange {
	InRange (T low, T high) {
		_low=low;
		_high=high;
	};

	bool operator() (const T& value) { 
		//printf("Val=%4.1f, Range:%4.1f ... %4.1f\n", value, _low, _high); 
		return (value >= _low) && (value <= _high) ; 
	};

	T _low;
	T _high;
};
);

BOLT_CREATE_TYPENAME(InRange<int>);
BOLT_CREATE_CLCODE(InRange<int>, InRange_CodeString);
BOLT_CREATE_TYPENAME(InRange<float>);
BOLT_CREATE_CLCODE(InRange<float>, InRange_CodeString);
BOLT_CREATE_TYPENAME(InRange<double>);
BOLT_CREATE_CLCODE(InRange<double>, InRange_CodeString);


TEST_P (testCountIfFloatWithStdVector, countFloatValueInRange)
{
	std::vector<float> A(aSize);
	std::vector<float> B(aSize);

	for (int i=0; i < aSize; i++) {
		A[i] = static_cast<float> (i+1);
		B[i] = A[i];
	};

	size_t stdCount = std::count_if (A.begin(), A.end(), InRange<float>(6,10)) ;
	size_t boltCount = bolt::cl::count_if (B.begin(), B.end(), InRange<float>(6,10)) ;

	EXPECT_EQ (stdCount, boltCount)<<"Failed as: STD Count = "<<stdCount<<"\nBolt Count = "<<boltCount<<std::endl;
	std::cout<<"STD Count = "<<stdCount<<std::endl<<"bolt Count = "<<boltCount<<std::endl;
}

TEST_P (testCountIfFloatWithStdVector, countFloatValueInRange2)
{
	std::vector<float> A(aSize);
	std::vector<float> B(aSize);

	for (int i=0; i < aSize; i++) {
		A[i] = static_cast<float> (i+1);
		B[i] = A[i];
	};

	size_t stdCount = std::count_if (A.begin(), A.end(), InRange<float>(1,10)) ;
	size_t boltCount = bolt::cl::count_if (B.begin(), B.end(), InRange<float>(1,10)) ;

	EXPECT_EQ (stdCount, boltCount)<<"Failed as: STD Count = "<<stdCount<<"\nBolt Count = "<<boltCount<<std::endl;
	std::cout<<"STD Count = "<<stdCount<<std::endl<<"bolt Count = "<<boltCount<<std::endl;
}

//
//Test case id: 6 (FAILED)

class countFloatValueOccuranceStdVect :public ::testing::TestWithParam<int>{
protected:
	int stdVectSize;
public:
	countFloatValueOccuranceStdVect():stdVectSize(GetParam()){}
};

TEST_P(countFloatValueOccuranceStdVect, floatVectSearchWithSameValue){
	std::vector<float> stdVect(stdVectSize);
	std::vector<float> boltVect(stdVectSize);
	
	float myFloatValue = 1.23f;

	for (int i =0; i < stdVectSize; i++){
		stdVect[i] = myFloatValue;
		boltVect[i] = stdVect[i];
	}

	size_t stdCount = std::count(stdVect.begin(), stdVect.end(), myFloatValue);
	size_t boltCount = bolt::cl::count(boltVect.begin(), boltVect.end(), myFloatValue);

	EXPECT_EQ(stdCount, boltCount)<<"Failed as: \nSTD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
	std::cout<<"STD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
}

TEST_P(countFloatValueOccuranceStdVect, floatVectSearchWithSameValue2){
	std::vector<float> stdVect(stdVectSize);
	std::vector<float> boltVect(stdVectSize);
	
	float myFloatValue2 = 9.87f;

	for (int i =0; i < stdVectSize; i++){
		stdVect[i] = myFloatValue2;
		boltVect[i] = stdVect[i];
	}

	size_t stdCount = std::count(stdVect.begin(), stdVect.end(), myFloatValue2);
	size_t boltCount = bolt::cl::count(boltVect.begin(), boltVect.end(), myFloatValue2);

	EXPECT_EQ(stdCount, boltCount)<<"Failed as: \nSTD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
	std::cout<<"STD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
}

INSTANTIATE_TEST_CASE_P (useStdVectWithFloatValues, countFloatValueOccuranceStdVect, ::testing::Values(1, 100, 1000, 10000, 100000));


//Test case id: 7 (Failed)
class countDoubleValueUsedASKeyInStdVect :public ::testing::TestWithParam<int>{
protected:
	int stdVectSize;
public:
	countDoubleValueUsedASKeyInStdVect():stdVectSize(GetParam()){}
};


TEST_P(countDoubleValueUsedASKeyInStdVect, doubleVectSearchWithSameValue){
	std::vector<double> stdVect(stdVectSize);
	std::vector<double> boltVect(stdVectSize);
	
	double myDoubleValueAsKeyValue = 1.23456789l;

	for (int i =0; i < stdVectSize; i++){
		stdVect[i] = myDoubleValueAsKeyValue;
		boltVect[i] = stdVect[i];
	}

	size_t stdCount = std::count(stdVect.begin(), stdVect.end(), myDoubleValueAsKeyValue);
	size_t boltCount = bolt::cl::count(boltVect.begin(), boltVect.end(), myDoubleValueAsKeyValue);

	EXPECT_EQ(stdCount, boltCount)<<"Failed as: \nSTD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
	std::cout<<"STD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
}

TEST_P(countDoubleValueUsedASKeyInStdVect, doubleVectSearchWithSameValue2){
	std::vector<double> stdVect(stdVectSize);
	std::vector<double> boltVect(stdVectSize);
	
	double myDoubleValueAsKeyValue2 = 9.876543210123456789l;

	for (int i =0; i < stdVectSize; i++){
		stdVect[i] = myDoubleValueAsKeyValue2;
		boltVect[i] = stdVect[i];
	}

	size_t stdCount = std::count(stdVect.begin(), stdVect.end(), myDoubleValueAsKeyValue2);
	size_t boltCount = bolt::cl::count(boltVect.begin(), boltVect.end(), myDoubleValueAsKeyValue2);

	EXPECT_EQ(stdCount, boltCount)<<"Failed as: \nSTD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
	std::cout<<"STD Count = "<<stdCount<<std::endl<<"Bolt Count = "<<boltCount<<std::endl;
}

//test case: 1, test: 1
TEST (testCountIf, intBtwRange) 
{
	int aSize = 1024;
	std::vector<int> A(aSize);
	
	for (int i=0; i < aSize; i++) {
		A[i] = rand() % 10 + 1;
	}

	std::iterator_traits<std::vector<int>::iterator>::difference_type stdInRangeCount = std::count_if (A.begin(), A.end(), InRange<int>(1,10)) ;
	std::iterator_traits<std::vector<int>::iterator>::difference_type boltInRangeCount = bolt::cl::count_if (A.begin(), A.end(), InRange<int>(1, 10)) ;

	EXPECT_EQ(stdInRangeCount, boltInRangeCount);
}


INSTANTIATE_TEST_CASE_P (useStdVectWithDoubleValues, countDoubleValueUsedASKeyInStdVect, ::testing::Values(1, 100, 1000, 10000, 100000));
INSTANTIATE_TEST_CASE_P (serialFloatValueWithStdVector, testCountIfFloatWithStdVector, ::testing::Values(10, 100, 1000, 10000, 100000));

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest( &argc, &argv[ 0 ] );

    //  Register our minidump generating logic
    bolt::miniDumpSingleton::enableMiniDumps( );

    int retVal = RUN_ALL_TESTS( );

    //  Reflection code to inspect how many tests failed in gTest
    ::testing::UnitTest& unitTest = *::testing::UnitTest::GetInstance( );

    unsigned int failedTests = 0;
    for( int i = 0; i < unitTest.total_test_case_count( ); ++i )
    {
        const ::testing::TestCase& testCase = *unitTest.GetTestCase( i );
        for( int j = 0; j < testCase.total_test_count( ); ++j )
        {
            const ::testing::TestInfo& testInfo = *testCase.GetTestInfo( j );
            if( testInfo.result( )->Failed( ) )
                ++failedTests;
        }
    }

    //  Print helpful message at termination if we detect errors, to help users figure out what to do next
    if( failedTests )
    {
        bolt::tout << _T( "\nFailed tests detected in test pass; please run test again with:" ) << std::endl;
        bolt::tout << _T( "\t--gtest_filter=<XXX> to select a specific failing test of interest" ) << std::endl;
        bolt::tout << _T( "\t--gtest_catch_exceptions=0 to generate minidump of failing test, or" ) << std::endl;
        bolt::tout << _T( "\t--gtest_break_on_failure to debug interactively with debugger" ) << std::endl;
        bolt::tout << _T( "\t    (only on googletest assertion failures, not SEH exceptions)" ) << std::endl;
    }
    std::cout << "Test Completed. Press Enter to exit.\n .... ";
    getchar();
	return retVal;
}


#else



#include "stdafx.h"
#include <vector>
#include <algorithm>

#include <bolt/cl/count.h>

//Count with a vector input
void testCount1(int aSize)
{
	std::vector<int> A(aSize);
	for (int i=0; i < aSize; i++) {
		A[i] = i+1;
	};

	bolt::cl::count (A.begin(), A.end(), 37);
};


// Count with an array input:
void testCount2()
{
	const int aSize = 13;
	int A[aSize] = {0, 10, 42, 55, 13, 13, 42, 19, 42, 11, 42, 99, 13};

	size_t count42 = bolt::cl::count (A, A+aSize, 42);
	size_t count13 = bolt::cl::count (A, A+aSize, 13);

	bolt::cl::control::getDefault().debug(bolt::cl::control::debug::Compile);

	std::cout << "Count42=" << count42 << std::endl;
	std::cout << "Count13=" << count13 << std::endl;
	std::cout << "Count7=" << bolt::cl::count (A, A+aSize, 7) << std::endl;
	std::cout << "Count10=" << bolt::cl::count (A, A+aSize, 10) << std::endl;
};



// This breaks the BOLT_CODE_STRING macro - need to move to a #include file or replicate the code.
std::string InRange_CodeString = 
BOLT_CODE_STRING(
template<typename T>
// Functor for range checking.
struct InRange {
	InRange (T low, T high) {
		_low=low;
		_high=high;
	};

	bool operator() (const T& value) { 
		//printf("Val=%4.1f, Range:%4.1f ... %4.1f\n", value, _low, _high); 
		return (value >= _low) && (value <= _high) ; 
	};

	T _low;
	T _high;
};
);

BOLT_CREATE_TYPENAME(InRange<int>);
BOLT_CREATE_CLCODE(InRange<int>, InRange_CodeString);
BOLT_CREATE_TYPENAME(InRange<float>);
BOLT_CREATE_CLCODE(InRange<float>, InRange_CodeString);







void testCountIf(int aSize) 
{
	std::vector<float> A(aSize);
	std::vector<float> B(aSize);
	for (int i=0; i < aSize; i++) {
		A[i] = static_cast<float> (i+1);
		B[i] = A[i];
	};

	std::cout << "STD Count7..15=" << std::count_if (A.begin(), A.end(), InRange<float>(7,15)) << std::endl;
	std::cout << "BOLT Count7..15=" << bolt::cl::count_if (B.begin(), B.end(), InRange<float>(7,15)) << std::endl;
}

void test_bug(int aSize) 
{
	//int aSize = 1024;
	std::vector<int> A(aSize);
	std::vector<int> B(aSize);
	for (int i=0; i < aSize; i++) {
		A[i] = rand() % 10 + 1;
		B[i] = A[i];
	}

	int stdInRangeCount = std::count_if (A.begin(), A.end(), InRange<int>(1,10)) ;
	int boltInRangeCount = bolt::cl::count_if (B.begin(), B.end(), InRange<int>(1, 10)) ;
	std:: cout << stdInRangeCount << "   "   << boltInRangeCount << "\n";
	//std::cout << "STD Count7..15=" << std::count_if (A.begin(), A.end(), InRange<float>(7,15)) << std::endl;
	//std::cout << "BOLT Count7..15=" << bolt::cl::count_if (A.begin(), A.end(), InRange<float>(7,15), InRange_CodeString) << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	testCount1(100);

	testCount2();

	testCountIf(1024);
	test_bug(1024);
	return 0;
}

#endif