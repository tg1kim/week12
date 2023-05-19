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
	aTwoWay.pLeft = aTwoWay.pRght = NULL;	// 양방향 linked list의 초기화
	int bDoing = true;
	char sMsg[100], *pMsg = sMsg;
	while (bDoing) {
		int nData;
		const char *pCmnd = sCmnds[nNdx++];		// 명령어를 하나씩 가져온다.
		switch (pCmnd[0] | 0x20) {			// 소문자로
		case 'i':							// 새로운 노드를 오른쪽에 삽입
			nData = atoi(pCmnd + 1);
			bSuccess = InsertNode(&aTwoWay, nData);
			break;
		case 'd':							// 오른쪽의 노드를 삭제
			bSuccess = DeleteNode(&aTwoWay);
			break;
		case 'l':							// 왼쪽으로 한번 이동
			bSuccess = MoveLeft(&aTwoWay);
			break;
		case 'r':							// 오른쪽으로 한번 이동
			bSuccess = MoveRight(&aTwoWay);
			break;
		case 'm':							// 오른(+)/왼(-)쪽으로 n번 이동
			nData = atoi(pCmnd+1);
			bSuccess = MoveRepeatedly(&aTwoWay, nData);
			break;
		case 'o':							// n번 이동 후 오른쪽 노드의 data를 반환
			nData = atoi(pCmnd+1);
			if((nData = OutputData(&aTwoWay, nData)) >= 0)
				*pMsg++ = nData;
			else
				bSuccess = false;
			break;
		case 'e':							// 끝
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
{	// 오른쪽으로 한번 이동한다.
	// 성공(실패)하면 true(false)를 return한다.
	if (pTwo->pRght == NULL)
		return false;
	NodePtr pTemp = pTwo->pRght;
	pTwo->pRght = XOR2(pTwo->pRght->link, pTwo->pLeft);
	pTwo->pLeft = pTemp;
	return true;
}

int MoveLeft(TwoWayPtr pTwo)
{	// 왼쪽으로 한번 이동한다.
	// 성공(실패)하면 true(false)를 return한다.
	if (pTwo->pLeft == NULL)
		return false;
	NodePtr pTemp = pTwo->pLeft;
	pTwo->pLeft = XOR2(pTwo->pLeft->link, pTwo->pRght);
	pTwo->pRght = pTemp;
	return true;
}

int InsertNode(TwoWayPtr pTwo, int nData)
{	// 오른쪽 리스트의 앞에 nData를 가지는 노드를 삽입한다.
	// 성공(실패)하면 true(false)를 return한다.
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
{	// 오른쪽 리스트의 첫 노드를 삭제한다.
	// 성공(실패)하면 true(false)를 return한다.
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
{	// 오른(+)/왼(-)쪽으로 nCtr번 이동한다.
	// 성공(실패)하면 true(false)를 return한다.
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
{	// 오른(+)/왼(-)쪽으로 nCtr번 이동하여 오른쪽 노드의 데이터를 반환한다.
	// 성공(실패)하면 오른쪽 노드의 nData(-1)를 return한다.
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

