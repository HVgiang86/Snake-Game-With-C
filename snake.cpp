/* ****Author: Hoang Giang CT5D****
    ***Yeu cau bo compiler 32-bit da cai library winbgim.h
    *** Vi du: TDM - GCC -32
*/
#include <stdio.h>
#include <winbgim.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <windows.h>
#include <math.h>
#include <string.h>
//
struct Point{
    int x;
    int y;
    int x0;
    int y0;
};
//Variables
int level = 1;
struct Point snake[100];
struct Point food;
struct Point direction;
const int step = 10;
int highscore[4];
int length;
bool gameStatus = true;
int bgcolor;
int Score;
int nowDirect;// 0: len tren; 1: xuong duoi; 2: sang trai; 3: sang phai
//
//Function
void drawGameBackGround();
void drawSnake(int color);
bool foodCheck();
bool snakeCheck();
void spawnFood();   //random new food location
void drawFood();
void drawMenu(int CurSelection); //draw menu's frames
void drawPause();
void drawGame(int func);
// func: 1: gameplaying, 2:pause, 3:ESC
void initGame(); // initialization the snake, food
void classic(); //game mode classic
void modern(); //game mode modern
void loop(void func()); //loop the game
void updateDirector(int key); //update direction
void updateSnake(); //update location
void runGame();
int gameMode;
void showHighScore();   //display highscore
void getHighScore();    //get highscore from file
void updateHighScore(); //arrange and update highscore array to file
bool checkFileSave(int x); //check file save game is empty?
void getFileSave(int x); //get saved info to game
void updateFileSave(int x); //update game to save file
char* inttotext(int x, char* str); // Convert int number to string
int stringtoint(char* str); //Convert string to int number
void ChoseLevel();

//
void initGame(){
    //Khoi tao con ran
    length = 3;
    snake[1].x = rand()%35 *10+80;
    snake[1].y = rand()%20 *10 +80;
    //
    nowDirect = rand() %3;
    switch(nowDirect){
    case 0:
        direction.x=0;
        direction.y=-step;
        //
        for(int i=2; i<=3; i++){
            snake[i].x=snake[i-1].x;
            snake[i].y=snake[i-1].y+step;
        }
        break;
    case 1:
        direction.x=0;
        direction.y=step;
        //
        for(int i=2; i<=3; i++){
            snake[i].x=snake[i-1].x;
            snake[i].y=snake[i-1].y-step;
        }
        break;
    case 2:
        direction.x=-step;
        direction.y=0;
        //
        for(int i=2; i<=3; i++){
            snake[i].x=snake[i-1].x-step;
            snake[i].y=snake[i-1].y;
        }
        break;
    case 3:
        direction.x=step;
        direction.y=0;
        //
        for(int i=2; i<=3; i++){
            snake[i].x=snake[i-1].x+step;
            snake[i].y=snake[i-1].y;
        }
        break;
    }
    spawnFood();
    Score =0;
}
//
void ChoseLevel(){
    cleardevice();
    setcolor(BLACK);
    settextstyle(TRIPLEX_FONT,HORIZ_DIR,7);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(324,100,"Choose Level");
    settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(324,150,"Level 1");
    outtextxy(324,220,"Level 2");
    outtextxy(324,290,"Level 3");
    outtextxy(324,360,"Level 4");
    delay(100);
    int choice =1;
    drawMenu(choice);
    while(1){
        if(kbhit()){
            int chr;
            chr = getch();
            if(chr == 72){
                if(choice > 1){
                    choice --;
                    drawMenu(choice);
                }
            }
            if(chr == 80){
                if(choice < 4){
                    choice ++;
                    drawMenu(choice);
                }
            }
            if(chr == 13){
                switch(choice){
                case 1: level = 1; break;
                case 2: level = 2; break;
                case 3: level = 3; break;
                case 4: level = 4; break;
                }
                break;
            }
        }
    }
}
//
void getHighScore(){
    FILE *score_file;
    char ss[10];
    score_file = fopen(".\\highscore\\score.txt","r");
    for(int i=1; i<4; i++){
        fgets(ss, 255, score_file);
        highscore[i] = stringtoint(ss);
    }
    fclose(score_file);
}
//
void updateHighScore(){
    FILE *score_file;
    score_file = fopen(".\\highscore\\score.txt","w");
    for(int i=1; i<4; i++){
        for(int j= i+1; j<4; j++){
            if(highscore[j] > highscore[j-1]){
                int temp;
                temp = highscore[j-1];
                highscore[j-1] = highscore[j];
                highscore[j] = temp;
            }
        }
    }
    for(int i=1; i<4; i++){
        fprintf(score_file,"%d\n",highscore[i]);
    }
    fclose(score_file);
}
//
void showHighScore(){
    cleardevice();
    setcolor(BLACK);
    settextstyle(TRIPLEX_FONT,HORIZ_DIR,6);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(324,100,"HIGH SCORE");
    settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    char s[20];
    char str[10];
    for(int i=1; i<4; i++){
        strcpy(s,inttotext(i,str));
        strcat(s,". ");
        strcat(s,inttotext(highscore[i],str));
        outtextxy(324,150+30*i,s);
    }

}
//
void getFileSave(int x){
    char path[] = ".\\save\\save";
    char str[5];
    long pointer = 0;
    int tmp[1000];
    //
    strcat(path,inttotext(x,str));
    strcat(path,".txt");
    FILE *save_file;
    save_file = fopen(path,"r");
    //
    int k = 0;
    fseek(save_file,0,SEEK_SET);
    while(!feof(save_file)){
        fscanf(save_file,"%d",&tmp[k]);
        k++;

    }
    //get gameData
    gameMode = tmp[0];
    level = tmp[1];
    length = tmp[2];
    Score = tmp[3];
    direction.x = tmp[4];
    direction.y = tmp[5];
    food.x = tmp[6];
    food.y = tmp[7];
    int j = 8;
    for(int i=1; i<= length; i++){
        snake[i].x = tmp[j];
        j++;
    }
    for(int i=1; i<= length; i++){
        snake[i].y = tmp[j];
        j++;
    }

    //
    fclose(save_file);
    fflush(save_file);
}
//
void updateFileSave(int x){
    char path[] = ".\\save\\save";
    char str[5];
    //
    strcat(path,inttotext(x,str));
    strcat(path,".txt");
    FILE *save_file;
    save_file = fopen(path,"r+");
    //
    //write gameMode
    fprintf(save_file,"%d\n",gameMode);
    //write level
    fprintf(save_file,"%d\n",level);
    //write length
    fprintf(save_file,"%d\n",length);
    //write score
    fprintf(save_file,"%d\n",Score);
    //write direction
    fprintf(save_file,"%d %d\n",direction.x,direction.y);
    //write now food
    fprintf(save_file,"%d %d\n",food.x,food.y);
    //write snake.x
    for(int i=1; i <length; i++){
        fprintf(save_file,"%d ",snake[i].x);
    }
    fprintf(save_file,"%d\n",snake[length].x);
    //write snake.y
    for(int i=1; i <length; i++){
        fprintf(save_file,"%d ",snake[i].y);
    }
    fprintf(save_file,"%d\n",snake[length].y);
    //write snake.x0
    for(int i=1; i <length; i++){
        fprintf(save_file,"%d ",snake[i].x0);
    }
    fprintf(save_file,"%d\n",snake[length].x0);
    //write snake.y0
    for(int i=1; i <length; i++){
        fprintf(save_file,"%d ",snake[i].y0);
    }
    fprintf(save_file,"%d\n",snake[length].y0);
    //
    fclose(save_file);
    fflush(save_file);
}
//
bool checkFileSave(int x){
    char path[] = ".\\save\\save";
    char str[5];
    //
    strcat(path,inttotext(x,str));
    strcat(path,".txt");
    FILE *save_file;
    save_file = fopen(path,"r");
    //
    long size=0;
    fseek(save_file,0,SEEK_END);
    size = ftell(save_file);
    return (size == 0);
}
//
void drawGameBackGround(){
    setcolor(BLACK);
    setlinestyle(SOLID_LINE,0,3);
    rectangle(0,0,648,432);
    rectangle(0,0,648,360);
    settextstyle(0,HORIZ_DIR,48);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(324,396,"Hoàng Giang");
    setcolor(CYAN);
    rectangle(69,69,435,291);
    setcolor(BLACK);
    settextstyle(TRIPLEX_FONT,HORIZ_DIR,2);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(540,100,"Game Mode:");
    settextstyle(TRIPLEX_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(540,210,"Score:");
    settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    if(gameMode==1){
        outtextxy(540,150,"Classic");
    }
    else{
        outtextxy(540,150,"Modern");
    }
    outtextxy(540,260,"");
    char score_str[]="** ";
    setcolor(BLACK);
    settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    outtextxy(540,260,inttotext(Score,score_str));
}
//
void drawSnake(int color){
    setcolor(color);
    setlinestyle(SOLID_LINE,0,1);
    setfillstyle(SOLID_FILL,color);
    for(int i=1; i<=length; i++){
        pieslice(snake[i].x, snake[i].y, 0, 360, 5);
    }
}
//
void drawFood(){
    int c = rand() % 14 + 1;
    setlinestyle(SOLID_LINE,0,1);
    setfillstyle(SOLID_FILL,c);
    setcolor(c);
    pieslice(food.x, food.y,0,360,5);
    setfillstyle(SOLID_FILL,bgcolor);
    setcolor(bgcolor);
    pieslice(food.x0, food.y0,0,360,5);
}
//
void drawGame(int func){
    switch(func){
    case 1:
        drawSnake(2);
        drawFood();
        drawGameBackGround();
        break;
    case 2:
        cleardevice();
        setcolor(0);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,4);
        outtextxy (324,200,"Game Pause");
        settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
        outtextxy(324,250,"Press Space to continue");
        break;
    case 3:
        cleardevice();
        setcolor(0);
        settextstyle(DEFAULT_FONT,HORIZ_DIR,5);
        outtextxy (324,100,"Paused!");
        settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
        outtextxy(324,170,"Save Game");
        outtextxy(324,225,"Quit Game");
    }
}
//
void drawMenu(int CurSelection){
    setcolor(WHITE);
    setlinestyle(SOLID_LINE,0,3);
    rectangle(180,120,470,165);
    rectangle(180,190,470,235);
    rectangle(200,260,450,305);
    rectangle(240,330,410,375);
    setcolor(RED);
    setlinestyle(SOLID_LINE,0,3);
    switch(CurSelection){
    case 1:
        rectangle(180,120,470,165);
        break;
    case 2:
        rectangle(180,190,470,235);
        break;
    case 3:
        rectangle(200,260,450,305);
        break;
    case 4:
        rectangle(240,330,410,375);
        break;
    }
}
//
bool foodCheck(){
    for(int i=1; i <= length; i++){
        if(food.x == snake[i].x && food.y == snake[i].y ){
            return false;
        }
        if(food.x == food.x0 && food.y == food.y0){
            return false;
        }
    }
    return true;
}
//
void spawnFood(){
    srand(time(NULL));
    do{
        food.x = rand()%35 *10+80;
        food.y = rand()%20 *10 +80;
    }while(foodCheck() == false);
    food.x0 = food.x;
    food.y0 = food.y0;
}
//
bool snakeCheck(){
    if(gameMode==2){
        if(snake[1].x < 72 || snake[1].x > 432){
            return false;
        }
        if(snake[1].y < 72 || snake[1].y > 288){
            return false;
        }
        for(int i=2; i <= length; i++){
            if(snake[1].x == snake[i].x && snake[1].y == snake[i].y){
                return false;
            }
        }
    }
    if(gameMode == 1){
        for(int i=2; i <= length; i++){
            if(snake[1].x == snake[i].x && snake[1].y == snake[i].y){
                return false;
            }
        }
    }
    return true;
}
//
void updateDirector(int ch){
    switch(ch){
    case 72:
        if(nowDirect == 1){
            break;
        }
        direction.x=0;
        direction.y=-step;
        nowDirect = 0;
        //
        break;
    case 80:
        if(nowDirect == 0){
            break;
        }
        direction.x=0;
        direction.y=step;
        nowDirect = 1;
        //
        break;
    case 75:
        if(nowDirect == 3){
            break;
        }
        direction.x=-step;
        direction.y=0;
        nowDirect = 2;
        //
        break;
    case 77:
        if(nowDirect == 2){
            break;
        }
        direction.x=step;
        direction.y=0;
        nowDirect = 3;
        //
        break;
    }
}
//
void updateSnake(){
    snake[1].x0 = snake[1].x;
    snake[1].y0 = snake[1].y;
    snake[1].x += direction.x;
    snake[1].y += direction.y;
    for(int i=2; i<=length; i++){
        snake[i].x0 = snake[i].x;
        snake[i].y0 = snake[i].y;
        snake[i].x = snake[i-1].x0;
        snake[i].y = snake[i-1].y0;
    }
    //
}
//
void modern(){
    drawSnake(bgcolor);
    updateSnake();
    if(snake[1].x == food.x && snake[1].y == food.y){
        snake[length].x = snake[length-1].x0;
        snake[length].y = snake[length-1].y0;
        length ++;
        Score +=10;
        spawnFood();
    }
}
//
void classic(){
    drawSnake(bgcolor);
    updateSnake();
    if(snake[1].x <80){
        snake[1].x=420;
    }
    if(snake[1].x > 420){
        snake[1].x = 80;
    }
    if(snake[1].y < 80){
        snake[1].y = 280;
    }
    if(snake[1].y > 280){
        snake[1].y = 80;
    }
    if(snake[1].x == food.x && snake[1].y == food.y){
        snake[length].x = snake[length-1].x0;
        snake[length].y = snake[length-1].y0;
        length ++;
        Score +=10;
        spawnFood();
    }
}
//
char* inttotext(int x, char* str){
    char text[6];
    int i=0;
    do{
        text[i] = x%10 + '0';
        i++;
        x/=10;
    }while(x != 0);
    text[i] = '\0';
    for(int j=0; j < strlen(text)/2; j++ ){
        char chr;
        chr = text[j];
        text[j] = text[strlen(text) -1 -j];
        text[strlen(text) -1 -j] = chr;
    }
    strcpy(str,text);
    return str;
}
//
int stringtoint(char* str){
    int inter=0;
    for(int i=0; i<strlen(str)-1; i++){
        inter = inter*10 + (str[i] - '0');
    }
    return inter;
}
//
void loop(void func()){
    if(!kbhit()){ // k tac dong ban phim
        func();
    }
    else{ //neu bam phim
        int key;
        key = getch();
        delay(0);
        if(key == 32){
            drawGame(2);
			key = getch();
			while (key != 32){
				key = getch();
				delay(0);
			}
			cleardevice();
			delay(500);
        }
        if(key == 27){
            drawGame(3);
            int selection =1;
            setcolor(RED);
            setlinestyle(SOLID_LINE,0,3);
            rectangle(235,145,410,180);
            while(1){
                if(kbhit()){
                    int ch;
                    ch = getch();
                    setcolor(bgcolor);
                    setlinestyle(SOLID_LINE,0,3);
                    rectangle(235,145,410,180);
                    rectangle(235,200,410,235);
                    if(ch == 72){
                        if(selection > 1){
                            selection --;
                        }
                    }
                    if(ch == 80){
                        if(selection < 2){
                            selection ++;
                        }
                    }
                    setcolor(RED);
                    if(selection == 1){
                        rectangle(235,145,410,180);
                    }
                    if(selection == 2){
                        rectangle(235,200,410,235);
                    }
                    if(ch == 27){
                        cleardevice();
                        delay(200);
                        break;
                    }
                    if(ch == 13){
                        if(selection == 2){
                            gameMode = 3;
                            break;
                        }
                        if(selection == 1){
                            updateFileSave(1);
                            cleardevice();
                            setcolor(BLACK);
                            settextstyle(DEFAULT_FONT,HORIZ_DIR,5);
                            outtextxy (324,100,"Saved!");
                            settextstyle(GOTHIC_FONT,HORIZ_DIR,2);
                            outtextxy(324,170,"Press any key to continue game!");
                            getch();
                            cleardevice();
                            delay(500);
                            break;
                        }
                    }

                }
            }
        }
        updateDirector(key);
        loop(func);

    }
}
void runGame(){
    //Khoi tao cua so do hoa
    initwindow(648,432,"Snake Game Playing");
    setbkcolor(WHITE);
    bgcolor = getbkcolor();
    cleardevice();

    //Loading screen
    setcolor(BLACK);
    SetConsoleTitle("Snake Game Console");
    settextstyle(DEFAULT_FONT,HORIZ_DIR,3);
    settextjustify(CENTER_TEXT,CENTER_TEXT);
    char s[20] = "LOADING......";
    for(int i=0 ; i<=100; i++){
        cleardevice();
        if(i<=9){
            s[13] = i+'0';
            s[14] = '%';
            s[15] = '\0';
        }
        else{
            s[13] = i/10 + '0';
            s[14] = i%10 + '0';
            s[15] = '%';
            s[16] = '\0';
        }

        outtextxy(324,216,s);
        i++;
        delay(30);
    }
    //Khoi tao High Score
    getHighScore();
    updateHighScore();
    //
    int selection=1;
    int selected =1;
    cleardevice();
    //
    while(1){
        Menu:
        setcolor(BLACK);
        settextstyle(TRIPLEX_FONT,HORIZ_DIR,7);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        outtextxy(324,100,"MENU");
        settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        outtextxy(324,150,"New Game");
        outtextxy(324,220,"Saved Game");
        outtextxy(324,290,"High Score");
        outtextxy(324,360,"Exit");
        delay(100);
        drawMenu(selection);
        if(kbhit()){
            int ch;
            ch = getch();
            if(ch == 72){
                if(selection > 1){
                    selection --;
                    drawMenu(selection);
                }
            }
            if(ch == 80){
                if(selection < 4){
                    selection ++;
                    drawMenu(selection);
                }
            }
            if(ch == 13){
                if(selection == 4){
                    exit(0);
                }
                if(selection == 3){
                    showHighScore();
                    getch();
                    cleardevice();
                    goto Menu;
                }
                if(selection == 2){
                    getFileSave(1);
                    drawGame(2);
                    int chr;
                    chr = getch();
                    while (chr != 32){
                        chr = getch();
                        delay(0);
                    }
                    cleardevice();
                    delay(500);
                    goto Playgame;
                }
                if(selection == 1){
                    //int selected = 1;
                    cleardevice();
                    setcolor(BLACK);
                    settextstyle(TRIPLEX_FONT,HORIZ_DIR,7);
                    settextjustify(CENTER_TEXT,CENTER_TEXT);
                    outtextxy(324,100,"MENU");
                    settextstyle(GOTHIC_FONT,HORIZ_DIR,4);
                    settextjustify(CENTER_TEXT,CENTER_TEXT);
                    outtextxy(324,150,"Classic");
                    outtextxy(324,220,"Modern");
                    setlinestyle(SOLID_LINE,0,3);
                    drawMenu(selected);
                    delay(100);
                    while(true){

                        if(!kbhit()){

                        }
                        if(kbhit()){
                            int khit;
                            khit = getch();
                            if(khit == 72){
                                if(selected > 1){
                                    selected --;
                                    drawMenu(selected);
                                }
                            }
                            if(khit == 80){
                                if(selected < 2){
                                    selected ++;
                                    drawMenu(selected);
                                }
                            }
                            if(khit == 13){
                                if(selected == 1){
                                    gameMode = 1;
                                }
                                if(selected == 2){
                                    gameMode = 2;
                                }
                                ChoseLevel();
                                delay(50);
                                initGame();
                                cleardevice();
                                drawGameBackGround();
                                Playgame:
                                while(snakeCheck()){
                                    delay(250 - (level-1)*50);
                                    if(gameMode == 1){
                                        loop(classic);
                                    }
                                    if(gameMode == 2){
                                        loop(modern);
                                    }
                                    if(gameMode == 3){
                                        cleardevice();
                                        goto Menu;
                                    }
                                    drawGame(1);
                                }
                                cleardevice();
                                setcolor(BLACK);
                                settextstyle(DEFAULT_FONT,HORIZ_DIR,5);
                                settextjustify(CENTER_TEXT,CENTER_TEXT);
                                outtextxy(324,100,"Game Over");
                                settextstyle(GOTHIC_FONT,HORIZ_DIR,3);
                                settextjustify(CENTER_TEXT,CENTER_TEXT);
                                char score_str[]="Your Score: ";
                                char str[5];
                                strcat(score_str,inttotext(Score,str));
                                outtextxy(324,200,score_str);
                                if(Score > highscore[3] && Score < highscore[1]){
                                    highscore[3] = Score;
                                    updateHighScore();
                                    outtextxy(324,250,"Your score is high score");
                                    char ss[15];
                                    strcpy(ss,"Best: ");
                                    char str[5];
                                    strcat(ss,inttotext(highscore[1],str));
                                    outtextxy(324,300,ss);
                                    outtextxy(324,350,"Press any key to continue");
                                }
                                else if(Score >= highscore[1]){
                                    highscore[3] = Score;
                                    updateHighScore();
                                    outtextxy(324,250,"Your score is the best score");
                                    outtextxy(324,340,"Press any key to continue");
                                }
                                else{
                                    outtextxy(324,250,"Press any key to continue");
                                }
                                getch();
                                cleardevice();
                                goto Menu;
                            }
                        }
                    }
                }
            }

        }

    }
}
int main(){
    runGame();
    getch();
    closegraph();
    return 0;
}
