//to do in this version
//1-add the missing variables to calculate the cost function (done)
//2-sort the vector after each step
//3-input from files
//4-visulaisation of shooting
//5-fix steppping problem

#include <SFML/Graphics.hpp>
#include <time.h>
#include <string.h>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <math.h>
#include <string>
#include <iostream>
#include <iostream>
#include <fstream> // file stream
#include <string>
using namespace std;



using namespace sf;
using namespace std;

//global variables-----------------------------------------------------------------------------------
int unite=9;                   //each 9 pixels resmpels one meter
const int width=1350;          //width of the window (in pixels)
const int hight= 750;
RenderWindow window(VideoMode(width ,hight), "The Game!");  //a glopal window to draw objects on

clock_t start;                 //variables to record time throught the code
clock_t end;
int time_step=100;           //length of time step in ms
int time_step_no=0;          //number of time steps passed
int this_is_new_step=0;      //this will be 1 if we are in a new step
double elasted;              //time elasted between two time steps in ms


int tower_damage[4]={0,0,0,0};  // a variable to communicate damage to each tower its the
                                 // communication channel between the enemys and canons

int TH, NE, TP , c1 , c2 , c3 , numOfEnemy = 8; //you have to enter number of enemies in the file 
//end of global variables----------------------------------------------------------------------------


//forwrd declaration---------------------------------------------------------------------------------
class bomb;
class canon;
class enemy;
class road;
//end of forwrd declaration---------------------------------------------------------------------------



//functions------------------------------------------------------------------------------------------
void draw_grid(int Gwidth,int Ghight)
{
	for (int i=0;i<width;i++)
	{
	   if(i % Gwidth==0)
	   {
	      sf::RectangleShape line(sf::Vector2f(1, 1000));
		  line.setPosition(i,0);
		  window.draw(line);
		
	   }
	}

	for (int i=0;i<hight;i++)
	{
	   if(i % Ghight==0)
	   {
	      sf::RectangleShape line(sf::Vector2f(1500, 1));
		  //line.rotate(90);
		  line.setPosition(0,i);
		  window.draw(line);


	
	   }

	}
}
//end of functions-----------------------------------------------------------------------------------


//classes -------------------------------------------------------------------------------------------
class road
{
public:
	road():length(30),rock_size(unite){}
	road(std::string direction, int road_center):direction(direction),road_center(road_center),length(30),rock_size(unite){}


	
	void draw_road()
	{
		
		if (direction=="right")
		{
			    start=809;
				int ende=start+length*rock_size;

						 Texture s1;
						 s1.loadFromFile("images/path.png");
						 Sprite s(s1);
						 s.setTextureRect(IntRect(0,0,length*unite,6*unite));
						 s.setPosition(start,road_center);
						 window.draw(s);
						
				
		}
		else if (direction=="left")
		{      start=800-250-unite*29;
				int ende=start-length*rock_size;
						 Texture s1;
						 s1.loadFromFile("images/path.png");
						 Sprite s(s1);
						 s.setTextureRect(IntRect(0,0,length*unite,6*unite));
						 s.setPosition(start+(30-length)*unite,road_center);
						 window.draw(s);
	     
		}
		
	}
	int get_length(){return length;}
	void decrement_length(){ length--;}

	int get_centr(){return road_center;}

private : 
	
	int length;                //the length of unpaved part of the road
	int start;                 //the pixel position of the left  side of the unpaved part
	int ende;                  //the pixel position of the right side of the unpaved part
	int road_center;           //the pixel position of the upper side of the unpaved part
	int rock_size;
	std::string direction;
};
class enemy
{

public:
	enemy(){}                                    //default constructor
	friend class bomb;

	// constructor1.. it takes a the five variables of an enmy + pointer to the road vector 
	//so it can modify it with pavers.

	enemy(int TY,int time_stamp,int health,int hit_power,int Hit_period,char R,std::vector<road*> *ROAD_VEC_ptr):Hit_period(Hit_period),shoot(1),shoot_counter(0),hit_power(hit_power),health(health),dead(0),time_stamp(time_stamp),R(R),d(60),steps(0),active(0),TY(TY),ROAD_VEC_ptr(ROAD_VEC_ptr)
	{

		if      (R=='A') {path=(*ROAD_VEC_ptr)[0];image_file="images/soldier1.png";}
		else if (R=='B') {path=(*ROAD_VEC_ptr)[1];image_file="images/soldier3.png";}
		else if (R=='C') {path=(*ROAD_VEC_ptr)[2];image_file="images/soldier3.png";}
		else if (R=='D') {path=(*ROAD_VEC_ptr)[3];image_file="images/soldier1.png";}

		old_step_no=time_step_no; // time variable used to calculate if a step has passed or not

		if (TY==3){k=2;}
		else {k=1;}


	}

	// constructor2.. (for testing purpose)
	enemy(int time_stamp,char R,int d,std::vector<road*> *ROAD_VEC_ptr):health(100),dead(0),time_stamp(time_stamp),R(R),d(d),steps(0),active(0),TY(2),ROAD_VEC_ptr(ROAD_VEC_ptr)
	{
		if      (R=='A') {path=(*ROAD_VEC_ptr)[0];;image_file="images/soldier1.png";}
		else if (R=='B') {path=(*ROAD_VEC_ptr)[1];;image_file="images/soldier3.png";}
		else if (R=='C') {path=(*ROAD_VEC_ptr)[2];;image_file="images/soldier3.png";}
	    else if (R=='D') {path=(*ROAD_VEC_ptr)[3];;image_file="images/soldier1.png";}

		if (TY==3){k=2;}
		else {k=1;}
		old_step_no=time_step_no;  // time variable used to calculate if a step has passed or not
	}
	
	
	
	void is_it_time()  //function used to find if it's time for the enemy to appear or not if so it sets 
		               // the variable active to true
	{
		if(time_step_no>=time_stamp && !dead)
		{
			active=1;
		}
		else
		{
			active =0;	
		}
	}
	void can_advance()     //function tests if time has passed to a new step time
		                   //if so it sets the variable advance
		      
	{
		new_step_no=time_step_no;
		if (new_step_no>old_step_no)
		{
			advance=1;
			old_step_no=new_step_no;
		}
		else
		{
			advance=0;
		}

	}
	void can_shoot()         //function to test if the enemy would shoot this time or would have to 
		                     //wait for a next step ...if it can it would set a variable "shoot" to true
	{
	  if(advance)
	  {
		  shoot_counter++;
	  }
	  
	  if(shoot_counter>=Hit_period && active)
	  {
		    
		    shoot=1;
			shoot_counter=0;
	  }
	  else
	  {shoot=0;}
	}


	void draw_enemy()     //the main function recponsiple for drawing and moving eenmies ..it's summond every step
	{
		
		
		is_it_time();    //check if it's time for enemy to appear
		if(active)       //only draw the enemy if it's active
		{

		//some calculation to be calculated every step 
			if (d==0) {d=1;}
			time_to_shoot=Hit_period-shoot_counter;
			damage= (1.0/((double)d)) *(double)hit_power;
			danger=   (  (1.0/(d+.01))  *   hit_power   *  (  1.0/(abs(time_to_shoot)+1)   ) *  c1  ) + health*c2 + TY *c3;
			can_advance();
			can_shoot();
			if (shoot )	 //in case it's shoot time set the value of the damage to "tower damage array" so
						 //it would be decreased out of canon heath later 
			{
				if      (R=='A'&& TY!=1) {tower_damage[0]+=damage;}  // type 1 can't cause damage
				else if (R=='B'&& TY!=1) {tower_damage[1]+=damage;}
				else if (R=='C'&& TY!=1) {tower_damage[2]+=damage;}
				else if (R=='D'&& TY!=1) {tower_damage[3]+=damage;}

			}


		//updating the position of the enemy
			if      (R=='C') 
			{
				pos=675+(250/2)+d*unite+5;     //pos is the postion of pixel on the window 
				if (advance)                   //in case it's allowed to advance ..update the positon "pos"
				{
					if (d>path->get_length() ){d-=1;} 
					else if (d<=path->get_length() && d>0 && TY==1 )
					{
					path->decrement_length();} 
				}
			}
			else if (R=='B') 
			{
				pos=675+(250/2)+d*unite+5;
				if (advance)
				{
					if (d>path->get_length()){d-=1;}
					else if (d<=path->get_length()&& d>0 && TY==1 ){path->decrement_length();} 
				}
			}
			else if (R=='A') 
			{
	
				pos=675-((250/2)+d*unite+62);
				if (advance)
				{
					if (d>path->get_length()){d-=1;}
					else if (d<=path->get_length() && d>0 && TY==1 ){path->decrement_length();} 

				}
			}
			else if (R=='D') 
			{
				pos=675-((250/2)+d*unite+62);
				if (advance)
				{
					if (d>path->get_length()){d-=1;}
					else if (d<=path->get_length() && d>0 && TY==1 ){path->decrement_length();} 

				}
			}
		
		//starting to upload the sutiable image file to draw in window
			 Texture enemyT;
			 enemyT.loadFromFile(image_file);
			 Sprite enemy(enemyT);
			 enemy.setPosition(pos,path->get_centr());
			 window.draw(enemy);

	  //the next part is used to creat stepping motion visualization by chaning the image to be drawn
			if (advance){steps++;}
			if (R=='A' || R=='D')
			{
				if (steps%2==0)
				{
					if      (TY==2 || TY==3){image_file="images/soldier1.png";}
					else if (TY==1)			{image_file="images/paver1.png"  ;}
				}
				else 
				{
					if      (TY==2 || TY==3){image_file="images/soldier2.png";}
					else if (TY==1)			{image_file="images/paver2.png"  ;}
				}
			}
			else if ((R=='B' || R=='C'))
			{
			if (steps%2==0)
				{
					if      (TY==2 || TY==3){image_file="images/soldier3.png";}
					else if (TY==1)			{image_file="images/paver3.png"  ;}
				}
				else 
				{
					if      (TY==2 || TY==3){image_file="images/soldier4.png";}
					else if (TY==1)			{image_file="images/paver4.png"	 ;}
				}
			}

	}
	}


//gets and sets function
	int get_distance(){return d;}
	int get_TY(){return TY;}
	int get_health(){return health;}
	int get_Hit_period(){return Hit_period;}
	int get_time_stamp(){return time_stamp;}
	int get_pos(){return pos;}
	double get_hit_power(){return hit_power;}
	int get_k(){return k;}
	
	void set_R(char r){R=r;}
	void set_pos( int pose){pos=pose;}

	int  Is_acitve(){return active;}
	bool is_dead()  {return dead;}
	
	double danger;
private:
	int TY;                      //type of enemy (paver,fighter,tanker)
	int time_stamp;              //time step at which the enemy arrived
	int health;                  //health of enemy
	double hit_power;
	int Hit_period;              //number of steps before the enemy attacks
	char R;                      //region
	double damage;

	bool active ;
	double d;                       //distance from the castle in meters
	int pos;                     //positon of pixel on the screen
	bool dead;
	int road_center;             //position of the road
	int steps;                   //number of steps setped( for simulation only)

	
	std::string image_file;
	std::vector<road*> *ROAD_VEC_ptr;   //pointer to the roads vector 
	road * path;

	int old_step_no;
	int new_step_no;
	bool advance;

	int shoot_counter;
	bool shoot;

	int k;
	
	int time_to_shoot;
};
class bomb  
{
public:
	
	bomb(int ypos,int power,enemy* target,std::string R):power(power),target(target),R(R),time_of_step(1),d(0),ypos(ypos),active(1),explosion_counter(0),image_file("images/bomb.png")
	{
	
		clock_t T = clock();
		birth_date=T;
	}
	
	void draw_bomb()
	{
		if(target->active)
		{
			  clock_t end = clock();
			  age = double(end - birth_date) ;

			 int pos=0;
			 if      (R=='C' && Is_active()) 
				{
					pos=675+(250/2)+d*unite+5;   
					if (age>time_of_step && pos<=(target->pos+18))	{d+=1; birth_date=clock();}
					if (pos>=(target->pos+18)) 
					{
						pos=(target->pos+18);  //to make the explosion stick with the enemy
						time_of_step=1000; //to make explosion slower so our eye can see it 
						if     (explosion_counter==0){image_file="images/explosion_1.png";explosion_counter++;}
						else if(explosion_counter==1){image_file="images/explosion_2.png";explosion_counter++;}
						else if(explosion_counter==2){image_file="images/explosion_3.png";explosion_counter++;}
						else if(explosion_counter==3){image_file="images/explosion_4.png";explosion_counter++;}
						else if(explosion_counter==4){image_file="images/explosion_5.png";explosion_counter++;}
						else if(explosion_counter==5){image_file="images/explosion_6.png";explosion_counter++;}
						else if(explosion_counter==6){image_file="images/explosion_7.png";explosion_counter++;}
						else if(explosion_counter==7){image_file="images/explosion_8.png";explosion_counter++;}
						/*else if(explosion_counter==8){image_file="images/explosion_9.png";explosion_counter++;}
						else if(explosion_counter==9){image_file="images/explosion_10.png";explosion_counter++;}
						else if(explosion_counter==10){image_file="images/explosion_11.png";explosion_counter++;}
						else if(explosion_counter==11){image_file="images/explosion_12.png";explosion_counter++;}
						else if(explosion_counter==12){image_file="images/explosion_13.png";explosion_counter++;}*/
						else if(explosion_counter==8)
						   {
							   image_file="images/explosion_14.png";explosion_counter++;
							   active=0;
							   target->health-=power;
							   if( target->health<=0)
							   {
								   target->active=0;
								   target->dead=1;
								   
							   }
						   }
			
					}
				}
				else if (R=='B' && Is_active()) 
				{
					pos=675+(250/2)+d*unite+5;
					if (age>time_of_step && pos<=(target->pos+18))	{d+=1; birth_date=clock();}
					if (pos>=(target->pos+18)) 
					{
						pos=target->pos+18;  //to make the explosion stick with the enemy
						if     (explosion_counter==0){image_file="images/explosion_1.png";explosion_counter++;}
						else if(explosion_counter==1){image_file="images/explosion_2.png";explosion_counter++;}
						else if(explosion_counter==2){image_file="images/explosion_3.png";explosion_counter++;}
						else if(explosion_counter==3){image_file="images/explosion_4.png";explosion_counter++;}
						else if(explosion_counter==4){image_file="images/explosion_5.png";explosion_counter++;}
						else if(explosion_counter==5){image_file="images/explosion_6.png";explosion_counter++;}
						else if(explosion_counter==6){image_file="images/explosion_7.png";explosion_counter++;}
						else if(explosion_counter==7){image_file="images/explosion_8.png";explosion_counter++;}
					/*	else if(explosion_counter==8){image_file="images/explosion_9.png";explosion_counter++;}
						else if(explosion_counter==9){image_file="images/explosion_10.png";explosion_counter++;}
						else if(explosion_counter==10){image_file="images/explosion_11.png";explosion_counter++;}
						else if(explosion_counter==11){image_file="images/explosion_12.png";explosion_counter++;}
						else if(explosion_counter==12){image_file="images/explosion_13.png";explosion_counter++;}*/
						else if(explosion_counter==8)
						   {
							   image_file="images/explosion_14.png";explosion_counter++;
							   active=0;
							  target->health-=power;
							   if( target->health<=0)
							   {
								   target->active=0;
								   target->dead=1;
							   }
						   }
			
					}
				}
				else if (R=='A' && Is_active()) 
				{
			
					pos=675-((250/2)+d*unite+62);
					if (age>time_of_step && pos>=(target->pos+18))	{d+=1; birth_date=clock();}
					if (pos<=(target->pos+18)) 
					{   
						pos=(target->pos+18);  //to make the explosion stick with the enemy
						time_of_step=1000; //to make explosion slower so our eye can see it 

						if     (explosion_counter==0){image_file="images/explosion_1.png";explosion_counter++;}
						else if(explosion_counter==1){image_file="images/explosion_2.png";explosion_counter++;}
						else if(explosion_counter==2){image_file="images/explosion_3.png";explosion_counter++;}
						else if(explosion_counter==3){image_file="images/explosion_4.png";explosion_counter++;}
						else if(explosion_counter==4){image_file="images/explosion_5.png";explosion_counter++;}
						else if(explosion_counter==5){image_file="images/explosion_6.png";explosion_counter++;}
						else if(explosion_counter==6){image_file="images/explosion_7.png";explosion_counter++;}
						else if(explosion_counter==7){image_file="images/explosion_8.png";explosion_counter++;}
						/*else if(explosion_counter==8){image_file="images/explosion_9.png";explosion_counter++;}
						else if(explosion_counter==9){image_file="images/explosion_10.png";explosion_counter++;}
						else if(explosion_counter==10){image_file="images/explosion_11.png";explosion_counter++;}
						else if(explosion_counter==11){image_file="images/explosion_12.png";explosion_counter++;}
						else if(explosion_counter==12){image_file="images/explosion_13.png";explosion_counter++;}*/
						else if(explosion_counter==8)
						   {
							   image_file="images/explosion_14.png";explosion_counter++;
							   active=0;
							   target->health-=power;
							   							  

							   if( target->health<=0)
							   {
								   target->active=0;
								   target->dead=1;
							   }
						   }
			
					}

				}
				else if (R=='D' && Is_active()) 
				{
					pos=675-((250/2)+d*unite+62);
					if (age>time_of_step && pos>=(target->pos+18))	{d+=1; birth_date=clock();}
					if (pos<=(target->pos+18)) 
					{
						pos=(target->pos+18);  //to make the explosion stick with the enemy
						time_of_step=1000; //to make explosion slower so our eye can see it 

						if     (explosion_counter==0){image_file="images/explosion_1.png";explosion_counter++;}
						else if(explosion_counter==1){image_file="images/explosion_2.png";explosion_counter++;}
						else if(explosion_counter==2){image_file="images/explosion_3.png";explosion_counter++;}
						else if(explosion_counter==3){image_file="images/explosion_4.png";explosion_counter++;}
						else if(explosion_counter==4){image_file="images/explosion_5.png";explosion_counter++;}
						else if(explosion_counter==5){image_file="images/explosion_6.png";explosion_counter++;}
						else if(explosion_counter==6){image_file="images/explosion_7.png";explosion_counter++;}
						else if(explosion_counter==7){image_file="images/explosion_8.png";explosion_counter++;}
						/*else if(explosion_counter==8){image_file="images/explosion_9.png";explosion_counter++;}
						else if(explosion_counter==9){image_file="images/explosion_10.png";explosion_counter++;}
						else if(explosion_counter==10){image_file="images/explosion_11.png";explosion_counter++;}
						else if(explosion_counter==11){image_file="images/explosion_12.png";explosion_counter++;}
						else if(explosion_counter==12){image_file="images/explosion_13.png";explosion_counter++;}*/
						else if(explosion_counter==8)
						   {
							   image_file="images/explosion_14.png";explosion_counter++;
							   active=0;
							   target->health-=power;
							   if( target->health<=0)
							   {
								   target->active=0;
								   target->dead=1;
							   }
						   }
			
					}

				}
		
		
			 Texture bombT;
			 bombT.loadFromFile(image_file);
			 Sprite bomb(bombT);
			 bomb.setPosition(pos,ypos+18);
			 if(active){	 window.draw(bomb);}
		}
	 }







    void   set_power(int p){power=p;}
	enemy* get_target(){return target;}
	bool   Is_active(){return active;}

private:
	bool active;
	int explosion_counter; //counter to count when bomb reach the enemy when it reachs "2" the bomb will be inactivated
	int d;
	int ypos;
	road* path;
	enemy* target;
	int x;
	int y;
	std::string R;
	std::string direction;
	double age;             //time since activation (in m sec)
	double birth_date;      //time of activation (in m seconds)
	double time_of_step;    //seconds between two steps
	int power;              //power of the bomb=power of the tower 
	std::string image_file;
};
class canon
{

public:
	canon(int x,int y,std::string direction,std::string R,int power,int N,int health):N(N),power(power),x(x),y(y),direction(direction),health(health),R(R),damage(0){}
	
	void draw_canon()
	{
		if (R=='A')    
		{
			if (tower_damage[0]!=0)
			{
				health-=tower_damage[0];
				tower_damage[0]=0;
			}
		}
		else if (R=='B') 
			{
			if (tower_damage[1]!=0)
			{
				health-=tower_damage[1];
				tower_damage[1]=0;
			}
		}
		else if (R=='C') 
		{
			if (tower_damage[2]!=0)
			{
				health-=tower_damage[2];
				tower_damage[2]=0;
			}
		}
		else if (R=='D') 
		{

			if (tower_damage[3]!=0)
			{
				health-=tower_damage[3];
				tower_damage[3]=0;

			}
		}
		
		if (health<=0){active=0;}


		Texture s1;
		if      (direction=="right"){s1.loadFromFile("images/right_gun.png");}
		else if (direction=="left") {s1.loadFromFile("images/left_gun.png") ;}

		if (health<=0){s1.loadFromFile("images/hole.png") ;}
		Sprite s(s1);
		s.setPosition(x,y);
		window.draw(s);
	}

	void shoot(enemy* target ,std::vector<bomb> & BOMB_VEC)  // shoot function takes a vecotr (BOMB_VEC) containg all bombs existing in the world
	{
		int flag=0;
		for(auto x:BOMB_VEC){if (x.get_target()==target ) {flag = 1;}} // if there is any bomb that already aims for the same target 
		                                                               //don't creat other bomb
		if(!target->Is_acitve()){flag=1;}                              //if the enemy dosn't exist don't creat a bomb
		if(!active){flag=1;}                                           //canon must be active 
		if (flag==0)
		{
			damage= (1.0/(double)(target->get_distance()+.0001))  *  (double)power  *  (double)target->get_k();

			//cout<<"power: "<<power<<"  damage: "<<damage <<endl;
			bomb BOMB(y,damage, target, R );
			BOMB_VEC.push_back(BOMB);

		}
	  
	}

	void shootN( std::vector<enemy> & enmey_vector,std::vector<bomb> & BOMB_VEC)
	{    int NN=N;
		if (enmey_vector.size()<N){NN=enmey_vector.size();}
		for(int i=0;i<NN;i++)
		{
			shoot(& enmey_vector[i] , BOMB_VEC);
		}
	}
	int gety(){return y;}
	int getx(){return x;}
	int get_power(){return power;}
	bool Is_active(){return active;}
protected:
	std::string R;
	int health;
	int x;
	int y;
	bool active;
	std::string direction;
	int power;
	int N;
	int damage;
};

bool is_bomb_not_active(bomb b){return !b.Is_active();}
bool is_enemy_dead(enemy e){return e.is_dead();}
class world
{ 
public:
	world():total_killed(0){}
	void draw_the_world()
		{
			
				for (auto x:ROAD_VEC)     {x->draw_road();} // first draw the unpaved roads so they appear at the pottom of the object

				//sort the enemy vectors using stl sort algorithm based on the danger of each enemy
				std::sort(ENEMYA_VEC.begin(), ENEMYA_VEC.end(), [](const enemy& lhs, const enemy& rhs){ return lhs.danger > rhs.danger; });
				std::sort(ENEMYB_VEC.begin(), ENEMYB_VEC.end(), [](const enemy& lhs, const enemy& rhs){ return lhs.danger > rhs.danger; });
				std::sort(ENEMYC_VEC.begin(), ENEMYC_VEC.end(), [](const enemy& lhs, const enemy& rhs){ return lhs.danger > rhs.danger; });
				std::sort(ENEMYD_VEC.begin(), ENEMYD_VEC.end(), [](const enemy& lhs, const enemy& rhs){ return lhs.danger > rhs.danger; });
			
				//draw the enemies
				for (auto & x:ENEMYA_VEC)  {if(x.get_TY()==1){x.draw_enemy();}} //draw pavers first so real enemys appear on the top
				for (auto & x:ENEMYA_VEC)  {if(x.get_TY()!=1){x.draw_enemy();}} 
				for (auto & x:ENEMYB_VEC)  {if(x.get_TY()==1){x.draw_enemy();}} //draw pavers first so real enemys appear on the top
				for (auto & x:ENEMYB_VEC)  {if(x.get_TY()!=1){x.draw_enemy();}} 
				for (auto & x:ENEMYC_VEC)  {if(x.get_TY()==1){x.draw_enemy();}} //draw pavers first so real enemys appear on the top
				for (auto & x:ENEMYC_VEC)  {if(x.get_TY()!=1){x.draw_enemy();}} 
				for (auto & x:ENEMYD_VEC)  {if(x.get_TY()==1){x.draw_enemy();}} //draw pavers first so real enemys appear on the top
				for (auto & x:ENEMYD_VEC)  {if(x.get_TY()!=1){x.draw_enemy();}} 


				int ka1=ENEMYA_VEC.size(); //measure the size of enemy vecotr befor and after removing dead enemies so we know the
									    	//number of the dead in this step
				ENEMYA_VEC.erase(std::remove_if(ENEMYA_VEC.begin(), ENEMYA_VEC.end(), is_enemy_dead), ENEMYA_VEC.end());      //remove dead enemys from the vector
				int ka2=ENEMYA_VEC.size();

				int kb1=ENEMYB_VEC.size();//measure the size of enemy vecotr befor and after removing dead enemies so we know the
										  //number of the dead in this step
				ENEMYB_VEC.erase(std::remove_if(ENEMYB_VEC.begin(), ENEMYB_VEC.end(), is_enemy_dead), ENEMYB_VEC.end());      //remove dead enemys from the vector
				int kb2=ENEMYB_VEC.size();

				int kc1=ENEMYC_VEC.size();//measure the size of enemy vecotr befor and after removing dead enemies so we know the
										  //number of the dead in this step
				ENEMYC_VEC.erase(std::remove_if(ENEMYC_VEC.begin(), ENEMYC_VEC.end(), is_enemy_dead), ENEMYC_VEC.end());      //remove dead enemys from the vector
				int kc2=ENEMYC_VEC.size();
			
				int kd1=ENEMYD_VEC.size();//measure the size of enemy vecotr befor and after removing dead enemies so we know the
										  //number of the dead in this step
				ENEMYD_VEC.erase(std::remove_if(ENEMYD_VEC.begin(), ENEMYD_VEC.end(), is_enemy_dead), ENEMYD_VEC.end());      //remove dead enemys from the vector
				int kd2=ENEMYD_VEC.size();
			

				for (auto & x:canon_VEC)  {x.draw_canon();}


				BOMB_VEC.erase(std::remove_if(BOMB_VEC.begin(), BOMB_VEC.end(), is_bomb_not_active), BOMB_VEC.end());      //remove exploded bombs
				for (auto & x:BOMB_VEC)	  {x.draw_bomb(); }      
															  //draw active bombs
				//calculating total kills of all the time .
				total_killed+=ka1-ka2 + kb1-kb2 + kc1-kc2 + kd1-kd2;

			if (this_is_new_step)
			{
				cout<<"step number : " << time_step_no<< endl;
				cout<<"killed from beging : " << total_killed<<endl;
				cout<<"R : A "<< "killed last step : " <<ka1-ka2<<"  no of enemies: "<< ka2<<" unbaved region : "<< ROAD_VEC[0]->get_length()<<endl;
				cout<<"R : B "<< "killed last step : " <<kb1-kb2<<"  no of enemies: "<< kb2<<" unbaved region : "<< ROAD_VEC[1]->get_length()<<endl;
				cout<<"R : C "<< "killed last step : " <<kc1-kc2<<"  no of enemies: "<< kc2<<" unbaved region : "<< ROAD_VEC[2]->get_length()<<endl;
				cout<<"R : D "<< "killed last step : " <<kd1-kd2<<"  no of enemies: "<< kd2<<" unbaved region : "<< ROAD_VEC[3]->get_length()<<endl;
				////*_sleep(100);*
			}


	


			if(canon_VEC[0].Is_active()==0) //move the enemies in region A to region b
			{
				for (auto & x:ENEMYA_VEC) 
				{
					enemy e1(x.get_TY(),x.get_time_stamp(),x.get_health(),x.get_hit_power(),x.get_Hit_period(),'B',& ROAD_VEC);
				    ENEMYB_VEC.push_back(e1);
				}
				ENEMYA_VEC.clear();
			}

			if(canon_VEC[1].Is_active()==0) //move the enemies in region A to region b
			{
				for (auto & x:ENEMYB_VEC) 
				{
					enemy e1(x.get_TY(),x.get_time_stamp(),x.get_health(),x.get_hit_power(),x.get_Hit_period(),'C',& ROAD_VEC);
				    ENEMYC_VEC.push_back(e1);
				}
				ENEMYB_VEC.clear();
			}

			if(canon_VEC[2].Is_active()==0) //move the enemies in region A to region b
			{
				for (auto & x:ENEMYC_VEC) 
				{
					enemy e1(x.get_TY(),x.get_time_stamp(),x.get_health(),x.get_hit_power(),x.get_Hit_period(),'D',& ROAD_VEC);
				    ENEMYD_VEC.push_back(e1);
				}
				ENEMYC_VEC.clear();
			}

			if(canon_VEC[3].Is_active()==0) //move the enemies in region A to region b
			{
				for (auto & x:ENEMYD_VEC) 
				{
					enemy e1(x.get_TY(),x.get_time_stamp(),x.get_health(),x.get_hit_power(),x.get_Hit_period(),'A',& ROAD_VEC);
				    ENEMYA_VEC.push_back(e1);
				}
				ENEMYD_VEC.clear();
			}



			if(total_killed==numOfEnemy)
			{
				while(1) 
				{
				  Texture t1;	t1.loadFromFile("images/castle_win.png");	Sprite  winner(t1);
					  window.draw(winner);	  //draw the back ground 
					  window.display();     
					  Event e1;
				      while (window.pollEvent(e1)){if(e1.type == Event::Closed)window.close();}
				}
			}

			int desroyed_castle_flag=1;
			for (auto & x:canon_VEC)  { if( x.Is_active() ) {desroyed_castle_flag=0;} }
			if(desroyed_castle_flag)
			{
				while(1) 
				{
				  Texture t1;	t1.loadFromFile("images/attacker_win.png");	Sprite  winner(t1);
					  window.draw(winner);	  //draw the back ground 
					  window.display();           

					   Event e1;
				      while (window.pollEvent(e1)){if(e1.type == Event::Closed)window.close();}
				}
			}


		}

	
		
	std::vector<enemy> ENEMYA_VEC;
	std::vector<enemy> ENEMYB_VEC;
	std::vector<enemy> ENEMYC_VEC;
	std::vector<enemy> ENEMYD_VEC;

	std::vector<canon> canon_VEC;
	std::vector<road*> ROAD_VEC;
	std::vector<bomb>  BOMB_VEC;

	int total_killed;                               //numer of total killed enemies of all the tim  


};
//end of classes ------------------------------------------------------------------------------------



int main()
{
	world my_world;   //first creat a world to be drawn

	ifstream fin; // object input file stream
	fin.open("data.txt");
	if(fin.fail())
	{
		cerr << "Error opening the file" << endl; // c error for cout-ing errors
		exit(1); // to exit the program without crashing
	}


	fin >> TH;	fin >> NE;	fin >> TP;	fin >> c1;	fin >> c2;	fin >> c3;
	int ID,arrival_time,health,power,hit_period,type; int region;
 

//creat four roads and add them to the world
	static road pathA("left",300);
	static road pathB("right",300);
	static road pathC("right",440);
	static road pathD("left",440);

	my_world.ROAD_VEC.push_back(&pathA);
	my_world.ROAD_VEC.push_back(&pathB);
	my_world.ROAD_VEC.push_back(&pathC);
	my_world.ROAD_VEC.push_back(&pathD);

//creat four canons and add them to the world

	canon canonA(600-3*9,300    ,"left" ,"A",TP,NE,TH);
	canon canonB(760-3*9,300    ,"right","B",TP,NE,TH);
	canon canonC(760-3*9,470-2*9,"right","C",TP,NE,TH);
	canon canonD(600-3*9,470-2*9,"left" ,"D",TP,NE,TH);

	my_world.canon_VEC.push_back(canonA);
	my_world.canon_VEC.push_back(canonB);
	my_world.canon_VEC.push_back(canonC);
	my_world.canon_VEC.push_back(canonD);


	   for (int i=0; i<numOfEnemy ; ++i)
	{
		fin >> ID;
		fin >> arrival_time;
		fin >> health;
		fin >>power;
		fin >> hit_period;
		fin >> type;
		fin >>region;
		
		if     (region==1)
		{
			//enemy ( TY , time_stamp , health , hit_power , Hit_period, R,....)
			  enemy e1(type,arrival_time,health,power,hit_period,'A',& my_world.ROAD_VEC);
			  my_world.ENEMYA_VEC.push_back(e1);
		}
		else if(region==2)
		{
			  enemy e2(type,arrival_time,health,power,hit_period,'B',& my_world.ROAD_VEC);
			  my_world.ENEMYB_VEC.push_back(e2);
		}
		else if(region==3)
		{
			  enemy e3(type,arrival_time,health,power,hit_period,'C',& my_world.ROAD_VEC);
			  my_world.ENEMYC_VEC.push_back(e3);
		}
		else if(region==4)
		{
			 enemy e4(type,arrival_time,health,power,hit_period,'D',& my_world.ROAD_VEC);
			 my_world.ENEMYD_VEC.push_back(e4);
		}


	}


	clock_t T1 = clock();
	


//  start creating the world 
	Texture t2;	t2.loadFromFile("images/background3.png");	Sprite  background(t2);
    while (window.isOpen())
    {
	    Event e;
        while (window.pollEvent(e)){if(e.type == Event::Closed)window.close();}

		this_is_new_step=0;

		clock_t T2 = clock();
		elasted = double(T2 - T1);
		if (elasted>=time_step)
		{
			time_step_no++;
			this_is_new_step=1;
			T1=clock();
		}



		window.clear(Color::White);	  //clear the window from the old world
		window.draw(background);	  //draw the back ground 
		my_world.draw_the_world();    //draw the new world on the background (all the work is done here)


	

		my_world.canon_VEC[0].shootN(my_world.ENEMYA_VEC,my_world.BOMB_VEC);  //tell canon A to shoot N enemies in A region
		my_world.canon_VEC[1].shootN(my_world.ENEMYB_VEC,my_world.BOMB_VEC);
		my_world.canon_VEC[2].shootN(my_world.ENEMYC_VEC,my_world.BOMB_VEC);
		my_world.canon_VEC[3].shootN(my_world.ENEMYD_VEC,my_world.BOMB_VEC);
		//draw_grid(9,9);


		window.display();             //display the window
	
	}

    return 0;
}