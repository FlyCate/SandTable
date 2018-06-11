#include "RoadPlan.h"
#include <list>

using namespace std;

#define DB_NAME "SandTable.db"

#define STRAIGHT 0
#define RIGHT	 1

#define WAI_HUAN_ID 1 //�⻷· ID

#define ROAD_TO_CROSSING_RIGHT_POS 80 //��·�յ����ת������� ��ֵ

#define PRINTF()\
printf("[%s][%s][%d]", __FILE__, __FUNCTION__, __LINE__);\

#define OPENDB()\
bRet = m_db.OpenDB(DB_NAME);\
if (bRet == false)\
{\
	PRINTF();\
	printf("��%s�ļ�ʧ��\n", DB_NAME);\
	return -1;\
}

#define OPENDB_BOOL()\
	bRet = m_db.OpenDB(DB_NAME);\
	if (bRet == false)\
{\
	PRINTF();\
	printf("��%s�ļ�ʧ��\n", DB_NAME);\
	return false;\
}

#define CLOSEDB()\
m_db.CloseDB();

CRoadPlan::CRoadPlan(void)
{
}


CRoadPlan::~CRoadPlan(void)
{
}

long CRoadPlan::RoadPlan(const int nStartCard, const int nEndCard)
{
	bool bRet = false;
	long lRet = -1;

	//�ж��Ƿ� ��ʼ����Ե����յ�
	bRet = StartCodeCardArriveEndCard(nStartCard, nEndCard);
	if (bRet == false)
	{
		return -1;
	}

	m_nStartCardID = nStartCard;
	m_nEndCardID = nEndCard;
	m_lstPlan.clear();

	//������ʼ������
	TAGPOS tStartPos;
	lRet = GetCardPos(nStartCard, tStartPos);
	if (lRet != 0)
	{
		m_ErrStr = "������ʼ���������";
		return -1;
	}

	//�����յ�����
	TAGPOS tEndPos;
	lRet = GetCardPos(nEndCard, tEndPos);
	if (lRet != 0)
	{
		m_ErrStr = "�����յ��������";
		return -1;
	}

	//������ʼ�����ڵ�·
	tagRoad tStartRoad;
	lRet = GetRoadInfoFromCardID(nStartCard, tStartRoad);
	if (lRet != 0)
	{
		m_ErrStr = "������ʼ�����ڵ�·����";
		return -1;
	}

	//�����յ����ڵ�·
	tagRoad tEndRoad;
	lRet = GetRoadInfoFromCardID(nEndCard, tEndRoad);
	if (lRet != 0)
	{
		m_ErrStr = "�����յ����ڵ�·����";
		return -1;
	}

	//�ж���ʼ����յ��Ƿ���һ��·��
	if (tStartRoad.nID == tEndRoad.nID)
	{
		//��һ��·�� �������ι滮
		AddTagPlan(NULL, &tStartRoad, STRAIGHT, true, true);
		return 0;
	}

	//������ʻ����·�յ�
	AddTagPlan(NULL, &tStartRoad, STRAIGHT, true);

	tagRoad tCurrentRoad = tStartRoad;
	tagCrossing tCurrentCrossing;
	while (1)
	{
		//���ҵ�·�յ��Ӧ·����Ϣ
		lRet = GetRoadEndCrossing(tCurrentRoad, tCurrentCrossing);
		if (lRet != 0)
		{
			m_ErrStr = "���ҵ�·�յ��Ӧ·����Ϣ����";
			return -1;
		}

		tagRoad tRightRoad;
		//��ǰ·��Ϊ4��·�� ������
		if (tCurrentCrossing.nCrossingID == 4 && tCurrentCrossing.nDirection == NORTH)
		{
			//����յ���ͣ��λ1 2 3ʱ����ǰ·��Ϊ4��·��
			if (tEndRoad.strRoadName.compare("ͣ��λ1") == 0)
			{
				//��һ��·�� �������ι滮
				TAGPLAN tPlan;
				tPlan.type = PLAN_TYPE_CROSSING;
				tPlan.tPlanCrossing.strDescription = "�� ";
				tPlan.tPlanCrossing.strDescription += tCurrentCrossing.strCrossingName.c_str();
				tPlan.tPlanCrossing.strDescription += " ��ת";
				tPlan.tPlanCrossing.nDirectLine = 5;
				tPlan.tPlanCrossing.nCrossingID = tCurrentCrossing.nCrossingID;
				m_lstPlan.push_back(tPlan);
				AddTagPlan(NULL, &tEndRoad, STRAIGHT, false, true);
				return 0;
			}
			else if (tEndRoad.strRoadName.compare("ͣ��λ2") == 0)
			{
				//��һ��·�� �������ι滮
				TAGPLAN tPlan;
				tPlan.type = PLAN_TYPE_CROSSING;
				tPlan.tPlanCrossing.strDescription = "�� ";
				tPlan.tPlanCrossing.strDescription += tCurrentCrossing.strCrossingName.c_str();
				tPlan.tPlanCrossing.strDescription += " ��ת";
				tPlan.tPlanCrossing.nDirectLine = 4;
				tPlan.tPlanCrossing.nCrossingID = tCurrentCrossing.nCrossingID;
				m_lstPlan.push_back(tPlan);
				AddTagPlan(NULL, &tEndRoad, STRAIGHT, false, true);
				return 0;
			}
			else if (tEndRoad.strRoadName.compare("ͣ��λ3") == 0)
			{
				//��һ��·�� �������ι滮
				TAGPLAN tPlan;
				tPlan.type = PLAN_TYPE_CROSSING;
				tPlan.tPlanCrossing.strDescription = "�� ";
				tPlan.tPlanCrossing.strDescription += tCurrentCrossing.strCrossingName.c_str();
				tPlan.tPlanCrossing.strDescription += " ��ת";
				tPlan.tPlanCrossing.nDirectLine = 3;
				tPlan.tPlanCrossing.nCrossingID = tCurrentCrossing.nCrossingID;
				m_lstPlan.push_back(tPlan);
				AddTagPlan(NULL, &tEndRoad, STRAIGHT, false, true);
				return 0;
			}
			else
			{
				//�յ� ����ͣ��λֱ�Ӳ���ֱ�е�·
				lRet = -1;
			}
		}
		else //��Ϊ4��·�� ������
		{
			//������ת��·
			lRet = GetNextRoad(tCurrentCrossing, RIGHT, tRightRoad);
		}

		if (lRet != 0)
		{
			m_ErrStr = "��ת��·������";
			printf("��ת��·������\n");

			//��ת��·������ֱ��ֱ��
			lRet = GetNextRoad(tCurrentCrossing, STRAIGHT, tCurrentRoad);
			if (lRet != 0)
			{
				m_ErrStr = "ֱ�е�·������";
				return -1;
			}
		}
		else
		{
			//���ҵ���ת��·
			//�ж���ת��Ӧ�ĵ�·�ǲ����յ����ڵ�·
			if (tRightRoad.nID == tEndRoad.nID)
			{
				//��һ��·�� �������ι滮
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT, false, true);
				return 0;
			}

			//�ж������ϵȷ����ʻ����
			int nDirection = -1;
			lRet = GetRunDirection(tCurrentRoad, tEndPos, nDirection);
			if (lRet != 0)
			{
				m_ErrStr = "�ж������ϵȷ����ʻ�������";
				return -1;
			}

			//�ж���ת��· �������ߵ�·��
			//�������Ϊ��ת �������¶�ȡ
			if ((nDirection == RIGHT)&&(IsRoadInList(tRightRoad) == false))
			{
				//��ֵ��ֱ�������ж�
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT);
				tCurrentRoad = tRightRoad;
				continue;
			}

			//�������Ϊֱ�� �����¶�ȡ
			//��ȡֱ�еĵ�·
			lRet = GetNextRoad(tCurrentCrossing, STRAIGHT, tCurrentRoad);
			if (lRet != 0)
			{
				//ֱ�е�·������ ֻ����ת ��ֵ�������ж�
				m_ErrStr = "ֱ�е�·������";
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT);
				tCurrentRoad = tRightRoad;
				continue;
			}

			//�ж�ֱ�е�· �������ߵ�·��
			if (IsRoadInList(tCurrentRoad) == true)
			{
				//��ת
				AddTagPlan(&tCurrentCrossing, NULL, RIGHT);
				AddTagPlan(NULL, &tRightRoad, STRAIGHT);
				tCurrentRoad = tRightRoad;
				continue;
			}

			//���������ж�
		}

		AddTagPlan(&tCurrentCrossing, NULL, STRAIGHT);

		//�ж�ֱ�ж�Ӧ�ĵ�·�ǲ����յ����ڵ�·
		if (tCurrentRoad.nID == tEndRoad.nID)
		{
			//��һ��·�� �������ι滮
			AddTagPlan(NULL, &tCurrentRoad, STRAIGHT, false, true);
			return 0;
		}
		AddTagPlan(NULL, &tCurrentRoad, STRAIGHT);
	}
}

bool CRoadPlan::StartCodeCardArriveEndCard(const int nStartCard, const int nEndCard)
{
	bool bRet = false;

	OPENDB_BOOL();

	tagRoad tStartRoad;
	tagRoad tEndRoad;
	bRet = m_db.FindRoadFromCard(nStartCard, tStartRoad);
	if (bRet == false)
	{
		PRINTF();
		printf("������ʼ���·��Ϣ����\n");
		m_ErrStr = "������ʼ���·��Ϣ����";
		CLOSEDB();
		return false;
	}

	bRet = m_db.FindRoadFromCard(nEndCard, tEndRoad);
	if (bRet == false)
	{
		PRINTF();
		printf("���ҽ������·��Ϣ����\n");
		m_ErrStr = "���ҽ������·��Ϣ����";
		CLOSEDB();
		return false;
	}

	CLOSEDB();

	if (tStartRoad.nID == WAI_HUAN_ID)
	{
		if (tEndRoad.nID == WAI_HUAN_ID)
		{
			printf("TRUE, �����⻷·\n");
			return true;
		}
		else
		{
			printf("FALSE ��ʼ�����⻷·����������%s\n", tEndRoad.strRoadName.c_str());
			m_ErrStr = "FALSE ��ʼ�����⻷·����������";
			m_ErrStr += tEndRoad.strRoadName.c_str();
			return false;
		}
	}
	else
	{
		if (tEndRoad.nID != WAI_HUAN_ID)
		{
			printf("TRUE �������⻷·, ��ʼ����%s, ��������%s\n", tStartRoad.strRoadName.c_str(), tEndRoad.strRoadName.c_str());
			return true;
		}
		else
		{
			printf("FALSE ��ʼ����%s�����������⻷·\n", tStartRoad.strRoadName.c_str());
			m_ErrStr = "FALSE ��ʼ����";
			m_ErrStr += tStartRoad.strRoadName.c_str();
			m_ErrStr += "�����������⻷·";
			return false;
		}
	}

	return bRet;
}

long CRoadPlan::GetRoadInfoFromCardID(const int nCardID, tagRoad & tRoad)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	bRet = m_db.FindRoadFromCard(nCardID, tRoad);
	if (bRet == false)
	{
		PRINTF();
		printf("���ҵ�·��Ϣ����\n");
		m_ErrStr = "���ҵ�·��Ϣ����";
		lRet = -1;
	}

	CLOSEDB();

	return lRet;
}

long CRoadPlan::GetCardPos(const int nCardID, TAGPOS & tPos)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	tagCard tCard;
	bRet = m_db.FindCardInfo(nCardID, tCard);
	if (bRet == false)
	{
		PRINTF();
		printf("���ҿ�Ƭ��Ϣ����\n");
		m_ErrStr = "���ҿ�Ƭ��Ϣ����";
		lRet = -1;
	}
	else
	{
		tPos.x = tCard.nXpos;
		tPos.y = tCard.nYpos;
	}

	CLOSEDB();

	return lRet;
}

long CRoadPlan::GetRunDirection(const tagRoad & tRoad, const TAGPOS tEndPos, int & nDirection)
{
	long lRet = -1;
	TAGPOS tRoadEndPos;

	lRet = GetCardPos(tRoad.nEndCard, tRoadEndPos);
	if (lRet != 0)
	{
		printf("���ҵ�·�յ�����ʧ�ܣ�\n");
		m_ErrStr = "���ҵ�·�յ�����ʧ��";
		return lRet;
	}
	
	lRet = 0;
	switch (tRoad.nEndCrossingDirect)
	{
	case EAST://�ɶ�����
		{
			if (tRoadEndPos.x - ROAD_TO_CROSSING_RIGHT_POS < tEndPos.x)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	case SOUTH://������
		{
			if (tRoadEndPos.y + ROAD_TO_CROSSING_RIGHT_POS > tEndPos.y)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	case WEST://������
		{
			if (tRoadEndPos.x + ROAD_TO_CROSSING_RIGHT_POS > tEndPos.x)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	case NORTH://�ɱ�����
		{
			if (tRoadEndPos.y - ROAD_TO_CROSSING_RIGHT_POS < tEndPos.y)
			{
				nDirection = RIGHT;
			}
			else
			{
				nDirection = STRAIGHT;
			}
		}
		break;
	default:
		m_ErrStr = "�������";
		lRet = -1;
		break;
	}

	return lRet;
}

long CRoadPlan::GetRoadEndCrossing(const tagRoad & tRoad, tagCrossing & tCrossing)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	bRet = m_db.FindCrossingInfo(tRoad, tCrossing);
	if (bRet == false)
	{
		PRINTF();
		printf("����·����Ϣ����\n");
		m_ErrStr = "����·����Ϣ����";
		lRet = -1;
	}

	CLOSEDB();

	return lRet;
}

long CRoadPlan::GetNextRoad(const tagCrossing & tCrossing, const int & nDirection, tagRoad & tNextRoad)
{
	bool bRet = false;
	long lRet = 0;

	OPENDB();

	switch (nDirection)
	{
	case STRAIGHT:
		{
			bRet = m_db.FindRoadFromCrossing(tCrossing.nCrossingID, tCrossing.nStraightDirect, tNextRoad);
			if (bRet == false)
			{
				PRINTF();
				printf("����·����Ϣ����\n");
				m_ErrStr = "����·����Ϣ����";
				lRet = -1;
			}
		}
		break;
	case RIGHT:
		{
			bRet = m_db.FindRoadFromCrossing(tCrossing.nCrossingID, tCrossing.nDextroversionDirect, tNextRoad);
			if (bRet == false)
			{
				PRINTF();
				printf("����·����Ϣ����\n");
				m_ErrStr = "����·����Ϣ����";
				lRet = -1;
			}
		}
		break;
	default:
		{
			printf("�������ʹ���\n");
			m_ErrStr = "�������ʹ���";
			return -1;
		}
		break;
	}
	
	CLOSEDB();

	return lRet;
}

bool CRoadPlan::EndPointIsInThisRoad(const int nCardID, const int nRoadID)
{
	bool bRet = false;

	OPENDB_BOOL();
	list<tagRoadCard> lstCard;
	bRet = m_db.ReadDB(nRoadID, lstCard);
	if (bRet == false)
	{
		PRINTF();
		printf("���ҵ�·��Ϣ����\n");
		m_ErrStr = "���ҵ�·��Ϣ����";
	}
	else
	{
		bRet = false;
		m_ErrStr = "���Ҳ�����Ӧ��nCardID";
		for (auto & it = lstCard.begin(); it != lstCard.end(); it++)
		{
			if (it->nCardID == nCardID)
			{
				bRet = true;
				printf("���ҵ���Ӧ��nCardID\n");
			}
		}
	}

	CLOSEDB();

	return bRet;
}

const char * CRoadPlan::GetErrStr()
{
	return m_ErrStr.c_str();
}

long CRoadPlan::AddTagPlan(const tagCrossing * pCrossing, const tagRoad * pRoad, const int nDirection, const bool bStart, const bool bEnd)
{
	TagPlan tPlan;

	if (pCrossing != NULL)
	{
		tPlan.type = PLAN_TYPE_CROSSING;
		tPlan.tPlanCrossing.strDescription = "�� ";
		tPlan.tPlanCrossing.strDescription += pCrossing->strCrossingName.c_str();
		tPlan.tPlanCrossing.strDescription += nDirection == STRAIGHT? " ֱ��" : " ��ת";
		tPlan.tPlanCrossing.nDirectLine = nDirection == STRAIGHT? pCrossing->nStraightLine : pCrossing->nDextroversionLine;
		tPlan.tPlanCrossing.nCrossingID = pCrossing->nCrossingID;
		printf("%s\n", tPlan.tPlanCrossing.strDescription.c_str());
	}

	if (pRoad != NULL)
	{
		tPlan.type = PLAN_TYPE_ROAD;
		if (bStart == true)
		{
			tPlan.tPlanRoad.strDescription = "����ʼ����� �� ";
		}
		else
		{
			tPlan.tPlanRoad.strDescription = "�� ";
		}
		
		tPlan.tPlanRoad.strDescription += pRoad->strRoadName.c_str();
		tPlan.tPlanRoad.strDescription += " ��ʻ";
		if (bEnd == true)
		{
			tPlan.tPlanRoad.strDescription += " ��ʻ�����յ�";
		}
		tPlan.tPlanRoad.nRoadID = pRoad->nID;

		printf("%s\n", tPlan.tPlanRoad.strDescription.c_str());
	}

	m_lstPlan.push_back(tPlan);

	return 0;
}

void CRoadPlan::PrintPlan()
{
	for (auto it = m_lstPlan.begin(); it != m_lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_CROSSING)
		{
			printf("%s\n", it->tPlanCrossing.strDescription.c_str());
		}

		if (it->type == PLAN_TYPE_ROAD)
		{
			printf("%s\n", it->tPlanRoad.strDescription.c_str());
		}
	}
}

long CRoadPlan::GetCardFromPos(TAGPOS pos, list<tagCard> & lstCard)
{

	bool bRet = false;

	OPENDB_BOOL();
	
	bRet = m_db.FindCardFromPos(pos.x, pos.y, lstCard);
	if (bRet == false)
	{
		PRINTF();
		printf("����Card��Ϣ����\n");
		m_ErrStr = "����Card��Ϣ����";
	}

	CLOSEDB();

	return bRet;
}

const list<TagPlan> & CRoadPlan::GetPlan()
{
	TagPlan tStartRoad = m_lstPlan.front();
	bool bRepeat = false;

	auto it = m_lstPlan.begin();
	it++;
	for (; it != m_lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_ROAD)
		{
			if (it->tPlanRoad.nRoadID == tStartRoad.tPlanRoad.nRoadID)
			{
				bRepeat = true;
			}
		}
	}

	if (bRepeat == true)
	{
		TagPlan tCurrentPlan;
		do 
		{
			m_lstPlan.pop_front();
			tCurrentPlan = m_lstPlan.front();
		} while ((tCurrentPlan.type != PLAN_TYPE_ROAD) || (tCurrentPlan.tPlanRoad.nRoadID != tStartRoad.tPlanRoad.nRoadID));
	}

	m_lstPlan.begin()->tPlanRoad.strDescription = tStartRoad.tPlanRoad.strDescription;

	return m_lstPlan;
}

bool CRoadPlan::IsRoadInList(const tagRoad & tRoad)
{
	for (auto it = m_lstPlan.begin(); it != m_lstPlan.end(); it++)
	{
		if (it->type == PLAN_TYPE_ROAD)
		{
			if (tRoad.nID == it->tPlanRoad.nRoadID)
			{
				return true;
			}
		}
	}

	return false;
}
