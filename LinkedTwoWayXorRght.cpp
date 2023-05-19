#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable: 4326 4996 6001 6054)

//#define DebugON

#define XOR2(x, y)    (NodePtr)((unsigned int)x ^ (unsigned int)y)
#define XOR3(x, y, z) (NodePtr)((unsigned int)x ^ (unsigned int)y ^ (unsigned int)z)

typedef struct node {
	int nData;
	struct node *link;
}	Node, *NodePtr;

typedef struct {
	NodePtr pLeft;
	NodePtr pRght;
}	TwoWay, *TwoWayPtr;

#include "CmdListTwoR.h"

void main()
{
	int InsertNode(TwoWayPtr pTwo, int nData);
	int DeleteNode(TwoWayPtr pTwo);
	int MoveRight(TwoWayPtr pTwo);
	int MoveLeft(TwoWayPtr pTwo);
	int MoveRepeatedly(TwoWayPtr pTwo, int nCtr);
	int OutputData(TwoWayPtr pTwo, int nCtr);
	void PrintList(TwoWayPtr pTwo, const char *pCmnd);
	int nNdx = 0;
	int bSuccess = true;
	TwoWay aTwoWay;
	aTwoWay.pLeft = aTwoWay.pRght = NULL;	// ����� linked list�� �ʱ�ȭ
	int bDoing = true;
	char sMsg[100], *pMsg = sMsg;
	while (bDoing) {
		int nData;
		const char *pCmnd = sCmnds[nNdx++];		// ��ɾ �ϳ��� �����´�.
		switch (pCmnd[0] | 0x20) {			// �ҹ��ڷ�
		case 'i':							// ���ο� ��带 �����ʿ� ����
			nData = atoi(pCmnd + 1);
			bSuccess = InsertNode(&aTwoWay, nData);
			break;
		case 'd':							// �������� ��带 ����
			bSuccess = DeleteNode(&aTwoWay);
			break;
		case 'l':							// �������� �ѹ� �̵�
			bSuccess = MoveLeft(&aTwoWay);
			break;
		case 'r':							// ���������� �ѹ� �̵�
			bSuccess = MoveRight(&aTwoWay);
			break;
		case 'm':							// ����(+)/��(-)������ n�� �̵�
			nData = atoi(pCmnd+1);
			bSuccess = MoveRepeatedly(&aTwoWay, nData);
			break;
		case 'o':							// n�� �̵� �� ������ ����� data�� ��ȯ
			nData = atoi(pCmnd+1);
			if((nData = OutputData(&aTwoWay, nData)) >= 0)
				*pMsg++ = nData;
			else
				bSuccess = false;
			break;
		case 'e':							// ��
			bSuccess = true;
			bDoing = false;
			break;
		default:
			bSuccess = false;
		}
		if (bSuccess == false)
			printf("---> %d-th command(%s) error, ....\n", nNdx, pCmnd);
#ifdef  DebugON
		PrintList(&aTwoWay, pCmnd);
#endif
	}
	*pMsg = NULL;
	printf("%s\n\n", sMsg);
}

int MoveRight(TwoWayPtr pTwo)
{	// ���������� �ѹ� �̵��Ѵ�.
	// ����(����)�ϸ� true(false)�� return�Ѵ�.
	if (pTwo->pRght == NULL)
		return false;
	NodePtr pTemp = pTwo->pRght;
	pTwo->pRght = XOR2(pTwo->pRght->link, pTwo->pLeft);
	pTwo->pLeft = pTemp;
	return true;
}

int MoveLeft(TwoWayPtr pTwo)
{	// �������� �ѹ� �̵��Ѵ�.
	// ����(����)�ϸ� true(false)�� return�Ѵ�.
	if (pTwo->pLeft == NULL)
		return false;
	NodePtr pTemp = pTwo->pLeft;
	pTwo->pLeft = XOR2(pTwo->pLeft->link, pTwo->pRght);
	pTwo->pRght = pTemp;
	return true;
}

int InsertNode(TwoWayPtr pTwo, int nData)
{	// ������ ����Ʈ�� �տ� nData�� ������ ��带 �����Ѵ�.
	// ����(����)�ϸ� true(false)�� return�Ѵ�.
	NodePtr pNew = (Node *)malloc(sizeof(Node));
	if (pNew) {
		pNew->nData = nData;
		NodePtr pLeft = pTwo->pLeft;
		NodePtr pRght = pTwo->pRght;
		pNew->link = XOR2(pLeft, pRght);
		if (pLeft)
			pLeft->link = XOR3(pLeft->link, pRght, pNew);
		if (pRght)
			pRght->link = XOR3(pRght->link, pLeft, pNew);
		pTwo->pRght = pNew;
	}
	return pNew != NULL;
}

int DeleteNode(TwoWayPtr pTwo)
{	// ������ ����Ʈ�� ù ��带 �����Ѵ�.
	// ����(����)�ϸ� true(false)�� return�Ѵ�.
	NodePtr pDel = pTwo->pRght;
	if (pDel) {
		NodePtr pLeft = pTwo->pLeft;
		NodePtr pRght = pTwo->pRght = XOR2(pDel->link, pLeft);
		if (pLeft)
			pLeft->link = XOR3(pLeft->link, pDel, pRght);
		if (pRght)
			pRght->link = XOR3(pRght->link, pDel, pLeft);
		free(pDel);
	}
	return pDel != NULL;
}

int MoveRepeatedly(TwoWayPtr pTwo, int nCtr)
{	// ����(+)/��(-)������ nCtr�� �̵��Ѵ�.
	// ����(����)�ϸ� true(false)�� return�Ѵ�.
	typedef int(*MoveFtnPtr)(TwoWayPtr pTwo);
	MoveFtnPtr pMoveFtn = MoveRight;
	if (nCtr < 0) {
		pMoveFtn = MoveLeft;
		nCtr = -nCtr;
	}
	for (int i = 0; i < nCtr; i++)
		if ((*pMoveFtn)(pTwo) == false)
			return false;
	return true;
}

int OutputData(TwoWayPtr pTwo, int nCtr)
{	// ����(+)/��(-)������ nCtr�� �̵��Ͽ� ������ ����� �����͸� ��ȯ�Ѵ�.
	// ����(����)�ϸ� ������ ����� nData(-1)�� return�Ѵ�.
	return (MoveRepeatedly(pTwo, nCtr) && pTwo->pRght) ? pTwo->pRght->nData : -1;
}

void PrintList(TwoWayPtr pTwo, const char *pCmnd)
{
	TwoWay aTwoWay = *pTwo;
	TwoWayPtr ptr = &aTwoWay;
	while (MoveLeft(ptr));
	printf("%s: ", pCmnd);
	int bDoing = true;
	while (bDoing) {
		if (ptr->pLeft)
			printf(" %d", ptr->pLeft->nData);
		if (ptr->pLeft == pTwo->pLeft)
			printf(" <--+-->");
		bDoing = MoveRight(ptr);
	}
	putchar('\n');
}

