/*
 * OrthoListTest.h
 *
 *  Created on: 2013-1-14
 *      Author: root
 */

#ifndef ORTHOLISTTEST_H_
#define ORTHOLISTTEST_H_

#include "OrthoList.h"

CONST int SparseMat[] =
{
  	1, 0,0, 0,0, 0,0,
		0,12,9, 0,0, 0,0,
		3,0,0, 0,0,14,0,
		0, 0,24,0,0, 0,0,
		0,18,0, 0,0, 0,0,
		15,0,0, -7,0,0,0,
		0,1,0, 0,0, 0,0
};

class OrthoListTest
{
public:
	~OrthoListTest()
	{
		DestroyOList();
	}

	static CONST int Row = 7;
	static CONST int Coll = 7;
	static OrthoList<int> *OList;
	static VOID CreateOList()
	{
		std::cout << std::endl << "CreateOList" << std::endl;
		std::cout << "/*-------------------------------------------------------*/" << std::endl;
		OList = new OrthoList<int>(Row, Coll);
		for(int i=0; i<sizeof(SparseMat)/sizeof(SparseMat[0]); i++)
		{
			if(SparseMat[i])
			{
				OList->InsertNode(i/Coll, i%Coll, SparseMat[i]);
			}
		}
		PrintMatrixByRow(*OList);
		std::cout << "/*-------------------------------------------------------*/" << std::endl;
	}

	static VOID OLIterTest()
	{
		std::cout << std::endl << "OLIterTest" << std::endl;
		std::cout << "/*-------------------------------------------------------*/" << std::endl;
		for(OrthoList<int>::const_row_iterator iter = OList->rBegin();
				iter != OList->rEnd();
				++iter)
		{
			std::cout << "( " << iter->GetRowPos() << " , " << iter->GetColPos();
			std::cout << " , " << iter->GetElemData() << " >\n";
		}
		std::cout << "/*-------------------------------------------------------*/" << std::endl;
		std::cout << "/*-------------------------------------------------------*/" << std::endl;
		for(OrthoList<int>::const_col_iterator iter = OList->cBegin(); iter != OList->cEnd(); ++iter)
		{
			std::cout << "( " << iter->GetRowPos() << " , " << iter->GetColPos();
			std::cout << " , " << iter->GetElemData() << " >\n";
		}
		std::cout << "/*-------------------------------------------------------*/" << std::endl;
	}

	static VOID DestroyOList()
	{
		if(OList)
		{
			delete OList;
		}
		OList = 0;
	}
	static VOID BeginTest()
	{
		CreateOList();
		OLIterTest();
		DestroyOList();
	}
};

OrthoList<int>* OrthoListTest::OList;

#endif /* ORTHOLISTTEST_H_ */
