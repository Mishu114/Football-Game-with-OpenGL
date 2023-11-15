//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "cylinder.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
float angle_ball = 0.0f;
float ball_z = 5.0f;
float ball_x = 0.0f;
bool ball_shoot = false;
float bz = 0.0f;
float bar_x = 0.0f;
//initial values

float gun_x=0.0f;
float gun_y=0.7f;
float balloon_x=1.5f;
float balloon_y = 1.2f;

bool first_block = true;
bool balloon = true;
bool gameOver = false;

int difficult = 0;

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
float inc = 0.1f;

int score = 0;
bool start = true;
float slope = 0.0f;
int ball_count = 0;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.50f,  3.0f,  0.0f),
    glm::vec3(2.5f,  3.0f,  10.0f),
    glm::vec3(-2.5f,  3.0f,  0.0f),
    glm::vec3(-2.5f,  3.0f,  10.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
int serial = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");

    string diffuseMapPath = "field.jpg";
    string specularMapPath = "field.jpg";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube field = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath2 = "seat.png";
    string specularMapPath2 = "seat.png";

    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seat = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath3 = "score.jpg";
    string specularMapPath3 = "score.jpg";

    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube scoreB = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPath4 = "GameOver.jpg";
    string specularMapPath4 = "GameOver.jpg";

    unsigned int diffMap4 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube GameOver = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cylinder bar = Cylinder();
    bar.setRadius(0.1);
    bar.diffuse = glm::vec3(1.0, 1.0, 1.0);
    bar.ambient = glm::vec3(1.0, 1.0, 1.0);
    bar.specular = glm::vec3(1.0, 1.0, 1.0);
    //ourShader.use();
    //lightingShader.use();

    Sphere ball = Sphere();

    // render loop
    // -----------
    camera.Position = glm::vec3(2.0, 3.0, 0.0);

    int count = 0;
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);
        func(cubeVAO, lightingShader, model);


        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        // 
        
        //bar
        glm::mat4 bar1 = glm::mat4(1.0f);
        bar1 = glm::translate(model, glm::vec3(0.4f, -0.2f, 0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
        bar1 = bar1 * scaleMatrix;
        bar.drawCylinder(lightingShader, bar1, 1.0,1.0,1.0);

        bar1 = glm::mat4(1.0f);
        bar1 = glm::translate(model, glm::vec3(-0.4f, -0.2f, 0.4f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
        bar1 = bar1 * scaleMatrix;
        bar.drawCylinder(lightingShader, bar1, 1.0, 1.0, 1.0);

        bar1 = glm::mat4(1.0f);
        bar1 = glm::translate(model, glm::vec3(0.0f, 0.8f, 0.4f));
        glm::mat4 bar1_r = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.4f, 0.1f));
        bar1 = bar1 * bar1_r * scaleMatrix;
        bar.drawCylinder(lightingShader, bar1, 1.0, 1.0, 1.0);

        if (difficult == 1)
            bar_x += (0.1 * inc);
        else if (difficult == 2)
            bar_x += (0.2 * inc);
        else if(difficult == 3)
            bar_x += (0.3 * inc);

        if (bar_x - 0.4 <= -2.5f)
            inc = 0.1f;
        if (bar_x + 0.4 >= 2.5f)
            inc = -0.1f;

        glm::mat4 bar2 = glm::mat4(1.0f);
        bar2 = glm::translate(model, glm::vec3(0.4f+bar_x, -0.2f, 9.6f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
        bar2 = bar2 * scaleMatrix;
        bar.drawCylinder(lightingShader, bar2, 1.0, 1.0, 1.0);

        bar2 = glm::mat4(1.0f);
        bar2 = glm::translate(model, glm::vec3(-0.4f + bar_x, -0.2f, 9.6f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.0f, 0.1f));
        bar2 = bar2 * scaleMatrix;
        bar.drawCylinder(lightingShader, bar2, 1.0, 1.0, 1.0);

        bar2 = glm::mat4(1.0f);
        bar2 = glm::translate(model, glm::vec3(0.0f + bar_x, 0.8f, 9.6f));
        glm::mat4 bar2_r = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.4f, 0.1f));
        bar2 = bar2 * bar2_r * scaleMatrix;
        bar.drawCylinder(lightingShader, bar2, 1.0, 1.0, 1.0);


        //scoreboard
        glm::mat4 modelForScore = identityMatrix;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.25f, 2.0f, 10.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
        modelForScore = modelForScore * translateMatrix  * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForScore, 0.9, 0.8, 0.9);

        glm::mat4 stand = glm::mat4(1.0f);
        stand = glm::translate(model, glm::vec3(1.4f , -1.0f, 10.1f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.0f, 0.1f));
        stand = stand * scaleMatrix;
        bar.drawCylinder(lightingShader, stand, 1.0, 1.0, 1.0);

        stand = glm::mat4(1.0f);
        stand = glm::translate(model, glm::vec3(-1.4f, -1.0f, 10.1f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 4.0f, 0.1f));
        stand = stand * scaleMatrix;
        bar.drawCylinder(lightingShader, stand, 1.0, 1.0, 1.0);

        stand = glm::mat4(1.0f);
        stand = glm::translate(model, glm::vec3(0.0f, 2.8f, 10.1f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 1.4f, 0.1f));
        stand = stand * bar2_r * scaleMatrix;
        bar.drawCylinder(lightingShader, stand, 1.0, 1.0, 1.0);

        float sbx = 0.0f;
        for (int i = 0; i < score; i++)
        {
            glm::mat4 modelForScore = identityMatrix;
            translateMatrix = glm::translate(model, glm::vec3(0.8f-sbx, 2.6f, 10.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
            modelForScore = modelForScore * translateMatrix * scaleMatrix;
            ball.drawSphere(lightingShader, modelForScore);
            sbx += 0.2f;
        }



        //ball shoot

        //printf("%f\n", angle_ball);
        
        if (ball_shoot)
        {
            bz += 0.03f;
            ball_x = slope * bz;

            printf("%f %f %f %f\n",angle_ball, slope, bz, ball_x);
        }

        if (ball_z + bz >= 10.0)
        {
            start = false;
            if (ball_x > -0.4f + bar_x && ball_x < 0.4 + bar_x)
            {
                if (serial == ball_count)
                {
                   if(score<9)
                        score++;
                    serial--;
                    
                }
                    
            }
            else
            {
                if (serial == ball_count)
                {
                    if(score>0)
                        score--;
                    serial--;

                }
            }
            ball_shoot = false;
            ball_x = 0.0f;
            bz = 0.0f;
            printf("%d\n", score);
        }

        glm::mat4 modelForBall = identityMatrix;
        translateMatrix = glm::translate(model, glm::vec3(ball_x, 0.36f, ball_z+bz));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f));
        modelForBall = modelForBall * translateMatrix * scaleMatrix;
        ball.drawSphere(lightingShader, modelForBall);

        glm::mat4 rotArrow = glm::rotate(identityMatrix, glm::radians(angle_ball), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 modelForArrow = identityMatrix;
        translateMatrix = glm::translate(model, glm::vec3( -0.02, 0.36f, 5.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.01f, 0.5f));
        modelForArrow = modelForArrow * translateMatrix * rotArrow * scaleMatrix;
        drawCube(cubeVAO, lightingShader, modelForArrow, 1.0, 1.0, 1.0);


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);



        //field
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.1f, 0.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.2f, 10.0f));
        glm::mat4 modelMatrixForfield = translateMatrix * scaleMatrix;
        field.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForfield);
        
        //right gallery - 1
        float zz = 0.5f;
        glm::mat4 rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.3f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 0.5f;
        glm::mat4 rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.3f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2* rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }
        
        //right gallery - 2
        zz = 0.5f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.8f, 1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 0.5f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.8f,1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        //right gallery - 2
        zz = 0.5f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.8f, 1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 0.5f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.8f,1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }


        //right gallery - 2
        zz = 0.5f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.3f, 1.5f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 0.5f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-4.3f, 1.5f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }



        //front-gallery 1
        float xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.71f, 11.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }
        glm::mat4 rotXSeat = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.71f, 11.3));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotXSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }


        //front-gallery 2
         xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.1f, 11.5));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }
        rotXSeat = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.1f, 11.8));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotXSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }

        //front-gallery 3
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.5f, 12.0));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }
        rotXSeat = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.5f, 12.3));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotXSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }






        //left gallery - 1
        zz = -0.6f;
        rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = -0.6f;
        rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.3f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 6.4f;
        rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 6.4f;
        rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.3f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }




        //left gallery -2
        zz = -0.6f;
        rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = -0.6f;
        rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.8f, 1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 6.4f;
        rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.5f, 1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 6.4f;
        rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(3.8f, 1.1f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }


        //left gallery -3
        zz = -0.6f;
        rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 1.5f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = -0.6f;
        rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.3f, 1.5f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 6.4f;
        rotSeat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.0f, 1.5f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }

        zz = 6.4f;
        rotSeat2 = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(4.3f, 1.5f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotSeat2 * rotSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            zz += 0.5f;
        }







        //back-gallery 1
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.71f, -1.3));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }
        rotXSeat = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 0.71f, -1.3));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotXSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }


        //front-gallery 2
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.1f, -1.8));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }
        rotXSeat = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.1f, -1.8));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotXSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }

        //front-gallery 3
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.5f, -2.3));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }
        rotXSeat = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        xx = 2.1f;
        for (int i = 0; i < 10; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(xx, 1.5f, -2.3));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.05f, 0.3f));
            glm::mat4 modelMatrixForseat = translateMatrix * rotXSeat * scaleMatrix;
            seat.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForseat);
            xx -= 0.5f;
        }


        //scoreboard
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.25f, 2.0f, 9.99f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
        glm::mat4 modelMatrixForScore = translateMatrix * scaleMatrix;
        scoreB.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForScore);

        //gameOver

        if (!start && score == 0)
            gameOver = true;

        if (gameOver && !start)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.25f, 2.0f, 9.98f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 1.5, 0.2));
            glm::mat4 modelMatrixForScore = translateMatrix * scaleMatrix;
            GameOver.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForScore);
        }

        /*float zz = 0.1f;
        for (int i = 0; i < 20; i++)
        {
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.5f, 0.71f, zz));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.01, 0.3));
            model = translateMatrix * scaleMatrix;
            drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);
            zz += 0.5f;
        }*/
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);
    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.1;
    float width = 5;
    float length = 10;
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //base
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-5.0f, 0.09f, -2.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(10.0, 0.2, 15.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.2, 0.3);

    //right gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 0.2f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 0.5, 10.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 0.6f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 0.5, 10.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5, 10.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 1.4f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.5, 10.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 1.4f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 1.5, 10.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.4, 0.3, 0.4);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-6.0f, 2.7f, 0.0f));
    glm::mat4 rot1 = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 1.3, 10.0));
    model = translateMatrix * rot1 * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.4, 0.3, 0.4);

    
    //glm::mat4 rotseat = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    //zz = 0.1f;
    //for (int i = 0; i < 20; i++)
    //{
    //    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.49f, 0.71f, zz));
    //    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.4, 0.01, 0.3));
    //    model = translateMatrix * rotseat* scaleMatrix;
    //    drawCube(cubeVAO, lightingShader, model, 1.0, 0.0, 0.0);
    //    zz += 0.5f;
    //}
    

    //front gallery
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.2f, 14.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix* scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.6f, 14.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 1.0f, 14.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 1.4f, 14.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    //back gallery
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.2f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 0.6f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 1.0f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(2.5f, 1.4f, -4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    //left gallery
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 0.2f, 11.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 0.6f, 11.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 1.0f, 11.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 1.4f, 11.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    
    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 0.2f, 4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 0.6f, 4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 1.0f, 4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.0, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 1.4f, 4.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 1.4f, 8.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.5, 0.5, 5.0));
    model = translateMatrix * rotateYMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.9, 0.8, 0.9);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 1.4f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 1.5, 10.0));
    model = translateMatrix * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.4, 0.3, 0.4);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(6.0f, 2.7f, 0.0f));
    rot1 = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 1.3, 10.0));
    model = translateMatrix * rot1 * scaleMatrix;
    drawCube(cubeVAO, lightingShader, model, 0.4, 0.3, 0.4);

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if(!ball_shoot)
            angle_ball += 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!ball_shoot)
            angle_ball -= 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {

        if (!gameOver)
        {
            
            ball_count++;
            serial = ball_count;
            ball_shoot = true;
            slope = tanf(3.1416 * angle_ball / 180.0);
        }
        
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        start = true;
        gameOver = false;
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        difficult = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        difficult = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        difficult = 3;
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
        difficult = 0;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}