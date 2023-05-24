#include <stdio.h>
#include <stdlib.h>
#pragma warning(disable: 4326 4996 6001 6031 6054)

typedef struct node {
	int nData;
	struct node *llink;
	struct node *rlink;
}	Node, *NodePtr;

void main()
{
	NodePtr Insert(NodePtr pList, int nData, NodePtr pLeft);
	NodePtr Delete(NodePtr pList, NodePtr pDel);
	NodePtr Find(NodePtr pList, int nData);
	void PrintList(NodePtr pList);
	NodePtr pFirst = NULL;
	NodePtr pNode = NULL;
	while (1) {
		printf("[idfe] ? ");
		char sCmnd[10];
		gets_s(sCmnd);
		if (*sCmnd == 'e')
			return;
		int nData;
		if (*sCmnd == 'f' || *sCmnd == 'i') {
			printf("Data ? ");
			char sData[10];
			gets_s(sData);
			nData = atoi(sData);
		}
		switch (*sCmnd) {
		case 'i':
			pFirst = Insert(pFirst, nData, pNode);
			break;
		case 'd':
			if (pNode) {
				pFirst = Delete(pFirst, pNode);
				pNode = NULL;
			}
			else
				printf("First, assign a node to be deleted, ...\n"); 
			break;
		case 'f':
			pNode = Find(pFirst, nData);
			printf("%s\n", (pNode) ? "Good" : "Bad");
		}
		PrintList(pFirst);
	}
}

NodePtr Insert(NodePtr pList, int nData, NodePtr pLeft)
{
	// pLeft ��� �����ʿ� nData�� ������ ��带 �����Ѵ�.
	// pLeft�� NULL�̸� ù ���� �����Ѵ�.
	// pLeft�� ȣ���ϱ� ������ find�� ���Ͽ� ���� �����Ͽ��� �Ѵ�.
	NodePtr pNew = new Node;
	if (pNew) {
		pNew->nData = nData;
		NodePtr pRight = (pLeft) ? pLeft->rlink : pList;
		pNew->llink = pLeft;
		pNew->rlink = pRight;
		if (pLeft)
			pLeft->rlink = pNew;
		else
			pList = pNew;
		if (pRight)
			pRight->llink = pNew;
	}
	return pList;
}

NodePtr Delete(NodePtr pList, NodePtr pDel)
{
	// pDel�� �����Ѵ�.
	// pDel�� ȣ���ϱ� ������ find�� ���Ͽ� ���� �����Ͽ��� �Ѵ�.
	NodePtr pLeft  = pDel->llink;
	NodePtr pRight = pDel->rlink;
	if (pLeft)
		pLeft->rlink = pRight;
	else
		pList = pRight;
	if (pRight)
		pRight->llink = pLeft;
	delete pDel;
	return pList;
}

NodePtr Find(NodePtr pList, int nData)
{
	// nData�� ������ ��带 ã�µ� ������ NULL�� return �Ѵ�.
	if (pList == NULL || pList->nData == nData)
		return pList;
	return Find(pList->rlink, nData);
}

void PrintList(NodePtr pList)
{
	NodePtr pPrev = NULL;
	while (pList) {
		printf("%d ", pList->nData);
		pPrev = pList;
		pList = pList->rlink;
	}
	printf("<---> ");
	pList = pPrev;
	while (pList) {
		printf("%d ", pList->nData);
		pList = pList->llink;
	}
	printf("\n\n");
}
