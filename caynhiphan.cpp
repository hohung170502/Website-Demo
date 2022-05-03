#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
typedef int KeyType;
struct Node
{
 KeyType key;
 Node *Left;
 Node *Right;
};
typedef Node *BTree;
struct Node1
{
 KeyType key,column,row;
 Node1 *Left,*Right;
};
typedef Node1 *BTree1;
/*------------------------*/
void Error(int i)
{
 switch(i)
 {
 case 1: printf("\n Khong du bo nho.");
 break;
 case 2: printf("\n Khong co chuc nang nay.Moi ban thu lai.");
 break;
 case 3: printf("\n Du lieu nhap ko dung");
 break;
 case 4: printf("\n So nay da duoc nhap.Moi ban chon so khac.");
 break;
 case 5: printf("\n Hai cay hien dang rong.Khong the thuc hien thao tac.");
 break;
 case 6: printf("\n Khong mo duoc file.Please try again");
 getch();
 break;
 }
 printf("\n\t********\n");
}
/*------------------------*/
void MakeNullBTree(BTree &Root)
{
 Root=NULL;
}
/*------------------------*/
char Empty(BTree Root)
{
 return (Root==NULL);
}
/*------------------------*/
Node *SearchNode(BTree Root,KeyType x)
{
 if (Empty(Root)) return NULL;
 else
 {
 if ((*Root).key==x) return Root;
 if ((*Root).key<x) return SearchNode((*Root).Right,x);
 else return SearchNode((*Root).Left,x);
 }
}
/*------------------------*/
Node *CreateNode(KeyType x)
{
 Node *p;
 p=new Node;
 if(p==NULL) Error(1);
 else
 {
 (*p).key=x;
 (*p).Left=NULL;
 (*p).Right=NULL;
 }
 return p;
}
/*------------------------*/
void InsertNode(KeyType x,BTree &Root)
{
 if(Root==NULL) // Neu cay nay dang rong
 {
 Root=CreateNode(x);
 }
 else
 {
 if (x<(*Root).key) InsertNode(x,(*Root).Left);
 if (x>(*Root).key) InsertNode(x,(*Root).Right);
 }
}
/*------------------------*/
void InputBTree(BTree &Root)
{
 Node *p;
 KeyType key;
 int n,i;
 printf("\n Ban muon nhap bao nhieu node:");
 scanf("%d",&n);
 for(i=1;i<=n;i++)
 {
 printf("\n Moi ban nhap 1 so nguyen cho node thu %d :",i);
 scanf("%d",&key);
 p=SearchNode(Root,key);
 if(p==NULL) InsertNode(key,Root);
 else
{
 Error(4);
 i--;
}
 }
}
/*------------------------*/
void InputBTreeFromFile(BTree &Root,const char *ten)
{
 int n;
 KeyType key;
 Node *p;
 FILE *fp;
 fp=fopen(ten,"rt");
 if (fp==NULL)
 {
 Error(6);
 exit(1);
 }
 else
 {
 fscanf(fp,"%d",&n);
 for(int i=1;i<=n;i++)
{
 fscanf(fp,"%d",&key);
 p=SearchNode(Root,key);
 if (p==NULL) InsertNode(key,Root);
 else
 {
 Error(3);
 Root=NULL;
 break;
 } 
}
 }
 fclose(fp);
}
/*------------------------*/
void PreOrder(BTree Root)
{
 if(Root!=NULL)
 {
 printf("\n %d ",(*Root).key);
 PreOrder((*Root).Left);
 PreOrder((*Root).Right);
 }
}
/*------------------------*/
char TuongTu(BTree Root1,BTree Root2)
{
 int t;
 if (!Empty(Root1)||!Empty(Root2)) // co it nhat 1 nut ko rong thi co the goi de quy
 {
 if (Empty(Root1) ^ Empty(Root2)) t=0; // ko tuong tu tai nut dang xet
 else if (!Empty(Root1) && !Empty(Root2)) // nut dang xet tuong tu nhau
 { //xet tiep 2 nhanh phai va trai
 if( (TuongTu((*Root1).Left,
(*Root2).Left))&&(TuongTu((*Root1).Right,(*Root2).Right)))
t=1;
 else t=0; //co it nhat mot nhanh khac nhau
 }
 }
 else t=1; // ca 2 nut deu rong
 return t;
}
/*------------------------*/
void OutputBTree(BTree Root,int i,int muc)
{
 if(Root)
 {
 if(i==muc)
{
 printf("%d",(*Root).key); }
  OutputBTree((*Root).Left,i+1,muc);
 OutputBTree((*Root).Right,i++,muc);
}
}
/*------------------------*/
void DestroyBTree(BTree &Root)
{
 if(Root)
 {
 DestroyBTree((*Root).Left);
 DestroyBTree((*Root).Right);
 delete Root;
 }
 Root=NULL;
}
/*------------------------*/
void DestroyBTree1(BTree1 &Root)
{
 if(Root)
 {
 DestroyBTree1((*Root).Left);
 DestroyBTree1((*Root).Right);
 delete Root;
 }
 Root=NULL;
}
/*------------------------*/
int Max(int a,int b)
{
 if(a>b) return a;
 else return b;
}
/*------------------------*/
int IsLeaf(Node *p)
{
 if((*p).Left==NULL && (*p).Right==NULL) return 1;
 else return 0;
}
/*------------------------*/
int HightBTree(BTree Root)
{
 if((Root==NULL) || (IsLeaf(Root))) return 0;
 else return (1 +
Max(HightBTree((*Root).Left),HightBTree((*Root).Right)));
}
/*------------------------*/
/* Ham copy tao ra cay nhi phan T1 co du lieu giong T 
nhung co them toa do tren man hinh cua moi node
Y tuong o day la 2 node co column cach nhau k(don vi),
Duyet tu trai-goc-phai, qua moi lan thay 1 phan tu
thi tang column(c) len k(don vi), r+=k va r-=k
Muc dich de tang so dong(r) khi ta xet mot lop moi va sau do
quay lui */
void Copy(BTree1 &T1,BTree T,int &r,int &c,int k)
{
 if(T!=NULL)
 {
 T1=new Node1;
 (*T1).key=(*T).key;
 r+=k;
 Copy((*T1).Left,(*T).Left,r,c,k);
 (*T1).column=c;
 (*T1).row=r;
 c+=k;
 Copy((*T1).Right,(*T).Right,r,c,k);
 r-=k;
 }
 else T1=NULL;
}
/*------------------------*/
//ham Line dung de ve duong noi giua 2 diem
void Line(int column1,int row1,int column2,int row2,int k)
{
 int n;
 n=(column2-column1)/k;
 for(int i=1;i<=k;i++)
 {
 gotoxy(column1+i*n,row1+i);
 printf("*");
 }
}
/*------------------------*/
/* Duyet theo goc-trai-phai. Chuong trinh kiem tra node con ben trai, neu ton tai thi ve duong dan den node do va in gia tri cua
node con trai do*/
void Display(BTree1 T,int k)
{
    BTree1 p;
    gotoxy((*T).column,(*T).row);
    printf("%d",(*T).key);
    if((*T).Left!=NULL)
      {
        p=(*T).Left;
        Line((*T).column,(*T).row,(*p).column,(*p).row,k);
        Display((*T).Left,k);
      }
    if((*T).Right!=NULL)
      {
        p=(*T).Right;
        Line((*T).column,(*T).row,(*p).column,(*p).row,k);
         Display((*T).Right,k);
      }
}
/*------------------------*/
void DisplayBTree(BTree T,int x,int y,int k)
{
    BTree1 T1;
    Copy(T1,T,x,y,k);
    if(T!=NULL) Display(T1,k);
    DestroyBTree1(T1);
}
/*------------------------*/
int Menu()
{
    int Router;
    printf("\n\t\t ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»";
        cout << "\n\t\t  CAY NHI PHAN TIM KIEM ";
        cout << "\n\t\t ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼";
        printf("\n\n ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»");
        printf("\n  MOI BAN CHON CHUC NANG ");
        printf("\n "ÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¹");
        cout << "\n0. THOAT KHOI CHUONG TRINH ");
        printf("\n "ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶");
        printf("\n º 1. NHAP 2 CAY NHI PHAN TIM KIEM º");
        printf("\n "ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶");
        printf("\n2. DUYET 2 CAY NHI PHAN TIM KIEM THEO THU TU TRUOC º");
printf("\n " ");
printf("\n º 3. " HUY CAY NHI PHAN TIM KIEM VUA TAO ");
printf("\n "ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¶");
printf("\n º 4. XEM 2 CAY NHI PHAN CO TUONG TU NHAU
KHONG º");
printf("\n "ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼");
printf("\n\n Ban chon chuc nang : ");
 scanf("%d", &Router);
 return Router;
}
/*------------------------*/
void Printout(BTree Root1,BTree Root2) //in ra 2 cay
{
 int k;
 clrscr();
 if ( HightBTree(Root1)+HightBTree(Root2) < 7 ) k=3; //tinh tong chieu cao de xac dinh k
 else k=2;
 printf("Cay thu nhat\n");
 DisplayBTree(Root1,-1,30,k);
 if (Root1==NULL) printf("\n Cay nay rong");
 printf("\n\nCay thu hai\n");
 DisplayBTree(Root2, HightBTree(Root1)*k + 2, 30,k);
 if (Root2==NULL) printf("\n Cay nay rong !");

