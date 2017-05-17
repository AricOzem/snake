#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<ctime>
#include<Windows.h>
#include "conio.h"

#define YELLOW 14
#define GREEN 10
#define RED 12
#define BULE 11
#define WHITE 15
#define BLACK 0
#define WRED 252

#define UP 72
#define DOWN 80
#define ESC  27
#define SPACE 32
#define LEFT 75
#define RIGHT 77
#define ENTER 13

#define U 0
#define D 23
#define L 0
#define R 54

#define head p[0]
#define tail p[len-2]

#define HOUT GetStdHandle(STD_OUTPUT_HANDLE)
#define color(x) SetConsoleTextAttribute(HOUT,x);
#define time() clock()/CLOCKS_PER_SEC

inline int random();
inline void winSound();
inline void makecur(int );
inline void gotoxy(int ,int );
inline int read();
inline void key();
inline void init();//初始化界面
inline void getchapter();//选择关卡
inline void table();//信息显示面板
inline bool play();
inline void gotoAndDraw(int _x,int _y,int _color,char _s[]){
	gotoxy(_x,_y);color(_color);printf("%s",_s);
}
int score,chapter,mod;
struct Things {
	int x;
	int y;
} food,heart,barrier;
struct MapPart {
	bool food,heart,barrier;
};
struct Map{
	MapPart g[30][30];
	void clear() {
		for(int i=1; i<D; i++) {
			gotoxy(2,i);
			printf("                                                    ");
		}
	};
	void init(){
		clear();
		for(int i=1;i<D;i++)
			for(int j=1;j<(R>>1);j++)
					g[j][i].food=false,g[j][i].heart=false,g[j][i].barrier=false;
	};
	void addFood(){
		while(1) {
			food.x=random()%26;
			food.y=random()%22;
			if(!g[food.x][food.y].barrier&&!g[food.x][food.y].heart) break;
		}
		g[food.x][food.y].food=1;
		gotoAndDraw(2*(food.x+1),food.y+1,YELLOW,"¤");
	};
	void addHeart(){
		while(1) {
			heart.x=random()%26;
			heart.y=random()%22;
			if(!g[heart.x][heart.y].barrier&&!g[heart.x][heart.y].food) break;
		}
		g[heart.x][heart.y].heart=true;
		gotoxy(2*(heart.x+1),heart.y+1);//占两格字符故乘2
		color(YELLOW);
		printf("★");
	};
	void build(){
		for(int i=1; i<=30; i++) {
			while(1) {
				barrier.x=random()%26;//产生随机横坐标
				barrier.y=random()%22;//产生随机纵坐标
				break;//当此处无其他元素是才生效
			}
			if(!g[heart.x][heart.y].heart&&!g[heart.x][heart.y].food) g[barrier.x][barrier.y].barrier=true;//随机出现障碍物
			gotoAndDraw(2*(barrier.x+1),barrier.y+1,WRED,"▇");
		}
	};
	void recover(){
		for(int i=1;i<D;i++)
			for(int j=1;j<(R>>1);j++)
					if(g[j][i].barrier) gotoAndDraw(2*(j+1),i+1,WRED,"▇");
		if(g[heart.x][heart.y].heart) gotoAndDraw(2*(heart.x+1),heart.y+1,YELLOW,"★");
		if(g[food.x][food.y].food) gotoAndDraw(2*(food.x+1),food.y+1,YELLOW,"¤");
	};
}map;
struct SnakePart {
	int x,y,dir;
};
struct Snake {
	int len;
	int HP;
	int sped;
	SnakePart p[10];
	void init() {
		len=4;//len用来记录蛇身长度,初始为3节
		HP=6;//记录蛇的生命值,初始化为6
		p[0].x=6;
		p[0].y=10;
		p[0].dir=RIGHT;
		p[1].x=4;
		p[1].y=10;
		p[1].dir=RIGHT;
		p[2].x=2;
		p[2].y=10;
		p[2].dir=RIGHT;
		p[3].x=4;
		p[3].y=4;
		p[3].dir=RIGHT;
	}
	void draw() {
		color(RED);
		printf("◆");//打印蛇头
		for(int i=1; i<len-1; i++) {
			gotoxy(p[i].x,p[i].y);//转到当前蛇身位置
			color(YELLOW);//改成黄色
			printf("●");//打印蛇身
		}
		gotoxy(tail.x,tail.y);//转到当前蛇尾位置
		color(RED);//改成红色
		printf("●");//打印蛇尾
		Sleep(sped);//控制蛇的移动速度
		gotoxy(tail.x,tail.y);//移到蛇尾所在地
		printf(" ");//清除上个循环的蛇尾
	}
	void move() {
		for(int i=len-1; i>0; i--) p[i]=p[i-1];
		switch (head.dir) {
			case UP: {
				head.y-=1;    //改变蛇头坐标，移动蛇头
				break;
			}
			case DOWN: {
				head.y+=1;    //改变蛇头坐标，移动蛇头
				break;
			}
			case LEFT: {
				head.x-=2;    //改变蛇头坐标，移动蛇头
				break;
			}
			case RIGHT: {
				head.x+=2;    //改变蛇头坐标，移动蛇头
				break;
			}
		}
	}
	void judge() {
		if(head.x==L) {
			printf("\a");
			--HP;//将生命值减一
			head.x=52;//将其穿墙
		}
		if(head.x==R) {
			printf("\a");
			--HP;//将生命值减一
			head.x=2;//将其穿墙
		}
		if(head.y==U) {
			printf("\a");
			--HP;//将生命值减一
			head.y=22;//将其穿墙
		}
		if(head.y==D) {
			printf("\a");
			--HP;//将生命值减一
			head.y=1;//将其穿墙
		}
		for(int i=1; i<len-1; i++)
			if(head.x==p[i].x&&head.y==p[i].y) { //当蛇撞到自己
				printf("\a");
				HP=0;
				break;
			}
		if(map.g[(head.x-1)/2][head.y-1].barrier==1) { //当蛇障碍物时
			printf("\a");
			--HP;
			map.g[(head.x-1)/2][head.y-1].barrier=0;
		}
		for(int i=0; i<len; i++) {
			if(map.g[(p[i].x-1)/2][p[i].y-1].food==1) { //判断蛇是否吃到食物
				++len;//让蛇长度加1
				score++;//将食物数加1
				map.g[(p[i].x-1)/2][p[i].y-1].food=0;//让食物标示归零
				break;
			}
		}
		if(map.g[(head.x-1)/2][head.y-1].heart==1) { //判断蛇是否吃到星星
			map.g[(head.x-1)/2][head.y-1].heart=0;//让星星标示归零
			if(HP<6) ++HP;//将生命值加1
		}
	}
} s;

int main() {
	init();
	while(1) {
		getchapter();
		while(play()) break;
		map.clear();
	}
	return 0;
}
inline int random() {
	srand(time()+rand());
	return rand();
}
inline void gotoxy(int x,int y) {
	COORD coord;
	coord.X=x;
	coord.Y=y;
	SetConsoleCursorPosition(HOUT,coord);
}
void makecur(int k) {
	HANDLE hOut =HOUT;
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(hOut,&cci);
	cci.bVisible=k;//赋1为显示，赋0为隐藏
	if(k)  cci.dwSize = 1;else cci.dwSize = 20;
	SetConsoleCursorInfo(hOut,&cci);
}
void winSound() {
	Beep(440,150);
	Sleep(50);
	Beep(440,150);
	Beep(554,150);
	Beep(659,350);
	Beep(554,150);
	Beep(659,450);
	Sleep(500);
	Beep(880,800);
}
inline void getkey() {
	if(kbhit()) {
		int ch=getch();
		if(ch==224) {
			ch=getch();
			if((ch==UP||ch==DOWN||ch==LEFT||ch==RIGHT)&&(ch+s.p[0].dir!=152)&&ch!=s.p[0].dir) s.p[0].dir=ch;
			else rewind(stdin);
		} else if(ch==SPACE) { //暂停
			gotoxy(20,1);
			color(YELLOW);
			printf("暂停中，请按空格键开始");
			makecur(0);
			while(1) if(kbhit()&&getch()==SPACE) {
					gotoxy(20,1);
					printf("                       ");
					map.recover();
					break;
				}
		} else if(ch==ESC) {
			getchapter();
			play();
		} else rewind(stdin); //将输入流重新指向开头
	}
}
inline int read() {
	char tmp[51];
	fgets(tmp,50,stdin);
	if(tmp[0]=='\n') return -1;
	if(tmp[1]!='\n') return -1;
	return tmp[0]-'0';
}
inline void init() {
	system("mode con cols=82 lines=25");//设置窗口大小
	system("chcp 936");//切换代码页
	gotoAndDraw(0,0,BLACK,"                 ");
	system("title 贪吃蛇"); //设置标题
	makecur(0);
	register int i,j,z;
	color(YELLOW);
	gotoxy(8,6);puts("╭──╯─╮┌┐┌─┐　┌╮╭─┴┐");
	gotoxy(8,7);puts("│┌─┴┐│││├──┐┌┼╮┌　│");
	gotoxy(8,8);puts("│　┌─╯│││╯──┐││││　╯");
	gotoxy(8,9);puts("┌────┐││┌──╯└┼┘│╭┘");
	gotoxy(8,10);puts("└　　│　╯│││　　┐　│┐│　　");
	gotoxy(8,11);puts("└──╯─┘└╯└──╯╰┴┘╰─┘");
	gotoxy(18,13);puts("╭──╮╭╮╭╮　╭──╮╭──╮╭──╮╭╭╮╮");
	gotoxy(18,14);puts("│╭╮││╰╯│　│╭╮│╰─╮││╭─╯│　　│");
	gotoxy(18,15);puts("│╰╯╯╰╮╭╯　││││　╭╯╯│╰─╮││││");
	gotoxy(18,16);puts("│╭╮╮　││　　││││╭╯╯　│╭─╯│╭╮│");
	gotoxy(18,17);puts("│╰╯│　││　　│╰╯││╰─╮│╰─╮││││");
	gotoxy(18,18);puts("╰──╯　╰╯　  ╰──╯╰──╯╰──╯╰╯╰╯");
	Sleep(3000);
	gotoxy(0,0);
	color(BLACK);//调成黑底色
	for(i=0; i<25; i++) {
		printf("                                                                                ");
	}
	color(WRED);//调整输出颜色
	for(i=0,j=60; i<60; i++,j--) { //if是为了异步输出
		if(j>20) {
			gotoxy(2*(j-21),0);
			printf("▉");//输出第一行
		}
		if(i<40) {
			gotoxy(2*i,23);
			printf("▉");// 输出最下面一行
		}
		if(j>22&&j<45) {
			gotoxy(78,j-22);
			printf("▉");//输出最右边列
		}
		if(j>22&&j<45) {
			gotoxy(0,i-15);
			printf("▉");//输出第一列
		}
		if(i>37&&i<60) {
			gotoxy(54,i-37);
			Sleep(10);
			printf("▉");//输出中间那列
		}
		Sleep(30);
	}
	gotoxy(56,9);
	printf("▉▉▉▉▉▉▉▉▉▉▉");                                            //56
	gotoxy(19,0);
	color(14);//调整输出颜色
	printf("| | |贪吃蛇| | |");//输出标题

	gotoxy(64,1);
	printf("第  关");
	gotoxy(57,3);
	printf("生命值：");
	gotoxy(57,5);
	printf("当前长度：");
	gotoxy(57,7);
	printf("已吃食物¤：");
	gotoxy(64,10);
	printf("提示：");
	gotoxy(56,12);
	printf("向上：↑   向上：←");
	gotoxy(56,13);
	printf("向下：↓   向右：→");
	gotoxy(56,14);
	printf("暂停/开始：空格键 ");
	gotoxy(56,15);
	printf("重新选关 ：Esc键");
	gotoxy(64,16);
	printf("注意！");
	gotoxy(56,17);
	printf("1:撞到障碍物或者墙生命");
	gotoxy(56,18);
	printf("  值减一 ");
	gotoxy(56,19);
	printf("2:吃到小星星生命值加一");
	gotoxy(56,20);
	printf("3.通关需吃到50份食物");
}
inline void getchapter() {
	makecur(1);
	score=0;
	s.init();
	gotoAndDraw(66,5,BLACK,"      ");//解决分数覆盖问题
	gotoAndDraw(69,7,BLACK,"        ");
	printf("%d",score);
	map.init();
	color(YELLOW);
	while(1) {
		gotoxy(15,9);
		printf("欢迎来到贪吃蛇世界！");
		gotoxy(15,11);
		printf("请输入关卡(1-7):");
		chapter=read();
		if(chapter>0&&chapter<8) break;
		gotoxy(30,11);
		printf("                       ");
		gotoxy(15,13);
		printf("好像并不存在这个关卡！请重新输入");
	}
	switch(chapter) {
		case 1:
			s.sped=350;
			break;
		case 2:
			s.sped=300;
			break;
		case 3:
			s.sped=250;
			break;
		case 4:
			s.sped=200;
			break;
		case 5:
			s.sped=100;
			break;
		case 6:
			s.sped=70;
			break;
		case 7:
			s.sped=40;
			break;
	}
	while(1) {
		gotoxy(15,9);
		printf("请选择挑战模式（见下）:");
		gotoxy(13,11);
		printf("1.地图模式(带障碍物)2.普通模式(无障碍物)");
		gotoxy(15,13);
		printf("                                   ");
		gotoxy(38,9);
		mod=read();
		if(mod>0&&mod<3) break;
		gotoxy(30,9);
		printf("                       ");
		gotoxy(15,13);
		printf("好像并不存在这种模式！请重新输入");
	}
	map.clear();
	if(mod==1) map.build();
}
inline void table() {
	gotoxy(67,1);
	printf("%d",chapter);
	//绘制生命条
	gotoxy(65,3);
	color(GREEN);
	switch (s.HP) {
		case 0:
			color(RED);
			printf("▁▂▃▅▆▇");
			break;
		case 1:
			printf("▁");
			color(RED);
			printf("▂▃▅▆▇");
			break;
		case 2:
			printf("▁▂");
			color(RED);
			printf("▃▅▆▇");
			break;
		case 3:
			printf("▁▂▃");
			color(RED);
			printf("▅▆▇");
			break;
		case 4:
			printf("▁▂▃▅");
			color(RED);
			printf("▆▇");
			break;
		case 5:
			printf("▁▂▃▅▆");
			color(RED);
			printf("▇");
			break;
		case 6:
			printf("▁▂▃▅▆▇");
			break;
		default:
			color(RED);
			printf("！超级模式 ！");
			break;
	}
	gotoxy(66,5);
	color(RED);
	printf("%d",s.len-1);
	gotoxy(69,7);
	printf("%d",score);
}
inline bool play() {
	while(1) {
		table();
		gotoxy(s.head.x,s.head.y);
		s.draw();
		getkey();
		s.move();
		s.judge();
		if(s.HP<=0) {
			table();
			gotoxy(25,10);
			color(WHITE);
			printf("\a\a\a游戏结束！！！");
			Sleep(2500);
			return false;
		}
		if(s.len==52) {
			gotoxy(25,10);
			color(WHITE);
			winSound();
			printf("恭喜你过关！！！");
			Sleep(3000);
			return false;
		}
		if(!map.g[food.x][food.y].food) map.addFood();
		if(time()%25==0&&!map.g[heart.x][heart.y].heart) map.addHeart();
	}
}
