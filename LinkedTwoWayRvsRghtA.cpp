#include <stdio.h>
#include <stdlib.h>
#pragma warning (disable: 4326 4996 6001 6054)

// DebugON

typedef struct node {
	int nData;
	struct node *link;
}	Node, *NodePtr;

typedef struct {				// 양쪽으로 이동하기 위하여 변수 2개가 필요합니다.
	Node *pLeft;				// left로 사용됩니다.
	Node *pRght;				// rght로 사용됩니다.
}	TwoWay, *TwoWayPtr;			// 두 변수를 하나로 묶어서 사용하려고 합니다.

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
	lnkdList.pLeft = lnkdList.pRght = NULL;	// 양방향 linked list의 초기화
	int bDoing = true;
	char sMsg[100], *pMsg = sMsg;
	while (bDoing) {
		int nData;
		const char *pCmnd = sCmnds[nNdx++];		// 명령어를 하나씩 가져온다.
		switch (pCmnd[0] | 0x20) {
		case 'i':			// 오른쪽에 새로운 노드를 삽입
			nData = atoi(pCmnd + 1);
			bSuccess = InsertNode(&lnkdList, nData);
			break;
		case 'd':			// 오른쪽의 첫 노드를 삭제
			bSuccess = DeleteNode(&lnkdList, &nData);
			break;
		case 'l':			// 왼쪽으로 한번 이동
			bSuccess = MoveLeft(&lnkdList);
			break;
		case 'r':			// 오른쪽으로 한번 이동
			bSuccess = MoveRight(&lnkdList);
			break;
		case 'm':			// 오른(+)/왼(-)쪽으로 n번 이동
			nData = atoi(pCmnd+1);
			bSuccess = MoveRepeatedly(&lnkdList, nData);
			break;
		case 'o':			// n번 이동 후, 오른쪽 노드의 data를 반환
			nData = atoi(pCmnd+1);
			if ((nData = OutputData(&lnkdList, nData)) >= 0)
				*pMsg++ = nData;
			else
				bSuccess = false;
			break;
		case 'e':			// 끝
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
{	// 오른쪽으로 한번 이동한다.
	// 성공하면 true를 return하고, 이동할 수 없으면 false를 return한다.
	NodePtr pLeft = pTwo->pLeft, pRght = pTwo->pRght;
	if (pRght) {
		pTwo->pLeft = pRght;
		pTwo->pRght = pRght->link;
		pRght->link = pLeft;
	}
	return pRght != NULL;
}

int MoveLeft(TwoWayPtr pTwo)
{	// 왼쪽으로 한번 이동한다.
	// 성공하면 true를 return하고, 이동할 수 없으면 false를 return한다.
	NodePtr pLeft = pTwo->pLeft, pRght = pTwo->pRght;
	if (pLeft) {
		pTwo->pRght = pLeft;
		pTwo->pLeft = pLeft->link;
		pLeft->link = pRght;
	}
	return pLeft != NULL;
}

int InsertNode(TwoWay *pTwo, int nData)
{	// 오른쪽 리스트의 앞에 nData를 가지는 노드를 삽입한다.
	// 성공하면 true, 실패하면 false를 return한다.
	Node *pNew = (Node *)malloc(sizeof(Node));
	if (pNew) {
		pNew->nData = nData;
		pNew->link = pTwo->pRght;
		pTwo->pRght = pNew;
	}
	return pNew != NULL;
}

int DeleteNode(TwoWayPtr pTwo, int *pnData)
{	// 오른쪽 리스트의 첫 노드의 데이타를 pnData에 저장하고
	// 그 첫 노드를 삭제한다.
	// 성공하면 true, 실패하면 false를 return한다.
	Node *pDel = pTwo->pRght;
	if (pDel) {
		*pnData = pDel->nData;
		pTwo->pRght = pDel->link;
		free(pDel);
	}
	return pDel != NULL;
}

int MoveRepeatedly(TwoWayPtr pTwo, int nCtr)
{	// 오른(+)/왼(-)쪽으로 nCtr번 이동한다.
	// 성공(실패)하면 true(false)를 return한다.
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
{	// 오른(+)/왼(-)쪽으로 nCtr번 이동하여 오른쪽 노드의 데이터를 반환한다.
	// 성공(실패)하면 오른쪽 노드의 nData(-1)를 return한다.
	return (MoveRepeatedly(pTwo, nCtr) && pTwo->pRght) ? pTwo->pRght->nData : -1;
}

void PrintList(TwoWayPtr pTwo, const char *pCmnd)
{
	// 전체 데이터를 %d로 출력하는데 
	// 왼쪽은 역순으로 출력하고 
	// 오른쪽은 순서대로 출력한다.
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
