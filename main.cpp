#include <iostream>
#include <windows.h>
#include <ctime>
#include <cstdio>
#include <conio.h>
#include <fstream>
#include <sstream>

using namespace std;
#define  COLSTART   3       // Tọa độ cột của ký tự trên Console của Cell(0,0)
#define  ROWSTART   5       // Tọa độ dòng của ký tự trên Console của Cell(0,0)
#define  COLSTART2  36      // Tọa độ cột của ký tự trên Console của Cell(0,0)
#define  ROWSTART2  5       // Tọa độ dòng của ký tự trên Console của Cell(0,0)
#define  CELL_CX    4       // Chiều rộng 1 Cell là 4 ký tự "+---" trong Console
#define  CELL_CY    2       // Chiều cao 1 Cell là 2 dòng ký tự trong Console
#define  OPEN       1       //trạng thái ô mở
#define  CLOSE      0       //trạng thái ô đóng
#define  OPEN_TRUE  2
#define  TRUE       1       //giá trị ô chứa tàu
#define  FALSE      0       //giá trị ô không chứa tàu

struct Cell{
  char kl[10][10];
  int status[10][10];
  int value[10][10];
};

struct Ship{
    int a[3][2];    // Tọa độ tàu dạng hệ tọa độ 2 chiều (tọa độ kép)
    int b[3];       // Tọa độ tàu dạng số đếm (tọa sộ đơn)
};

void gotocell(int ,int );               //đưa con trỏ đến một ô trên sân của người choi 2
void gotocell2(int ,int );              //đưa con trỏ đến một ô trên sân của người choi 1
void gotoxy(short ,short );                 //đưa con trỏ đến tọa độ cho trước trên màn hình console
int control(int ,int& ,int& ,int);      //điều khiển con trỏ nháy
void input(Ship &sh1,Ship &sh2,int );   //nhập hoặc random dữ liệu về tọa độ tàu từ người chơi
int play(int& ,int& ,int& ,int& ,int );  //thao tác chơi
void auto_play(int& ,int& ,int& ,int& ,int& ,int& ,int&,int *p,int *q,bool ,int,Ship &vsh1);  //phân tích của máy đưa ra tọa độ bắn(khi chơi với máy)
bool isAdjoining(Ship sh1, Ship sh2);     //kiểm tra hai tàu có sát nhau không
void chose_mode(int, int&);              //cho phép lựa chọn tự nhập tọa độ tàu hoặc để máy random tọa độ tàu
void levelf(int, int&);                 //chọn level
void menuf(int,int &,int &);            //tùy chọn menu
void number_of_player(int,int&);        //thiết lập chế độ số người chơi
void playing_interface(int);                    //in ra màn hình 2 sân chơi
bool checkShip(int , int , int , int , int , int ); //kiểm tra xem tọa độ hai tàu người choi nhập có thảo mãn không
bool check2Ships(Ship sh1, Ship sh2);   // Kiểm tra 2 tàu có đè lên nhau không. Trả về true nếu không bị đè, ngược lại false
void setShip(int ,int ,int ,int ,int ,int ,Ship& );   // gán giá trị tàu qua tham số
void randShip(Ship &sh);                // Sinh ngẫu nhiên tọa độ 1 tàu
void set_rand(Ship &sh1, Ship &sh2);    // Sinh ngẫu nhiên tọa độ 2 tàu sao cho 2 tàu ko chồng lên nhau
void displayShip(Ship  );              // Hiển thị tọa độ tàu (Chỉ để test)
bool statusShip(Ship ,int);             // kiểm tra tình trạng của từng tàu
void value_ship(Ship,int);              //tạo giá trị cho các ô chứa tàu
int check_win(Ship , Ship, int);         // kiểm tra chiến thắng
int opencell(int ,int ,int );           //tọa giá trị cho ô được mở
void open_file(ifstream &file);         // Hiển thị nội dung file ra màn hình
void show_result(int player);           //Hiện toàn bản đồ của người chơi player

Cell cell1,cell2;

int main()
{
    int out = 0;
    do{
        srand(time(0));
        system("cls");
        HANDLE hConsoleColor;
        hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsoleColor, 11);
        for(int i = -1; i <= 6; i++)
            for(int j = -1; j <= 6 ; j++){
                cell1.status[i][j] = 10;
                cell2.status[i][j] = 10;
            }
        for(int i = 0; i <= 5; i++)
            for(int j = 0; j <= 5 ; j++){
                cell1.kl[i][j] = ' ';
                cell1.status[i][j] = CLOSE;
                cell1.value[i][j] = FALSE;
                cell2.kl[i][j] = ' ' ;
                cell2.value[i][j] = FALSE;
                cell2.status[i][j] = CLOSE;
            }
        int iCol = 0,iRow = 0;
        Ship sh1, sh2, sh3, sh4, vsh1;
        int num_of_player = 0, cskey, cfkey;
        int index = 0, index_shoot = 0, *q, *p, level = 0, num_shoot = 0, cell_true = 0,
            x, y, check_break = 0, condition_random = 0, menu = 0, keylevel , _y = 12;
        bool isA;
        ifstream help("help.txt"),heading("start.txt");
        q = new int[40];
        p = new int[40];

        //--------------------------------------------------------------------

        while(menu != 1 && menu != 2 && menu != 3){
            gotoxy(0,1);
            open_file(heading);
            gotoxy(44,10); cout << "MENU" <<endl;
            gotoxy(34,12); cout << "====>>";
            gotoxy(46,12); cout << " PLAY" << endl;
            gotoxy(46,13); cout << " HELP" << endl;
            gotoxy(46,14); cout << " EXIT";
            gotoxy(45,12);
            cfkey = _getch();
            if(cfkey == 13)
                menu = 1;
            else
                do{
                    cskey=_getch();
                    if(cskey == 72 || cskey == 80 )
                        menuf(cskey,menu,_y);
                }while(cskey != 13);
        }
        if(menu == 1)
        {
            system("cls");
            while(num_of_player != 1 && num_of_player != 2){
                gotoxy(40,10); cout << "CHOSE GAME MODE" <<endl;
                gotoxy(40,12); cout << "====>>";
                gotoxy(46,12); cout << " 1 player" << endl;
                gotoxy(46,13); cout << " 2 player" ;
                gotoxy(45,12);
                cfkey = _getch();
                if(cfkey == 13)
                    num_of_player = 1;
                else
                    do{
                        cskey = _getch();
                        if(cskey == 72 || cskey == 80 )
                            number_of_player(cskey,num_of_player);
                    }while(cskey != 13);
            }
        cout << endl ;

        //----------------------------------------------------------------

        if( num_of_player == 2)
        {
            playing_interface(1);
            input(sh1,sh2,1);
            cout << "Press [ENTER] to continue ..." ;
            cin.ignore();
            playing_interface(2);
            cout << endl ;
            input(sh3,sh4,2);
            playing_interface(1);
            int ch;
            do{
                gotocell2(iRow,iCol);
                do{
                    int h;
                    ch = getch();
                    if(play(iCol,iRow,h,ch,2) == 0)
                        check_break = 1;
                    if(check_win(sh3,sh4,2) == 1 ){
                        gotoxy(0,18);
                        SetConsoleTextAttribute(hConsoleColor, 10);
                        cout << "Player 1 WIN !!" <<endl << "Press [ENTER] to continue ..." ;
                        SetConsoleTextAttribute(hConsoleColor, 11);
                        check_break = 1;
                    }
                    gotocell2(iRow,iCol);
                    if(h == 1)
                        break;
                }while(ch != 32 );
                if(check_break == 1){
                    gotoxy(0,19);
                    cout <<"Press [ENTER] to continue ...";
                    cin.ignore(1);
                    show_result(1);
                    break;
                }
                gotocell(iRow,iCol);
                do{
                    int h;
                    ch = getch();
                    if(play(iCol,iRow,h,ch,1) == 0)
                        check_break = 1;
                    if(check_win(sh1,sh2,1) == 1 ){
                        gotoxy(0,18);
                        SetConsoleTextAttribute(hConsoleColor, 10);
                        cout << "Player 2 WIN !!" << endl<< "Press [ENTER] to continue ..." ;
                        SetConsoleTextAttribute(hConsoleColor, 11);
                        check_break = 1;
                    }
                    gotocell(iRow,iCol);
                    if(h == 1)
                        break;
                }while(ch != 32 );
            if(check_break==1){
                gotoxy(0,19);
                cout <<"Press [ENTER] to continue ...";
                cin.ignore(1);
                show_result(2);
                break;
            }
            }while(ch);
        }

    //------------------------------------------------------------------------------

        else if(num_of_player == 1){
            system("cls");
            while(level != 1 && level != 2){
                gotoxy(40,10); cout << "CHOSE LEVEL" <<endl;
                gotoxy(40,12); cout << "====>>";
                gotoxy(46,12); cout << " easy" << endl;
                gotoxy(46,13); cout << " hard" ;
                gotoxy(45,12);
                keylevel = _getch();
                if(keylevel == 13)
                    level = 1;
                else
                    do{
                        keylevel = _getch();
                        if(keylevel == 72 || keylevel == 80 )
                            levelf(keylevel,level);
                    }while(keylevel != 13);
            }

            //--------------------------------------------------------------

            input(sh1,sh2,1);
            isA = isAdjoining(sh1,sh2);
            cout << "Press [ENTER] to continue ..." ;
            cin.ignore();
            playing_interface(3);
            do{
                set_rand(sh3 ,sh4);
            }while(check2Ships(sh3,sh4));
            value_ship(sh3,2);
            value_ship(sh4,2);
            playing_interface(3);
            int ch;
            do{
                gotocell2(iRow,iCol);
                do{
                    int h;
                    ch = getch();
                    if(play(iCol,iRow,h,ch,2) == 0)
                        check_break = 1;
                    gotocell2(iRow,iCol);
                    if(h == 1)
                        break;
                }while(ch != 32 );
                if(check_break == 1){
                    gotoxy(0,19);
                    break;
                }
                if(check_win(sh3,sh4,2) == 1 ){
                    gotoxy(0,18);
                    SetConsoleTextAttribute(hConsoleColor, 10);
                    cout << "Player 1 WIN !!" <<endl<< "Press [ENTER] to continue ...";
                    cin.ignore(1);
                    SetConsoleTextAttribute(hConsoleColor, 11);
                    show_result(1);
                    break;
                }

                //--------------------------------------------------

                auto_play(x,y,num_shoot,cell_true,condition_random,index,index_shoot,p,q,isA,level,vsh1);
                opencell(x,y,1);
                if(cell_true == 3){
                    for(int t = 0; t <= 5; t++)
                        for(int j = 0;j <= 5; j++)
                            if(cell1.status[t][j] == OPEN && cell1.value[t][j] == TRUE)
                                cell1.value[t][j] = OPEN_TRUE;
                }
                num_shoot ++;
                if(check_win(sh1,sh2,1) == 1){
                    gotoxy(0,18);
                    opencell(x,y,1);
                    playing_interface(3);
                    SetConsoleTextAttribute(hConsoleColor, 10);
                    cout << "The Machine WIN !!" <<endl<< "Press [ENTER] to continue ...";
                    cin.ignore(1);
                    SetConsoleTextAttribute(hConsoleColor, 11);
                   show_result(3);
                    break;
                }
                playing_interface(3);
            }while(1);
        }
        SetConsoleTextAttribute(hConsoleColor, 11);
        cout << "Press [ENTER] to come back the menu ..." ;
        cin.ignore(1);
        }
        else if(menu == 2){
            system("cls");
            open_file(help);
            cin.ignore(1);
        }
    else if(menu == 3)
        return 0;
    }while(out == 0);
     return 0;
}

//==========================================================

int opencell(int i, int j,int a)
{
    if(a == 1){
        if(cell1.status[i][j] == CLOSE ){
            cell1.status[i][j] = OPEN;
            if(cell1.value[i][j] == TRUE || cell1.value[i][j] == OPEN_TRUE){
                cell1.kl[i][j] = 254;
            }
            else
                cell1.kl[i][j] = 248;
            return 1;
        }
        else
            return 0;
    }
    else
        if(a == 2){
            if(cell2.status[i][j] == CLOSE ){
                cell2.status[i][j] = OPEN;
                if(cell2.value[i][j] == TRUE || cell2.value[i][j] == OPEN_TRUE)
                    cell2.kl[i][j] = 254;
                else
                    cell2.kl[i][j] = 248;
                return 1;
            }
            else
                return 0;
        }
        else return 0;
}

//==========================================================

void gotoxy(short x, short y)
{
    COORD xyPos = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xyPos);
}

//==========================================================

void gotocell(int iRow, int iCol)
{
    gotoxy(COLSTART + iCol * CELL_CX, ROWSTART + iRow * CELL_CY);
}

//==========================================================

void gotocell2(int iRow, int iCol)
{
    gotoxy(COLSTART2 + iCol * CELL_CX, ROWSTART2 + iRow * CELL_CY);
}

//==========================================================

void playing_interface(int a)
{
    system("cls");
    HANDLE hConsoleColor;
    hConsoleColor = GetStdHandle(STD_OUTPUT_HANDLE);
    cout << "         ============" << " BATTLE SHIPS " << "============" << endl ;
    cout << "        ----------------------------------------" << endl;
    cout << "       Player 1                          Player 2";
    int j = 0;
    int r = 0;
    cout << endl ;
    for(int i = 0; i < 6; i++)
        cout << "   " << i ;
    cout << "         ";
    for(int i = 0; i < 6; i++)
        cout << "   " << i ;
    cout << endl;
    cout << " ";
    for(int i = 0; i < 6; i++)
        cout << "+---";
    cout << "+" ;
    cout << "        ";
    for(int i = 1; i <= 6; i++)
        cout << "+---";
    cout << "+" ;
    cout << endl;
    while( j <= 5 && r <= 5){
        cout << j;
        for(int i = 0; i < 6; i++){
            cout << "| ";
            if(cell1.value[r][i] == TRUE || cell1.value[r][i] == OPEN_TRUE){
                SetConsoleTextAttribute(hConsoleColor, 12);
                cout << cell1.kl[r][i];
                SetConsoleTextAttribute(hConsoleColor, 11);
            }
            else{
                SetConsoleTextAttribute(hConsoleColor, 14);
                cout << cell1.kl[r][i] ;
                SetConsoleTextAttribute(hConsoleColor, 11);
            }
            cout<< " ";
        }
        j++;
        cout << "|";
        cout << "       " << r;
        for(int i =0; i < 6 ;i++){
            cout << "| ";
            if(cell2.value[r][i] == TRUE ){
                SetConsoleTextAttribute(hConsoleColor, 12);
                cout << cell2.kl[r][i];
                SetConsoleTextAttribute(hConsoleColor, 11);
            }
            else{
                SetConsoleTextAttribute(hConsoleColor, 14);
                cout << cell2.kl[r][i] ;
                SetConsoleTextAttribute(hConsoleColor, 11);
            }
            cout<< " ";
        }
        r++;
        cout << "|";
        cout << endl ;
        cout << " ";
        for(int i = 0; i < 6 ;i++)
            cout << "+---";
        cout << "+" ;
        cout << "        ";
        for(int i = 0; i < 6 ;i++)
            cout << "+---";
        cout << "+" ;
        cout << endl;
    }
    cout << "(while playing)    [Esc]: come back the menu     [Space]: End turn" << endl;
    if(a == 1)
        cout <<"Turn: Player 1" << endl;
    else if(a == 2)
        cout <<"Turn: Player 2" << endl;
    else if(a == 3)
        cout << "Play with machine" <<endl;
}

//==========================================================

int control(int sKey, int & iRow, int & iCol,int a)
{
    int k = 0;
    if(a == 1){
        switch (sKey){
            case 75: case 97:       if (iCol > 0) iCol--;
                                    else iCol = 6 - 1;    break;      // Left
            case 77: case 100:      if (iCol < 6 - 1) iCol++;
                                    else iCol = 0;    break;      // Right
            case 72: case 119:      if (iRow > 0) iRow--;
                                    else iRow = 6 - 1;    break;      // Up
            case 80: case 115:      if (iRow < 6 - 1) iRow++;
                                    else iRow = 0;    break;      // Down
            case 13: case 103:      k = opencell(iRow,iCol,a);
                                    playing_interface(1);
                                    return k ;   break;
        }
    }
    else if(a == 2){
        switch (sKey){
           case 75: case 97:        if (iCol > 0) iCol--;
                                    else iCol = 6 - 1;    break;      // Left
           case 77: case 100:       if (iCol < 6 - 1) iCol++;
                                    else iCol = 0;    break;      // Right
           case 72: case 119:       if (iRow > 0) iRow--;
                                    else iRow = 6 - 1;    break;      // Up
           case 80: case 115:       if (iRow < 6 - 1) iRow++;
                                    else iRow = 0;    break;      // Down
           case 13: case 103:       k = opencell(iRow,iCol,a);
                                    playing_interface(2);
                                    return k;    break;
        }
    }
    return 0;
}

//==========================================================

void randShip(Ship &sh){
    do {
        sh.b[1] = rand() % 35;   // Sinh ngãu nhiên tọa độ giữa của con tàu
    }while(
        sh.b[1] == 0 ||     // Nếu tọa độ giữa của tàu vào các góc của bảng thì sinh lại
        sh.b[1] == 5 ||     //
        sh.b[1] == 30||     //
        sh.b[1] == 35       //
    );
    int dir = rand() % 2; // dir = 0 thì ngang, khác 0 thì dọc
    if ( sh.b[1] % 6 == 0 || sh.b[1] % 6 == 5 ) { // Các trường hợp tàu dọc
        sh.b[0] = sh.b[1] - 6;
        sh.b[2] = sh.b[1] + 6;
    }else if ( sh.b[1] < 5 || sh.b[1] > 30 ){     // Các trường hợp tàu ngang
        sh.b[0] = sh.b[1] - 1;
        sh.b[2] = sh.b[1] + 1;
    }else if ( dir ){                             // Các trường hợp tàu dọc
        sh.b[0] = sh.b[1] - 6;
        sh.b[2] = sh.b[1] + 6;
    }else{                                        // Các trường hợp tàu ngang
        sh.b[0] = sh.b[1] - 1;
        sh.b[2] = sh.b[1] + 1;
    }
    for (int i = 0; i < 3; ++i){        // Chuyển từ tọa độ đơn sang tọa độ kép
        sh.a[i][0] = sh.b[i] / 6;
        sh.a[i][1] = sh.b[i] % 6;
    }
}

//==========================================================

bool checkShip(int r1, int c1, int r2, int c2, int r3, int c3){
    if (r1 < 0 || r2 < 0 || r3 < 0 ||
        c1 < 0 || c2 < 0 || c3 < 0 ||
        r1 > 5 || r2 > 5 || r3 > 5 ||
        c1 > 5 || c2 > 5 || c3 > 5)
        return false;
    if (r1 == r2 && r2 == r3){   // Nếu cùng dòng
        if ( c1 == c2 || c1 == c3 || c2 == c3 )     // Nếu các ô trùng nhau
            return false;
        else
            if (abs(c1 - c2) == 1){
                if (abs(c1 - c3) == 1 || abs(c2 - c3) == 1)
                    return true;
            }else if (abs(c1 - c3) == 1 && abs(c2 - c3) == 1)
                    return true;
    }
    if (c1 == c2 && c2 == c3){      // Nếu cùng cột
        if ( r1 == r2 || r1 == r3 || r2 == r3 )     // Nếu các ô trùng nhau
            return false;
        else
            if (abs(r1 - r2) == 1){
                if (abs(r1 - r3) == 1 || abs(r2 - r3) == 1)
                    return true;
            }else if (abs(r1 - r3) == 1 && abs(r2 - r3) == 1)
                    return true;
    }
    return false;
}

//==========================================================

bool check2Ships(Ship sh1, Ship sh2){
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j){
            if (sh1.a[i][0] == sh2.a[j][0] && sh1.a[i][1] == sh2.a[j][1]){
                return true;
                break;
            }
        }
    return false;
}

//==========================================================

void set_rand(Ship &sh1, Ship &sh2){
    randShip(sh1);      // Sinh ngẫu nhiên tàu sh1
    do {                // Sinh ngẫu nhiên tàu sh2 cho đến khi sh1 không chồng lên sh2
        randShip(sh2);
    }while (
        sh1.b[0] == sh2.b[0] ||
        sh1.b[1] == sh2.b[1] ||
        sh1.b[2] == sh2.b[2]
    );
}

//==========================================================

void setShip(int r1, int r2, int r3, int r4, int r5, int r6, Ship &sh){
    sh.a[0][0]= r1;     sh.a[0][1] = r2;     sh.a[1][0] = r3;
    sh.a[1][1] = r4;     sh.a[2][0] = r5;     sh.a[2][1] = r6;
}

//==========================================================

void displayShip(Ship sh){
    cout  << endl
         << sh.a[0][0] << "," << sh.a[0][1] << " "
         << sh.a[1][0] << "," << sh.a[1][1] << " "
         << sh.a[2][0] << "," << sh.a[2][1] << endl;
}

//==========================================================

void value_ship(Ship sh,int a)
{
    if(a == 1){
            cell1.value[sh.a[0][0]][sh.a[0][1]] = TRUE;
            cell1.value[sh.a[1][0]][sh.a[1][1]] = TRUE;
            cell1.value[sh.a[2][0]][sh.a[2][1]] = TRUE;
    }
    else{
            cell2.value[sh.a[0][0]][sh.a[0][1]] = TRUE;
            cell2.value[sh.a[1][0]][sh.a[1][1]] = TRUE;
            cell2.value[sh.a[2][0]][sh.a[2][1]] = TRUE;
    }
}

//==========================================================

bool statusShip(Ship sh,int a)
{
    if(a == 1){
            if(   cell1.status[sh.a[0][0]][sh.a[0][1]] == OPEN
               && cell1.status[sh.a[1][0]][sh.a[1][1]] == OPEN
               && cell1.status[sh.a[2][0]][sh.a[2][1]] == OPEN )
                return true;
            else
                return false;
    }
    else if(a == 2){
            if(   cell2.status[sh.a[0][0]][sh.a[0][1]] == OPEN
               && cell2.status[sh.a[1][0]][sh.a[1][1]] == OPEN
               && cell2.status[sh.a[2][0]][sh.a[2][1]] == OPEN )
                return true;
            else
                return false;
    }
    else
        return false;
}

//==========================================================

int check_win(Ship sh1, Ship sh2,int a)
{
    if(statusShip(sh1,a) && statusShip(sh2,a) )
        return 1;
    else if(  (statusShip(sh1,a) && !statusShip(sh2,a) )
            ||(statusShip(sh2,a) && !statusShip(sh1,a)) )
        return 2;
    else
        return 0;
}

//==========================================================

void chose_mode(int skey,int &numa)
{
    switch(skey){
        case 72:    gotoxy(13,20); cout << ">" ;
                    numa = 1;
                    gotoxy(14,20); cout << " Define the ships yourself";
                    gotoxy(13,21); cout << "  Let the machine define the ships";
                    gotoxy(13,20); break;
        case 80:    gotoxy(13,21); cout << ">" ;
                    numa = 2;
                    gotoxy(13,20); cout << "  Define the ships yourself";
                    gotoxy(14,21); cout << " Let the machine define the ships";
                    gotoxy(13,21); break;
    }
}

//==========================================================

void number_of_player(int cskey,int &num_of_player)
{
    switch(cskey){
        case 72:    gotoxy(40,12); cout << "====>>";
                    num_of_player = 1;
                    gotoxy(46,12); cout << " 1 player ";
                    gotoxy(40,13); cout << "       2 player ";
                    gotoxy(45,12); break;
        case 80:    gotoxy(40,13); cout << "====>>";
                    num_of_player = 2;
                    gotoxy(40,12); cout << "       1 player ";
                    gotoxy(46,13); cout << " 2 player ";
                    gotoxy(45,13); break;
    }
}

//==========================================================

void levelf(int level_key,int &level)
{
    switch(level_key){
        case 72:    gotoxy(40,12); cout << "====>>" ;
                    level = 1;
                    gotoxy(46,12); cout << " easy ";
                    gotoxy(40,13); cout << "       hard ";
                    gotoxy(45,12); break;
        case 80:    gotoxy(40,13); cout << "====>>" ;
                    level = 2;
                    gotoxy(40,12); cout << "       easy ";
                    gotoxy(46,13); cout << " hard ";
                    gotoxy(45,13); break;
    }
}

//==========================================================

void menuf(int level_key,int &menu,int &_y)
{
    int _yt = _y;
    switch(level_key){
        case 72:    if(_y == 12)
                        _y = 14;
                    else
                        _y--;
                    gotoxy(34,_y); cout << "====>>";
                    gotoxy(34,_yt);cout <<"      " ;
                    gotoxy(46,12); cout << " PLAY ";
                    gotoxy(46,13); cout << " HELP ";
                    gotoxy(46,14); cout << " EXIT ";
                    gotoxy(45,_y); break;
        case 80:    if(_y == 14)
                        _y = 12;
                    else
                        _y ++;
                    gotoxy(34,_y); cout << "====>>";
                    gotoxy(34,_yt);cout <<"      " ;
                    gotoxy(46,12); cout << " PLAY";
                    gotoxy(46,13); cout << " HELP ";
                    gotoxy(46,14); cout << " EXIT";
                    gotoxy(45,_y); break;
    }
    menu = _y-11;
}

//==========================================================

void input(Ship &sha,Ship &shb,int numa)
{
    int style1 = 0,skey;
    playing_interface(1);
    if(numa == 1)
        cout<< endl << "player 1";
    else
        cout<< endl << "player 2";
    gotoxy(15,20); cout << "Define the ships yourself";
    gotoxy(15,21); cout << "Let the machine define the ships";
    gotoxy(13,20); cout << ">";
    do{

        skey=_getch();
        if(skey == 72 || skey == 80 )
            chose_mode(skey,style1);
    }while(skey != 13);
    gotoxy(0,22);
    if(style1 == 2)
        do{
            set_rand(sha ,shb);
        }while(check2Ships(sha,shb));
    else{
        int a1,a2,a3,a4,a5,a6;
        do{
            do{
                cout << "Input the coodinates of the ship 1 (from left to right or top down to bottom): ";
                cin >> a1 >> a2 >> a3 >> a4 >> a5 >> a6;
            }while( !checkShip(a1,a2,a3,a4,a5,a6) );
            setShip(a1,a2,a3,a4,a5,a6,sha);
            do{
                cout << "Input the coodinates of the ship 2 (from left to right or top down to bottom): ";
                cin >> a1 >> a2 >> a3 >> a4 >> a5 >> a6;
            }while(!checkShip(a1,a2,a3,a4,a5,a6));
            setShip(a1,a2,a3,a4,a5,a6,shb);
        }while(check2Ships(sha,shb));
    }
    value_ship(sha,numa);
    value_ship(shb,numa);
}

//==========================================================

bool isAdjoining(Ship sh1, Ship sh2){
    int i, j;
    for (i = 0; i < 3; ++i)
        for (j = 0; j < 3; ++j){
            if ( sh1.a[i][0] == sh2.a[j][0] ){              // Nếu 2 ô của 2 tàu cùng hàng
                if ( abs(sh1.a[i][1] - sh2.a[j][1]) == 1 )  // và nếu 2 ô đó ở 2 cột liền nhau
                    return true;                            // thì 2 tàu ở sát nhau
            }else
            if ( sh1.a[i][1] == sh2.a[j][1] )               // Nếu 2 ô của 2 tàu cùng cột
                if ( abs(sh1.a[i][0] - sh2.a[j][0]) == 1)   // và nếu 2 ô đó ở 2 hàng liền nhau
                    return true;                            // thì 2 tàu sát nhau
        }
    return false;
}

//==========================================================

int play(int &iCol,int &iRow,int &h,int &ch,int n )
{
    h = control(ch, iRow, iCol, n);
    if(ch == 27){
        h = 1;
        return 0;
    }
    return 1;
}

//==========================================================

void auto_play(int &x,int &y,int &num_shoot,int &cell_true,int &condition_random,int &index,int &index_shoot,int *p,int *q,bool isA,int level,Ship &vsh1)
{
    int check;
    if(level == 2 && isA == false){
        if(cell_true % 3 == 0){
            if(num_shoot <= 5){
                do{
                    x = num_shoot;
                    y = rand() % 6;
                }while(cell1.status[x][y]!=CLOSE);
            }
            if(num_shoot > 5 && num_shoot <= 11){
                do{
                    y = num_shoot - 6;
                    x = rand() % 6;
                }while(cell1.status[x][y]!=CLOSE);
            }
            if(num_shoot > 11){
                check = 0;
                for(int t = 0; t <= 5; t++){
                    for(int j = 0;j <= 5; j++)
                        if( (cell1.status[t][j] == CLOSE
                          && cell1.status[t][j+1] == CLOSE
                          && cell1.status[t][j-1] == CLOSE)
                          ||(cell1.status[t][j] == CLOSE
                          && cell1.status[t+1][j] == CLOSE
                          && cell1.status[t-1][j] == CLOSE)){
                                check = 1;
                                x = t;
                                y = j;
                                break;
                        }
                    if(check == 1)
                        break;
                }
            }
        }
   //--------------------------------------------------------
        else
            if(cell_true % 3 == 1){
                int chose;
                for(int t = 0; t <= 5; t++)
                    for(int j = 0;j <= 5; j++)
                    {
                        if(cell1.status[t][j] == OPEN && cell1.value[t][j] == TRUE)
                        {
                            vsh1.a[0][0] = t;
                            vsh1.a[0][1] = j;
                        }
                    }
                do{
                    chose = rand() % 4;
                    switch(chose){
                        case 0: vsh1.a[1][0] = vsh1.a[0][0];
                                vsh1.a[1][1] = vsh1.a[0][1] - 1; break;
                        case 1: vsh1.a[1][0] = vsh1.a[0][0];
                                vsh1.a[1][1] = vsh1.a[0][1] + 1; break;
                        case 2: vsh1.a[1][0] = vsh1.a[0][0] - 1;
                                vsh1.a[1][1] = vsh1.a[0][1]; break;
                        case 3: vsh1.a[1][0] = vsh1.a[0][0] + 1;
                                vsh1.a[1][1] = vsh1.a[0][1]; break;
                    }
                }while( cell1.status[ vsh1.a[1][0] ][ vsh1.a[1][1] ] != CLOSE
                     ||(vsh1.a[1][0] < 0 || vsh1.a[1][0] > 5)
                     ||(vsh1.a[1][1] < 0 || vsh1.a[1][1] > 5) );
                x = vsh1.a[1][0];
                y = vsh1.a[1][1];
            }

        //---------------------------------------------------------------------------

            else if(cell_true % 3  == 2){
                int out1=0,out2=0;
                for(int t = 0; t <= 5; t++){
                    for(int j = 0;j <= 5; j++){
                        if(cell1.status[t][j] == OPEN && cell1.value[t][j] == TRUE){
                            vsh1.a[0][0] = t;
                            vsh1.a[0][1] = j;
                            out1 = 1;
                            break;
                        }
                    }
                    if(out1 == 1)
                        break;
                }
                for(int t = 0; t <= 5; t++){
                    for(int j = 0;j <= 5; j++){
                        if( cell1.status[t][j] == OPEN && cell1.value[t][j] == TRUE
                         &&(t != vsh1.a[0][0] || j !=  vsh1.a[0][1]) ){
                            vsh1.a[1][0] = t;
                            vsh1.a[1][1] = j;
                            out2 = 1;
                            break;
                        }
                    }
                    if(out2 == 1)
                        break;
                }
                if(vsh1.a[1][0] - vsh1.a[0][0] == 1){
                    if(cell1.status[vsh1.a[0][0]-1][vsh1.a[0][1]] == CLOSE){
                        x = vsh1.a[0][0] - 1;
                        y = vsh1.a[0][1];
                    }
                    else if(cell1.status[vsh1.a[1][0]+1][vsh1.a[1][1]] == CLOSE){
                        x = vsh1.a[1][0] + 1;
                        y = vsh1.a[1][1];
                    }
                }
                else if(vsh1.a[1][1] - vsh1.a[0][1]  == 1){
                    if(cell1.status[vsh1.a[0][0]][vsh1.a[0][1]- 1] == CLOSE){
                        x = vsh1.a[0][0];
                        y = vsh1.a[0][1] - 1;
                    }
                    else if(cell1.status[vsh1.a[1][0]][vsh1.a[1][1]+1] == CLOSE){
                            x = vsh1.a[1][0];
                            y = vsh1.a[1][1] + 1;
                    }
                }
            }
            if(cell1.value[x][y] == TRUE && cell1.status[x][y] == CLOSE)
                 cell_true++;
    }

    //-----------------------------------------------------------

    else{
         if((cell_true  % 3 == 0) && condition_random == 0 ){
            if(num_shoot <= 5){
                do{
                    x = num_shoot;
                    y = rand() % 6;
                }while(cell1.status[x][y] != CLOSE);
            }
            if(num_shoot > 5 && num_shoot <= 11){
                do{
                    y = num_shoot - 6;
                    x = rand() % 6;
                }while(cell1.status[x][y]!=CLOSE);
            }
            if(num_shoot > 11){
                check = 0;
                for(int t = 0; t <= 5; t++){
                    for(int j = 0;j <= 5; j++)
                        if( (cell1.status[t][j] == CLOSE
                          && cell1.status[t][j+1] == CLOSE
                          && cell1.status[t][j-1] == CLOSE)
                          ||(cell1.status[t][j] == CLOSE
                          && cell1.status[t+1][j] == CLOSE
                          && cell1.status[t-1][j] == CLOSE) ){
                            check = 1;
                            x = t;
                            y = j;
                            break;
                        }
                        if(check == 1)
                            break;
                }
            }
            if(level == 1){
                if(cell_true==3){
                    if( (vsh1.a[0][0]==vsh1.a[1][0] && vsh1.a[0][0]==vsh1.a[2][0])
                     || (vsh1.a[0][1]==vsh1.a[1][1] && vsh1.a[0][1]==vsh1.a[2][1]) )
                        condition_random = 0;
                    else
                        condition_random = 1;
                }
            }
        }
        else{
            x = q[index_shoot];
            y = p[index_shoot];
            index_shoot ++;
        }
        if(cell1.value[x][y] == TRUE && cell1.status[x][y] == CLOSE){
            cell_true++;
            if(level == 2 && (cell_true > 0))
                condition_random = 1;
            if(cell_true <= 3)
            {
                vsh1.a[cell_true-1][0] = x;
                vsh1.a[cell_true-1][1] = y;
            }
            if( (cell_true == 4 || cell_true == 5) ){
                if(cell1.status[x+1][y] == CLOSE){
                    for(int wh = index; wh >= index_shoot; wh--){
                        q[wh+1] = q[wh];
                        p[wh+1] = p[wh];
                    }
                    q[index_shoot] = x + 1;
                    p[index_shoot] = y;
                    index++;
                }
                if(cell1.status[x][y+1] == CLOSE){
                    for(int wh = index; wh >=index_shoot;wh--){
                        q[wh+1] = q[wh];
                        p[wh+1] = p[wh];
                    }
                    q[index_shoot] = x;
                    p[index_shoot] = y+1;
                    index++;
                }
                if(cell1.status[x-1][y] == CLOSE){
                    for(int wh = index; wh >=index_shoot;wh--){
                        q[wh+1] = q[wh];
                        p[wh+1] = p[wh];
                    }
                    q[index_shoot] = x-1;
                    p[index_shoot] = y;
                    index++;
                }
                if(cell1.status[x][y-1] == CLOSE){
                    for(int wh = index; wh >=index_shoot;wh--)
                    {
                        q[wh+1] = q[wh];
                        p[wh+1] = p[wh];
                    }
                    q[index_shoot] = x;
                    p[index_shoot] = y-1;
                    index++;
                }
            }
            else if( cell_true <=3 ){
                if(cell1.status[x+1][y] == CLOSE){
                    q[index] = x+1;
                    p[index] = y;
                    index++;
                }
                if(cell1.status[x][y+1] == CLOSE){
                    q[index] = x;
                    p[index] = y+1;
                    index++;
                }
                if(cell1.status[x-1][y] == CLOSE){
                    q[index] = x-1;
                    p[index] = y;
                    index++;
                }
                if(cell1.status[x][y-1] == CLOSE){
                    q[index] = x;
                    p[index] = y-1;
                    index++;
                }
            }
        }
    }
}

//==========================================================

void open_file(ifstream &file){
    stringstream ss;
    char c;
    while ( !file.eof() ){
        file.get(c);
        ss << c;
    }
    cout << ss.str() << endl;
}

//==========================================================

void show_result(int player){
    for(int i=0;i <= 5;i++)
                            for(int j=0;j <= 5;j++){
                                opencell(i,j,1);
                                opencell(i,j,2);
                            }
    playing_interface(player);
}
