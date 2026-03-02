//All #includes
#include <GL/glew.h>
#include <GL/glut.h>
#include <Shader.h>
#include <Vector.h>
#include <Matrix.h>
#include <Mesh.h>
#include <Texture.h>
#include <SphericalCameraManipulator.h>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include <C:/Program Files/Msys/mingw64/include/stb/stb_image.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "C:/Program Files/Msys/mingw64/include/stb/stb_truetype.h"
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <fstream>
#include <vector>
#include <glm/glm.hpp>

//All #defines
#define GRID_SIZE 10
//BMP file definitions
#define CRATE_BMP "C:/Users/tsour/CLionProjects/TankGame/models/crate.bmp"
#define BALL_BMP "C:/Users/tsour/CLionProjects/TankGame/models/ball.bmp"
#define COIN_BMP "C:/Users/tsour/CLionProjects/TankGame/models/coin.bmp"
#define HUMVEE_BMP "C:/Users/tsour/CLionProjects/TankGame/models/humvee.bmp"
//OBJ file definitions
#define BALL_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/ball.obj"
#define CUBE_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/cube.obj"
#define BACK_WHEEL_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/back_wheel.obj"
#define CHASSIS_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/chassis.obj"
#define COIN_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/coin.obj"
#define FRONT_WHEEL_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/front_wheel.obj"
#define TURRET_OBJ "C:/Users/tsour/CLionProjects/TankGame/models/turret.obj"


using namespace std;

//! Array of key states
bool keyStates[256];

bool shoot;

int width, height, channels;

//! Maze size
const int rows = 10;
const int columns = 10;
unsigned int previousTime = 0;
float wheelangle = 0.0f;
float ballangle = 0.0f;
const float maxBallDistance = 2.0f;
bool renderBall = false;

Vector3f frontWheelPos(0.0,1.0,2.1);
Vector3f backWheelPos(0.0,1.0,-1.3);
Vector3f TankPos(0.0,0.0,0.0);
Vector3f TurretPos(-0.1,0.0,0.3);
Vector3f GridPos;
float angle = 0;
float turretangle = 0;

float x = 0;
int y = 0;

Vector3f CoinPos;
Vector3f BallPos(-0.45, 11.3, 16.0);
float coinangle = 0;
int coincounter = 0; //coin counter to be implemeted later
int level = 1; //level starts at 1, to be implemented later

//Class declarations
Mesh cube;
Mesh chassis;
Mesh frontWheel;
Mesh backWheel;
Mesh turret;
Mesh ball;
Mesh coin;


//Global GLuint variables
GLuint crate_texture = Texture::LoadBMP(CRATE_BMP);
GLuint chassis_texture = Texture::LoadBMP(HUMVEE_BMP);
GLuint ball_texture = Texture::LoadBMP(BALL_BMP);
GLuint coin_texture = Texture::LoadBMP(COIN_BMP);
GLuint shaderProgramID;
GLuint skyboxShader;
GLuint vertexPositionAttribute;
GLuint vertexNormalAttribute;
GLuint vertexTexcoordAttribute;
GLuint TextureMapUniformLocation;

GLuint textureID;

GLuint LightPositionUniformLocation;
GLuint AmbientUniformLocation;
GLuint SpecularUniformLocation;
GLuint SpecularPowerUniformLocation;

Vector3f lightPosition = Vector3f(15.0,15.0,5.0);
Vector3f ambient    = Vector3f(0.08,0.08,0.08);
Vector3f specular   = Vector3f(1,1,1);
float specularPower = 100.0;


//Viewing variables
SphericalCameraManipulator cameraManip;
Matrix4x4 ModelViewMatrix;
GLuint MVMatrixUniformLocation;
Matrix4x4 ProjectionMatrix;
GLuint ProjectionUniformLocation;
Matrix4x4 cameraPosition;
Matrix4x4 humveeModelMatrix;
Matrix4x4 frontWheelModelMatrix;
Matrix4x4 backWheelModelMatrix;
Matrix4x4 turretModelMatrix;

float cameraPan = 0.0;
float cameraTilt = 0.0;
float cameraRadius = 0.0;


unsigned int loadCubemap(vector<std::string> faces) {
    unsigned int skyboxTextureID;
    glGenTextures(1, &skyboxTextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(
                    GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                    0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            cerr << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
            break;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return skyboxTextureID;
}

vector<std::string> facesCubemap = {
        "C:/Users/tsour/CLionProjects/TankGame/skybox/right.jpg",
        "C:/Users/tsour/CLionProjects/TankGame/skybox/left.jpg",
        "C:/Users/tsour/CLionProjects/TankGame/skybox/top.jpg",
        "C:/Users/tsour/CLionProjects/TankGame/skybox/bottom.jpg",
        "C:/Users/tsour/CLionProjects/TankGame/skybox/front.jpg",
        "C:/Users/tsour/CLionProjects/TankGame/skybox/back.jpg"
};

float skyboxVertices[] = {
        // positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
};


// Function declarations
bool initGL(int argc, char** argv);
void display();
void keyboard(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void handleKeys();
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void Timer(int value);
void initShader();
void initTexture(std::string filename, GLuint & crate_texture);

// Define a struct to represent a ball object
struct Ball {
    Vector3f position;
    Vector2f angle;
    bool isActive;

    Ball() : isActive(false) {}
};

vector<Ball> balls; // Maintain a vector of balls

void Text(std::string text, float r, float g, float b, float x, float y)
{
    glColor3f(r,g,b);
    glRasterPos2f(x, y);
    for(unsigned int i = 0; i < text.size(); i++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
    }
}

// Declare maze globally so it's accessible in the entire program
int maze[10][10];
int numberofcoins = 0;
int playerscore = 0;
int maxtime = 60; //make a tag for level as in an int which will then correlate to a string which will then be passed onto the input file stream which will be controlled by the ammount of coins collected, if the ammount of coins collected is equal to the total ammount of coins in the map then increment level counter and change the string

void loadLevel(int level) {
    string filename = "C:/Users/tsour/CLionProjects/TankGame/maps/maze-level" + to_string(level) + ".txt";
    ifstream file(filename);

    if (file.is_open()) {
        // Read maze data from the file
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                file >> maze[i][j];
            }
        }
        file.close();
    } else {
        // Error handling if the file couldn't be opened
        cerr << "Unable to open file!" << endl;
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if(maze[i][j] == 2){
                numberofcoins++;
            }
        }
    }
}

unsigned int cubemapTexture = loadCubemap(facesCubemap);
unsigned int skyboxVAO, skyboxVBO;

//! Main Program Entry
int main(int argc, char** argv)
{
    loadLevel(1);

    // Create a set of balls
    for (int i = 0; i < 5; ++i) {
        Ball newBall;
        balls.push_back(newBall);
    }

    //init OpenGL
    if(!initGL(argc, argv))
        return -1;

    //Init Key States to false;
    for(int i = 0 ; i < 256; i++)
        keyStates[i] = false;


    //glutPassiveMotionFunc(motion);
    glutSetCursor(GLUT_CURSOR_NONE);
    initShader();
    //initialise objects
    cube.loadOBJ(CUBE_OBJ);
    chassis.loadOBJ(CHASSIS_OBJ);
    frontWheel.loadOBJ(FRONT_WHEEL_OBJ);
    backWheel.loadOBJ(BACK_WHEEL_OBJ);
    turret.loadOBJ(TURRET_OBJ);
    ball.loadOBJ(BALL_OBJ);
    coin.loadOBJ(COIN_OBJ);

    //Init Camera Manipultor
    cameraManip.setPanTiltRadius(0.0,2*M_PI,4.0);
    cameraManip.setFocus(chassis.getMeshCentroid());

    //initialise textures
    initTexture(CRATE_BMP, crate_texture);
    initTexture(HUMVEE_BMP, chassis_texture);
    initTexture(BALL_BMP, ball_texture);
    initTexture(COIN_BMP, coin_texture);


    // Create VAO, VBO, and EBO for the skybox
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glUseProgram(skyboxShader);
    glUniform1i(glGetUniformLocation(skyboxShader,"skybox"), (int)0);


    // Creates the cubemap texture object
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    // Cycles through all the textures and attaches them to the cubemap object
    for (unsigned int i = 0; i < 6; i++)
    {
        int width, height, nrChannels;
        unsigned char* data = stbi_load(facesCubemap[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            stbi_set_flip_vertically_on_load(false);
            glTexImage2D
                    (
                            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                            0,
                            GL_RGB,
                            width,
                            height,
                            0,
                            GL_RGB,
                            GL_UNSIGNED_BYTE,
                            data
                    );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture: " << facesCubemap[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // These are very important to prevent seams
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // This might help with seams on some systems
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    //Enter main loop
    glutMainLoop();

    //Delete shader program
    glDeleteProgram(shaderProgramID);

    return 0;
}

//! Function to Initlise OpenGL
bool initGL(int argc, char** argv)
{
    // Init GLUT
    glutInit(&argc, argv);

    // Set Display Mode
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
    glEnable(GL_MULTISAMPLE);

    // Create Window
    glutCreateWindow("Andrew's Tank Trouble");

    // Init GLEW
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return false;
    }

    // Set Display function
    glutDisplayFunc(display);

    // Set Keyboard Interaction Functions
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyUp);

    // Set Mouse Interaction Functions
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);

    // Set up depth testing
    glEnable(GL_TEXTURE_2D);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Set up viewport
    glViewport(0, 0, 1920, 1080);

    glutInitWindowSize(1920, 1080);
    glutReshapeWindow(1920, 1080);
    glutFullScreen();

    return true;
}

void initShader()
{
    //Create shader
    shaderProgramID = Shader::LoadFromFile("C:/Users/tsour/CLionProjects/TankGame/shader.vert","C:/Users/tsour/CLionProjects/TankGame/shader.frag");
    skyboxShader = Shader::LoadFromFile("C:/Users/tsour/CLionProjects/TankGame/cubemap.vert","C:/Users/tsour/CLionProjects/TankGame/cubemap.frag");

    // Get a handle for our vertex position buffer
    vertexPositionAttribute = glGetAttribLocation(shaderProgramID, "aVertexPosition");
    vertexNormalAttribute = glGetAttribLocation(shaderProgramID, "aVertexNormal");
    vertexTexcoordAttribute = glGetAttribLocation(shaderProgramID,"aVertexTexcoord");

    MVMatrixUniformLocation = glGetUniformLocation(shaderProgramID,     "MVMatrix_uniform");
    ProjectionUniformLocation = glGetUniformLocation(shaderProgramID,   "ProjMatrix_uniform");
    LightPositionUniformLocation    = glGetUniformLocation(shaderProgramID, "LightPosition_uniform");
    AmbientUniformLocation          = glGetUniformLocation(shaderProgramID, "Ambient_uniform");
    SpecularUniformLocation         = glGetUniformLocation(shaderProgramID, "Specular_uniform");
    SpecularPowerUniformLocation    = glGetUniformLocation(shaderProgramID, "SpecularPower_uniform");
    TextureMapUniformLocation = glGetUniformLocation(shaderProgramID,"TextureMap_uniform");

}

void initTexture(std::string filename, GLuint & texture)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

    //Get texture Data
    int width, height;
    char* data;
    Texture::LoadBMP(filename, width, height, data);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE,data);
    glBindTexture(GL_TEXTURE_2D, 0);

    //Cleanup data as copied to the GPU
    delete[] data;
}

void Draw_Maze() {
    // Crate texture and lighting
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, crate_texture);
    glUniform1i(TextureMapUniformLocation, 0);
    glUniform4f(AmbientUniformLocation, ambient.x, ambient.y, ambient.z, 1.0);
    glUniform4f(SpecularUniformLocation, specular.x, specular.y, specular.z, 1.0);
    glUniform1f(SpecularPowerUniformLocation, specularPower);

    for (int j = 0; j < GRID_SIZE; j++) {
        for (int i = 0; i < GRID_SIZE; i++) {
            if (maze[i][j] == 1 || maze[i][j] == 2) {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, crate_texture);
                glUniform1i(TextureMapUniformLocation, 0);
                Matrix4x4 cubeModelMatrix = ModelViewMatrix;

                cubeModelMatrix.translate(i*2, -0.8, j*2);


                glUniformMatrix4fv(
                        MVMatrixUniformLocation,  // Uniform location
                        1,                         // Number of Uniforms
                        false,                     // Transpose Matrix
                        (cubeModelMatrix).getPtr());  // Combined Model-View Matrix

                cube.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
            }
            if (maze[i][j] == 2){
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, coin_texture);
                glUniform1i(TextureMapUniformLocation, 0);

                Matrix4x4 c = ModelViewMatrix;

                c.scale(0.5, 0.5, 0.5);
                c.translate(i*4, 3.0, j*4); //if the ball[i] position (BallPos.x/4 and BallPos.z/4) is somewhere within the hit-box of the maze position with the coin then increment coin counter.
                coinangle+=0.1;
                c.rotate(coinangle, 0.0 , 1.0 , 0.0);
                glUniformMatrix4fv(
                        MVMatrixUniformLocation,  	//Uniform location
                        1,					        //Number of Uniforms
                        false,				        //Transpose Matrix
                        c.getPtr());	        //Pointer to Matrix Values

                coin.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);
            }
        }
    }
}

Vector3f turretTip;
bool ballActive = false;


void DrawBall() {
    if (shoot && !renderBall) {
        renderBall = true;
    }

    if (ballActive) {
        for (int i = 0; i < balls.size(); ++i) {
            if (balls[i].isActive) {
                // Update ball position using the turret direction
                float BallSpeed = 0.19f;

                // Update ball position using the turret direction and its current position
                balls[i].position.x += balls[i].angle.x * BallSpeed;
                balls[i].position.y -= 0.02;
                balls[i].position.z += balls[i].angle.y * BallSpeed;

                float XBallPos = balls[i].position.x+TankPos.x;
                float ZBallPos = balls[i].position.z+TankPos.z;

                float GridBallPosX_float = XBallPos / 2.0f;
                float GridBallPosZ_float = ZBallPos / 2.0f;

                int GridBallPosX = round(GridBallPosX_float);
                int GridBallPosZ = round(GridBallPosZ_float);

                if (balls[i].position.y+1.3 <= 0.3){
                    balls[i].isActive = false;
                }

                if (maze[GridBallPosX][GridBallPosZ] == 2 && balls[i].position.y+1.3>=0.6){
                    balls[i].isActive = false;
                    maze[GridBallPosX][GridBallPosZ] -=1;
                    coincounter++;
                    playerscore+=100;
                }

                // Render the ball at its current position
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, ball_texture);
                glUniform1i(TextureMapUniformLocation, 0);

                ballangle += 20;


                Matrix4x4 ballModelMatrix = ModelViewMatrix;
                ballModelMatrix.translate(XBallPos, balls[i].position.y+1.3, ZBallPos);
                ballModelMatrix.scale(0.20, 0.20, 0.20);
                ballModelMatrix.rotate(ballangle, 1, 1, 1);

                glUniformMatrix4fv(
                        MVMatrixUniformLocation,    // Uniform location
                        1,                          // Number of Uniforms
                        false,                      // Transpose Matrix
                        ballModelMatrix.getPtr());  // Pointer to Matrix Values

                ball.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

            } else {
                renderBall = false;
            }
        }
    }
}

int playerlives = 5;

void Tank() {
    glUniformMatrix4fv(MVMatrixUniformLocation, 1, false, (humveeModelMatrix).getPtr());
    glUniform1i(TextureMapUniformLocation, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, chassis_texture);

    //Humvee/chassis
    humveeModelMatrix = ModelViewMatrix;

    humveeModelMatrix.translate(TankPos.x,TankPos.y,TankPos.z);
    humveeModelMatrix.rotate(angle,0,1,0);
    humveeModelMatrix.translate(-TankPos.x,-TankPos.y,-TankPos.z);
    humveeModelMatrix.translate(TankPos.x,TankPos.y,TankPos.z);

    humveeModelMatrix.scale(0.4, 0.4, 0.4);

    //Humvee matrix
    glUniformMatrix4fv(
            MVMatrixUniformLocation,  	//Uniform location
            1,					        //Number of Uniforms
            false,				        //Transpose Matrix
            humveeModelMatrix.getPtr());	        //Pointer to Matrix Values
    chassis.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

    //Front Wheel
    frontWheelModelMatrix = humveeModelMatrix;

    frontWheelModelMatrix.translate(frontWheelPos.x,frontWheelPos.y,frontWheelPos.z);
    frontWheelModelMatrix.rotate(wheelangle, 1.0, 0.0, 0.0);
    frontWheelModelMatrix.translate(-frontWheelPos.x,-frontWheelPos.y,-frontWheelPos.z);


    glUniformMatrix4fv(
            MVMatrixUniformLocation,  	//Uniform location
            1,					        //Number of Uniforms
            false,				        //Transpose Matrix
            frontWheelModelMatrix.getPtr());	        //Pointer to Matrix Values
    frontWheel.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

    //Back Wheel
    backWheelModelMatrix = humveeModelMatrix;

    backWheelModelMatrix.translate(backWheelPos.x,backWheelPos.y,backWheelPos.z);
    backWheelModelMatrix.rotate(wheelangle,1.0, 0.0, 0.0);
    backWheelModelMatrix.translate(-backWheelPos.x,-backWheelPos.y,-backWheelPos.z);

    glUniformMatrix4fv(
            MVMatrixUniformLocation,  	//Uniform location
            1,					        //Number of Uniforms
            false,				        //Transpose Matrix
            backWheelModelMatrix.getPtr());	        //Pointer to Matrix Values
    backWheel.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

    //Turret
    turretModelMatrix = ModelViewMatrix;

    turretModelMatrix.translate(TankPos.x,TankPos.y,TankPos.z);
    turretModelMatrix.rotate(turretangle,0,1,0);
    turretModelMatrix.translate(-TankPos.x,-TankPos.y,-TankPos.z);
    turretModelMatrix.translate(TankPos.x,TankPos.y,TankPos.z);
    turretModelMatrix.scale(0.4,0.4,0.4);

    glUniformMatrix4fv(
            MVMatrixUniformLocation,  	//Uniform location
            1,					        //Number of Uniforms
            false,				        //Transpose Matrix
            turretModelMatrix.getPtr());	        //Pointer to Matrix Values
    turret.Draw(vertexPositionAttribute, vertexNormalAttribute, vertexTexcoordAttribute);

    float XPos = TankPos.x;
    float ZPos = TankPos.z;

    float GridPosX_float = XPos / 2.0f;
    float GridPosZ_float = ZPos / 2.0f;

    int GridPosX = round(GridPosX_float);
    int GridPosZ = round(GridPosZ_float);

    GridPos.x = GridPosX;
    GridPos.y = 0;
    GridPos.z = GridPosZ;


    if (maze[GridPosX][GridPosZ]==2){
        //delete coin and increment the coin counter
        maze[GridPosX][GridPosZ] -=1;
        coincounter++;
        playerscore+=100;
    }
    else if (maze[GridPosX][GridPosZ]!=1 && maze[GridPosX][GridPosZ]!=2){
        TankPos.y-=0.1;
        if(TankPos.y<=-28){
            TankPos.x = 0;
            TankPos.y = 0;
            TankPos.z = 0;
            angle = 0;
            turretangle = 0;
            playerlives--;
        }
    }

    DrawBall();

}

const int fpsBufferSize = 10;
float fpsBuffer[fpsBufferSize] = {0}; // Buffer to store recent FPS values
int fpsBufferIndex = 0; // Index to update the buffer cyclically
float fpsSum = 0; // Sum of FPS values in the buffer

void fps() {
    // Calculate FPS
    unsigned int currentTime = glutGet(GLUT_ELAPSED_TIME);
    unsigned int elapsedTime = currentTime - previousTime;
    previousTime = currentTime;
    float fps = 1000.0f / elapsedTime;

    // Update FPS buffer and sum
    fpsSum -= fpsBuffer[fpsBufferIndex]; // Subtract oldest FPS value
    fpsSum += fps; // Add latest FPS value
    fpsBuffer[fpsBufferIndex] = fps; // Update buffer with latest FPS
    fpsBufferIndex = (fpsBufferIndex + 1) % fpsBufferSize; // Increment index cyclically

    // Calculate moving average FPS
    float movingAverageFPS = fpsSum / fpsBufferSize;

    // Convert FPS to integer
    int fpsInt = static_cast<int>(movingAverageFPS);

    // Draw FPS on screen
    char buffer[20];
    sprintf(buffer, "FPS: %d", fpsInt);
    Text(buffer, 1.0, 1.0, 1.0, -0.97, 0.96);
}

unsigned int timeelapsed = 0;
unsigned int lastTime = 0;
unsigned int timeElapsedInSeconds = 0;
unsigned int timeRemaining = maxtime*1000;
bool gameover = false;

void Timer(int value)
{
    unsigned int currentTime = glutGet(GLUT_ELAPSED_TIME);

    // Calculate the time elapsed since the last call to the Timer function
    unsigned int elapsedTime = currentTime - lastTime;

    // Update the time remaining
    if (timeRemaining > elapsedTime) {
        timeRemaining -= elapsedTime;
    } else {
        timeRemaining = 0; // Ensure time remaining doesn't go negative
        gameover = true;
    }

    // Update the total elapsed time
    timeelapsed += elapsedTime;
    timeElapsedInSeconds = timeRemaining / 1000; // Convert milliseconds to seconds
    unsigned int decimalPart = (timeRemaining % 1000) / 100; // Extract the tenths of a second
    char timerbuffer[50];
    sprintf(timerbuffer, "Time remaining: %u.%u sec", timeElapsedInSeconds, decimalPart);
    Text(timerbuffer, 1.0, 1.0, 1.0, -0.97, 0.91);

    lastTime = currentTime;
}

void renderBackground() {
    glDisable(GL_DEPTH_TEST);  // Disable depth test for background
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);  // Set up 2D orthographic projection

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Render the background quad
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID);  // Bind the loaded texture
    glBegin(GL_QUADS);
    glTexCoord2f(0, 1); glVertex2f(0, 0); // Flip the y-axis by swapping 0 and 1
    glTexCoord2f(1, 1); glVertex2f(width, 0);
    glTexCoord2f(1, 0); glVertex2f(width, height);
    glTexCoord2f(0, 0); glVertex2f(0, height);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);  // Re-enable depth test
}

void renderSkybox() {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    glUseProgram(skyboxShader);
    Matrix4x4 skyBoxMatrix = ModelViewMatrix;

    skyBoxMatrix.scale(10000,10000,10000);
    glUniformMatrix4fv(
            glGetUniformLocation(skyboxShader, "view"),  // Uniform location
            1,                           // Number of Uniforms
            false,                       // Transpose Matrix
            skyBoxMatrix.getPtr());  // Pointer to ModelViewMatrixValues
    glUniformMatrix4fv(
            glGetUniformLocation(skyboxShader, "projection"),  // Uniform location
            1,                           // Number of Uniforms
            false,                       // Transpose Matrix
            ProjectionMatrix.getPtr());  // Pointer to ModelViewMatrixValues
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

void display(void) {
    // Handle keys
    handleKeys();

    // Clear the screen with the clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //renderBackground();
    renderSkybox();

    glMatrixMode(GL_PROJECTION );
    glUseProgram(shaderProgramID);
    ProjectionMatrix.perspective(140, 1.0, 0.0001, 100000);

    // Set Projection Matrix
    glUniformMatrix4fv(
            ProjectionUniformLocation,  // Uniform location
            1,                           // Number of Uniforms
            false,                       // Transpose Matrix
            ProjectionMatrix.getPtr());  // Pointer to ModelViewMatrixValues

    if (playerlives != 0 && !gameover){

        ModelViewMatrix.toIdentity();
        ModelViewMatrix = cameraManip.apply(ModelViewMatrix);
        cameraManip.setFocus(Vector3f(TankPos.x, TankPos.y, TankPos.z));
        cameraManip.setPanTiltRadius(turretangle*(M_PI/180), -0.8, 4.0);
        //Calls Draw maze function.
        Draw_Maze();
        //Calls Tank function.
        Tank();
        //Unused Shader
        glUseProgram(0);
        glDisable(GL_TEXTURE_2D);

        //All printing functions should go here
        fps(); // Render fixed text
        Timer(0);

        char coinbuffer[20];
        sprintf(coinbuffer, "Coins collected: %i/%i", coincounter, numberofcoins);
        Text(coinbuffer, 1.0, 1.0, 1.0, 0.80, 0.96);

        if (coincounter == numberofcoins){
            level++;
            coincounter = 0;
            numberofcoins = 0;
            TankPos.x = 0;
            TankPos.z = 0;
            angle = 0;
            turretangle = 0;
            loadLevel(level);
        }

        char scorebuffer[20];
        sprintf(scorebuffer, "Highscore: %i", playerscore);
        Text(scorebuffer, 1.0, 1.0, 1.0, 0.80, 0.91);

        char livesbuffer[20];
        sprintf(livesbuffer, "Lives: %i/5", playerlives);
        Text(livesbuffer, 1.0, 1.0, 1.0, 0.80, 0.86);

        char levelbuffer[20];
        sprintf(levelbuffer, "LEVEL %i", level);
        Text(levelbuffer, 1.0, 1.0, 1.0, -0.05, 0.96);

        GLenum error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << error << std::endl;
        }

        // Redraw frame
        glutPostRedisplay();
    }
    else{
        glUseProgram(0);
        if (playerlives == 0){
            glColor3f(1.0, 1.0, 1.0); // Set text color to white
            char gamestatebuffer[20];
            sprintf(gamestatebuffer, "Game Over!");
            Text(gamestatebuffer, 1, 1, 1, -0.05, 0);
            char livesbuffer[20];
            sprintf(livesbuffer, "Out of lives");
            Text(livesbuffer, 1, 1, 1, -0.048, -0.03);
            char scorebuffer[20];
            sprintf(scorebuffer, "Score: %i", playerscore);
            Text(scorebuffer, 1, 1, 1, -0.046, -0.08);
        }
        else if (gameover){
            char gamestatebuffer[20];
            sprintf(gamestatebuffer, "Game Over!");
            Text(gamestatebuffer, 1, 1, 1, -0.05, 0);
            char livesbuffer[20];
            sprintf(livesbuffer, "Timer ran out");
            Text(livesbuffer, 1, 1, 1, -0.0538, -0.03);
            char scorebuffer[20];
            sprintf(scorebuffer, "Score: %i", playerscore);
            Text(scorebuffer, 1, 1, 1, -0.046, -0.08);
        }
        glEnable(GL_DEPTH_TEST); // Re-enable depth testing if needed
    }
    glutSwapBuffers();
}

//! Keyboard Interaction
void keyboard(unsigned char key, int x, int y)
{
    //Quits program when esc is pressed
    if (key == 27)	//esc key code
    {
        exit(0);
    }

    //Set key status
    keyStates[key] = true;
}

float timePressed = 0;

void keyUp(unsigned char key, int x, int y)
{
    keyStates[key] = false;
    timePressed = 0;
}
// Define debounce time in milliseconds
const int debounceTime = 100;

// Define timer variables for debouncing
unsigned int shootTimer = 0;
bool shootReady = true;

float velocity = 0.02;
const float maxSpeed = 0.04;
float speed = 0;

double timePressedStart = 0;
double timePressedDuration = 0;

double timePressedStartS = 0;
double timePressedDurationS = 0;

// Handle Keys
void handleKeys()
{
    shoot = false;
    if((maze[int(GridPos.x)][int(GridPos.z)]==1 || maze[int(GridPos.x)][int(GridPos.z)]==2) && playerlives!=0 && !gameover){
        if (keyStates['w'] || keyStates['W'])
        {
            if (timePressedStart == 0) // Button is just pressed now
            {
                timePressedStart = glutGet(GLUT_ELAPSED_TIME)/1000.0;
            }

            timePressedDuration = (glutGet(GLUT_ELAPSED_TIME)/1000.0) - timePressedStart;

            if (speed <= maxSpeed){
                float acceleration = velocity * timePressedDuration * 5;
                speed = acceleration;
            }
            float forwardX = sin(angle * (M_PI / 180.0)); // Convert angle to radians
            float forwardZ = cos(angle * (M_PI / 180.0)); // Convert angle to radians
            TankPos.x += speed * forwardX;
            TankPos.z += speed * forwardZ;
            wheelangle+=10;
            speed = 0;
        } else{
            if (timePressedStart != 0) // Button is released now
            {
                timePressedDuration = (glutGet(GLUT_ELAPSED_TIME)/1000.0) - timePressedStart;
                timePressedStart = 0;
            }
            timePressedDuration = 0;
        }

        if (keyStates['s'] || keyStates['S'])
        {
            if (timePressedStartS == 0) // Button is just pressed now
            {
                timePressedStartS = glutGet(GLUT_ELAPSED_TIME)/1000.0;
            }

            timePressedDurationS = (glutGet(GLUT_ELAPSED_TIME)/1000.0) - timePressedStartS;

            if (speed >= -maxSpeed){
                float deceleration = -velocity * timePressedDurationS;
                speed += deceleration;
            }

            float backwardX = -sin(angle * (M_PI / 180.0)); // Convert angle to radians
            float backwardZ = -cos(angle * (M_PI / 180.0)); // Convert angle to radians
            TankPos.x -= speed * backwardX;
            TankPos.z -= speed * backwardZ;
            wheelangle-=10;
            speed = 0;
        } else{
            if (timePressedStartS != 0) // Button is released now
            {
                timePressedDurationS = (glutGet(GLUT_ELAPSED_TIME)/1000.0) - timePressedStartS;
                timePressedStartS = 0;
            }
            timePressedDurationS = 0;
        }


        if (keyStates['a'] || keyStates['A'])
        {
            angle+=2;
        }

        if (keyStates['d'] || keyStates['D'])
        {
            angle-=2;
        }

        if (keyStates[' '] && shootReady) {
            shoot = true;
            shootTimer = glutGet(GLUT_ELAPSED_TIME);
            shootReady = false;

            const float newturretangleX =  sin(turretangle * (M_PI / 180.0));
            const float newturretangleZ =  cos(turretangle * (M_PI / 180.0));
            // Find an inactive ball object and activate it
            for (int i = 0; i < balls.size(); ++i) {
                if (!balls[i].isActive) {
                    balls[i].position = turretTip; // Set the ball's initial position
                    balls[i].angle.x = sin(turretangle * (M_PI / 180.0));
                    balls[i].angle.y = cos(turretangle * (M_PI / 180.0));
                    balls[i].isActive = true; // Activate the ball
                    ballActive = true;
                    break; // Exit the loop after activating one ball
                }
            }

            // Reset the shoot flag after finding an inactive ball object
            if (balls[0].isActive) {
                shoot = false;
            }

        }

        // Check if debounce time has passed
        if (!shootReady && (glutGet(GLUT_ELAPSED_TIME) - shootTimer) > debounceTime) {
            shootReady = true;
        }
    }
}


void mouse(int button, int state, int x, int y)
{
    /*cameraManip.handleMouse(button, state,((x-960)%960),((y-540)%540));
    glutPostRedisplay();*/

}

int previousX = 0;

void motion(int x, int y) {
    if((maze[int(GridPos.x)][int(GridPos.z)]==1 || maze[int(GridPos.x)][int(GridPos.z)]==2) && playerlives!=0 && !gameover) {
        cameraPan = cameraManip.getPan();
        // Calculate the change in mouse position from the previous position
        int deltaX = x - previousX;

        if (deltaX < 0) {
            turretangle += 1.0;
            cameraPan = turretangle;
            fmod(turretangle, 360);
        }

        if (deltaX > 0) {
            turretangle -= 1.0;
            cameraPan = turretangle;
            fmod(turretangle, 360);
        }

        // Calculate the center of the screen
        int centerX = glutGet(GLUT_WINDOW_WIDTH) / 2;
        int centerY = glutGet(GLUT_WINDOW_HEIGHT) / 2;

        // Wrap the cursor position if it exceeds the screen boundaries
        if (x <= 0 || x >= glutGet(GLUT_WINDOW_WIDTH) - 1) {
            glutWarpPointer(centerX, centerY);
        }
        if (y <= 0 || y >= glutGet(GLUT_WINDOW_HEIGHT)) {
            glutWarpPointer(centerX, centerY);
        }

        // Update the previous mouse position for the next iteration
        previousX = x;
        glutPostRedisplay();
    }
}

