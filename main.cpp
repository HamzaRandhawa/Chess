#include <iostream>
#include <windows.h>
#include <conio.h>
#include <fstream>

using namespace std;
char B [8][8],choice;
char C [8][8];
int capital=0;
int Small=1;
void Save (char B[][8], int Turn);
void Load (char B[][8],int & Turn);
void Save_2 (char B[][8], int Turn);
void Load_2 (char B[][8],int & Turn);
bool Check (char B[][8], int Sr,int Sc,int dr,int dc,int Turn);
bool IsLegalMove (char B[][8], int Sr,int Sc,int dr,int dc,int turn);
void wait (int T)
{
    for (int t=0;t<T;t++)
    {
         for(int c=0; c<=99999;c++)
    {
    }
    }
}

void getRowColbyLeftClick(int &rpos,int &cpos)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD Events;
    INPUT_RECORD InputRecord;
    SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
    do
    {
        ReadConsoleInput(hInput, &InputRecord, 1, &Events);
        if(InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
            rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
            break;
        }
    }
    while(true);
}


void gotoRowCol(int rpos, int cpos)
{
    int xpos=cpos, ypos = rpos;
    COORD scrn;
    HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);

    scrn.X = xpos;
    scrn.Y = ypos;
    SetConsoleCursorPosition(hOuput,scrn);
}
void PrintBox (int Sr,int Sc, int cdim,int rdim,char S)
{
    for (int ri=Sr;ri<rdim+Sr;ri++)
    {
        for (int ci=Sc; ci<cdim+Sc;ci++)
        {
            if(ri==Sr|| ci==Sc||ri==Sr+rdim-1||ci==Sc+cdim-1)
            {
                gotoRowCol(ri,ci);
                cout<<S;
            }
        }
    }
}
void PrintBN(int BN,int rdim,int cdim,char S)
{
    int Sr=((BN-1)/8)*rdim;
    int Sc=(((BN-1)%8)*cdim);
    PrintBox(Sr,Sc,cdim,rdim,S);
}
void PrintInCenter (int BN, char S, int rdim, int cdim)
{
    int Cri=((BN-1)/8)*rdim+rdim/3+1;
    int Cci=(((BN-1)%8)*cdim)+cdim/2-1;

    gotoRowCol(Cri,Cci);
    cout<<S;
}
void PrintInCenter2 (int BN, char S, int rdim, int cdim)
{
    int Cri=((BN-1)/8)*rdim+rdim/3+1-2;
    int Cci=(((BN-1)%8)*cdim)+cdim/2-1+2;

    gotoRowCol(Cri,Cci);
    cout<<S;
}

void Init (char B[][8])
{
    B[0][0]=B[0][7]='R';
    B[0][1]=B[0][6]='H';
    B[0][2]=B[0][5]='B';
    B[0][3]='K';
    B[0][4]='Q';

    B[7][0]=B[7][7]='r';
    B[7][1]=B[7][6]='h';
    B[7][2]=B[7][5]='b';
    B[7][3]='k';
    B[7][4]='q';

    for (int i=0;i<8;i++)
    {
        B[1][i]='P';
        B[6][i]='p';

        B[2][i]='.';
        B[3][i]='.';
        B[4][i]='.';
        B[5][i]='.';
    }
}
void PrintGrid (int rdim, int cdim, char Board[][8])
{
    for (int c=1;c<=64;c++)
    {
        PrintBN(c,rdim,cdim,-37);
    }

    PrintBox(40,90,cdim,rdim,-37);

    gotoRowCol(34,100);
    cout<<"Click";

    gotoRowCol(38,90);
    cout<<"Here To";
    gotoRowCol(44,93);
    cout<<"Save";

    PrintBox(40,110,cdim,rdim,-37);
    gotoRowCol(38,110);
    cout<<"Here To";
    gotoRowCol(44,113);
    cout<<"Undo";

    PrintBox(40,130,cdim,rdim,-37);
    gotoRowCol(38,130);
    cout<<"Here To";
    gotoRowCol(44,132);
    cout<<"Restart";

}
void Print_Board (int rdim, int cdim, char Board[][8])
{
    int p=1;
    for (int k=0;k<8;k++)
    {
        for (int i=0;i<8;i++)
        {
            PrintInCenter(p,Board[k][i],rdim,cdim);
            p++;
        }
    }
}
void Print_Board2 (int rdim, int cdim, char Board[][8])
{
    int p=1;
    for (int k=0;k<8;k++)
    {
        for (int i=0;i<8;i++)
        {
            PrintInCenter2(p,Board[k][i],rdim,cdim);
            p++;
        }
    }
}
bool IsCapital (char sym)
{
    if (sym>='A' && sym<='Z')
        return true;
    return false;
}
bool IsSmall (char sym)
{
    if (sym>='a' && sym<='z')
        return true;
    return false;
}

bool IsValidSelect (char B[][8],int Turn, int Sr, int Sc)
{
    if (Sr<0||Sr>7|| Sc<0||Sc>7)
        return false;
    char sym=B[Sr][Sc];
    if (capital==Turn && IsCapital(sym))
        return true;
    else if ((Small==Turn) && (IsSmall(sym)) )
        return true;
    return false;

}
bool IsValidDest (char B[][8], int Turn,int Sr, int Sc ,int dr, int dc)
{
    if (dr<0||dr>7|| dc<0||dc>7)
        return false;
    char sym=B[dr][dc];
    if (capital==Turn && !IsCapital(sym)  && (IsLegalMove(B,Sr,Sc,dr,dc,Turn)))
    {
        return true;
//        &&!Check(B,Sr,Sc,dr,dc,Turn)
    }

    else if ((Small==Turn)&& !(IsSmall(sym))&& (IsLegalMove(B,Sr,Sc,dr,dc,Turn)))
        return true;
    return false;

}

void MakeMoveOnBoard(char B[][8],int Sri,int Sci,int dri,int dci)
{
    B[dri][dci]=B[Sri][Sci];
    B[Sri][Sci]='.';
}
void TurnChanger (int & Turn)
{
    Turn=(Turn+1)%2;
}
int ChangedTurnOf (int Turn)
{
    return (Turn+1)%2;
}
void AskForPoints (char B[][8],int & r, int & c,int rdim,int cdim,int & Turn)
{
//    cin>>r;
//    cin>>c;

    getRowColbyLeftClick(r,c);
    r=r/rdim;
    c=c/cdim;

    if (r==4 && c==13)
    {
        Init(B);
        Turn=0;
        Print_Board(rdim,cdim,B);
    }
    if (r==4 && c==11)
    {
        Load_2(B,Turn);
//        TurnChanger(Turn);
        Print_Board(rdim,cdim,B);
    }
    Save_2(B,Turn);

    if (r==4 && c==9)
    {
        Save(B,Turn);
        exit(1);
    }
}

bool Is_NS_move(int Sr, int Sc, int dr, int dc)
{
    if (Sc==dc)
        return true;
    return false;
}
bool Is_EW_move (int Sr, int Sc, int dr, int dc)
{
    if (Sr==dr)
        return true;
    return false;

}
bool Is_Sloppy_move (int Sr, int Sc, int dr, int dc)
{
    int Rchange=dr-Sr;
    int Cchange=dc-Sc;

    return (Rchange==Cchange || Rchange==-Cchange);
}

bool Is_NS_pathClear (char B[][8], int Sr, int Sc, int dr, int dc)
{
//    if (Sr==dr)
//        return false;
    if (Sr<dr)
    {
        for(int i=Sr+1; i<dr; i++)
        {
            if ( B[i][Sc]!='.')
                return false;
        }
    }
    else if (Sr>dr)
    {
        for (int i=Sr-1; i>dr;i--)
        {
            if ( B[i][Sc]!='.')
                return false;
        }
    }
    return true;
}

bool Is_EW_pathClear (char B[][8], int Sr, int Sc, int dr, int dc)
{
    if (Sc<dc)
    {
        for(int i=Sc+1; i<dc; i++)
        {
            if ( B[Sr][i]!='.')
                return false;
        }
    }
    if (Sc>dc)
    {
        for (int i=Sc-1; i>dc;i--)
        {
            if ( B[Sr][i]!='.')
                return false;
        }
    }
    return true;
}
bool Is_Sloppy_PathClear (char B[][8], int Sr, int Sc, int dr, int dc)
{
    if (Sr<dr&&Sc<dc)
    {
        for (int r=Sr+1, c=Sc+1;r<dr; r++,c++)
        {
            if ( B[r][c]!='.')
                return false;
        }
    }

    else if (Sr>dr&&Sc>dc)
    {
        for (int r=Sr-1, c=Sc-1;r>dr; r--,c--)
        {
            if ( B[r][c]!='.')
                return false;
        }
    }

    else if (Sr<dr&&Sc>dc)
    {
        for (int r=Sr+1, c=Sc-1;r<dr; r++,c--)
        {
            if ( B[r][c]!='.')
                return false;
        }
    }

    else
    {
         for (int r=Sr-1, c=Sc+1;r>dr; r--,c++)
        {
            if ( B[r][c]!='.')
                return false;
        }
    }
    return true;
}
bool IsLegal_Rook_Move (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    return (((Is_NS_move(Sr,Sc,dr,dc)) && (Is_NS_pathClear(B,Sr,Sc,dr,dc)) )
            || ((Is_EW_move(Sr,Sc,dr,dc)) && (Is_EW_pathClear(B,Sr,Sc,dr,dc)) ) );
}
bool IsLegal_Bishop_Move (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    return ( (Is_Sloppy_move(Sr,Sc,dr,dc) ) && (Is_Sloppy_PathClear(B,Sr,Sc,dr,dc)) );
}
bool IsKill (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    char sym=B[dr][dc];
    if (turn==capital && IsSmall(sym))
        return true;
    if (turn==Small && IsCapital(sym))
        return true;
    return false;

}

bool IsLegalMove (char B[][8], int Sr,int Sc,int dr,int dc,int turn)
{
    char sym=B[Sr][Sc];

    switch(sym)
    {
        case 'R':
        case 'r':
            {
                return IsLegal_Rook_Move(B,Sr,Sc,dr,dc,turn);
            }
        case 'B':
        case 'b':
            {
                return IsLegal_Bishop_Move(B,Sr,Sc,dr,dc,turn);
            }
        case 'Q':
        case 'q':
            {
                return (IsLegal_Bishop_Move(B,Sr,Sc,dr,dc,turn)||IsLegal_Rook_Move(B,Sr,Sc,dr,dc,turn));
            }
        case 'H':
        case 'h':
            {
                int deltaR= abs(dr-Sr);
                int deltaC= abs(dc-Sc);
                if ( (deltaR==1&&deltaC==2) ||  (deltaR==2&&deltaC==1) )
                    return true;
                return false;
            }
        case 'K':
        case 'k':
            {
                int deltaR= abs(dr-Sr);
                int deltaC= abs(dc-Sc);
                if (deltaR==1 || deltaC==1)
                {
                    return (IsLegal_Bishop_Move(B,Sr,Sc,dr,dc,turn)||IsLegal_Rook_Move(B,Sr,Sc,dr,dc,turn));
                }
                else
                    return false;
            }
        case 'P':
            {
                int deltaR=(dr-Sr);
                int deltaC=abs(dc-Sc);
                if ((deltaC==0 && !IsKill(B,Sr,Sc,dr,dc,turn)) || ( deltaC==1 && IsKill(B,Sr,Sc,dr,dc,turn)) )
                {
//                    &&!(Check(B,Sr,Sc,dr,dc,turn))
                    if ( Sr==1 && (deltaR<=2 && deltaR>0 && !IsKill(B,Sr,Sc,dr,dc,turn)) )
                    {
                        return true;
                    }
                    else if ( deltaR==1)
                    {
                        return true;
                    }

                    return false;
                }
            }
        case 'p':
            {
                int deltaR=(dr-Sr);
                int deltaC= abs(dc-Sc);
                if ( (deltaC==0 && !IsKill(B,Sr,Sc,dr,dc,turn)) ||( deltaC==1  && IsKill(B,Sr,Sc,dr,dc,turn)) )
                {
                    if (Sr==6 && deltaR>=-2 && !IsKill(B,Sr,Sc,dr,dc,turn))
                        return true;
                    else if (deltaR==-1  )
                        return true;
                    return false;
                }
            }
    }
    return false;
}
void Highlight (char C[][8],char B[][8],int Sr,int Sc,int dr,int dc,int Turn)
{
    for (int i=0;i<8;i++)
    {
        for (int k=0;k<8;k++)
        {
            if(i==2 && k==3)
            {
                i++;
                i--;
            }
            if (IsValidDest(B,Turn,Sr,Sc,i,k))
                C[i][k]='*';
        }
    }
}
void Save (char B[][8], int Turn)
{
    ofstream GivingTo ("file.txt");
    GivingTo<<Turn<<" ";
    for (int i=0; i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            GivingTo<<B[i][c]<<" ";
        }
    }
}
void Load (char B[][8],int & Turn)
{
    ifstream Take ("case 3.txt");
    Take>>Turn;
    for (int i=0; i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            Take>>B[i][c];
        }
    }
}
void Save_2 (char B[][8], int Turn)
{
    ofstream GivingTo ("file2.txt");
    GivingTo<<Turn<<" ";
    for (int i=0; i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            GivingTo<<B[i][c]<<" ";
        }
    }
}
void Load_2 (char B[][8],int & Turn)
{
    ifstream Take ("file2.txt");
    Take>>Turn;
    for (int i=0; i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            Take>>B[i][c];
        }
    }
}

void King_Finder (char B[][8],int & rk, int & ck,int Turn)
{
    for (int r=0;r<8;r++)
    {
        for (int c=0;c<8;c++)
        {
            if (Turn==capital && B[r][c]=='K')
            {
                rk=r;
                ck=c;
            }
            else if (Turn==Small && B[r][c]=='k')
            {
                rk=r;
                ck=c;
            }
        }
    }
}


bool Check (char B[][8], int Sr,int Sc,int dr,int dc,int Turn)
{
    int rk,ck;
//    int w;
    King_Finder(B,rk,ck,Turn);
    TurnChanger(Turn);

    for (int i=0;i<8;i++)
    {
        for (int c=0;c<8;c++)
        {
            if ( IsValidSelect(B,Turn,i,c) && IsValidDest(B,Turn,i,c,rk,ck))
            {

                gotoRowCol(60,90);
                cout<<"Check! Player "<<ChangedTurnOf(Turn)+1<<"'s King is In Danger ....";
                return true;
//                w=c;
                break;
            }
            else
            {
                gotoRowCol(60,90);
                cout<<"                                        ";
            }

        }
//        if ( IsValidSelect(B,Turn,i,w) && IsValidDest(B,Turn,i,w,rk,ck))
//            {
//                break;
//            }
    }
    return false;
}

bool CheckMate (char B[][8],int dr,int dc,int Turn)
{
    int rk,ck;
    King_Finder(B,rk,ck,Turn);
    for (int rk=0;rk<8;rk++)
    {
        for (int ck=0;ck<8;ck++)
        {
            for (int i=0;i<8;i++)
            {
                for (int c=0;c<8;c++)
                {
                    if ( (IsValidSelect(B,Turn,rk,ck)) && IsValidDest(B,Turn,rk,ck,i,c) && !Check(B,rk,ck,i,c,Turn) )
                    {
                        return false;
                    }
                }
            }
        }
    }
//    for (int i=0;i<8;i++)
//    {
//        for (int c=0;c<8;c++)
//        {
//            if ( (IsValidSelect(B,Turn,rk,ck)) && IsValidDest(B,Turn,rk,ck,i,c) && !Check(B,rk,ck,i,c,Turn) )
//            {
//                return false;
//            }
//        }
//    }
    return true;
}
void Pawn_Promote (char B[][8], int dr, int dc, int Turn)
{
    for (int i=0;i<8;i++)
    {
        if (B[0][i]=='p')
        {
            B[0][i]='q';
            gotoRowCol(70,90);
            cout<<"Promoted to Queen ";
        }
        else if (B[7][i]=='P')
        {
            B[7][i]='Q';
            gotoRowCol(70,90);
            cout<<"Promoted to Queen ";
        }
        else
        {
            gotoRowCol(70,90);
            cout<<"                    ";
        }

    }
//    if (Turn==capital && B[dr][dc]=='P' && dr==7)
//    {
//        B[dr][dc]='Q';
//        gotoRowCol(70,90);
//        cout<<"Promoted to Queen ";
//    }
//    else if (Turn==Small && B[dr][dc]=='p' && dr==0)
//    {
//        B[dr][dc]='q';
//        gotoRowCol(70,90);
//        cout<<"Promoted to Queen ";
//    }
}

int PrintDebug(char B[][8])
{
    for(int r=0; r<8; r++)
    {
        for(int c=0; c<8; c++)
        {
            gotoRowCol(75+r,95+c);
            cout<<B[r][c];
        }
    }

    return 0;
}


int main()
{
//    system("Color 2B");
//    system("Color 2B");
//    system("Color 3C");
//    system("Color 4D");
//    system("Color 5E");
//    system("Color 6F");
//    system("Color A1");
//    system("Color B2");
//    system("Color C3");
//    system("Color D4");
//    system("Color E5");
    system("Color F0");

    int Turn=0,rdim=10,cdim=10;
    int Sr,Sc,dr,dc;

    cout<<"New Game Or Load game (N/L) : ";
    cin>>choice;
//    ChangeColor(9);
    if (choice=='N' || choice =='n')
    {
        Init(B);
    }
    else if (choice=='L' || choice =='l')
    {
        Load(B,Turn);
    }
//    getch();
    PrintGrid(rdim,cdim,B);
    Print_Board(rdim,cdim,B);
    gotoRowCol(20,100);

    cout<<"Select Again To Unselect!";
    do
    {
      //  char C [8][8];
        for (int i=0;i<8;i++)
        {
            for (int c=0;c<8;c++)
            {
                C[i][c]=' ';
            }
        }
        Print_Board2(rdim,cdim,C);
        do
        {
            if (Check(B,Sr,Sc,dr,dc,ChangedTurnOf(Turn) ) )
            {

                Load_2(B,Turn);
                Print_Board(rdim,cdim,B);

            }
            do
            {
                gotoRowCol(5,83);
                cout<<"P"<<Turn+1<<"'s Turn : ";
                cout<<"Select Piece....: ";
                AskForPoints(B,Sr,Sc,rdim,cdim,Turn);
            }
            while(IsValidSelect(B,Turn,Sr,Sc)!=true);

            for (int i=0;i<8;i++)
            {
                for (int c=0;c<8;c++)
                {
                    C[i][c]=' ';
                }

            }
            dr++;

            dr--;

            PrintDebug(B);
            Highlight(C,B,Sr,Sc,dr,dc,Turn);

            Print_Board2(rdim,cdim,C);

            gotoRowCol(10,83);
            cout<<"P"<<Turn+1<<"'s Turn : ";
            cout<<"Select Piece Destination...: ";
            AskForPoints(B,dr,dc,rdim,cdim,Turn);
        }
        while(IsValidDest(B,Turn,Sr,Sc,dr,dc)!=true);

        Pawn_Promote(B,dr,dc,Turn);
        MakeMoveOnBoard(B,Sr,Sc,dr,dc);
        Print_Board(rdim,cdim,B);
        TurnChanger(Turn);
        if (Check(B,Sr,Sc,dr,dc,Turn))
        {
                if (CheckMate(B,dr,dc,Turn) )
                {
                    gotoRowCol(65,90);
                    cout<<"Check Mate! ";
                }

                else
                {
                    gotoRowCol(65,90);
                    cout<<"            ";
                }
        }


    }
    while (true);


    cout << "\n\n\n\n\n\n";
    getch();
    return 0;
}
