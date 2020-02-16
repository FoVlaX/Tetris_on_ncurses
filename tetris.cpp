#include <ncurses.h>
#include <unistd.h>
#include <algorithm>
#include <stdio.h>
#include <locale.h>
#include <time.h>

const int x_size = 20;
const int y_size = 34;

int field[y_size][x_size];


int Score = 0;

struct Pos{
	int x;
	int y;
	friend bool operator==(const Pos &a, const Pos &b){
		return a.x==b.x && a.y==b.y;
	}
	Pos operator+(Pos a){
		Pos ans;
		ans.x = (*this).x+a.x;
		ans.y = (*this).y+a.y;
		return ans;
	}
};


void Draw(Pos a, const char &c){
	move(a.y,2*a.x);
	addch(c);
	addch(c);
	refresh();
}

void GenerateFig(const int &tf, Pos *coords){
	if (tf == 0){
		coords[0] = {0, 0};
		coords[1] = {0, 1};
		coords[2] = {1, 0};
		coords[3] = {1, 1};			
	}	
	if (tf == 1){
		coords[0] = {-1, 0};
		coords[1] = {0, 0};
		coords[2] = {1, 0};
		coords[3] = {2, 0};			
	}
	if (tf == 2){
		coords[0] = {-1, 0};
		coords[1] = {-1, 1};
		coords[2] = {0, 0};
		coords[3] = {1, 0};		
	}
	if (tf == 3){
		coords[0] = {-1, 0};
		coords[1] = {0, 0};
		coords[2] = {0, 1};
		coords[3] = {-1, 1};			
	}
	if (tf == 4){
		coords[0] = {-1, 0};
		coords[1] = {0, 1};
		coords[2] = {0, 0};
		coords[3] = {1, 1};			
	}
	if (tf == 5){
		coords[0] = {-1, -1};
		coords[1] = {-1, 0};
		coords[2] = {0, 0};
		coords[3] = {0, 1};			
	}
	if (tf == 6){
		coords[0] = {0, -1};
		coords[1] = {1, 0};
		coords[2] = {0, 0};
		coords[3] = {-1, 0};					
	}
}

bool mycoord(Pos p, Pos *c){
	for (int i=0;i<4;i++){
		if (p == c[i]){
			return true;
		}	
	}
	return false;
}

bool collision(Pos *c,Pos *k){
	for (int i=0;i<4;i++){
		if ((c[i].x >= x_size-1 || c[i].y >= y_size-1 || c[i].x < 1 || c[i].y < 0 || field[c[i].y][c[i].x] !=0)&&!mycoord(c[i],k)){
			return true;
		}
	}
	return false;
}

class Figure{
	public:
		Figure(Pos p, int tpf,bool &gmovr, int clr){
			tf = tpf;
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
		}
		~Figure(){}
		Pos coords[4];
		Pos pos;
		int color;
		int tf;
		void draw(){
			attron(COLOR_PAIR(color));
			for (int i=0;i<4;i++){
				Draw(coords[i]+pos,'#');
				Pos c = coords[i]+pos;
				if (c.y >= 0 && c.y < y_size && c.x>=0 && c.x<x_size){
					field[c.y][c.x] = color;
				}			
			}		
		}
		void clear(){
			attron(COLOR_PAIR(10));
			for (int i=0;i<4;i++){
				Draw(coords[i]+pos,' ');
				Pos c = coords[i]+pos;
				if (c.y >=0 && c.y < y_size && c.x>=0 && c.x<x_size){
					field[c.y][c.x] = 0;
				}				
			}		
		}
		bool down(){
			Pos newc[4];
			Pos c[4];
			for (int i=0;i<4;i++){
				newc[i] = pos + coords[i] + (Pos){0, 1};
				c[i] = pos + coords[i];
			}
			if (!collision(newc, c)){
				clear();
				pos = pos + (Pos){0,1};
				draw();
				return true;
			}
			return false;
		}
		bool right(){
			Pos newc[4];
			Pos c[4];
			for (int i=0;i<4;i++){
				newc[i] = pos + coords[i] + (Pos){1, 0};
				c[i] = pos + coords[i];
			}
			if (!collision(newc, c)){
				clear();
				pos = pos + (Pos){1,0};
				draw();
				return true;
			}
			return false;
		}
		bool left(){
			Pos newc[4];
			Pos c[4];
			for (int i=0;i<4;i++){
				newc[i] = pos + coords[i] + (Pos){-1, 0};
				c[i] = pos + coords[i];
			}
			if (!collision(newc, c)){
				clear();
				pos = pos + (Pos){-1,0};
				draw();
				return true;
			}
			return false;
		}
		bool rotate(){
			if (tf != 0){
				Pos newc[4];
				Pos np[4];
				Pos mnp[4];
				for (int i=0;i<4;i++){
					newc[i].x = coords[i].y;
					newc[i].y = -coords[i].x;
					np[i]=pos+newc[i];
					mnp[i]=pos+coords[i];			
				}
				if (!collision(np,mnp)){
					clear();
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i]=np[i] + (Pos){1,0};			
				}
				if (!collision(np,mnp)){
					clear();
					pos=pos+(Pos){1,0};
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i]=np[i] + (Pos){1,0};			
				}
				if (!collision(np,mnp)){
					clear();
					pos=pos+(Pos){2,0};
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i]=np[i] + (Pos){-3,0};			
				}
				if (!collision(np,mnp)){
					clear();
					pos=pos+(Pos){-1,0};
					for(int i=0;i<4;i++){
						coords[i] = newc[i];
					}
					draw();
					return true;			
				}
				for (int i=0;i<4;i++){
					np[i]=np[i] + (Pos){-1,0};			
				}
				if (!collision(np,mnp)){
					clear();
					pos=pos+(Pos){-2,0};
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
				Pos newc[4];
				
				for (int i=0;i<4;i++){
					newc[i].x = coords[i].y;
					newc[i].y = -coords[i].x;
							
				}
				
				clear();
				for(int i=0;i<4;i++){
					coords[i] = newc[i];
				}
				draw();
								
				
			}		
		}
};

void offset(int n){
	
		for (int i = n;i>=0;i--){
			for (int j=1;j<x_size-1;j++){
				if (field[i][j] !=0){
					attron(COLOR_PAIR(10));
					Draw({j,i},' ');			
				}			
			}
		}

		for (int i = n;i>=1;i--){
			for (int j=1;j<x_size-1;j++){
				field[i][j] = field[i-1][j];
				if (field[i][j] != 0){
					attron(COLOR_PAIR(field[i][j]));
					Draw({j,i},' ');			
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
			Draw({0,i},'#');
			Draw({x_size-1,i},'#');	
		}	
		if (i < x_size){		
			field[y_size-1][i] = 1;
			Draw({i,y_size-1},'#');
		}
	}
	srand(time(NULL));
	int tf = rand()%7;
	bool GameOver = false;
	int clr = 3+rand()%6;
	Figure *fg = new Figure({x_size/2, 2},tf,GameOver,clr);
	tf = rand()%7;
	clr = 3+rand()%6;
	int rot = rand()%100;
	Figure *Next_fg = new Figure({x_size+3,y_size/4},tf,GameOver,clr);
	if (rot < 50) Next_fg->force_rotate();
	unsigned int wait = 300000;
	while(!GameOver){

		int ch = getch();
		wait = 300000;
		switch(ch){
			case 'd': fg->right(); break;
			case 'a': fg->left();  break;
			case 'w': fg->rotate();break;
			case 's': wait = 70000;break;		
		}
		char s[100];
		scanw("%s",&s);
		if (!fg->down()){
			delete fg;
			proov();
			fg = new Figure({x_size/2, 2}, tf,GameOver,clr);
			tf = rand()%7;	
			clr = 3+rand()%6;
			if (rot<50) fg->rotate();
			rot = rand()%100;
			Next_fg->color = clr;
			Next_fg->clear();
			GenerateFig(tf,Next_fg->coords);
			Next_fg->draw();
			if (rot<50) Next_fg->force_rotate();
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
