#include "iostream"
#include "gencrawl.h"
using namespace std;
#include <atlimage.h>
 
Cube cube[30];
std::vector<Gene> individual;

std::vector<std::vector<int>> population;
std::vector<std::vector<int>> selectedpop;

std::vector<double> popdistance;


double constantk = 3000;
double constantg = 9.81;
double constantr = 3000;
double constantf = 2.5;
double constants = 0.99;
double timestep = 0.001;
//double T = 0;
int breath_flag = 0;
int mass_num = MASSNUM;
int spring_num = SPRINGNUM;
double potentialk = 0;
double potentialm = 0;
double potentialg = 0;
double potentialE = 0;
double kineticE = 0;
double totalE = 0;

std::ofstream bestgeneF("bestgene.txt");
std::ofstream dotplotF("dotplot.txt");
std::ofstream bestdistF("bestdist.txt");

GLfloat worldRotation[16] = { 1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,1 };


template <typename T>
vector<size_t> sort_indexes(const vector<T>& v) {

    // initialize original index locations
    vector<size_t> idx(v.size());
    iota(idx.begin(), idx.end(), 0);

    // sort indexes based on comparing values in v
    sort(idx.begin(), idx.end(),
        [&v](size_t i1, size_t i2) {return v[i1] < v[i2]; });

    return idx;
}

double normDistance(Mass m1,Mass m2) {
    double dx = m2.p[0] - m1.p[0];
    double dy = m2.p[1] - m1.p[1];
    double dz = m2.p[2] - m1.p[2];
    double dxy = std::sqrt(std::pow(dx, 2) + std::pow(dy, 2));
    double dxyz = std::sqrt(std::pow(dxy, 2) + std::pow(dz, 2));
    return dxyz;
}
void init_mass(double mass, double length, double X, double Y, double Z, int indexc) {
    Mass mass_base[MASSNUM];
    mass_base[0] = { mass,{X,Y,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[1] = { mass,{X + length,Y,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[2] = { mass,{X + 2 * length,Y,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[3] = { mass,{X,Y + length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[4] = { mass,{X + length,Y + length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[5] = { mass,{X + 2 * length,Y + length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[6] = { mass,{X,Y + 2 * length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[7] = { mass,{X + length,Y + 2 * length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[8] = { mass,{X + 2 * length,Y + 2 * length,Z},{0,0,0},{0,0,0},{0,0,0} };

    mass_base[9] = { mass,{X,Y,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[10] = { mass,{X + length,Y,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[11] = { mass,{X + 2 * length,Y,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[12] = { mass,{X,Y + length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[13] = { mass,{X + length,Y + length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[14] = { mass,{X + 2 * length,Y + length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[15] = { mass,{X,Y + 2 * length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[16] = { mass,{X + length,Y + 2 * length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[17] = { mass,{X + 2 * length,Y + 2 * length,Z + length},{0,0,0},{0,0,0},{0,0,0} };

    mass_base[18] = { mass,{X,Y,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[19] = { mass,{X + length,Y,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[20] = { mass,{X + 2 * length,Y,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[21] = { mass,{X,Y + length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[22] = { mass,{X + length,Y + length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[23] = { mass,{X + 2 * length,Y + length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[24] = { mass,{X,Y + 2 * length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[25] = { mass,{X + length,Y + 2 * length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[26] = { mass,{X + 2 * length,Y + 2 * length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };

    //srand((unsigned)time(NULL));
    vector<int> mass_element;
    for (int j=0; j < MASSNUM; j++) {
        double mass_flag = (double)(rand() / (double)RAND_MAX);
        if (mass_flag > 0.7) {
            mass_element.push_back(j);
        }
    }
    mass_num = int(mass_element.size());
    if (mass_num == 0) {
        mass_element.push_back(0);
        mass_element.push_back(2);
        mass_element.push_back(7);
        mass_element.push_back(9);
        mass_element.push_back(22);
    }
    cube[indexc].cube_mass.clear();
    cube[indexc].cube_spring.clear();
    mass_num = int(mass_element.size());
    for (int k = 0; k < mass_num; k++) {
        cube[indexc].cube_mass.push_back(mass_base[mass_element[k]]);
    }
    population.push_back(mass_element);
}

void init_spring(double constantk, int indexc) {
    Spring spring_temp;
    mass_num = int(cube[indexc].cube_mass.size());
    for (int i = 0; i < mass_num; i++) {
        for (int j = i + 1; j < mass_num; j++) {
            spring_temp = { 6000, normDistance(cube[indexc].cube_mass[i],cube[indexc].cube_mass[j]), normDistance(cube[indexc].cube_mass[i],cube[indexc].cube_mass[j]),i, j };
            cube[indexc].cube_spring.push_back(spring_temp);
        }
    }
}
// After this function, there will be 15 cubes generated with different shape
void init_cube(int cubenum) {
    
    float originalx = 0.0;
	float originaly = 0.0f;
	float originalz = 0.2;;
    
    for (int c = 0; c < cubenum; c++) {
        init_mass(0.1, 0.1, originalx, originaly, originalz, c);
        init_spring(constantk, c);
    }
}

void update_cube(int cubenum) {
    double originalx = 0.0;
    double originaly = 0.0f;
    double originalz = 0.2;;
    vector<int> mass_element;
    float mass = 0.1;
    float length = 0.1;
    float X = originalx;
    float Y = originaly;
    float Z = originalz;
    Mass mass_base[MASSNUM];
    mass_base[0] = { mass,{X,Y,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[1] = { mass,{X + length,Y,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[2] = { mass,{X + 2 * length,Y,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[3] = { mass,{X,Y + length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[4] = { mass,{X + length,Y + length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[5] = { mass,{X + 2 * length,Y + length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[6] = { mass,{X,Y + 2 * length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[7] = { mass,{X + length,Y + 2 * length,Z},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[8] = { mass,{X + 2 * length,Y + 2 * length,Z},{0,0,0},{0,0,0},{0,0,0} };

    mass_base[9] = { mass,{X,Y,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[10] = { mass,{X + length,Y,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[11] = { mass,{X + 2 * length,Y,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[12] = { mass,{X,Y + length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[13] = { mass,{X + length,Y + length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[14] = { mass,{X + 2 * length,Y + length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[15] = { mass,{X,Y + 2 * length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[16] = { mass,{X + length,Y + 2 * length,Z + length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[17] = { mass,{X + 2 * length,Y + 2 * length,Z + length},{0,0,0},{0,0,0},{0,0,0} };

    mass_base[18] = { mass,{X,Y,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[19] = { mass,{X + length,Y,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[20] = { mass,{X + 2 * length,Y,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[21] = { mass,{X,Y + length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[22] = { mass,{X + length,Y + length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[23] = { mass,{X + 2 * length,Y + length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[24] = { mass,{X,Y + 2 * length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[25] = { mass,{X + length,Y + 2 * length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    mass_base[26] = { mass,{X + 2 * length,Y + 2 * length,Z + 2 * length},{0,0,0},{0,0,0},{0,0,0} };
    
    for (int i = 0; i < cubenum; i++) {
        cube[i].cube_mass.clear();
        cube[i].cube_spring.clear();
        mass_element = population[i];
        mass_num = int(mass_element.size());
        for (int k = 0; k < mass_num; k++) {
            cube[i].cube_mass.push_back(mass_base[mass_element[k]]);
            bestgeneF << mass_element[k] << " ";
        }
        init_spring(constantk, i);
        bestgeneF << endl;
    }
    for (int c = cubenum; c < 2*cubenum; c++) {
        init_mass(0.1, 0.1, originalx, originaly, originalz, c);
        init_spring(constantk, c);
    }

}


void drawSomething()
{
    glColor3f(1, 0, 0);

    GLUquadric* quad;
    quad = gluNewQuadric();
    glPushMatrix();
    glMultMatrixf(worldRotation);
    glTranslated(0, 0, 0);
    gluSphere(quad, 1.0 / 10, 10, 10);
    glPopMatrix();

    glPushMatrix();
    glMultMatrixf(worldRotation);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 1, 1);
    glEnd();
    glPopMatrix();

}

void forceCube(int cubenum) {
    T += timestep;
    //cout << T << endl;
    for (int c = 0; c < cubenum; c++) {
        mass_num = int(cube[c].cube_mass.size());
        spring_num = int(cube[c].cube_spring.size());
        potentialk = 0;
        potentialm = 0;
        potentialg = 0;
        potentialE = 0;
        kineticE = 0;
        totalE = 0;
        // Adding gravity to each mass
        for (int j = 0; j < mass_num; j++) {
            cube[c].cube_mass[j].f[0] = 0;
            cube[c].cube_mass[j].f[1] = cube[c].cube_mass[j].m* constantg;
            cube[c].cube_mass[j].f[2] = -cube[c].cube_mass[j].m * constantg ;
        }
        if (T >= 1.5){
            double geneb;
            double genec;
            for (int genum = 0; genum < spring_num; genum++) {
                int march_point = 1000;
                int ppp1 = cube[c].cube_spring[genum].m1;
                int ppp2 = cube[c].cube_spring[genum].m2;
                Mass massp1 = cube[c].cube_mass[ppp1];
                Mass massp2 = cube[c].cube_mass[ppp2];
                if (march_point > 0) {
                    if (massp1.p[2] < 0.001) {
                        march_point = ppp1;
                    }
                    if (massp2.p[2] < 0.001) {
                        march_point = ppp2;
                    }

                }
                if (ppp1 == march_point || ppp2 == march_point) {
                    geneb = 0.0225518;
                    genec = -4.17;
                    cube[c].cube_spring[genum].L0 = cube[c].cube_spring[genum].oriL + geneb / (2 * M_PI) * sin(10 * T + genec);
                }
                else {
                    geneb = 0.0116782;
                    genec = -3.86;
                    cube[c].cube_spring[genum].L0 = cube[c].cube_spring[genum].oriL + geneb / (2 * M_PI) * sin(10 * T + genec);
                }
                
            }
            /*for (int sn = 0; sn < SPRINGNUM; sn++) {
                    cube[c].cube_spring[sn].L0 = cube[c].cube_spring[sn].oriL + pow(-1, sn)* 0.2 * 0.005 * sin(10 * T);
                }*/
            
        }
        for (int i = 0; i < spring_num; i++) {
            int p1 = cube[c].cube_spring[i].m1;
            int p2 = cube[c].cube_spring[i].m2;
            //cout << "p1: " << p1 << endl;
            //cout << "p2: " << p2 << endl;
            Mass mass1 = cube[c].cube_mass[p1];
            Mass mass2 = cube[c].cube_mass[p2];
            double dxyz[3] = { mass2.p[0] - mass1.p[0],mass2.p[1] - mass1.p[1],mass2.p[2] - mass1.p[2] };
            double L = normDistance(mass1, mass2);
            double springforce = cube[c].cube_spring[i].k * (L - cube[c].cube_spring[i].L0);
        
            double forceDire[3] = { dxyz[0] / L,dxyz[1] / L,dxyz[2] / L };
            cube[c].cube_mass[p1].f[0] = cube[c].cube_mass[p1].f[0] + springforce * forceDire[0];
            cube[c].cube_mass[p1].f[1] = cube[c].cube_mass[p1].f[1] + springforce * forceDire[1];
            cube[c].cube_mass[p1].f[2] = cube[c].cube_mass[p1].f[2] + springforce * forceDire[2];
            cube[c].cube_mass[p2].f[0] = cube[c].cube_mass[p2].f[0] - springforce * forceDire[0];
            cube[c].cube_mass[p2].f[1] = cube[c].cube_mass[p2].f[1] - springforce * forceDire[1];
            cube[c].cube_mass[p2].f[2] = cube[c].cube_mass[p2].f[2] - springforce * forceDire[2];
            //potentialk = potentialk + cube[c].cube_spring[i].k * std::pow((L - cube[c].cube_spring[i].L0), 2) / 2;
        }
        
        for (int j = 0; j < mass_num; j++) {
            if (cube[c].cube_mass[j].p[2] < 0) {
                cube[c].cube_mass[j].f[2] = cube[c].cube_mass[j].f[2] + constantr * fabs(cube[c].cube_mass[j].p[2]);
                double hor_f = sqrt(pow(cube[c].cube_mass[j].f[0], 2) + pow(cube[c].cube_mass[j].f[1], 2));
                double ver_f = cube[c].cube_mass[j].f[2];
                if (hor_f < ver_f * constantf) {
                    cube[c].cube_mass[j].f[0] = 0;
                    cube[c].cube_mass[j].f[1] = 0;
                    cube[c].cube_mass[j].v[0] = 0;
                    cube[c].cube_mass[j].v[1] = 0;
                }
                else {
                    for (int h = 0; h < 2; h++) {
                        if (cube[c].cube_mass[j].f[h] > 0) {
                            cube[c].cube_mass[j].f[h] = cube[c].cube_mass[j].f[h] - ver_f * constantf * cube[c].cube_mass[j].f[h] / hor_f;
                            if (cube[c].cube_mass[j].f[h] < 0) {
                                cube[c].cube_mass[j].f[h] = 0;
                            }
                        }
                        else {
                            cube[c].cube_mass[j].f[h] = cube[c].cube_mass[j].f[h] + ver_f * constantf * fabs(cube[c].cube_mass[j].f[h]) / hor_f;
                            if (cube[c].cube_mass[j].f[h] > 0) {
                                cube[c].cube_mass[j].f[h] = 0;
                            }
                        }
                    }
                }
                //potentialg = potentialg + constantr * pow(cube[c].cube_mass[j].p[2], 2) / 2;
            }
            //potentialm = potentialm + cube[c].cube_mass[j].m * constantg * cube[c].cube_mass[j].p[2];
            //kineticE = kineticE + cube[c].cube_mass[j].m * (pow(cube[c].cube_mass[j].v[0], 2) + pow(cube[c].cube_mass[j].v[1], 2) + pow(cube[c].cube_mass[j].v[2], 2)) / 2;
            for (int a = 0; a < 3; a++) {
                cube[c].cube_mass[j].a[a] = cube[c].cube_mass[j].f[a] / cube[c].cube_mass[j].m;
                /*if (T < 0.0003) {
                    cout << "j: " << j << " a: " << a << cube[c].cube_mass[j].a[a] << endl;
                }*/
                //cout << "j: " << j << " a: " << a << cube[c].cube_mass[j].a[a] << endl;
            }
            for (int v = 0; v < 3; v++) {
                cube[c].cube_mass[j].v[v] = constants * (cube[c].cube_mass[j].v[v] + cube[c].cube_mass[j].a[v] * timestep);
                /*if (T < 0.0003) {
                    cout << "j: " << j << " v: " << v << cube[c].cube_mass[j].v[v] << endl;
                }*/
                //cout << "j: " << j << " v: " << v << cube[c].cube_mass[j].v[v] << endl;
            }
            for (int p = 0; p < 3; p++) {
                cube[c].cube_mass[j].p[p] = cube[c].cube_mass[j].p[p]+cube[c].cube_mass[j].v[p] * timestep;
                /*if (T < 0.0003) {
                    cout << "j: " << j << " p: " << p << cube[c].cube_mass[j].p[p] << endl;
                }*/
                //cout << "j: " << j << " p: " << p << cube[c].cube_mass[j].p[p] << endl;
            }

        }
    }
    
}
GLuint g_texture;
void drawground()
{
    /*
    glColor3f(0.06078, 0.96078, 0.86274);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glVertex3f(-100,  -100,0);
    glVertex3f(100, -100,0);
    glVertex3f(100, 100,0);
    glVertex3f(-100, 100,0);
    glEnd();
    */

    glPushMatrix();
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    //glColor3f(0.06078, 0.96078, 0.86274);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0.0, 0.0);  glVertex3f(-1, -0.001, -1);
    glTexCoord2f(0.0, 5);  glVertex3f(1, -0.001, -1);
    glTexCoord2f(5,5);  glVertex3f(1, -0.001, 1);
    glTexCoord2f(5, 0.0);  glVertex3f(-1, -0.001, 1);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    //glDisable(GL_TEXTURE_2D);
    //for (int i = 0; i < 10; i++) {
    //    for (int j = -9; j < 10; j++) {
    //        glColor3f(0, 1, 0);
    //        glPushMatrix();
    //        glMultMatrixf(worldRotation);
    //        glBegin(GL_LINES);
    //        glLineWidth(10);
    //        glVertex3f(-0.5 * i / 10, 0.02, 0.5 * j / 10);
    //        glEnd();
    //        glPopMatrix();
    //    }
    //}
}


void loadTexture(void)
{
    BITMAP bm;
    CImage img;
    HRESULT hr = img.Load("./ChessFloor.jpg");
    HBITMAP hbmp = img;
    if (!GetObject(hbmp, sizeof(bm), &bm))
        return ;
    glGenTextures(1, &g_texture);
    glBindTexture(GL_TEXTURE_2D, g_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bm.bmWidth, bm.bmHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bm.bmBits);
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, (const GLvoid*)data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}

static int g_count = 1;

void draw_single_spring(Spring cube_spring, int c) {
    g_count++;
    if (g_count >= 50)
    {
        g_count = 0;
    }
    if (c < 15) {
        int m1 = cube_spring.m1;
        int m2 = cube_spring.m2;
        
        glPushMatrix();
        glMultMatrixf(worldRotation);
        glBegin(GL_LINES);
        glColor3f(0.02 * g_count, 0.0, 1.0 - 0.02 * g_count);
        
        glVertex3f(cube[c].cube_mass[m1].p[0], cube[c].cube_mass[m1].p[1], cube[c].cube_mass[m1].p[2]);
        glColor3f(0.02 * g_count, 1.0, 1.0 - 0.02 * g_count);
        glVertex3f(cube[c].cube_mass[m2].p[0], cube[c].cube_mass[m2].p[1], cube[c].cube_mass[m2].p[2]);
        glEnd();
        glPopMatrix();
    }
    else {
        int m1 = cube_spring.m1;
        int m2 = cube_spring.m2;
        glColor3f(0.02 * g_count, 0.0, 1.0 - 0.02 * g_count);
        glPushMatrix();
        glMultMatrixf(worldRotation);
        glBegin(GL_LINES);
        glVertex3f(cube[c].cube_mass[m1].p[0], cube[c].cube_mass[m1].p[1], cube[c].cube_mass[m1].p[2]);
        glVertex3f(cube[c].cube_mass[m2].p[0], cube[c].cube_mass[m2].p[1], cube[c].cube_mass[m2].p[2]);
        glEnd();
        glPopMatrix();
    }
    
}


void drawCube( ) {
    if (T == 0) {
        std::cout << "population: " << population.size() << endl;
        best_gene_init();
        update_cube(population.size()); 
    }
    forceCube(30);
    if (T > 10) {
        cubeDistance(population.size());
        T = 0;
    }
    //drawground();
    /*for(int i=0;i<8;i++){
        std::cout << cube_mass[i].p[1] << std::endl;*/

    for (int c = 0; c < CUBENUM; c++) {
       //if (c == 8) {
            glColor3f(0.8, 0.8, 0.8);
            mass_num = int(cube[c].cube_mass.size());
            GLUquadric* quad;
            for (int i = 0; i < mass_num; i++) {
                quad = gluNewQuadric();
                glPushMatrix();
                glMultMatrixf(worldRotation);
                glTranslatef(cube[c].cube_mass[i].p[0], cube[c].cube_mass[i].p[1], cube[c].cube_mass[i].p[2]);
                gluSphere(quad, 1.0 / 200, 10, 10);
                glPopMatrix();
            }
            spring_num = int(cube[c].cube_spring.size());
            for (int i = 0; i < spring_num; i++) {
                draw_single_spring(cube[c].cube_spring[i], c);
            }
        //}
        /*
        if (c < 15) {
            glColor3f(1, 0, 0);
            mass_num = int(cube[c].cube_mass.size());
            GLUquadric* quad;
            for (int i = 0; i < mass_num; i++) {
                quad = gluNewQuadric();
                glPushMatrix();
                glMultMatrixf(worldRotation);
                glTranslatef(cube[c].cube_mass[i].p[0], cube[c].cube_mass[i].p[1], cube[c].cube_mass[i].p[2]);
                gluSphere(quad, 1.0 / 200, 10, 10);
                glPopMatrix();
            }
            spring_num = int(cube[c].cube_spring.size());
            for (int i = 0; i < spring_num; i++) {
                draw_single_spring(cube[c].cube_spring[i], c);
            }
        }
        else {
            glColor3f(0, 0, 1);
            mass_num = int(cube[c].cube_mass.size());
            GLUquadric* quad;
            for (int i = 0; i < mass_num; i++) {
                quad = gluNewQuadric();
                glPushMatrix();
                glMultMatrixf(worldRotation);
                glTranslatef(cube[c].cube_mass[i].p[0], cube[c].cube_mass[i].p[1], cube[c].cube_mass[i].p[2]);
                gluSphere(quad, 1.0 / 200, 10, 10);
                glPopMatrix();
            }
            spring_num = int(cube[c].cube_spring.size());
            for (int i = 0; i < spring_num; i++) {
                draw_single_spring(cube[c].cube_spring[i], c);
            } 
        }*/
    }
    
}

//void init_population() {
//    srand((unsigned)time(NULL));
//    for (int i = 0; i < POPNUM; i++) {
//        std::vector<Gene> tempindividual;
//        for (int j = 0; j < CHROMENUM; j++) {
//            double geneb = (-2 * M_PI + (double)(rand() / (double)RAND_MAX) * 4 * M_PI)/500;
//            double genec = -2 * M_PI + (double)(rand() / (double)RAND_MAX) * 4 * M_PI;
//            Gene chrome{ geneb, genec };
//            tempindividual.push_back(chrome);
//        }
//        population.push_back(tempindividual);
//    }
//}

void cubeDistance(int popsize) {
    popdistance.clear();
    popdistance.shrink_to_fit();
    double indistance = 0;
    double maxdistance = 0;
    for (int i = 0; i < popsize; i++) {
        indistance = sqrt(pow(cube[i].cube_mass[0].p[0], 2) + pow(cube[i].cube_mass[0].p[1], 2));
        if (indistance > 100) {
            indistance = 0;
        }
        if (indistance < 0) {
            indistance = 0;
        }
        //cout << "indistance: "<<indistance << endl;
        maxdistance = std::fmax(maxdistance, indistance);
        //cout << "Maxdistance: " << maxdistance << endl;
        popdistance.push_back(indistance);
    }
    bestdistF << maxdistance << endl;
    for (int disnum = 0; disnum < popdistance.size(); disnum++) {
        dotplotF << popdistance[disnum]<< " ";
    }
    dotplotF << endl;

    //sort(popdistance.begin(), popdistance.end(), greater<float>());
    std::cout << "Maxdistance: "<< maxdistance << endl;
    std::vector<size_t> index = sort_indexes(popdistance);
    selectedpop.clear();
    selectedpop.shrink_to_fit();
    for (int i = popsize-1; i >= 0; i--) {
        //cout << "index: "<<index[i] << endl;
        selectedpop.push_back(population[index[i]]);
    }
    population.clear();
    population.shrink_to_fit();
    for (int i = 0; i < POPNUM; i++) {
        population.push_back(selectedpop[i]);
    }
}


void best_gene_init() {
    population.clear();
    vector<int> mass_element;
    mass_element = { 10, 13, 14, 15, 18, 19, 20, 22, 23 };
    population.push_back(mass_element);
    mass_element = { 0, 1, 9, 15, 16, 17, 19, 24, 25, 26 };
    population.push_back(mass_element);
    mass_element = { 0, 1, 9, 13, 17, 18, 19, 22, 24, 25 };
    population.push_back(mass_element);
    mass_element = { 0, 1, 2, 4, 5, 6, 11, 12, 15, 16, 19, 21, 24, 26 };
    population.push_back(mass_element);
    mass_element = { 0, 12, 16, 18, 19, 21, 24, 25, 26 };
    population.push_back(mass_element);
    mass_element = { 7, 8, 13, 17, 21, 23 };
    population.push_back(mass_element);
    mass_element = { 0, 3, 4, 7, 9, 10, 13, 14, 15, 16 };
    population.push_back(mass_element);
    mass_element = { 3, 6, 7, 8, 9, 14, 15, 18, 19, 22, 23 };
    population.push_back(mass_element);
    mass_element = { 1, 2, 3, 13, 14, 15, 20, 21, 22 };
    population.push_back(mass_element);
    mass_element = { 2, 5, 7, 11, 16, 18, 24, 25, 26 };
    population.push_back(mass_element);
    mass_element = { 5, 6, 7, 10, 11, 12, 15, 19, 22, 23, 25, 26 };
    population.push_back(mass_element);
    mass_element = { 6, 8, 17, 19, 23, 25 };
    population.push_back(mass_element);
    mass_element = { 0, 3, 6, 11, 15, 16, 17, 24 };
    population.push_back(mass_element);
    mass_element = { 2, 6, 15, 16, 19, 22 };
    population.push_back(mass_element);
    mass_element = { 2, 3, 4, 11, 12, 13, 23, 24, 25 };
    population.push_back(mass_element);
}

void Print(const char* format, ...)
{
    char    buf[LEN];
    char* ch = buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args, format);
    vsnprintf(buf, LEN, format, args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    const double len = 1.5;  //  Length of axes
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Eye position
    double Ex = -2 * dim * Sin(th) * Cos(ph)+0.5;
    double Ey = +2 * dim * Sin(ph)+0.5;
    double Ez = +2 * dim * Cos(th) * Cos(ph);
    gluLookAt(Ex, Ey, Ez, 0, 0, 0, 0, Cos(ph), 0);


    //drawSomething();
    drawground();
    drawCube();

    //  Draw axes
    glColor3f(1, 1, 1);
    if (axes)
    {
        glBegin(GL_LINES);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(len, 0.0, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, len, 0.0);
        glVertex3d(0.0, 0.0, 0.0);
        glVertex3d(0.0, 0.0, len);
        glEnd();
        //  Label axes
        glRasterPos3d(len, 0.0, 0.0);
        Print("X");
        glRasterPos3d(0.0, len, 0.0);
        Print("Z");
        glRasterPos3d(0.0, 0.0, len);
        Print("Y");
    }
    //  Render the scene
    glFlush();
    //  Make the rendered scene visible
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int x, int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
    {
        if (ph + 5 < 90)
        {
            ph += 5;
        }
    }
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
    {
        if (ph - 5 > 0)
        {
            ph -= 5;
        }
    }
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  Set projection
 */
void Project(double fov, double asp, double dim)
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    if (fov)
        gluPerspective(fov, asp, dim / 16, 16 * dim);
    //  Orthogonal transformation
    else
        glOrtho(-asp * dim, asp * dim, -dim, +dim, -dim, +dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch, int x, int y)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
    //  Toggle axes
    else if (ch == 'a' || ch == 'A')
        axes = 1 - axes;
    //  Change field of view angle
    else if (ch == '-' && ch > 1)
        fov++;
    else if (ch == '=' && ch < 179)
        fov--;
    //  PageUp key - increase dim
    else if (ch == GLUT_KEY_PAGE_DOWN) {
        dim += 0.1;
    }
    //  PageDown key - decrease dim
    else if (ch == GLUT_KEY_PAGE_UP && dim > 1) {
        dim -= 0.1;
    }
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Reproject
    Project(fov, asp, dim);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width, int height)
{
    //  Ratio of the width to the height of the window
    asp = (height > 0) ? (double)width / height : 1;
    //  Set the viewport to the entire window
    glViewport(0, 0, width, height);
    //  Set projection
    Project(fov, asp, dim);
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
    glutPostRedisplay();
}

int main(int argc, char* argv[])
{
    init_cube(15);
    //init_population();
    //best_gene_init();
    // Initialize GLUT and process user parameters
    glutInit(&argc, argv);
    // double buffered, true color 600*600
    glutInitWindowSize(1000, 800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    // create the window
    glutCreateWindow("sg4017");
    //  Tell GLUT to call "idle" when there is nothing else to do
    glutIdleFunc(idle);
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Pass control to GLUT so it can interact with the user

    loadTexture();
    glutMainLoop();
    return 0;
};
