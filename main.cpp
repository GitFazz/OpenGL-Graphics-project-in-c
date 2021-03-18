#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))
#define no_of_circle 5
#define circle_red 75
#define square_side 120

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle,step=1.002;
bool inCircle[no_of_circle],isPaused;
double speed,prev_speed;

struct point
{
	double x,y,z;
	point(double xx,double yy) {
        x = xx; y = yy;
	}
	point() {
	    x = 0; y = 0; z = 0;
	}
}p[no_of_circle];

struct vect
{
    double x,y,z;

    vect(double xx,double yy) {
        x = xx; y = yy;
    }

    vect() {
        x = 0; y = 0; z = 0;
    }

}v[no_of_circle];

point add_vect_point(struct point p,struct vect v)
{
    p.x = p.x + v.x;
    p.y = p.y + v.y;
    p.z = p.z + v.z;

    return p;
}

vect rotate_vect(vect v,double a) {
    vect r;

    r.x = cos(a)*v.x - sin(a)*v.y;
    r.y = sin(a)*v.x + cos(a)*v.y;

    return r;
}

vect add_vect_vect(struct vect one,struct vect two) {

    return vect( one.x+two.x, one.y+two.y);
}


void print_vect(struct vect v) {
    printf("( %lf , %lf ) \n",v.x,v.y);
}

vect vect_from_two_point(struct point a,struct point b) {
    vect r;
    r.x = b.x-a.x;
    r.y = b.y-a.y;

	return r;
}

vect neg_vect(struct vect v) {

    return vect(-v.x,-v.y);
}

vect perpendiculer_vect( vect v) {
    return vect(v.y,-v.x);
}

double dist_two_point(struct point a,struct point b) {
    return sqrt( (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) );
}

double dot_product (struct vect a,struct vect b) {
    return (a.x*b.x+a.y*b.y);
}

double dist_from_origin(struct point a) {
    return sqrt(a.x*a.x+a.y*a.y);
}

double value_of_vect(struct vect a) {
    return sqrt(a.x*a.x+a.y*a.y);
}

vect projection_over_another_vect(struct vect a,struct vect b) {

   double val =  dot_product(a,b)/(a.x*a.x+a.y*a.y);

   return vect(a.x*val,a.y*val);
}




void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    glColor3f(0,1,0);

		//glVertex2i( a, a);
		//glVertex2i( a,-a);
		//glVertex2i(-a,-a);
		//glVertex2i(-a, a);
    glBegin(GL_LINES);
        glVertex2f(a,a);
        glVertex2f(a,-a);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(a,-a);
        glVertex2f(-a,-a);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(-a,-a);
        glVertex2f(-a,a);
    glEnd();

    glBegin(GL_LINES);
        glVertex2f(-a,a);
        glVertex2f(a,a);
    glEnd();

}



void drawCircle(double radius,int segments)
{
    // a comment added
    int i;
    struct point points[100];
    glColor3f(1,0,0);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }


}




void set_speed(double speed) {
    for(int i=0;i<no_of_circle;i++) {
        v[i].x = v[i].x * speed;
        v[i].y = v[i].y * speed;
    }
}



void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case 'p':

            isPaused = !isPaused;

			break;

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			speed = 0.75;
			set_speed(speed);
			break;
		case GLUT_KEY_UP:		// up arrow key
			speed = 1.25;
			set_speed(speed);
			break;

		case GLUT_KEY_RIGHT:
			cameraAngle += 0.03;
			break;
		case GLUT_KEY_LEFT:
			cameraAngle -= 0.03;
			break;

		case GLUT_KEY_PAGE_UP:
			break;
		case GLUT_KEY_PAGE_DOWN:
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}



void movingCircle(int N) {

    for(int i=0;i<N;i++)
    {
       glPushMatrix();
        {
           glTranslatef(p[i].x,p[i].y,0);
           drawCircle(10,30);
        }
       glPopMatrix();
    }

}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);
	//initialize the matrix
	glLoadIdentity();

	//now give three info
	//1. where is the camera (viewer)?
	//2. where is the camera looking?
	//3. Which direction is the camera's UP direction?

	//gluLookAt(100,100,100,	0,0,0,	0,0,1);
	//gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	//drawAxes();
	drawGrid();
    //drawSS();
    //glColor3f(1,0,0);
    //drawSquare(10);




    drawSquare(square_side);
    drawCircle(circle_red,50);

    movingCircle(no_of_circle);
    //drawCone(20,50,24);

	//drawSphere(30,24,20);




	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}

void check_collision_with_side_bar(int i) {

    if(square_side-abs(p[i].x)<=10) {
        v[i].x = -v[i].x;
    }

    if(square_side-abs(p[i].y)<=10) {
        v[i].y = -v[i].y;
    }

}

void check_collision_with_circle(int i) {

     if( (circle_red-dist_from_origin(p[i]))<=10 ) {


        vect v_radious(p[i].x,p[i].y) ;

        if(dot_product(v_radious,v[i])<0) return;

        vect v_perp_radious = vect(p[i].y,-p[i].x);
        vect v_normal = projection_over_another_vect(v_radious,v[i]);
        vect v_tangent = projection_over_another_vect(v_perp_radious,v[i]);
        v[i] = add_vect_vect(  neg_vect(v_normal), v_tangent  );


     }

}

void check_collision_between_bubble(int i) {
    for(int ii=0;ii<no_of_circle;ii++) {
        if(ii!=i && dist_two_point(p[i],p[ii])<=20) {


            vect v_lr = vect_from_two_point(p[i],p[ii]);
            vect v_lr_perp = perpendiculer_vect(v_lr);
            vect v_l_ver = projection_over_another_vect(v_lr_perp,v[i]);
            vect v_l_hor = projection_over_another_vect(v_lr,v[i]);


            vect v_rl = vect_from_two_point(p[ii],p[i]);
            vect v_rl_perp = perpendiculer_vect(v_rl);
            vect v_r_ver = projection_over_another_vect(v_rl_perp,v[ii]);
            vect v_r_hor = projection_over_another_vect(v_rl,v[ii]);

            v[i] = add_vect_vect( v_r_hor,v_l_ver );
            v[ii] = add_vect_vect( v_l_hor,v_r_ver );


        }
    }
}

void check_in_circle(int i) {

    if( dist_from_origin(p[i]) <= (circle_red-10) )
        inCircle[i] = true;

}




void animate(){
	angle+=0.01;



    for(int i=0;i<no_of_circle;i++) {

        if(!inCircle[i]) {
            check_in_circle(i);
            check_collision_with_side_bar(i);
        }

        else {

            check_collision_with_circle(i);
            check_collision_between_bubble(i);

        }

        if(!isPaused) {
            p[i] = add_vect_point(p[i],v[i]);
        }

    }



	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;
	speed = 1;
	isPaused = false;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance

	vect init_v(0.009,0.006);

    for(int i=0;i<no_of_circle;i++) {

        p[i].x = -(square_side-10.001);
        p[i].y = -(square_side-10.001);

        v[i] = rotate_vect(init_v,i*10);

        inCircle[i] = false;
    }




}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	////////// for circle //////////

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	////////////// test ////////////



    ///////////////////////////////
	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
