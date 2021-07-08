#define _CRT_SECURE_NO_WARNINGS

#include "Shader.h"
#include "glew.h"
#include "glfw3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <math.h>

#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x)  GLClearError();\
					x;\
					ASSERT(GLLogCall(#x, __FILE__, __LINE__));
bool key_press[360];

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error]:(0x" << std::hex << error << "): " << function << " " << file << " (line " << std::dec << line << ")" << std::endl;
		return false;
	}
	return true;
}

void key_check()
{

}

const char* get_key_name(int);

void key_is_fun(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		std::cout << get_key_name(key) << std::endl;

		if (key == GLFW_KEY_RIGHT)
			key_press[GLFW_KEY_RIGHT] = true;
		if (key == GLFW_KEY_LEFT)
			key_press[GLFW_KEY_LEFT] = true;
		if (key == GLFW_KEY_UP)
			key_press[GLFW_KEY_UP] = true;
		if (key == GLFW_KEY_DOWN)
			key_press[GLFW_KEY_DOWN] = true;
		if (key == GLFW_KEY_W)
			key_press[GLFW_KEY_W] = true;
		if (key == GLFW_KEY_S)
			key_press[GLFW_KEY_S] = true;
		if (key == GLFW_KEY_KP_4)
			key_press[GLFW_KEY_KP_4] = true;
		if (key == GLFW_KEY_KP_6)
			key_press[GLFW_KEY_KP_6] = true;
		if (key == GLFW_KEY_KP_8)
			key_press[GLFW_KEY_KP_8] = true;
		if (key == GLFW_KEY_KP_2)
			key_press[GLFW_KEY_KP_2] = true;

	}
	if (action == GLFW_RELEASE)
	{
		std::cout << get_key_name(key) << std::endl;

		if (key == GLFW_KEY_RIGHT)
			key_press[GLFW_KEY_RIGHT] = false;
		if (key == GLFW_KEY_LEFT)
			key_press[GLFW_KEY_LEFT] = false;
		if (key == GLFW_KEY_UP)
			key_press[GLFW_KEY_UP] = false;
		if (key == GLFW_KEY_DOWN)
			key_press[GLFW_KEY_DOWN] = false;
		if (key == GLFW_KEY_W)
			key_press[GLFW_KEY_W] = false;
		if (key == GLFW_KEY_S)
			key_press[GLFW_KEY_S] = false;
		if (key == GLFW_KEY_KP_4)
			key_press[GLFW_KEY_KP_4] = false;
		if (key == GLFW_KEY_KP_6)
			key_press[GLFW_KEY_KP_6] = false;
		if (key == GLFW_KEY_KP_8)
			key_press[GLFW_KEY_KP_8] = false;
		if (key == GLFW_KEY_KP_2)
			key_press[GLFW_KEY_KP_2] = false;
	}
}

static unsigned int readFile(std::string& source, const std::string& path)
{
	std::ifstream f(path);
	std::stringstream stream;
	stream << f.rdbuf();
	std::cout << stream.str();
	std::cout << std::endl;
	source = stream.str();
	return 1;
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
	GLCall(unsigned int id = glCreateShader(type));
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr)); // adica pana la nullptr
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));

		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Shader failed to compile" << std::endl;
		if (type == GL_VERTEX_SHADER)
			std::cout << "(vertex shader)" << std::endl;
		if (type == GL_FRAGMENT_SHADER)
			std::cout << "(fragment shader)" << std::endl;

		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return -1;
	}

	return id;
}


static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	GLCall(unsigned int program = glCreateProgram());
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}




int main()
{
	glfwInit();
	GLFWwindow* window;
	window = glfwCreateWindow(800, 960, "Bau!", NULL, NULL);
	int height, width;
	glfwGetWindowSize(window, &width, &height);
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK) return -1;


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSetKeyCallback(window, key_is_fun);

	glfwSwapInterval(0);
	glClearColor(0, 0, 0, 0.2);



	//Shader newShader("shaders/vertex_1.shader", "shaders/fragment_1.shader");


	float positions[]{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f,0.0f,
		1.0f, 1.0f, 0.0f,

		-1.0, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao; // vertex array object. Nu exista by default ca 0 decat in OpenGL compatibility profile
	GLCall(glGenVertexArrays(1, &vao));
	GLCall(glBindVertexArray(vao));

	unsigned int buffer;
	GLCall(glGenBuffers(1, &buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));


	unsigned int ibo; // ibo = index buffer object
					// (ale catelea puncte le deseneaza)

	GLCall(glGenBuffers(1, &ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW));

	std::string vertexShader, fragmentShader;


	readFile(vertexShader, "shaders/vertex_1.shader");
	readFile(fragmentShader, "shaders/fragment_1.shader");

	unsigned int shader = createShader(vertexShader, fragmentShader);
	GLCall(glUseProgram(shader));

	GLCall(int color1loc = glGetUniformLocation(shader, "u_Color"));
	//ASSERT(color1loc != -1);

	GLCall(int pos1loc = glGetUniformLocation(shader, "u_Pos"));
	//ASSERT(pos1loc != -1);

	//unbinds everything
	GLCall(glBindVertexArray(0));
	GLCall(glEnableVertexAttribArray, 0);
	GLCall(glUseProgram(0));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	//GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));


	glClearColor(0.0, 0.0, 0.0, 1.0);
	float red = 0, green = 0, blue = 0;

	while (!glfwWindowShouldClose(window))
	{

		GLCall(glUseProgram(shader));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
		GLCall(glBindVertexArray(vao));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

		//glClear(GL_COLOR_BUFFER_BIT);


		//glDrawArrays(GL_TRIANGLES, 0, sizeof(positions)/sizeof(float));

		GLCall(glDrawElements(GL_TRIANGLE_STRIP, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr));

		glfwSwapBuffers(window);

		glfwPollEvents();
		key_check();

		red += 0.002;
		green += 0.0015;
		blue += 0.003;
		
		glClearColor(fabs(sinf(red)), fabs(sinf(green)), 1 - fabs(sinf(blue)), 1.0);
		GLCall(glUniform4f(color1loc, 1 - fabs(sinf(red)), fabs(sinf(green)), 1 - fabs(sinf(blue)), 1.0));
		GLCall(glUniform4f(pos1loc, 1 - fabs(sinf(red)), fabs(sinf(green)), 0.0, 1.0));


	}
	glDeleteProgram(shader);
	return 0;
}

const char* get_key_name(int key)
{
	switch (key)
	{
		// Printable keys
	case GLFW_KEY_A:            return "A";
	case GLFW_KEY_B:            return "B";
	case GLFW_KEY_C:            return "C";
	case GLFW_KEY_D:            return "D";
	case GLFW_KEY_E:            return "E";
	case GLFW_KEY_F:            return "F";
	case GLFW_KEY_G:            return "G";
	case GLFW_KEY_H:            return "H";
	case GLFW_KEY_I:            return "I";
	case GLFW_KEY_J:            return "J";
	case GLFW_KEY_K:            return "K";
	case GLFW_KEY_L:            return "L";
	case GLFW_KEY_M:            return "M";
	case GLFW_KEY_N:            return "N";
	case GLFW_KEY_O:            return "O";
	case GLFW_KEY_P:            return "P";
	case GLFW_KEY_Q:            return "Q";
	case GLFW_KEY_R:            return "R";
	case GLFW_KEY_S:            return "S";
	case GLFW_KEY_T:            return "T";
	case GLFW_KEY_U:            return "U";
	case GLFW_KEY_V:            return "V";
	case GLFW_KEY_W:            return "W";
	case GLFW_KEY_X:            return "X";
	case GLFW_KEY_Y:            return "Y";
	case GLFW_KEY_Z:            return "Z";
	case GLFW_KEY_1:            return "1";
	case GLFW_KEY_2:            return "2";
	case GLFW_KEY_3:            return "3";
	case GLFW_KEY_4:            return "4";
	case GLFW_KEY_5:            return "5";
	case GLFW_KEY_6:            return "6";
	case GLFW_KEY_7:            return "7";
	case GLFW_KEY_8:            return "8";
	case GLFW_KEY_9:            return "9";
	case GLFW_KEY_0:            return "0";
	case GLFW_KEY_SPACE:        return "SPACE";
	case GLFW_KEY_MINUS:        return "MINUS";
	case GLFW_KEY_EQUAL:        return "EQUAL";
	case GLFW_KEY_LEFT_BRACKET: return "LEFT BRACKET";
	case GLFW_KEY_RIGHT_BRACKET: return "RIGHT BRACKET";
	case GLFW_KEY_BACKSLASH:    return "BACKSLASH";
	case GLFW_KEY_SEMICOLON:    return "SEMICOLON";
	case GLFW_KEY_APOSTROPHE:   return "APOSTROPHE";
	case GLFW_KEY_GRAVE_ACCENT: return "GRAVE ACCENT";
	case GLFW_KEY_COMMA:        return "COMMA";
	case GLFW_KEY_PERIOD:       return "PERIOD";
	case GLFW_KEY_SLASH:        return "SLASH";
	case GLFW_KEY_WORLD_1:      return "WORLD 1";
	case GLFW_KEY_WORLD_2:      return "WORLD 2";

		// Function keys
	case GLFW_KEY_ESCAPE:       return "ESCAPE";
	case GLFW_KEY_F1:           return "F1";
	case GLFW_KEY_F2:           return "F2";
	case GLFW_KEY_F3:           return "F3";
	case GLFW_KEY_F4:           return "F4";
	case GLFW_KEY_F5:           return "F5";
	case GLFW_KEY_F6:           return "F6";
	case GLFW_KEY_F7:           return "F7";
	case GLFW_KEY_F8:           return "F8";
	case GLFW_KEY_F9:           return "F9";
	case GLFW_KEY_F10:          return "F10";
	case GLFW_KEY_F11:          return "F11";
	case GLFW_KEY_F12:          return "F12";
	case GLFW_KEY_F13:          return "F13";
	case GLFW_KEY_F14:          return "F14";
	case GLFW_KEY_F15:          return "F15";
	case GLFW_KEY_F16:          return "F16";
	case GLFW_KEY_F17:          return "F17";
	case GLFW_KEY_F18:          return "F18";
	case GLFW_KEY_F19:          return "F19";
	case GLFW_KEY_F20:          return "F20";
	case GLFW_KEY_F21:          return "F21";
	case GLFW_KEY_F22:          return "F22";
	case GLFW_KEY_F23:          return "F23";
	case GLFW_KEY_F24:          return "F24";
	case GLFW_KEY_F25:          return "F25";
	case GLFW_KEY_UP:           return "UP";
	case GLFW_KEY_DOWN:         return "DOWN";
	case GLFW_KEY_LEFT:         return "LEFT";
	case GLFW_KEY_RIGHT:        return "RIGHT";
	case GLFW_KEY_LEFT_SHIFT:   return "LEFT SHIFT";
	case GLFW_KEY_RIGHT_SHIFT:  return "RIGHT SHIFT";
	case GLFW_KEY_LEFT_CONTROL: return "LEFT CONTROL";
	case GLFW_KEY_RIGHT_CONTROL: return "RIGHT CONTROL";
	case GLFW_KEY_LEFT_ALT:     return "LEFT ALT";
	case GLFW_KEY_RIGHT_ALT:    return "RIGHT ALT";
	case GLFW_KEY_TAB:          return "TAB";
	case GLFW_KEY_ENTER:        return "ENTER";
	case GLFW_KEY_BACKSPACE:    return "BACKSPACE";
	case GLFW_KEY_INSERT:       return "INSERT";
	case GLFW_KEY_DELETE:       return "DELETE";
	case GLFW_KEY_PAGE_UP:      return "PAGE UP";
	case GLFW_KEY_PAGE_DOWN:    return "PAGE DOWN";
	case GLFW_KEY_HOME:         return "HOME";
	case GLFW_KEY_END:          return "END";
	case GLFW_KEY_KP_0:         return "KEYPAD 0";
	case GLFW_KEY_KP_1:         return "KEYPAD 1";
	case GLFW_KEY_KP_2:         return "KEYPAD 2";
	case GLFW_KEY_KP_3:         return "KEYPAD 3";
	case GLFW_KEY_KP_4:         return "KEYPAD 4";
	case GLFW_KEY_KP_5:         return "KEYPAD 5";
	case GLFW_KEY_KP_6:         return "KEYPAD 6";
	case GLFW_KEY_KP_7:         return "KEYPAD 7";
	case GLFW_KEY_KP_8:         return "KEYPAD 8";
	case GLFW_KEY_KP_9:         return "KEYPAD 9";
	case GLFW_KEY_KP_DIVIDE:    return "KEYPAD DIVIDE";
	case GLFW_KEY_KP_MULTIPLY:  return "KEYPAD MULTIPLY";
	case GLFW_KEY_KP_SUBTRACT:  return "KEYPAD SUBTRACT";
	case GLFW_KEY_KP_ADD:       return "KEYPAD ADD";
	case GLFW_KEY_KP_DECIMAL:   return "KEYPAD DECIMAL";
	case GLFW_KEY_KP_EQUAL:     return "KEYPAD EQUAL";
	case GLFW_KEY_KP_ENTER:     return "KEYPAD ENTER";
	case GLFW_KEY_PRINT_SCREEN: return "PRINT SCREEN";
	case GLFW_KEY_NUM_LOCK:     return "NUM LOCK";
	case GLFW_KEY_CAPS_LOCK:    return "CAPS LOCK";
	case GLFW_KEY_SCROLL_LOCK:  return "SCROLL LOCK";
	case GLFW_KEY_PAUSE:        return "PAUSE";
	case GLFW_KEY_LEFT_SUPER:   return "LEFT SUPER";
	case GLFW_KEY_RIGHT_SUPER:  return "RIGHT SUPER";
	case GLFW_KEY_MENU:         return "MENU";

	default:                    return "UNKNOWN";
	}
}