#include <stdio.h>


class CNode;
typedef bool (*pfnCb_t)(const CNode*, void* f_pData);

class CNode
{
public:
	CNode(int f_val): m_pLeft(NULL), m_pRight(NULL), m_val(f_val) {}

	CNode* addLeft(int f_val)
	{
		if(m_pLeft)
			return NULL;

		CNode* pN = new CNode(f_val);
		m_pLeft = pN;
		return pN;
	}
	CNode* addRight(int f_val)
	{
		if(m_pRight)
			return NULL;

		CNode* pN = new CNode(f_val);
		m_pRight = pN;
		return pN;
	}

	bool traverseInOrder(pfnCb_t f_pFunc, void* f_pData)
	{
		if(!f_pFunc)
			return true;

		if(m_pLeft && !m_pLeft->traverseInOrder(f_pFunc, f_pData))
			return false;
		if( !f_pFunc(this, f_pData) )
			return false;
		if(m_pRight && !m_pRight->traverseInOrder(f_pFunc, f_pData))
			return false;

		return true;
	}

	// Accessors
	const CNode* getLeft () const { return m_pLeft;  }
	const CNode* getRgiht() const { return m_pRight; }

	const int& getVal() const { return m_val; }

private:
	CNode(CNode* f_pParent, int f_val): m_pLeft(NULL), m_pRight(NULL), m_val(f_val) {}

	CNode();

private:
	CNode* m_pLeft;
	CNode* m_pRight;

	int m_val;
};


bool isBST(const CNode* f_pNode, void* f_pData)
{
	int val = f_pNode->getVal();
	printf("%s: %d\n", __FUNCTION__, val);

	int* pData = (int*)f_pData;
	bool bOK = (*pData <= val);
	*pData = val;

	return bOK;
}


int main(int, char**)
{
	CNode root(10);
	CNode* p;

	p = root.addLeft(6);
	p->addLeft(4);
	p->addRight(8);
	//p->addRight(11);

	p = root.addRight(14);
	p->addLeft(12);
	p->addRight(16);

	int valMin = -0x7FFFFFFF;
	printf("The tree is%s a BST\n", root.traverseInOrder(isBST, &valMin) ? "" : " NOT");

	return 0;
}

