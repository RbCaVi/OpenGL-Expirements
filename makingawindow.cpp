#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shader.h>


//functions used later in the program for, framebuffer & getting input
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//framebuffer variables
int framebufferWidth;
int framebufferHeight;

//Vertex Array Object (i.e stores vertex attributes)
unsigned int VAO;

//Vertex Buffer Object (stores GPU memory for the vertex shader)
unsigned int VBO;

//Element Buffer Object (stores element array gpu memory)
unsigned int EBO;

//vertex shader object (i.e. holds vertex shader id)
unsigned int vertexShader;

//fragment shader object (i.e. holds frag shader id)
unsigned int fragmentShader;

//ref id to the final shader program
unsigned int shaderProgram;

//for error handling later to see if compilation was successful
int success;
//an infolog to explain any error that may show up
char infoLog[512];

//const default screen sizes
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//vertex data for a cube
//first 3 values are the (x,y,z), the last 2 values are (s,t) for textures
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
//EBA indices
unsigned int indices[] = {
    0,1,2,
    0,2,3
};

//translation for the cube positions using vec3
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

/* float texCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
}; */




int main()
{
    //glfw initilization
    glfwInit();

    //glfw window hints (setting context vers. & profile to core)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef  __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    //Creates glfw window to render pixels within
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, ":3 UwU XD SillyWindow", NULL, NULL);

    //checks that the new window isnt null, if it is then it will terminate with an error
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    //sets the current context to the new window
    glfwMakeContextCurrent(window);

    //sets the framebuffersize (i.e. storing how much of frame is stored)
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //gets the framebuffer setting the height & width to two variables
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    //loads in glad, sending an error if it doesnt
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initalize GLAD" << std::endl;
        return -1;
    }

    //sets the gl viewport (normalized for -1 to 1)
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    //gets the max vertex attributes attributed to each shader
    float nrAttributes;
    glGetFloatv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "max num of vertex attributes supported: " << nrAttributes << std::endl;

    //compiles the shader
    Shader ourShader("D:/Documents/Code/C & C++/GraphicsProgrammingVentures/GraphicsAPIWork/shaders/shader.vs", "D:/Documents/Code/C & C++/GraphicsProgrammingVentures/GraphicsAPIWork/shaders/shader.fs");

    
    //generates a vertex attribute array
    glGenVertexArrays(1, &VAO);
    //Generates a vertex buffer, setting VBO as an id to it
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    

    //binds the vertex array object
    glBindVertexArray(VAO);
    //binds the array buffer to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //binds ebo buffer to the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    //loads the vertices data into the buffer for the gpu to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //loads indicies data into the ebo buffer for the gpu
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //sets the proper attributes for the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //enables vertex attributes
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //--glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //--glEnableVertexAttribArray(2);


    //textures
    unsigned int texture1, texture2;
    //generates silly milly texture
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("D:/Documents/Code/C & C++/GraphicsProgrammingVentures/GraphicsAPIWork/assets/milly.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    stbi_image_free(data);

    //generates a texture for boba tea
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char* secondata = stbi_load("D:/Documents/Code/C & C++/GraphicsProgrammingVentures/GraphicsAPIWork/assets/boba.png", &width, &height, &nrChannels, 0);
    if (secondata) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, secondata);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    //sets the texture uniforms
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    //Enables the Z-BUFFER
    glEnable(GL_DEPTH_TEST);

    //the render loop
    while (!glfwWindowShouldClose(window))
    {
        //swaps the rendered buffer with the next image render buffer
        glfwSwapBuffers(window);
        //a function to handle input
        processInput(window);
        //rendering commands
        //sets the back color of the toberendered buffer to the rgba values
        glClearColor(0.4f, 0.3f, 0.5f, 1.0f);
        //clears it to the the color buffer (i.e. the clear color setting) & uses the z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //sets & binds each of the textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        //uses the program
        ourShader.use();

        //sets up uniforms for the coordinate spaces
        //--unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");
        unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
        unsigned int projectionLoc = glGetUniformLocation(ourShader.ID, "projection");
        unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");

        //--glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
        //--glm::mat4 trans = glm::mat4(1.0f);

        //Base mat4 coordinate transformations
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        //sets the value for each mat4 transformation in coordinate spaces
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        //Sets the above matrix values to their corresponding uniforms
        //--glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
        
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        //--trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        //--trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        
        

        for (unsigned int i = 0; i < 10; i++) {

            glm::mat4 model = glm::mat4(1.0f);

            float amountRotatedAngle = -55.0f * i;


            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(amountRotatedAngle), glm::vec3(1.0f, 0.3f, 0.5f));


            //rotates the local space by 50 rads over time
            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            ourShader.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        
        //draws vertexs from the VAO that pulls each vertex point to draw,
        //and draws each VAO as an element of a triangle
        //--glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        
        
        //checks if any events were triggered (i.e. input from kb&m)
        glfwPollEvents();
    }
    //delete the unused arrays
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    //ends the glfw library
    glfwTerminate();
    return 0;
}

//takes in the input while window is active
void processInput(GLFWwindow* window) {
    //if esc is pressed then close the window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
}

//sets the framebuffersize to change so the viewport adjusts
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
