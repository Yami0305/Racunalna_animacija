#include <stdio.h>
#include <freeglut.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <TIME.H>

GLint window;
GLint width = 1000, height = 1000;

int br_tocaka = 0;
int vertex_count = 0;
int poly_count = 0;
double* vertices;
int* polygons; 
double* koefs;
double* copy;
int* krivulja;
int calculated = 1;
int read1 = 0;
double okox,okoy,okoz;
double glex,gley,glez;
double H;
double TPkon[4][4];
double rotate_vect [3];
double* vertices_backup;
double sredx, sredy, sredz;
double B_matrica[4][4] = {{-1./6., 3./6., -3./6., 1./6.}, {3./6., -1., 3./6., 0.},{-1./2., 0., 1./2., 0.}, {1./6., 4./6., 1./6., 0.}};
double B_matrica_tangente[4][4] = {{-1./2., 3./2., -3./2., 1./2.}, {1., -2., 1., 0.},{-1./2., 0., 1./2., 0.},{0.,0.,0.,0.}};
double* b_spline_arr;
double* b_backup;
double* b_spline_vector;
double phi;

void myDisplay();
void myReshape(int width, int height);
void function();
//void myMouse(int button, int state, int x, int y);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);
void myObject();
void calculateVertices();
void MatrixxMatrix(double arr1[4][4], double arr2[4][4], double result[4][4]);
void VectorxMatrix(double vec2[4], double mat1[4][4], double result[4]);
void reset();
void b_spline();
void b_draw();
void animate();
void move(); 
void multiplyVertices();
void rotate();
void reset_b();
void myIdle();
int main(int argc, char ** argv)
{
    if (calculated == 1){
        function();
    }

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	glutInit(&argc, argv);
    glutIdleFunc(myIdle);
	window = glutCreateWindow("Glut OpenGL Tijelo");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	//glutMouseFunc(myMouse);
	glutKeyboardFunc(myKeyboard);

		
	glutMainLoop();
	return 0;
}

void rotate(){
    b_spline();
    animate();
    for (int i = 0; i < ((br_tocaka - 3)*10) * 3; i+=3){
        rotate_vect[0] = b_spline_vector[i+1]*0. - 0.*b_spline_vector[i+2];
        rotate_vect[1] = -(b_spline_vector[i]*1. - b_spline_vector[i+2]*0.);
        rotate_vect[2] = 0.*b_spline_vector[i+1] - 0.*b_spline_vector[i];

        phi = acos((b_spline_vector[i]*0+b_spline_vector[i+1]*0+b_spline_vector[i+2]*1)/(sqrt(b_spline_vector[i]*b_spline_vector[i] + b_spline_vector[i+1]*b_spline_vector[i+1] + b_spline_vector[i+2]*b_spline_vector[i+2])))*180/3.1415926;
        //glRotatef((float) phi, rotate_vect[0], rotate_vect[1], rotate_vect[2]);
        //glutPostRedisplay();
        //glPopMatrix();
        //glutPostRedisplay();
        //sleep(1);
    }


}
void move() {

    struct timespec delay;
    delay.tv_sec = 0;      // Seconds
    delay.tv_nsec = 50000000; // Nanoseconds (0.5 seconds)
    b_spline();
    printf("Prije: %lf, %lf, %lf\n", sredx, sredy, sredz);
    sredx = b_spline_arr[0]/10;
    sredy = b_spline_arr[1]/10;
    sredz = b_spline_arr[2]/10;
    //calculateVertices();
    //multiplyVertices();
    printf("Poslije: %lf, %lf, %lf\n", sredx, sredy, sredz);
    for (int i = 0; i < ((br_tocaka - 3)*10) * 3; i+=3){
        
        double vektor_pomaka [3] = {(b_spline_arr[i] - b_spline_arr[0])/4.2, (b_spline_arr[i+1] - b_spline_arr[1])/4.2, (b_spline_arr[i+2] - b_spline_arr[2])/4.2 };
        for (int j = 0; j < vertex_count*3; j+=3){
            double rez[3] = {vertices[j] + vektor_pomaka[0], vertices[j+1] + vektor_pomaka[1], vertices[j+2] + vektor_pomaka[2]};
            vertices[j] = rez[0];
            vertices[j+1] = rez[1];
            vertices[j+2] = rez[1];
        }
        rotate_vect[0] = b_spline_vector[i+1]*0. - 0.*b_spline_vector[i+2];
        rotate_vect[1] = -(b_spline_vector[i]*1. - b_spline_vector[i+2]*0.);
        rotate_vect[2] = 0.*b_spline_vector[i+1] - 0.*b_spline_vector[i];

        phi = acos((b_spline_vector[i]*0+b_spline_vector[i+1]*0+b_spline_vector[i+2]*1)/(sqrt(b_spline_vector[i]*b_spline_vector[i] + b_spline_vector[i+1]*b_spline_vector[i+1] + b_spline_vector[i+2]*b_spline_vector[i+2])))*180/3.1415926;
        //calculateVertices();
        //multiplyVertices();
        //glRotatef((float) phi, rotate[0], rotate[1], rotate[2]);
        myDisplay();
        glPopMatrix();
        reset();
        nanosleep(&delay, NULL);
    }
}
void b_spline() {

    /*printf("b_spline\n");
    printf("matrica");
    for (int j = 0; j < 4; j++){
        printf("\n");
        for (int k = 0; k < 4; k++){
            printf("%lf ", B_matrica[j][k]);
            }
        }*/
    b_spline_arr = (double*)calloc(((br_tocaka - 3)*100) * 3, sizeof(double));
    for (int i = 1; i < (br_tocaka - 3)+1; i++){
        double kt_matrix[4][4] = {{krivulja[(i-1)*3], krivulja[(i-1)*3+1], krivulja[(i-1)*3+2],1},
                                {krivulja[(i)*3], krivulja[(i)*3+1], krivulja[(i)*3+2],1},
                                {krivulja[(i+1)*3], krivulja[(i+1)*3+1], krivulja[(i+1)*3+2],1},
                                {krivulja[(i+2)*3], krivulja[(i+2)*3+1], krivulja[(i+2)*3+2],1}};


        //printf("\n");
        for (int t = 0; t < 100; t+=1){
            double vect[4] = {(t/10.)*(t/10.)*(t/10.), (t/10.)*(t/10.), (t/10.), 1};
            double temp[4] = {0.0,0.0,0.0,0.0};
            VectorxMatrix(vect,B_matrica,temp);
            //printf("temp\n");
            //printf("%lf, %lf, %lf, %lf\n", B_matrica[0][0],B_matrica[1][0],B_matrica[2][0],B_matrica[3][0]);
            double res_tocka[4] = {0,0,0,0};
            
            VectorxMatrix(temp,kt_matrix,res_tocka);
            //printf("%lf, %lf, %lf, %lf\n", res_tocka[0],res_tocka[1],res_tocka[2],res_tocka[3]);
            b_spline_arr[(i-1)*10*3+t*3] = res_tocka[0];
            b_spline_arr[(i-1)*10*3+t*3+1] = res_tocka[1];
            b_spline_arr[(i-1)*10*3+t*3+2] = res_tocka[2];
        }
    }

    b_backup = (double*)calloc(br_tocaka * 3, sizeof(double));
    memcpy(b_backup, b_spline_arr, br_tocaka * 3 * sizeof(double));


    animate();
    //b_draw(); 
    //reset_b();
}

void animate(){

    b_spline_vector = (double*)calloc(((br_tocaka - 3)*100) * 3, sizeof(double));

    for (int i = 1; i < (br_tocaka - 3)+1; i++){
        double kt_matrix[4][4] = {{krivulja[(i-1)*3], krivulja[(i-1)*3+1], krivulja[(i-1)*3+2],1},
                                {krivulja[(i)*3], krivulja[(i)*3+1], krivulja[(i)*3+2],1},
                                {krivulja[(i+1)*3], krivulja[(i+1)*3+1], krivulja[(i+1)*3+2],1},
                                {krivulja[(i+2)*3], krivulja[(i+2)*3+1], krivulja[(i+2)*3+2],1}};
        for (int t = 0; t < 100; t+=1){
            double vect[4] = {(t/10.)*(t/10.), t/10., 1, 0};
            double temp[4] = {0,0,0,0};
            VectorxMatrix(vect,B_matrica_tangente,temp);
            //printf("temp \n%lf, %lf, %lf, %lf\n", temp[0],temp[1],temp[2],temp[3]);
            double res_tocka[4] = {0,0,0,0};
            VectorxMatrix(temp,kt_matrix,res_tocka);
            b_spline_vector[(i-1)*10*3+t*3] = res_tocka[0];
            b_spline_vector[(i-1)*10*3+t*3+1] = res_tocka[1];
            b_spline_vector[(i-1)*10*3+t*3+2] = res_tocka[2];
        }
    }

}
void function() {
    
    if (read1 == 1){
        return;
    }
    okox = okoy = okoz = 1;
    glex = gley = glez = 0.0;
    FILE* object = fopen("model.obj", "r");

    char line[256];
   
    while (fgets (line, sizeof(line), object)) {
        if (line[0] == 'v') {
            vertex_count++;
        } else if (line[0] == 'f'){
            poly_count++;
        } else if (line[0] == 'o'){
            sscanf(line, "o %lf %lf %lf", &okox, &okoy, &okoz);
            printf("%lf, %lf, %lf", okox, okoy, okoz);
        }
        else if (line[0] == 'G'){
            sscanf(line, "G %lf %lf %lf", &glex, &gley, &glez);
            printf("\n%lf, %lf, %lf", glex, gley, glez);
        }
    }
    fclose(object);
    //printf("Tu sam");
    
    vertices = (double*)calloc(vertex_count * 3, sizeof(double));
    polygons = (int*)calloc(poly_count * 3, sizeof(int));
    koefs = (double*)calloc(poly_count * 4, sizeof(double));
    krivulja = (int*)calloc(12 * 3, sizeof(int));

    vertices_backup = (double*)calloc(vertex_count * 3, sizeof(double));


    copy = (double*)calloc(vertex_count * 3, sizeof(double));
    int br_vertex = 0;
    int br_poly = 0;
    int br_krivulja = 0;

    /*for (int i = 0; i < vertex_count*3; i+=3){
        printf("\n ORIGINAL %lf, %lf, %lf\n", vertices[i],vertices[i+1],vertices[i+2]);
    }
    for (int i = 0; i < vertex_count*3; i+=3){
        printf("\n KOPIJA %lf, %lf, %lf\n", vertices_backup[i],vertices_backup[i+1],vertices_backup[i+2]);
    }*/
    object = fopen("model.obj", "r");
    

    while (fgets (line, sizeof(line), object)) {
        if (line[0] == 'v') {
            sscanf(line, "v %lf %lf %lf", &vertices[(br_vertex) * 3], &vertices[(br_vertex) * 3 + 1], &vertices[(br_vertex) * 3 + 2]);
            br_vertex++;
        } else if (line[0] == 'f'){
            sscanf(line, "f %d %d %d", &polygons[(br_poly) * 3], &polygons[(br_poly) * 3 + 1], &polygons[(br_poly) * 3 + 2]);
            br_poly++;
        } else if (line[0] == 't'){
            sscanf(line, "t %d %d %d", &krivulja[(br_krivulja) * 3], &krivulja[(br_krivulja) * 3 + 1], &krivulja[(br_krivulja) * 3 + 2]);
            br_krivulja++;
        }
    }
    br_tocaka = br_krivulja;
    //printf("Broj tocaka: %d \n", br_tocaka);

    fclose(object);

    //printf("Krivulja");
    //for (int i = 0; i < 12*3; i+=3){
        //printf("%d, %d, %d \n", krivulja[i], krivulja[i+1], krivulja[i+2]);
        //printf("\n");
    //}
    double xmin = 100000.0;
    double xmax = -100000.0;   
    double ymin = 100000.0;
    double ymax = -100000.0; 
    double zmin = 100000.0;
    double zmax = -100000.0; 

    for (int i = 0; i < vertex_count*3; i += 3){  
        if (vertices[i] < xmin){
            xmin = vertices[i];
        }
        if (vertices[i+1] < ymin){
            ymin = vertices[i+1];
        }
        if (vertices[i+2] < zmin){
            zmin = vertices[i+2];
        }
        if (vertices[i] > xmax){
            xmax = vertices[i];
        }
        if (vertices[i+1] > ymax){
            ymax = vertices[i+1];
        } 
        if (vertices[i+2] > zmax){
            zmax = vertices[i+2];
        }
        
    }



    double xmiddle = 0;
    double ymiddle = 0;
    double zmiddle = 0;

    for (int i = 0; i < vertex_count*3; i+=3){
        xmiddle += vertices[i];
        ymiddle += vertices[i+1];
        zmiddle += vertices[i+2];
    }

    xmiddle /= vertex_count;
    ymiddle /= vertex_count;
    zmiddle /= vertex_count;

    sredx = xmiddle;
    sredy = ymiddle;
    sredz = zmiddle;

    double diff = fmaxf(fmaxf(xmax - xmin, ymax - ymin), zmax - zmin);
    double scale = 2.0 / diff;

    for (int i = 0; i < vertex_count*3; i+=3){
        vertices[i] = (vertices[i] - xmiddle)*scale;
        vertices[i+1] = (vertices[i+1] - ymiddle)*scale;
        vertices[i+2] = (vertices[i+2] - zmiddle)*scale;
    }
    memcpy(vertices_backup, vertices, vertex_count * 3 * sizeof(double));
    read1 = 1;
    //calculateVertices();
    b_spline();
    
}

void reset(){
    memcpy(vertices,vertices_backup, vertex_count * 3 * sizeof(double));
    memcpy(b_spline_arr,b_backup, br_tocaka * 3 * sizeof(double));
}

void reset_b(){

    memcpy(b_spline_arr,b_backup, br_tocaka * 3 * sizeof(double));
}

void MatrixxMatrix(double arr1[4][4], double arr2[4][4], double result[4][4]){
    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            for (int k = 0; k < 4; k++){
                result[i][j] += arr1[i][k] * arr2[k][j];
            }
        }
    }
    for (int i = 0; i < 4; i++){
        printf("\n");
        for (int j = 0; j < 4; j++){
            //printf("%lf ", result[i][j]);
        }
    }
    //printf("\n");

}

void VectorxMatrix(double vec2[4], double mat1[4][4], double result[4]){
    for (int i = 0; i < 4; i++){
        result[i] = 0;
        for (int j = 0; j < 4; j++){
            result[i] += mat1[j][i] * vec2[j];

        }
    }
    /*printf("\n");
    for (int i = 0; i < 4; i++){
        printf("%lf ", result[i]);
    }
    */
}


void calculateVertices(){
    double G[4] = {glex,gley,glez,1};
    double A[4][4] = {{1.0,0.0,0.0,0.0},{0.0,1.0,0.0,0.0},{0.0,0.0,1.0,0.0},{0.0,0.0,0.0,1.0}};
    double T1[4][4] = {{1.0,0.0,0.0,0.0},{0.0,1.0,0.0,0.0},{0.0,0.0,1.0,0.0},{-okox, -okoy, -okoz, 1}};
    double T[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    double G1[4] = {0.0,0.0,0.0,0.0};
    //printf("\n");
    MatrixxMatrix(A,T1,T);
    VectorxMatrix(G, T1, G1);
    double xg1,yg1,zg1;
    xg1 = G1[0];
    yg1 = G1[1];
    zg1 = G1[2];

    double sin_alpha, cos_alpha;
    double naz = sqrt(xg1*xg1 + yg1*yg1);
    if (naz == 0) {
        sin_alpha = 0;
        cos_alpha = 1;
    } else {
        sin_alpha = yg1 / naz;
        cos_alpha = xg1 / naz;
    }   
    //printf("\n%lf, %lf\n", sin_alpha, cos_alpha);
    double T2[4][4] = {{cos_alpha, -sin_alpha,0,0},{sin_alpha, cos_alpha, 0,0},{0,0,1,0},{0,0,0,1}};
    double G2[4] = {0,0,0,0};
    VectorxMatrix(G1, T2, G2);

    double xg2,yg2,zg2;
    xg2 = G2[0];
    yg2 = G2[1];
    zg2 = G2[2];



    double sin_beta, cos_beta;
    double naz1 = sqrt(xg2*xg2 + zg2*zg2);
    if (naz1 == 0) {
        sin_beta = 0;
        cos_beta = 1;
    } else {
        sin_beta = xg2 / naz1;
        cos_beta = zg2 / naz1;
    }  
    //printf("\n%lf, %lf\n", sin_beta, cos_beta);

    double T3[4][4] = {{cos_beta, 0.0, sin_beta, 0.0},{0.0,1.0,0.0,0.0},{-sin_beta,0.0,cos_beta,0.0},{0.0,0.0,0.0,1.0}};
    double G3[4] = {0.0,0.0,0.0,0.0};
    VectorxMatrix(G2,T3,G3);

    double T4[4][4] = {{0.0,-1.0,0.0,0.0},{1.0,0.0,0.0,0.0},{0.0,0.0,1.0,0.0},{0.0,0.0,0.0,1.0}};
    double T5[4][4] = {{-1.0,0.0,0.0,0.0},{0.0,1.0,0.0,0.0},{0.0,0.0,1.0,0.0},{0.0,0.0,0.0,1.0}};
    double G4[4] = {0.0,0.0,0.0,0.0};
    double G5[4] = {0.0,0.0,0.0,0.0};
    VectorxMatrix(G3,T4,G4);
    VectorxMatrix(G4,T5,G5);

    double T23[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    MatrixxMatrix(T2,T3, T23);

    double T24[4][4]= {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    MatrixxMatrix(T23,T4,T24);

    double T25[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    MatrixxMatrix(T24,T5,T25);
    
    double T15[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    MatrixxMatrix(T,T25,T15);


    double TP[4][4] = {{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0},{0.0,0.0,0.0,0.0}};
    
    H = sqrt((okox - glex)*(okox - glex) + (okoy - gley)*(okoy - gley) + (okoz - glez)*(okoz - glez));  

    double P[4][4] = {{1.0,0.0,0.0,0.0},{0.0,1.0,0.0,0.0},{0.0,0.0,0.0,1/H},{0.0,0.0,0.0,0.0}};

    MatrixxMatrix(T15,P,TP);
    //for (int i = 0; i < vertex_count*3; i+=3){
        //printf("%lf, %lf, %lf\n", vertices[i], vertices[i+1], vertices[i+2]);
    //}

    for (int i = 0; i < 4; i++){
        for (int j = 0; j < 4; j++){
            TPkon[i][j] = TP[i][j];
            //printf("%lf ", TPkon[i][j]);
        }
        //printf("\n");
    }
}

void multiplyVertices(){
    for (int i = 0; i < vertex_count*3; i+=3){
        double vektor[4] = {vertices[i], vertices[i+1], vertices[i+2], 1};
        //printf("\n%lf, %lf, %lf\n", vektor[0], vektor[1], vektor[2]);
        double rez[4] = {0.0,0.0,0.0,0.0};
        VectorxMatrix(vektor, TPkon, rez);
        //printf("\nTocke : %lf, %lf, %lf\n", rez[0], rez[1], rez[2]);
        for (int j = 0; j < 4; j++){
            rez[j] = rez[j]/H;
        }
        vertices[i] = rez[0];
        vertices[i+1] = rez[1];
        vertices[i+2] = rez[2];
        printf("\n%lf, %lf, %lf\n", vertices[i], vertices[i+1], vertices[i+2]);
    }
}

int brojac = 0;
void myDisplay(){
    glLoadIdentity();
    //printf("%lf, %lf, %lf", okox, okoy, okoz);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(20, 20, -100.0);
    //okox = 1; okoy = 1, okoz = 1;
    gluLookAt(okox, okoy, okoz, 0, 0, 0, 0.0, 1.0, 0.0);
    glPointSize(4);
    glColor3f(1.0f, 0.0f, 1.0f);

    glBegin(GL_LINES);
    for (int i = 0; i < (br_tocaka-3)*100; i++){
        glVertex3f(b_spline_arr[i*3], b_spline_arr[i*3+1], b_spline_arr[i*3+2]);
        glVertex3f(b_spline_arr[i*3] + b_spline_vector[i*3]/100, b_spline_arr[i*3+1] +  b_spline_vector[i*3+1]/100, b_spline_arr[i*3+2] + b_spline_vector[i*3+2]/100);
    }
    glEnd();

    glTranslatef(b_spline_arr[brojac*3],b_spline_arr[brojac*3+1],b_spline_arr[brojac*3+2]);



    rotate_vect[0] = b_spline_vector[brojac*3+1]*0. - 0.*b_spline_vector[brojac*3+2];
    rotate_vect[1] = -(b_spline_vector[brojac*3]*-1. - b_spline_vector[brojac*3+2]*0.);
    rotate_vect[2] = -1*b_spline_vector[brojac*3+1] - 0.*b_spline_vector[brojac*3];

    phi = acos((b_spline_vector[brojac*3]*0+b_spline_vector[brojac*3+1]*0+b_spline_vector[brojac*3+2]*1)/(sqrt(b_spline_vector[brojac*3]*b_spline_vector[brojac*3] + b_spline_vector[brojac*3+1]*b_spline_vector[brojac*3+1] + b_spline_vector[brojac*3+2]*b_spline_vector[brojac*3+2])))*180/3.1415926;

    
	//myObject();   
	glFlush();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslatef(-sredx, -sredy, -sredz);
    glRotatef(phi,rotate_vect[0],rotate_vect[1],rotate_vect[2]);
    glBegin(GL_LINES);
            
    for (int i = 0; i < poly_count*3; i+=3){
        int p1 = polygons[i] - 1;
        int p2 = polygons[i + 1] - 1;
        int p3 = polygons[i + 2] - 1;

        glVertex3f(vertices[p1 * 3], vertices[p1 * 3 + 1], vertices[p1 * 3 + 2]);
        glVertex3f(vertices[p2 * 3], vertices[p2 * 3 + 1], vertices[p2 * 3 + 2]);

        glVertex3f(vertices[p2 * 3], vertices[p2 * 3 + 1], vertices[p2 * 3 + 2]);
        glVertex3f(vertices[p3 * 3], vertices[p3 * 3 + 1], vertices[p3 * 3 + 2]);

        glVertex3f(vertices[p3 * 3], vertices[p3 * 3 + 1], vertices[p3 * 3 + 2]);
        glVertex3f(vertices[p1 * 3], vertices[p1 * 3 + 1], vertices[p1 * 3 + 2]);
    }
    glEnd();
    brojac++;
    if (brojac == 100*br_tocaka) brojac = 0;
    

	glFlush();
}



void myReshape(int w, int h)
{
    width = w; height = h;

    glViewport(0, 0, width, height);	
	glMatrixMode(GL_PROJECTION);                
    glLoadIdentity();                          
 
   
    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,200.0f);
 
    glMatrixMode(GL_MODELVIEW);                 
    glLoadIdentity();                           

	glLoadIdentity();							
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );		
	glClear( GL_COLOR_BUFFER_BIT );				
	glPointSize(1.0);							
	glColor3f(0.0f, 0.0f, 0.0f);
    //okox = 5;  okoy = 0; okoz = 0;
    //gluLookAt(okox, okoy, okoz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch (theKey)
	{
    case 'r':
        brojac = 0;
        break;
    
    case 'p':
    	calculateVertices();
        multiplyVertices();
        myDisplay();
        reset();
		break;
	case 'f':
        calculateVertices();
        multiplyVertices();
        b_spline();
        b_draw();
        animate();
        move();   
		break;
	case 'w':
        okoy += 0.3;
		break;
	case 's':
        okoy -= 0.3;
		break;
    case 'a':
        okox += 0.3;		
		break;
    case 'd':
        okox -= 0.3;
		break;
    case 'e':
        okoz += 0.3;
		break;
    case 'q':
        okoz -= 0.3;
		break;
	case 'i':
        gley += 0.3;
		break;
	case 'k':
        gley -= 0.3;
		break;
    case 'j':
        glex += 0.3;
		break;
    case 'l':
        glex -= 0.3;
		break;
    case 'o':
        glez += 0.3;
		break;
    case 'u':
        glez -= 0.3;
		break;
    case 'm':
        b_draw();
		break;
	}
    //calculateVertices();
    //multiplyVertices();
    myReshape(width, height);
    glutPostRedisplay();
}

void myObject (){
    if (calculated == 1){
        function();
    }
    glColor3f(0.0f, 0.0f, 0.0f);
    //glPushMatrix();
    //glRotatef((float)phi, rotate_vect[0], rotate_vect[1], rotate_vect[2]);
    //glRotatef((float)15, 1, 0, 1);
    //printf("tu sam");
    for (int i = 0; i < poly_count*3; i += 3) {
        double xTranslate = width/2;
        double yTranslate = height/2;
        glBegin(GL_LINES);
        {
        int p1 = polygons[i] - 1;
        int p2 = polygons[i + 1] - 1;
        int p3 = polygons[i + 2] - 1;
        //printf("%f", vertices[p1 * 3]*100.0 + xTranslate);
        glVertex2i(vertices[p1 * 3]*100.0 + xTranslate, vertices[p1 * 3 + 1]*100.0 + yTranslate);
        glVertex2i(vertices[p2 * 3]*100.0 + xTranslate, vertices[p2 * 3 + 1]*100.0 + yTranslate);

        glVertex2i(vertices[p2 * 3]*100.0 + xTranslate, vertices[p2 * 3 + 1]*100.0 + yTranslate);
        glVertex2i(vertices[p3 * 3]*100.0 + xTranslate, vertices[p3 * 3 + 1]*100.0 + yTranslate);

        glVertex2i(vertices[p3 * 3]*100.0 + xTranslate, vertices[p3 * 3 + 1]*100.0 + yTranslate);
        glVertex2i(vertices[p1 * 3]*100.0 + xTranslate, vertices[p1 * 3 + 1]*100.0 + yTranslate);
        }
    }
    glEnd();
    glPopMatrix();
    //memcpy(vertices, vertices_backup, vertex_count * 3 * sizeof(double));
    
}
void b_draw(){  
    //drawn = 0;
    //glColor3f(1.0f, 0.0f, 0.0f);
    double xTranslate = width/2;
    double yTranslate = height/2;

    /*for (int i = 0; i < n - 1; i += 1) {
        glBegin(GL_LINES);
        {
        glVertex2i(kt[i*3]*100.0 + xTranslate, kt[i*3+1]*100.0 + yTranslate);
        glVertex2i(kt[(i+1)*3]*100.0 + xTranslate, kt[(i+1)*3+1]*100.0 + yTranslate);
        }
    }*/
    //glEnd();
    glPointSize(4);
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < 90; i++){
        glVertex3f(b_spline_arr[i*3], b_spline_arr[i*3+1], b_spline_arr[i*3+2]);
        glVertex3f(b_spline_arr[i*3] + b_spline_vector[i*3], b_spline_arr[i*3+1]+  b_spline_vector[i*3+1], b_spline_arr[i*3+2] + b_spline_vector[i*3+2]);
    }
    glEnd();
    
}

int currentTime = 0; int previousTime = 0;

void myIdle() {
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	//printf("%d\n", timeInterval);
	if(timeInterval > 20) {
		myDisplay();
		previousTime = currentTime;
	}
}