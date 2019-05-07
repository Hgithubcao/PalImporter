#include <iostream>
#include <vector>
using namespace std;

//�����ļ�ͷ
struct fileHeader
{
	//�ļ��Ϸ����ж�
	char valid[4];
	//δ֪
	int iDontKnow;
	//ģ�ͼ���
	int polyCount;
};

//ģ��ͷ
struct polyHeader
{
	char iDontKnow[52];
};

//����ͷ
struct vertexHeader
{
	char iDontKnow[28];
	int vertexCount;
};

//����
struct vertex
{
	//���������
	float x;
	float y;
	float z;
	//FFFF FFFF 
	long e;
	//�������ͼ����
	float u;
	float v;
};

//��ͼ������
struct betweenVertexAndFace
{
	//��ͼ���������̶�����
	char iDontKnow[144];
};

//��������ͷ
struct faceHeader
{
	int iDontKnow[2];
	int whoCares;
	int faceCount;
};

//����������
struct face
{
	short a;
	short b;
	short c;
};

//���
int main(int argc, char* argv[])
{
	//�����ļ�ͷ
	struct fileHeader theFileHeader;
	struct polyHeader tempPolyHeader;

	//��ʾָ�����ļ���
	printf("input:%s\n",argv[1]);

	//��ָ�����ļ�
	FILE *fp;
	fp=fopen(argv[1],"rb");

	//��ȡ�ļ�ͷ
	fread(&theFileHeader,sizeof(struct fileHeader),1,fp);
	//����ļ��Ϸ�
	if(theFileHeader.valid[0]=='P'&&theFileHeader.valid[1]=='O'&&theFileHeader.valid[2]=='L'&&theFileHeader.valid[3]=='Y')
	{
		printf("the file seems to be valid!\n");
	}
	else
	{
		//���Ϸ��˳�
		printf("invalid file format!\n");
		getchar();
		exit(0);
	}

	//��ȡģ��ͷ
	for(int i=0;i<theFileHeader.polyCount;i++)
	{
		fread(&tempPolyHeader,sizeof(struct polyHeader),1,fp);
	}

	//�洢����������ݵĿռ�
	vector<struct vertex> vertexList;
	vector<struct face> faceList;

	//����ȫ����ģ��
	for(int i=0;i<theFileHeader.polyCount;i++)
	{
		//��մ洢�ռ�	
		vertexList.clear();
		faceList.clear();
		//�õ�����ͷ
		struct vertexHeader tempVertexHeader;
		fread(&tempVertexHeader,sizeof(struct vertexHeader),1,fp);
		printf("vertex count [%d]\n",tempVertexHeader.vertexCount);
		//����ȫ���Ķ���
		for(int e=0;e<tempVertexHeader.vertexCount;e++)
		{
			//���涥�������
			struct vertex tempVertex;
			fread(&tempVertex,sizeof(struct vertex),1,fp);
			vertexList.push_back(tempVertex);
		}
		//��ȡ��ͼ����
		struct betweenVertexAndFace tempBetween;
		fread(&tempBetween,sizeof(struct betweenVertexAndFace),1,fp);
		//�õ������ݵ�ͷ��
		struct faceHeader tempFaceHeader;
		fread(&tempFaceHeader,sizeof(struct faceHeader),1,fp);
		printf("face count [%d]\n",tempFaceHeader.faceCount);
		//�������е���
		for(int e=0;e<tempFaceHeader.faceCount;e++)
		{
			//��������
			struct face tempFace;
			fread(&tempFace,sizeof(struct face),1,fp);
			faceList.push_back(tempFace);
		}
		//����obj��ʽ���ļ�
		char outName[32]={0};
		sprintf(outName,"out%s%d.obj",argv[1],i);
		FILE *outFp;
		outFp=fopen(outName,"w");

		for(int e=0;e<vertexList.size();e++)
		{
			fprintf(outFp,"v %f %f %f \n",vertexList[e].x,vertexList[e].y,vertexList[e].z);
		}

		fprintf(outFp,"\n\n");

		for(int e=0;e<vertexList.size();e++)
		{
			fprintf(outFp,"vt %f %f\n",vertexList[e].u,vertexList[e].v);
		}
	
		fprintf(outFp,"\n\ng model%d\n",i);
	
		for(int e=0;e<faceList.size();e++)
		{
			fprintf(outFp,"f %d/%d %d/%d %d/%d\n",faceList[e].a+1,faceList[e].a+1,faceList[e].b+1,faceList[e].b+1,faceList[e].c+1,faceList[e].c+1);
		}
		fprintf(outFp,"\n\ng\n\n");
		fclose(outFp);
	}
	fclose(fp);
	getchar();
}