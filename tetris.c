#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

enum { x_size = 20, y_size = 34 };

static int field[y_size][x_size];


int Score = 0;

struct Pos{
	int x;
	int y;
};

struct Pos sum(struct Pos A, struct Pos B){
	struct Pos ans;
	ans.x = A.x+B.x;
	ans.y = A.y+B.y;
	return ans;
}

bool comp(struct Pos A,struct Pos B){
	return A.x == B.x && A.y == B.y;
}

void Draw(struct Pos a, const char c){
	move(a.y,2*a.x);
	addch(c);
	addch(c);
	refresh();
}

struct Pos makePos(int x, int y){
	struct Pos ans;
	ans.x = x;
	ans.y = y;
	return ans;
}

void GenerateFig(const int tf, struct Pos *coords){
	if (tf == 0){
		coords[0] = makePos(0, 0);
		coords[1] = makePos(0, 1);
		coords[2] = makePos(1, 0);
		coords[3] = makePos(1, 1);			
	}	
	if (tf == 1){
		coords[0] = makePos(-1, 0);
		coords[1] = makePos(0, 0);
		coords[2] = makePos(1, 0);
		coords[3] =makePos(2, 0);			
	}
	if (tf == 2){
		coords[0] = makePos(-1, 0);
		coords[1] = makePos(-1, 1);
		coords[2] = makePos(0, 0);
		coords[3] = makePos(1, 0);		
	}
	if (tf == 3){
		coords[0] = makePos(-1, 0);
		coords[1] = makePos(0, 0);
		coords[2] = makePos(0, 1);
		coords[3] = makePos(-1, 1);			
	}
	if (tf == 4){
		coords[0] = makePos(-1, 0);
		coords[1] = makePos(0, 1);
		coords[2] = makePos(0, 0);
		coords[3] = makePos(1, 1);			
	}
	if (tf == 5){
		coords[0] = makePos(-1, -1);
		coords[1] = makePos(-1, 0);
		coords[2] = makePos(0, 0);
		coords[3] = makePos(0, 1);			
	}
	if (tf == 6){
		coords[0] = makePos(0, -1);
		coords[1] = makePos(1, 0);
		coords[2] = makePos(0, 0);
		coords[3] = makePos(-1, 0);					
	}
	
}

bool mycoord(struct Pos p,struct Pos *c){
	for (int i=0;i<4;i++){
		if (comp(p, c[i])){
			return true;
		}	
	}
	return false;
}

bool collision(struct Pos *c,struct Pos *k){
	for (int i=0;i<4;i++){
		if ((c[i].x >= x_size-1 || c[i].y >= y_size-1 || c[i].x < 1 || c[i].y < 0 || field[c[i].y][c[i].x] !=0)&&!mycoord(c[i],k)){
			return true;
		}
	}
	return false;
}


struct Pos coords[4];
struct Pos pos;
int color;
int tf;

		
		void clearF(){
			attron(COLOR_PAIR(10));
			for (int i=0;i<4;i++){
				Draw(sum(coords[i],pos),' ');
				struct Pos c = sum(coords[i],pos);
				if (c.y >=0 && c.y < y_size && c.x>=0 && c.x<x_size){
					field[c.y][c.x] = 0;
				}				
			}		
		}

		void draw(){
			attron(COLOR_PAIR(color));
			for (int i=0;i<4;i++){
				Draw(sum(coords[i],pos),'#');
				struct Pos c = sum(coords[i],pos);
				if (c.y >= 0 && c.y < y_size && c.x>=0 && c.x<x_size){
					field[c.y][c.x] = color;
				}			
			}		
		}


		bool Figure(struct Pos p, int tpf, int clr){
			tf = tpf;
			bool gmovr = false;
			GenerateFig(tf,coords);
			color = clr;
			pos = p;
			if (p.x >=0 && p.x < x_size && p.y >=0 && p.y < y_size){
				for (int i=0;i<4;i++){
					if (field[pos.y+coords[i].y][pos.x+coords[i].x]!=0){
						gmovr = true;
						break;			
					}
				}
			}
			draw();
			return gmovr;
		}
		
		
		
		
		bool down(){
			struct Pos newc[4];
			struct Pos c[4];
			for (int i=0;i<4;i++){
				newc[i] = sum(pos, coords[i]);
				newc[i].y+=1;
				c[i] = sum(pos, coords[i]);
			}
			if (!collision(newc, c)){
				clearF();
				pos.y = pos.y + 1;
				draw();
				return true;
			}
			return false;
		}
		bool right(){
			struct Pos newc[4];
			struct Pos c[4];
			for (int i=0;i<4;i++){
				newc[i] = sum(pos, coords[i]);
				newc[i].x+=1;
				c[i] = sum(pos, coords[i]);
			}
			if (!collision(newc, c)){
				clearF();
				pos.x = pos.x+1;
				draw();
				return true;
			}
			return false;
		}
		bool left(){
			struct Pos newc[4];
			struct Pos c[4];
			for (int i=0;i<4;i++){
				newc[i] = sum(pos, coords[i]);
				newc[i].x-=1;
				c[i] = sum(pos, coords[i]);
			}
			if (!collision(newc, c)){
				clearF();
				pos.x = pos.x-1;
				draw();
				return true;
			}
			return false;
		}
		bool rotate(){
			if (tf != 0){
				struct Pos newc[4];
				struct Pos np[4];
				struct Pos mnp[4];
				for (int i=0;i<4;i++){
					newc[i].x = coords[i].y;
					newc[i].y = -coords[i].x;
					np[i]=sum(pos,newc[i]);
					mnp[i]=sum(pos,coords[i]);			
				}
				if (!collision(np,mnp)){
					clearF();
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i].x=np[i].x + 1;			
				}
				if (!collision(np,mnp)){
					clearF();
					pos.x=pos.x+1;
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i].x=np[i].x + 1;			
				}
				if (!collision(np,mnp)){
					clearF();
					pos.x=pos.x+2;
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i].x=np[i].x - 3;			
				}
				if (!collision(np,mnp)){
					clearF();
					pos.x=pos.x-1;
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i].x=np[i].x - 1;			
				}
				if (!collision(np,mnp)){
					clearF();
					pos.x=pos.x-2;
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
					
			}
			return false;		
		}
		void force_rotate(){
			if (tf != 0){
				struct Pos newc[4];
				
				for (int i=0;i<4;i++){
					newc[i].x = coords[i].y;
					newc[i].y = -coords[i].x;
							
				}
				
				clearF();
				for(int i=0;i<4;i++){
					coords[i] = newc[i];
				}
				draw();
								
				
			}		
		}


void offset(int n){
	
		for (int i = n;i>=0;i--){
			for (int j=1;j<x_size-1;j++){
				if (field[i][j] !=0){
					attron(COLOR_PAIR(10));
					struct Pos dr = {j, i};
					Draw(dr,' ');			
				}			
			}
		}

		for (int i = n;i>=1;i--){
			for (int j=1;j<x_size-1;j++){
				field[i][j] = field[i-1][j];
				if (field[i][j] != 0){
					attron(COLOR_PAIR(field[i][j]));
					struct Pos dr = {j, i};
					Draw(dr,' ');			
				}			
			}
		}

		for (int i = 0;i>=0;i--){
			for (int j=1;j<x_size-1;j++){
				field[i][j] = 0;		
			}
		}
		
}

int proov(){
	for (int i=y_size-2;i>0;i--){
		bool fool = true;
		for  (int j=1;j<x_size-1;j++){
			if (field[i][j] == 0){
				fool = false;			
			}	
		}
		if (fool){
			offset(i);
			Score+=x_size-2;
			i++;
		}
	}
}

int main(){

	initscr();
	start_color();
	noecho();
	curs_set(0);
	nodelay(stdscr, TRUE);
	init_pair(4,  COLOR_YELLOW,  COLOR_YELLOW);
        init_pair(5,  COLOR_BLUE,    COLOR_BLUE);
        init_pair(6,  COLOR_MAGENTA, COLOR_MAGENTA);
        init_pair(7,  COLOR_CYAN,    COLOR_CYAN);
        init_pair(8,  COLOR_BLUE,    COLOR_BLUE);
        init_pair(9,  COLOR_WHITE,   COLOR_WHITE);
	init_pair(3, COLOR_GREEN, COLOR_GREEN );
	init_pair(2, COLOR_BLUE, COLOR_BLACK );
	init_pair(1, COLOR_BLUE, COLOR_BLUE );
	init_pair(10, COLOR_BLACK, COLOR_BLACK );
	init_pair(11, COLOR_WHITE, COLOR_BLACK );
	attron(COLOR_PAIR(1));
	for (int i=0;i<y_size;i++){
		if (i>3){		
			field[i][0] = 1;
			field[i][x_size-1] = 1;
			Draw(makePos(0,i),'#');
			Draw(makePos(x_size-1,i),'#');	
		}	
		if (i < x_size){		
			field[y_size-1][i] = 1;
			Draw(makePos(i,y_size-1),'#');
		}
	}
	srand(time(NULL));
	int tf = rand()%7;
	bool GameOver = false;
	int clr = 3+rand()%6;
	GameOver = Figure(makePos(x_size/2, 2),tf,clr);
	tf = rand()%7;
	clr = 3+rand()%6;
	int rot = rand()%100;
	unsigned int wait = 300000;
	while(!GameOver){

		int ch = getch();
		wait = 300000;
		switch(ch){
			case 'd': right(); break;
			case 'a': left();  break;
			case 'w': rotate();break;
			case 's': wait = 70000;break;		
		}
		char s[100];
		scanw("%s",&s);
		if (!down()){
			
			proov();
			GameOver = Figure(makePos(x_size/2, 2), tf,clr);
			tf = rand()%7;	
			clr = 3+rand()%6;
			if (rot<50) rotate();
			rot = rand()%100;
			
		}
		attron(COLOR_PAIR(11));
		move(y_size/6-1,2*x_size+4);
		printw("%s%i","Score: ",Score);
		refresh();
		usleep(wait);
	}
	
	endwin();
	return 0;
}
