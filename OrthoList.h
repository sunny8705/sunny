/*
 * MultiThread.h
 *
 *  Created on: 2013-1-7
 *      Author: root
 */
#ifndef ORTHOLIST_H_
#define ORTHOLIST_H_

#include <iostream>
#include <assert.h>
#define SAFE_DELETE(p)  { if(p) { delete p; p = NULL; } }
#define ENUM            enum
#define CONST  		const
#define VOID			void
#define BOOL			bool
#define INLINE			inline
#define ASSERT			assert

template<class ListDataType> class OrthoList;
template<class ListDataType> class ConstRowIter;
template<class ListDataType> class ConstColIter;

//class of cross chain node define
template<class NodeDataType>
class OLNode
{
	//friendly class
	friend class ConstRowIter<NodeDataType>;
	friend class ConstColIter<NodeDataType>;
	friend class OrthoList<NodeDataType>;

public:
	OLNode(size_t iPos = 0, size_t jPos = 0, NodeDataType elem = 0)
		:m_wRowPos(iPos), m_wColPos(jPos), m_Elem(elem),
		 m_pRight(NULL), m_pDown(NULL)
	{}

	OLNode(CONST OLNode<NodeDataType> &node)
		:m_wRowPos(node.m_wRowPos), m_wColPos(node.m_wColPos),
		 m_Elem(node.m_Elem),
		 m_pRight(NULL), m_pDown(NULL)
	{}

	~OLNode()
	{};

	size_t GetRowPos() CONST { return m_wRowPos; }
	size_t GetColPos() CONST { return m_wColPos; }
	NodeDataType GetElemData() CONST { return m_Elem; }

	OLNode<NodeDataType>* GetDownPtr() CONST { return m_pDown; }
	OLNode<NodeDataType>* GetRightPtr() CONST { return m_pRight; }
	VOID SetElemData(NodeDataType val) { m_Elem = val; }

private:
	size_t m_wRowPos;
	size_t m_wColPos;
	NodeDataType m_Elem;
	OLNode<NodeDataType> *m_pRight;
	OLNode<NodeDataType> *m_pDown;
};

//definition of cross chain
//just data struct
template<class ListDataType>
class OrthoList
{
	friend class ConstRowIter<ListDataType>;
	friend class ConstColIter<ListDataType>;

public:
	OrthoList();
	OrthoList(size_t rowNum, size_t collNum);
	OrthoList(CONST ListDataType *buffer, size_t row, size_t coll);  //one array
	OrthoList(CONST OrthoList<ListDataType> &cMatrix);
	~OrthoList();

public:
	typedef OLNode<ListDataType> *OLink;

	//内部类——行迭代器遍历类
	class ConstRowIter
	{
	public:
		typedef ListDataType	value_type;
		typedef OLNode<value_type> node_type;
		typedef OrthoList<value_type> list_type;

		//初始化迭代器，记录初始化行列
		ConstRowIter() {};
		//找到olist的第一个有效行首元素
		ConstRowIter(OLink node, const list_type *olist)
			:_current(node),
			m_iNum(0),
			m_pOList(olist)
		{
			m_iCurrentRow = m_pOList->availableRow[0];  //minRow
		}
		~ConstRowIter(){};

		//遍历链表方式：只能行优先
		ConstRowIter& operator++()
		{
			m_iNum++;
			_current = _current->m_pRight;
			AdjustRowImpl__();
			return *this;
		}

		//OListIter& operator++(int)
		//temporary object reference
		const ConstRowIter operator++(int)
		{
			ConstRowIter temp = *this;
			++*this;
			return temp;
		}

		//一般的遍历方式
		node_type& operator*() const
		{
			return *_current;
		}
		node_type* operator->() const
		{
			return _current;
		}
		node_type* get() const
		{
			return _current;
		}

		//判断结束
		bool valid() const
		{
			return m_iNum != m_pOList->GetTotalSize();
		}

		//重载二目运算符
		BOOL operator ==(const ConstRowIter& rhs) const
		{
			return _current == rhs._current;
		}
		BOOL operator !=(const ConstRowIter& rhs) const
		{
			return _current != rhs._current;
		}

	private:
		inline VOID AdjustRowImpl__()
		{
			while(!_current)
			{
				//调节_current指向下一个有效行第一个元素
				if(++m_iCurrentRow < m_pOList->m_wRowSize)
				{
					_current = m_pOList->m_pRowHead[m_iCurrentRow];
				}
			}
		}
		const list_type *m_pOList;
		node_type *_current;  //内部链表元素遍历器
		size_t    m_iCurrentRow;
		size_t    m_iNum;
	};
	typedef ConstRowIter const_row_iterator;


	//列遍历迭代器类
	class ConstColIter
	{
	public:
		typedef ListDataType  value_type;
		typedef OLNode<value_type> node_type;
		typedef OrthoList<value_type> list_type;

		//初始化迭代器，记录初始化行列
		ConstColIter(){};
		ConstColIter(OLink node, const list_type *olist)
			:_current(node),
			m_iNum(0),
			m_pOList(olist)
		{
			m_iCurrentCol = m_pOList->availableCol[0]; //minCol
		}
		~ConstColIter(){};

		//遍历链表方式：只能列优先
		ConstColIter& operator++()
		{
			m_iNum++;
			_current = _current->m_pDown;
			AdjuestColImpl__();
			return *this;
		}

		//OListIter& operator++(int)
		//temporary object reference
		const ConstColIter operator++(int)
		{
			ConstColIter temp = *this;
			++*this;
			return temp;
		}

		//一般的遍历方式
		node_type& operator*() const
		{
			return *_current;
		}
		node_type* operator->() const
		{
			return _current;
		}
		node_type* get() const
		{
			return _current;
		}
		//判断结束
		bool valid() const
		{
			return m_iNum != m_pOList->GetTotalSize();
		}

		//重载二目运算符
		BOOL operator ==(ConstColIter &rhs) const
		{
			return _current == rhs._current;
		}
		BOOL operator !=(ConstColIter &rhs) const
		{
			return _current != rhs._current;
		}

	private:
		VOID AdjuestColImpl__()
		{
			while(!_current)
			{
				//调节_current指向下一个有效列第一个元素
				if(++m_iCurrentCol < m_pOList->m_wColSize)
				{
					_current = m_pOList->m_pColHead[m_iCurrentCol];
				}
			}
		}
		const list_type *m_pOList;
		node_type *_current;
		size_t m_iCurrentCol;
		size_t m_iNum;
	};
	typedef ConstColIter const_col_iterator;

	//迭代器
	const_row_iterator rBegin() const
	{
		return (const_row_iterator(m_pRowHead[ availableRow[0] ], this));
	}
	const_row_iterator rEnd() const
	{
		return (const_row_iterator(rowCapNode, this));
	}
	const_col_iterator cBegin() const
	{
		return (const_col_iterator(m_pColHead[ availableCol[0] ], this));
	}
	const_col_iterator cEnd() const
	{
		return (const_col_iterator(colCapNode, this));
	}

	size_t GetRowSize() CONST { return m_wRowSize; }
	size_t GetColSize() CONST { return m_wColSize; }
	size_t GetTotalSize() CONST { return m_wTotalSize; }
	CONST OLink GetRow(size_t row) CONST { return m_pRowHead[row]; }
	CONST OLink GetCol(size_t col) CONST { return m_pColHead[col]; }
	CONST OLink GetFirstNodeByRow() CONST;
	CONST OLink GetFirstNodeByCol() CONST;
	VOID InsertNode(OLink& node);  //创建十字链表
	VOID InsertNode(size_t iPos, size_t jPos, ListDataType elem);
	VOID InsertInRow(OLink& node);  //插入元素到行
	VOID InsertInCol(OLink& node);  //插入元素到列
	VOID CreateListHeads();         //创建行列头结点
	VOID Free();                    //释放十字链表

	//深拷贝
	template<class ListDataType>
	friend VOID Copy(OrthoList<ListDataType>& src, CONST OrthoList<ListDataType>& dest);

private:
	OLink  *m_pRowHead;
	OLink  *m_pColHead;
	size_t m_wRowSize;
	size_t m_wColSize;
	size_t m_wTotalSize;

	size_t availableRow[2];  //min max
	size_t availableCol[2]; //min max
	OLink  rowCapNode;
	OLink  colCapNode;
};

//OrthoList类的实现
template<class ListDataType>
OrthoList<ListDataType>::OrthoList()
	:m_wRowSize(0), m_wColSize(0), m_wTotalSize(0),
	m_pRowHead(NULL), m_pColHead(NULL)
{}

template<class ListDataType>
OrthoList<ListDataType>::OrthoList(CONST ListDataType *buffer, size_t row, size_t coll)
	:m_wRowSize(row), m_wTotalSize(0), m_wColSize(coll),
	m_pRowHead(NULL), m_pColHead(NULL)
{
	CreateListHeads();
	for(size_t i=0; i<row*coll; i++)
	{
		if(buffer[i])
		{
			this->InsertNode(i/coll, i%coll, buffer[i]);
		}
	}
}

template<class ListDataType>
OrthoList<ListDataType>::OrthoList(size_t rowNum, size_t collNum)
	:m_wRowSize(rowNum), m_wColSize(collNum), m_wTotalSize(0),
	m_pRowHead(NULL), m_pColHead(NULL)
{
	this->CreateListHeads();
}

template<class ListDataType>
OrthoList<ListDataType>::OrthoList(CONST OrthoList<ListDataType> &cMatrix)
	:m_wTotalSize(0),
	m_pRowHead(NULL), m_pColHead(NULL)
{
	this->Copy(*this, cMatrix);
}

template<class ListDataType>
OrthoList<ListDataType>::~OrthoList()
{
	this->Free();
}

template<class ListDataType>
VOID OrthoList<ListDataType>::Free(void)
{
	//列优先释放内存
	for(size_t i=0; i<this->m_wColSize; i++)
	{
		OLink temp;
		while(this->m_pColHead[i])
		{
			temp = this->m_pColHead[i];
			this->m_pColHead[i] = this->m_pColHead[i]->m_pDown;
			SAFE_DELETE(temp);
		}
	}
	SAFE_DELETE(this->m_pColHead);
	SAFE_DELETE(this->m_pRowHead);
	SAFE_DELETE(this->rowCapNode);
}

template<class ListDataType>
VOID OrthoList<ListDataType>::CreateListHeads(void)
{
	//no exception handle
	this->m_pRowHead = new OLink[this->m_wRowSize];  //create table of row head
	this->m_pColHead = new OLink[this->m_wColSize];  //create table of col head
	for(size_t i=0; i<this->m_wRowSize; i++)
	{
		this->m_pRowHead[i] = NULL;
	}
	for(size_t i=0; i<this->m_wColSize; i++)
	{
		this->m_pColHead[i] = NULL;
	}

	this->availableRow[0] = this->m_wRowSize - 1;
	this->availableRow[1] = 0;

	this->availableCol[0] = this->m_wColSize - 1;
	this->availableCol[1] = 0;

	this->rowCapNode = new OLNode<ListDataType>;
	this->colCapNode = new OLNode<ListDataType>;
	this->m_pRowHead[ this->availableRow[1] ] = this->rowCapNode;
	this->m_pColHead[ this->availableCol[1] ] = this->colCapNode;
}

template<class ListDataType>
CONST typename OrthoList<ListDataType>::OLink
	OrthoList<ListDataType>::GetFirstNodeByRow() CONST
	{
		int i(0);
		while(!this->m_pRowHead[i] && i<this->m_wRowSize)
		{
			i++;
		}
		return this->m_pRowHead[i];
	}

template<class ListDataType>
CONST typename OrthoList<ListDataType>::OLink
	OrthoList<ListDataType>::GetFirstNodeByCol() CONST
	{
		int i(0);
		while(!this->m_pColHead[i] && i<this->m_wColSize)
		{
			i++;
		}
		return this->m_pColHead[0];
	}

template<class ListDataType>
VOID OrthoList<ListDataType>::InsertNode(OLink &node)
{
	ASSERT(node != NULL);
	if(!this->m_pColHead || !this->m_pRowHead)
	{
		this->CreateListHeads();
	}
	this->m_wTotalSize++;
	this->InsertInRow(node);
	this->InsertInCol(node);
}

template<class ListDataType>
VOID OrthoList<ListDataType>::InsertNode(size_t iPos, size_t jPos, ListDataType elem)
{
	OLink node = new OLNode<ListDataType>(iPos, jPos, elem);
	this->InsertNode(node);
}

template<class ListDataType>
VOID OrthoList<ListDataType>::InsertInRow(OLink &node)
{
	ASSERT(node != NULL);
	size_t i = node->m_wRowPos;
	size_t j = node->m_wColPos;

	size_t &minRow = this->availableRow[0];
	size_t &maxRow = this->availableRow[1];

	if(i < minRow)
	{
		minRow = i;  //update min row.
	}
	if(i > maxRow)//update max row
	{
		this->m_pRowHead[i] = node; //insert the node in the i row
		//exchange the right of node and the right of last element of maxRow row
		if(this->m_pRowHead[maxRow] == this->rowCapNode)  //there is no element in ths maxRow row
		{
			this->m_pRowHead[maxRow] = NULL;
		}
		else  //at least have one elem in maxRow, and look for the last elem of maxRow
		{
			OLink bg;
			for(bg = this->m_pRowHead[maxRow]; bg->m_pRight != this->rowCapNode; bg = bg->m_pRight);

			bg->m_pRight = NULL;
		}
		node->m_pRight = this->rowCapNode;

		maxRow = i;  //update maxRow
	}
	else if(i == maxRow)  //insert node into maxRow
	{
		//insert the front of row
		if(this->m_pRowHead[i] == this->rowCapNode || j < this->m_pRowHead[i]->m_wColPos)
		{
			node->m_pRight = this->m_pRowHead[i];
			this->m_pRowHead[i] = node;
		}
		else //look for the index to insert in the row
		{
			OLink bg;
			for(bg = this->m_pRowHead[i]; //
				 bg->m_pRight != this->rowCapNode && bg->m_pRight->m_wColPos < j; //
				 bg = bg->m_pRight);
			//insert the node behind bg
			node->m_pRight = bg->m_pRight;
			bg->m_pRight = node;
		}
	}
	else //i < maxRow
	{
		//直接插入：空，或者列坐标大于此行链表中的最大列坐标
		//同一行里面，链表按照列的坐标排序，
		//行第一个元素(0,2)，插入(0,1)时。
		if(this->m_pRowHead[i] == NULL || this->m_pRowHead[i]->m_wColPos > j)
		{
			node->m_pRight = this->m_pRowHead[i];
			this->m_pRowHead[i] = node;
		}
		else  //查寻在行表中的插入位置
		{
			OLink bg;
			for(bg = this->m_pRowHead[i]; bg->m_pRight && bg->m_pRight->m_wColPos < j; bg = bg->m_pRight);

			node->m_pRight = bg->m_pRight;
			bg->m_pRight = node;
		}
	}
}

template<class ListDataType>
VOID OrthoList<ListDataType>::InsertInCol(OLink &node)
{
	ASSERT(node != NULL);
	size_t i = node->m_wRowPos;
	size_t j = node->m_wColPos;

	size_t &minCol = this->availableCol[0];
	size_t &maxCol = this->availableCol[1];

	if(j < minCol) //update min col
	{
		minCol = j;
	}
	if(j > maxCol) //update max col
	{
		this->m_pColHead[j] = node;
		if(this->m_pColHead[maxCol] == this->colCapNode)
		{
			this->m_pColHead[maxCol] = NULL;
		}
		else
		{
			OLink bg;
			for(bg = this->m_pColHead[maxCol]; bg->m_pDown != this->colCapNode; bg=bg->m_pDown);
			bg->m_pDown = NULL;
		}
		node->m_pDown = this->colCapNode;
		maxCol = j;
	}
	else if(j == maxCol)
	{
		if(this->m_pColHead[j] == this->colCapNode || i < this->m_pColHead[j]->m_wRowPos)
		{
			node->m_pDown = this->m_pColHead[j];
			this->m_pColHead[j] = node;
		}
		else
		{
			OLink bg;
			for(bg=this->m_pColHead[j];
				 bg->m_pDown != this->colCapNode && bg->m_pDown->m_wRowPos < i;
				 bg = bg->m_pDown);

			node->m_pDown = bg->m_pDown;
			bg->m_pDown = node;
		}
	}
	else //j<maxCol
	{
		if(this->m_pColHead[j] == NULL || this->m_pColHead[j]->m_wColPos > i)
		{
			node->m_pDown = this->m_pColHead[j];
			this->m_pColHead[j] = node;
		}
		else
		{
			OLink bg;
			for(bg = this->m_pColHead[j]; bg->m_pDown && bg->m_pDown->m_wRowPos < i; bg=bg->m_pDown);
			node->m_pDown = bg->m_pDown;
			bg->m_pDown = node;
		}
	}
}

template<class ListDataType>
VOID PrintMatrixByRow(CONST OrthoList<ListDataType> &olist)
{
	size_t last_row(0), last_col(-1);
	bool isNewRow(false);
	OrthoList<ListDataType>::const_row_iterator lastIter;

	for(OrthoList<ListDataType>::const_row_iterator iter = olist.rBegin();
		 iter != olist.rEnd(); iter++)
	{
		size_t cur_row = iter->GetRowPos();
		size_t cur_col = iter->GetColPos();

		if(last_row < cur_row)
		{
			isNewRow = true;
		}

		if(isNewRow)
		{
			for(size_t c(last_col+1); c < olist.GetColSize(); c++)
			{
				std::cout.width(6);
				std::cout << 0 << " ";
			}
			std::cout << "\n";

			for(size_t r = last_row+1; r < cur_row; r++)
			{
				for(size_t c(0); c < olist.GetColSize(); c++)
				{
					std::cout.width(6);
					std::cout << 0 << " ";
				}
				std::cout << "\n";
			}

			last_col = 0;
			isNewRow = false;

			for(size_t c(last_col); c < cur_col; c++)
			{
				std::cout.width(6);
				std::cout << 0 << " ";
			}
		}
		else
		{
			for(size_t c(last_col); c < cur_col - 1; c++)
			{
				std::cout.width(6);
				std::cout << 0 << " ";
			}
		}

		std::cout.width(6);
		std::cout << iter->GetElemData() << " ";

		last_row = cur_row;
		last_col = cur_col;
		lastIter = iter;
	}

	for(size_t c = lastIter->GetColPos()+1; c < olist.GetColSize(); c++)
	{
		std::cout.width(6);
		std::cout << 0 << " ";
	}
	std::cout << "\n";
}

template<class ListDataType>
VOID Copy(OrthoList<ListDataType> &src, CONST OrthoList<ListDataType> &dest)
{
	src.m_wColSize = dest.m_wColSize;
	src.m_wRowSize = dest.m_wRowSize;

	src.CreateListHeads();

	for(OrthoList<ListDataType>::const_row_iterator iter = dest.rBegin();
		 iter != dest.rEnd(); ++iter)
	{
		src.InsertNode(iter->GetRowPos(), iter->GetColPos(), iter->GetElemData());
	}
}


#endif /* ORTHOLIST_H_ */
