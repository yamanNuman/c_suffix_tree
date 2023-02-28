#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_CHAR 256

struct SonekAgaciDugum {
	struct SonekAgaciDugum *cocuklar[MAX_CHAR];

	struct X *sonekLink;
	int baslangic;
	int *bitis;
	int sonekIndex;
};

typedef struct SonekAgaciDugum Dugum;

char text[100];
Dugum *root = NULL;
Dugum *sonYeniDugum = NULL;
Dugum *aktifDugum = NULL;

int aktifKenar = -1;
int aktifUzunluk = 0;

int kalanSonekSayisi = 0;
int yaprakSon = -1;
int *rootSon = NULL;
int *splitSon = NULL;
int uzunluk = -1;

Dugum *newNode(int baslangic, int *bitis)
{
	Dugum *dugum =(Dugum*) malloc(sizeof(Dugum));
	int i;
	for (i = 0; i < MAX_CHAR; i++)
		dugum->cocuklar[i] = NULL;

	dugum->sonekLink = root;
	dugum->baslangic = baslangic;
	dugum->bitis = bitis;
	dugum->sonekIndex = -1;
	return dugum;
}

int edgeLength(Dugum *n) {
	if(n == root)
		return 0;
	return *(n->bitis) - (n->baslangic) + 1;
}

int walkDown(Dugum *currNode)
{

	if (aktifUzunluk >= edgeLength(currNode))
	{
		aktifKenar += edgeLength(currNode);
		aktifUzunluk -= edgeLength(currNode);
		aktifDugum = currNode;
		return 1;
	}
	return 0;
}

void extendSuffixTree(int pos)
{
	yaprakSon = pos;
	kalanSonekSayisi++;
	sonYeniDugum = NULL;

	while(kalanSonekSayisi > 0) {

		if (aktifUzunluk == 0)
			aktifKenar = pos;

		if (aktifDugum->cocuklar[text[aktifKenar]] == NULL)
		{
			aktifDugum->cocuklar[text[aktifKenar]] = newNode(pos, &yaprakSon);

			if (sonYeniDugum != NULL)
			{
				sonYeniDugum->sonekLink = aktifDugum;
				sonYeniDugum = NULL;
			}
		}
		else
		{
			Dugum *sonraki = aktifDugum->cocuklar[text[aktifKenar]];
			if (walkDown(sonraki))
			{
				continue;
			}
			if (text[sonraki->baslangic + aktifUzunluk] == text[pos])
			{
				if(sonYeniDugum != NULL && aktifDugum != root)
				{
					sonYeniDugum->sonekLink = aktifDugum;
					sonYeniDugum = NULL;
				}
				aktifUzunluk++;
				break;
			}
			splitSon = (int*) malloc(sizeof(int));
			*splitSon = sonraki->baslangic + aktifUzunluk - 1;

			Dugum *split = newNode(sonraki->baslangic, splitSon);
			aktifDugum->cocuklar[text[aktifKenar]] = split;

			split->cocuklar[text[aktifKenar]] = newNode(pos, &yaprakSon);
			sonraki->baslangic += aktifUzunluk;
			split->cocuklar[text[aktifKenar]] = sonraki;

			if (sonYeniDugum != NULL)
			{

				sonYeniDugum->sonekLink = split;
			}

			sonYeniDugum = split;
		}

		kalanSonekSayisi--;
		if (aktifDugum == root && aktifUzunluk > 0)
		{
			aktifUzunluk--;
			aktifKenar = pos - kalanSonekSayisi + 1;
		}
		else if (aktifDugum != root)
		{
			aktifDugum = aktifDugum->sonekLink;
		}
	}
}

void bastir(int i, int j)
{
	int k;
	for (k = i; k <= j; k++)
		printf("%c", text[k]);
}

void setSuffixIndexByDFS(Dugum *n, int labelHeight)
{
	if (n == NULL) return;

	if (n->baslangic != -1)
	{
	 bastir(n->baslangic, *(n->bitis));

	}
	int yaprak = 1;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->cocuklar[i] != NULL)
		{
			yaprak = 0;
			setSuffixIndexByDFS(n->cocuklar[i], labelHeight +
								edgeLength(n->cocuklar[i]));
		}
	}
	if (yaprak == 1)
	{
		n->sonekIndex = uzunluk - labelHeight;
		printf(" [%d]\n", n->sonekIndex);
	}
}

void freeSuffixTreeByPostOrder(Dugum *n)
{
	if (n == NULL)
		return;
	int i;
	for (i = 0; i < MAX_CHAR; i++)
	{
		if (n->cocuklar[i] != NULL)
		{
			freeSuffixTreeByPostOrder(n->cocuklar[i]);
		}
	}
	if (n->sonekIndex == -1)
		free(n->bitis);
	free(n);
}
void sonekAgaciOlusturma()
{
	uzunluk = strlen(text);
	int i;
	rootSon = (int*) malloc(sizeof(int));
	*rootSon = - 1;
	root = newNode(-1, rootSon);

	aktifDugum = root;
	for (i = 0; i < uzunluk; i++)
		extendSuffixTree(i);
	int labelHeight = 0;
	setSuffixIndexByDFS(root, labelHeight);
}

int traverseEdge(char *x, int y, int basla, int bitir)
{
    int k = 0;
    for(k = basla; k <= bitir && x[y] != '\0'; k++, y++)
    {
        if(text[k] != x[y])
            return -1;
    }
    if(x[y] == '\0')
        return 1;
    return 0;
}

int doTraversalToCountLeaf(Dugum *n)
{
    if(n == NULL)
        return 0;
    if(n->sonekIndex > -1)
    {
        printf("\nBulundugu Pozisyon : %d", n->sonekIndex);
        return 1;
    }
    int sayac = 0;
    int i = 0;
    for (i = 0; i < MAX_CHAR; i++)
    {
        if(n->cocuklar[i] != NULL)
        {
            sayac += doTraversalToCountLeaf(n->cocuklar[i]);
        }
    }
    return sayac;
}

int countLeaf(Dugum *n)
{
    if(n == NULL)
        return 0;
    return doTraversalToCountLeaf(n);
}

int doTraversal(Dugum *n, char* x, int y)
{
    if(n == NULL)
    {
        return -1;
    }
    int res = -1;

    if(n->baslangic != -1)
    {
        res = traverseEdge(x, y, n->baslangic, *(n->bitis));
        if(res == -1)
            return -1;
        if(res == 1)
        {
            if(n->sonekIndex > -1)
                printf("\nSubstring sayisi: 1 and Pozisyon : %d",n->sonekIndex);
            else
                printf("\nSubstring sayisi: %d", countLeaf(n));
            return 1;
        }
    }
    y = y + edgeLength(n);

    if(n->cocuklar[x[y]] != NULL)
        return doTraversal(n->cocuklar[x[y]], x, y);
    else
        return -1;
}

void SubstringKontrolEtme(char* x)
{
    int res = doTraversal(root, x, 0);
    if(res == 1)
        printf("\nBu kalip <%s> bir Substring.\n", x);
    else
        printf("\nBu kalip <%s> bir Substring degil.\n", x);
}

void doTraversalForLRS(Dugum *n, int etiketYuksekligi, int* maksYukseklik,int* substringBaslangicIndexi)
{
    if(n == NULL)
    {
        return;
    }
    int i=0;
    if(n->sonekIndex == -1)
    {

        for (i = 0; i < MAX_CHAR; i++)
        {
            if(n->cocuklar[i] != NULL)
            {
                doTraversalForLRS(n->cocuklar[i], etiketYuksekligi + edgeLength(n->cocuklar[i]), maksYukseklik, substringBaslangicIndexi);
            }
        }


    }
    else if(n->sonekIndex > -1 &&
                (*maksYukseklik < etiketYuksekligi - edgeLength(n)))
    {
        *maksYukseklik = etiketYuksekligi - edgeLength(n);
        *substringBaslangicIndexi = n->sonekIndex;
    }

}

void enUzunTekrarEdenSubstring()
{
    int maksYukseklik = 0;
    int substringBaslangicIndexi = 0;
    doTraversalForLRS(root, 0, &maksYukseklik, &substringBaslangicIndexi);
    printf("En uzun tekrar eden Substring : %s -> ", text);
    int k;
    for (k=0; k<maksYukseklik; k++)
        printf("%c", text[k + substringBaslangicIndexi]);

    if(k == 0)
        printf("Tekrar eden Substring yok.");
    printf("\n");
}


int main()
{
    char inputText[1000];
    FILE *fptr;
    if ((fptr = fopen("program.txt", "r")) == NULL) {
        printf("Dikkat! Dosya açilamadi.");
        exit(1);
    }


    fscanf(fptr, "%[^\n]", inputText);
    printf("Dosyadaki veri: %s\n", inputText);
    fclose(fptr);
	strcpy(text, inputText);

	int control;
    int flag=1;
    char arananKatar[1000];
	while(1)
    {
        printf("Sonek Agaci Olusturmak icin: 1      Alt Katar Kontrol Etmek icin: 2     En Uzun Tekrar Eden Alt katar icin: 3	Cikis icin:4\n");
	    scanf("%d",&control);

        switch(control)
        {
            case 1:
                sonekAgaciOlusturma();
                break;

            case 2:
                printf("Aranan Alt Katar: ");
                scanf("%s", &arananKatar);
                SubstringKontrolEtme(arananKatar);
                break;

            case 3:
                enUzunTekrarEdenSubstring();
                break;
            case 4:
            	break;
                
        }

    }

	return 0;
}
