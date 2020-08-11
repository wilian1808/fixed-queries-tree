#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

#include "resource.h"

HINSTANCE hInst;
using namespace std;

int EditDist(string& a,string& b) // Distancia de Edicion // Prog dinamica O(m*n)
{
    int m = a.length(), n = b.length();
    //int dp[m+1][n+1]={{0}};
    vector < vector <int> > dp(m+1, vector<int>(n+1, 0));
    for (int i=0; i<=m; i++) // llenando casos base
        dp[i][0] = i;
    for (int j=0; j<=n; j++)
        dp[0][j] = j;
    for (int i=1; i<=m; i++) // llenar matriz prog dinamica
    {   for (int j=1; j<=n; j++)
        {   if (a[i-1] != b[j-1])
                dp[i][j] = min(1+dp[i-1][j],     //borrar
                            min(1 + dp[i][j-1],  //insertar
                             1 + dp[i-1][j-1])); //reemplazar
            else
                dp[i][j] = dp[i-1][j-1];
        }
    }
    return dp[m][n];
}

class Nodo
{   public:
    string Palabra;
    map <int,Nodo *> Sig;
    Nodo(string P="")
    {   Palabra=P;
    }
};

class ArbolBK
{
public:
    Nodo *Raiz;
    ArbolBK()
    {   Raiz=NULL;
    }

    void InsertarBloque(string S)
    {   string token;
        stringstream cins(S);
        while(getline(cins,token,' '))
            Insertar(token);
    }

    void Insertar(string S)
    {   if(Raiz==NULL)
            Raiz=new Nodo(S);
        else
            Ins(Raiz,S);
    }

    void Ins(Nodo *&R,string S)
    {
        int dist=EditDist(R->Palabra,S);
        if(dist>0)
        {
            if((R->Sig).find(dist)==(R->Sig).end())
                (R->Sig)[dist]=new Nodo(S);
            else
                Ins((R->Sig)[dist],S);
        }
    }

    void Mostrar(long T)
    {
        if (Raiz != NULL)
            Mos(Raiz, T);
    }

    void Mos(Nodo *R,long T)
    {
        cout<<T<<" - "<<R->Palabra<<endl;
        map <int,Nodo *>::iterator it;

        for(it=(R->Sig).begin();it!=(R->Sig).end();it++)
            Mos((*it).second,T*10+(*it).first);
    }

    void MostrarW(HDC hdc,int x,int y,int a)
    {   if(Raiz!=NULL)
            MosW(hdc,Raiz,x,y,a);
    }

    void MosW(HDC hdc,Nodo *R,int x,int y,int a)
    {   map <int,Nodo *>::iterator it;
        int ii,ancho,tam=(R->Sig).size();
        if(tam>0)
        {   ancho=a/tam;
            for(ii=0,it=(R->Sig).begin();it!=(R->Sig).end();it++,ii++)
            {   MoveToEx(hdc,x,y,NULL);
                LineTo(hdc,x+ii*ancho-a/2,y+100);
                MosW(hdc,(*it).second,x+ii*ancho-a/2,y+100,ancho-15);
                char Cad2[10];
                sprintf(Cad2,"%i",(*it).first);
                TextOut(hdc,x+ii*ancho-a/2-10,y+100-5,Cad2,strlen(Cad2));
            }
        }
        char Cad[10]=" ";
        for(ii=0;ii<strlen((R->Palabra).c_str());ii++)
        {   Cad[0]=((R->Palabra).c_str())[ii];
            TextOut(hdc,x+1,y+1+ii*14,Cad,1);
        }
    }

    vector <string> similar(Nodo *R,string S,int Tol)
    {   int dist, var;
        vector <string> ret;
        if(R!=NULL)
        {   dist=EditDist(R->Palabra,S);
            if(dist <= Tol)
                ret.push_back(R->Palabra);
            var = dist-Tol;
            if (var < 0)
                var=1;
            vector <string> tmp;
            for(;var<dist+Tol;var++)
            {   if((R->Sig).find(var)!=(R->Sig).end())
                {   tmp=similar((R->Sig)[var],S,Tol);
                    ret.insert(ret.end(),tmp.begin(),tmp.end());
                }
            }
        }
        return ret;
    }
};

ArbolBK A;

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{   switch(uMsg)
    {
        case WM_PAINT:
        {   PAINTSTRUCT ps;
            HDC hdc;
            hdc=BeginPaint(hwndDlg,&ps);
            A.MostrarW(hdc,650,50,700);
            EndPaint(hwndDlg,&ps);
        }
        case WM_INITDIALOG:
        {
        }
        return TRUE;
        case WM_CLOSE:
        {
            EndDialog(hwndDlg, 0);
        }
        return TRUE;
        case WM_COMMAND:
        {
            switch(LOWORD(wParam))
            {   case INSERTAR:
                {   char Cad[500];
                    GetDlgItemText(hwndDlg,PAL,Cad,500);
                    A.InsertarBloque(Cad);
                    InvalidateRect(hwndDlg,NULL,true);
                }
                return TRUE;
                case BUSCAR:
                {   char Cad2[20];
                    GetDlgItemText(hwndDlg,BUSQ,Cad2,20);
                    int Tol=GetDlgItemInt(hwndDlg,TOL,NULL,NULL);
                    vector <string> R=A.similar(A.Raiz,Cad2,Tol);
                    stringstream ss;
                    for(int i=0;i<R.size();i++)
                        ss<<R[i]<<" ";
                    SetDlgItemTextA(hwndDlg,RESUL,(ss.str()).c_str());
                }
                return TRUE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst=hInstance;
    InitCommonControls();
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}
