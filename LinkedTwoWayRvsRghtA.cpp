#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable: 4326 4996 6001 6054)

// DebugON

typedef struct node {
	int nData;
	struct node *link;
}	Node, *NodePtr;

typedef struct {				// �������� �̵��ϱ� ���Ͽ� ���� 2���� �ʿ��մϴ�.
	Node *pLeft;				// left�� ���˴ϴ�.
	Node *pRght;				// rght�� ���˴ϴ�.
}	TwoWay, *TwoWayPtr;			// �� ������ �ϳ��� ��� ����Ϸ��� �մϴ�.

#include "CmdListTwoR.h"

void main()
{
	int InsertNode(TwoWayPtr pTwo, int nData);
	int DeleteNode(TwoWayPtr pTwo, int *pnData);
	int MoveRight(TwoWayPtr pTwo);
	int MoveLeft(TwoWayPtr pTwo);
	int MoveRepeatedly(TwoWayPtr pTwo, int nCtr);
	int OutputData(TwoWayPtr pTwo, int nCtr);
	void PrintList(TwoWayPtr pTwo, const char *pCmnd);

	int nNdx = 0;
	int bSuccess = true;
	TwoWay lnkdList;
	lnkdList.pLeft = lnkdList.pRght = NULL;	// ����� linked list�� �ʱ�ȭ
	int bDoing = true;
	char sMsg[100], *pMsg = sMsg;
	while (bDoing) {
		int nData;
		const char *pCmnd = sCmnds[nNdx++];		// ��ɾ �ϳ��� �����´�.
		switch (pCmnd[0] | 0x20) {
		case 'i':			// �����ʿ� ���ο� ��带 ����
			nData = atoi(pCmnd + 1);
			bSuccess = InsertNode(&lnkdList, nData);
			break;
		case 'd':			// �������� ù ��带 ����
			bSuccess = DeleteNode(&lnkdList, &nData);
			break;
		case 'l':			// �������� �ѹ� �̵�
			bSuccess = MoveLeft(&lnkdList);
			break;
		case 'r':			// ���������� �ѹ� �̵�
			bSuccess = MoveRight(&lnkdList);
			break;
		case 'm':			// ����(+)/��(-)������ n�� �̵�
			nData = atoi(pCmnd+1);
			bSuccess = MoveRepeatedly(&lnkdList, nData);
			break;
		case 'o':			// n�� �̵� ��, ������ ����� data�� ��ȯ
			nData = atoi(pCmnd+1);
			if ((nData = OutputData(&lnkdList, nData)) >= 0)
				*pMsg++ = nData;
			else
				bSuccess = false;
			break;
		case 'e':			// ��
			bSuccess = true;
			bDoing = false;
			break;
		default:			// Error Message
			bSuccess = false;
		}
		if (bSuccess == false)
			printf("---> %d-th command(%s) error, ....\n", nNdx, pCmnd);
#ifdef	DebugON
		PrintList(&lnkdList, pCmnd);
#endif
	}
	*pMsg = NULL;
	printf("%s\n\n", sMsg);
}

int MoveRight(TwoWayPtr pTwo)
{	// ���������� �ѹ� �̵��Ѵ�.
	// �����ϸ� true�� return�ϰ�, �̵��� �� ������ false�� return�Ѵ�.
	NodePtr pLeft = pTwo->pLeft, pRght = pTwo->pRght;
	if (pRght) {
		pTwo->pLeft = pRght;
		pTwo->pRght = pRght->link;
		pRght->link = pLeft;
	}
	return pRght != NULL;
}

int MoveLeft(TwoWayPtr pTwo)
{	// �������� �ѹ� �̵��Ѵ�.
	// �����ϸ� true�� return�ϰ�, �̵��� �� ������ false�� return�Ѵ�.
	NodePtr pLeft = pTwo->pLeft, pRght = pTwo->pRght;
	if (pLeft) {
		pTwo->pRght = pLeft;
		pTwo->pLeft = pLeft->link;
		pLeft->link = pRght;
	}
	return pLeft != NULL;
}

int InsertNode(TwoWay *pTwo, int nData)
{	// ������ ����Ʈ�� �տ� nData�� ������ ��带 �����Ѵ�.
	// �����ϸ� true, �����ϸ� false�� return�Ѵ�.
	Node *pNew = (Node *)malloc(sizeof(Node));
	if (pNew) {
		pNew->nData = nData;
		pNew->link = pTwo->pRght;
		pTwo->pRght = pNew;
	}
	return pNew != NULL;
}

int DeleteNode(TwoWayPtr pTwo, int *pnData)
{	// ������ ����Ʈ�� ù ����� ����Ÿ�� pnData�� �����ϰ�
	// �� ù ��带 �����Ѵ�.
	// �����ϸ� true, �����ϸ� false�� return�Ѵ�.
	Node *pDel = pTwo->pRght;
	if (pDel) {
		*pnData = pDel->nData;
		pTwo->pRght = pDel->link;
		free(pDel);
	}
	return pDel != NULL;
}

int MoveRepeatedly(TwoWayPtr pTwo, int nCtr)
{	// ����(+)/��(-)������ nCtr�� �̵��Ѵ�.
	// ����(����)�ϸ� true(false)�� return�Ѵ�.
	typedef int (*MoveFtnPtr)(TwoWayPtr pTwo);
	MoveFtnPtr pMoveFtn = MoveRight;
	if (nCtr < 0) {
		nCtr = -nCtr;
		pMoveFtn = MoveLeft;
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
	// ��ü �����͸� %d�� ����ϴµ� 
	// ������ �������� ����ϰ� 
	// �������� ������� ����Ѵ�.
	void PrintForward(NodePtr pNode);
	void PrintBackward(NodePtr pNode);

	printf("%s: ", pCmnd);
	PrintBackward(pTwo->pLeft);
	printf("<--+--> ");
	PrintForward(pTwo->pRght);
	putchar('\n');
}

void PrintForward(NodePtr pNode)
{
	if (pNode) {
		printf("%d ", pNode->nData);
		PrintForward(pNode->link);
	}
}

void PrintBackward(NodePtr pNode)
{
	if (pNode) {
		PrintBackward(pNode->link);
		printf("%d ", pNode->nData);
	}
}
