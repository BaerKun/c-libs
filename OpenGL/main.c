#include <stdio.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

int screenWidth = 800;
int screenHeight = 600;

void framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // 设置视口大小
    glViewport(0, 0, screenWidth, screenHeight);
}

void processInput(GLFWwindow *window) {
    // ESC键退出
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    // 初始化GLFW
    glfwInit();

    // 设置OpenGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // 设置OpenGL为核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 创建窗口
    GLFWwindow *window = glfwCreateWindow(screenWidth, screenHeight, "OpenGL", NULL, NULL);
    if(window == NULL){
        puts("Failed to create GLFW window");
        glfwTerminate();
        return -1;
    }

    // 设置窗口上下文
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    // 初始化GLAD
    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)){
        puts("Failed to initialize GLAD");
        return -1;
    }

    // 渲染循环
    while(!glfwWindowShouldClose(window)){

        // 输入
        processInput(window);

        // 渲染
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 交换缓冲区
        glfwSwapBuffers(window);

        // 检查事件
        glfwPollEvents();
    }

    // 释放资源
    glfwTerminate();

    return 0;
}
