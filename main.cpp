#include<iostream>
#include<filesystem>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "Model.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processinput(GLFWwindow* window);
void CaluculateTangentAndBitangent(glm::vec3& tang, glm::vec3& bitan, glm::vec3 pos1,glm::vec3 pos2,glm::vec3 pos3,glm::vec2 uv1,glm::vec2 uv2, glm::vec2 uv3);
unsigned int LoadCubeMap(std::vector<std::string> faces);

const unsigned int Height = 800;
const unsigned int Width = 800;
bool gamma = false;
bool hdr = false;
bool gammakeypressed = false;

float exposure = 0.3f;
float deltatime = 0.0f;
float lasttime = 0.0f;

glm::vec3 lightpos = glm::vec3(0.0f, 0.0f, 2.0f);
Camera camera(glm::vec3(0.0f,0.0f,3.0f),glm::vec3(0.0f,0.1f,0.0f));
bool FirstMouseInput = true;
float lastx = Height / 2.0f;
float lastY = Width / 2.0f;

int main()
{
	
	// Initiallizing GLFW and Version of OpenGl
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// Creating GLFW window and checking if window is initiallize or not
	GLFWwindow* window = glfwCreateWindow(Width, Height, "Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create the window." << std::endl;
		glfwTerminate();
		return -1;
	}

	// Making window object as main context 

	glfwMakeContextCurrent(window);

	// Resizing the window 
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// Load the GLAD and checking if GLAD is loaded or not
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initiallize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Reading and constructing shaders

	Shader shader("default.vert", "default.frag");
	Shader instanceshader("instance.vert", "instance.frag");
	Shader normalshader("normal.vert", "normal.frag","normal.geo");
	Shader lightshader("light.vert", "light.frag");
	Shader stencilshader("default.vert", "stencil.frag");
	Shader blendingshader("default.vert", "Blending.frag");
	Shader postprocessingshader("Postprocessing.vert", "Postprocessing.frag");
	Shader skyboxshader("skybox.vert", "skybox.frag");
	Shader shadowshader("shadowdepth.vert", "shadowdepth.frag");
	Shader debugshader("debug.vert", "debug.frag");
	Shader pointshadowshader("pointshadowdepth.vert", "pointshadowdepth.frag", "pointshadowdepth.geo");
	Shader GaussainBlurshader("gaussianBlur.vert", "gaussianBlur.frag");
	Shader HDRshader("hdr.vert", "hdr.frag");


	// positions
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent and bitangent for both triangles 

	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;

	CaluculateTangentAndBitangent(tangent1, bitangent1, pos1, pos2, pos3, uv1, uv2, uv3);
	CaluculateTangentAndBitangent(tangent2, bitangent2, pos1, pos3, pos4, uv1, uv3, uv4);

	Vertex normalquadvertices[] =
	{
		Vertex{glm::vec3(pos1.x,pos1.y,pos1.z),glm::vec2(uv1.x,uv1.y),glm::vec3(nm.x,nm.y,nm.z),glm::vec3(tangent1.x,tangent1.y,tangent1.z),glm::vec3(bitangent1.x,bitangent1.y,bitangent1.z)},
		Vertex{glm::vec3(pos2.x,pos2.y,pos2.z),glm::vec2(uv2.x,uv2.y),glm::vec3(nm.x,nm.y,nm.z),glm::vec3(tangent1.x,tangent1.y,tangent1.z),glm::vec3(bitangent1.x,bitangent1.y,bitangent1.z)},
		Vertex{glm::vec3(pos3.x,pos3.y,pos3.z),glm::vec2(uv3.x,uv3.y),glm::vec3(nm.x,nm.y,nm.z),glm::vec3(tangent1.x,tangent1.y,tangent1.z),glm::vec3(bitangent1.x,bitangent1.y,bitangent1.z) },

		Vertex{glm::vec3(pos1.x,pos1.y,pos1.z),glm::vec2(uv1.x,uv1.y),glm::vec3(nm.x,nm.y,nm.z),glm::vec3(tangent2.x,tangent2.y,tangent2.z),glm::vec3(bitangent2.x,bitangent2.y,bitangent2.z)},
		Vertex{glm::vec3(pos3.x,pos3.y,pos3.z),glm::vec2(uv3.x,uv3.y),glm::vec3(nm.x,nm.y,nm.z),glm::vec3(tangent2.x,tangent2.y,tangent2.z),glm::vec3(bitangent2.x,bitangent2.y,bitangent2.z)},
		Vertex{glm::vec3(pos4.x,pos4.y,pos4.z),glm::vec2(uv4.x,uv4.y),glm::vec3(nm.x,nm.y,nm.z),glm::vec3(tangent2.x,tangent2.y,tangent2.z),glm::vec3(bitangent2.x,bitangent2.y,bitangent2.z)}
	};

	unsigned int normalquadindices[] =
	{
		0,1,2,
		3,4,5
	};

	std::string nomaldiffpath = "textures/rock.png";
	std::string nomalspecpath = "textures/rockspecular.png";
	std::string normalnormpath = "textures/rocknormal.png";
	std::string normaldisppath = "textures/rockdisp.png";

	Texture normalquadtex[]
	{
		Texture(nomaldiffpath.c_str(),"diffuse",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma),
		Texture(nomalspecpath.c_str(),"specular",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma),
		Texture(normalnormpath.c_str(),"normalMap",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma),
		Texture(normaldisppath.c_str(),"depthMap",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma)
	};

	std::vector<Vertex> normalquadVerticies(normalquadvertices, normalquadvertices + sizeof(normalquadvertices) / sizeof(Vertex));
	std::vector<unsigned int> normalquadIndicies(normalquadindices, normalquadindices + sizeof(normalquadindices) / sizeof(unsigned int));
	std::vector<Texture> normalquadTextures(normalquadtex, normalquadtex + sizeof(normalquadtex) / sizeof(Texture));

	Mesh NormalQuad(normalquadVerticies, normalquadIndicies, normalquadTextures);

	Vertex vertex[] = {
		//coordinates             // color              //texcoords     //Normal
		
		Vertex{glm::vec3(-1.0f,0.0f,1.0f),glm::vec2(0.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)}, //0
		Vertex{glm::vec3(- 1.0f,0.0f,-1.0f),glm::vec2(0.0f,1.0f),glm::vec3(0.0f,-1.0f,0.0f)}, //1   // bottom
		Vertex{glm::vec3(1.0f,0.0f,-1.0f),glm::vec2(1.0f,1.0f),glm::vec3(0.0f,-1.0f,0.0f)}, //2
		Vertex{glm::vec3(1.0f,0.0f,1.0f),glm::vec2(1.0f,0.0f),glm::vec3(0.0f,-1.0f,0.0f)}, //3

		Vertex{glm::vec3(-1.0f,0.0f,1.0f),glm::vec2(0.0f,0.0f),glm::vec3(0.0f, 0.0f,1.0f)}, //4
		Vertex{glm::vec3(-1.0f,1.0f,1.0f),glm::vec2(0.0f,1.0f),glm::vec3(0.0f, 0.0f,1.0f)}, //5   // front
		Vertex{glm::vec3(1.0f,1.0f,1.0f),glm::vec2(1.0f,1.0f),glm::vec3(0.0f, 0.0f,1.0f)}, //6
		Vertex{glm::vec3(1.0f,0.0f,1.0f),glm::vec2(1.0f,0.0f),glm::vec3(0.0f, 0.0f,1.0f)}, //7

		Vertex{glm::vec3(-1.0f,1.0f,1.0f),glm::vec2(0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)}, //8
		Vertex{glm::vec3(-1.0f,1.0f,-1.0f),glm::vec2(0.0f,1.0f),glm::vec3(0.0f,1.0f,0.0f)}, //9   // up
		Vertex{glm::vec3(1.0f,1.0f,-1.0f),glm::vec2(1.0f,1.0f),glm::vec3(0.0f,1.0f,0.0f)}, //10
		Vertex{glm::vec3(1.0f,1.0f,1.0f),glm::vec2(1.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f)}, //11

		Vertex{glm::vec3(-1.0f,0.0f,-1.0f),glm::vec2(0.0f,0.0f),glm::vec3(0.0f, 0.0f,-1.0f)}, //12
		Vertex{glm::vec3(-1.0f,1.0f,-1.0f),glm::vec2(0.0f,1.0f),glm::vec3(0.0f, 0.0f,-1.0f)}, //13  // back
		Vertex{glm::vec3(1.0f,1.0f,-1.0f),glm::vec2(1.0f,1.0f),glm::vec3(0.0f, 0.0f,-1.0f)}, //14
		Vertex{glm::vec3(1.0f,0.0f,-1.0f),glm::vec2(1.0f,0.0f),glm::vec3(0.0f, 0.0f,-1.0f)}, //15

		Vertex{glm::vec3(1.0f,0.0f,1.0f),glm::vec2(0.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f)},//16 
		Vertex{glm::vec3(1.0f,1.0f,1.0f),glm::vec2(0.0f,1.0f),glm::vec3(1.0f,0.0f,0.0f)},//17  // right side
		Vertex{glm::vec3(1.0f,1.0f,-1.0f),glm::vec2(1.0f,1.0f),glm::vec3(1.0f,0.0f,0.0f)},//18 
		Vertex{glm::vec3(1.0f,0.0f,-1.0f),glm::vec2(1.0f,0.0f),glm::vec3(1.0f,0.0f,0.0f)},//19

		Vertex{glm::vec3(-1.0f,0.0f,1.0f),glm::vec2(0.0f,0.0f),glm::vec3(-1.0f,0.0f,0.0f)},//20
		Vertex{glm::vec3(-1.0f,1.0f,1.0f),glm::vec2(0.0f,1.0f),glm::vec3(-1.0f,0.0f,0.0f)},//21  // left side 
		Vertex{glm::vec3(-1.0f,1.0f,-1.0f),glm::vec2(1.0f,1.0f),glm::vec3(-1.0f,0.0f,0.0f)},//22 
		Vertex{glm::vec3(-1.0f,0.0f,-1.0f),glm::vec2(1.0f,0.0f),glm::vec3(-1.0f,0.0f,0.0f)},//23 
	};

	unsigned int indicies[] = {
		0,1,2,
		0,2,3,
		4,5,6,
		4,6,7,
		8,9,10,
		8,10,11,
		12,13,14,
		12,14,15,
		16,17,18,
		16,18,19,
		20,21,22,
		20,22,23
	};

	Vertex lightsourcevertex[] =
	{
		Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)}, //0
		Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)}, //1
		Vertex{glm::vec3(0.1f, -0.1f, -0.1f)}, //2
		Vertex{glm::vec3(0.1f, -0.1f,  0.1f)}, //3
		Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)}, //4
		Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)}, //5
		Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},  //6
		Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}   //7
	};

	unsigned int lighsourceindices[] =
	{
		0,1,2,
		0,2,3,
		0,4,7,
		0,7,3,
		3,7,6,
		3,6,2,
		2,6,5,
		2,5,1,
		1,5,4,
		1,4,0,
		4,5,6,
		4,6,7
	};

	Vertex grassvertex[] =
	{
		Vertex{glm::vec3(-0.5f,0.0f,1.0f),glm::vec2(0.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f)},//0
		Vertex{glm::vec3(-0.5f,1.0f,1.0f),glm::vec2(0.0f,1.0f),glm::vec3(0.0f,0.0f,1.0f)},//1
		Vertex{glm::vec3(0.5f,1.0f,1.0f),glm::vec2(1.0f,1.0f),glm::vec3(0.0f,0.0f,1.0f)},//2
		Vertex{glm::vec3(0.5f,0.0f,1.0f),glm::vec2(1.0f,0.0f),glm::vec3(0.0f,0.0f,1.0f)}//3
	};

	unsigned int grassindices[] =
	{
		0,1,2,
		0,2,3
	};

	std::string grasspath = "textures/window.png";
	Texture grasstex[]
	{
		Texture(grasspath.c_str(),"diffuse",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma)
	};

	float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	float debugquadVertices[] =
	{
		// positions         // texture Coords
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f
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

	// Sky box texture's path

	std::vector<std::string> faces
	{
		"textures/skybox/right.jpg",
		"textures/skybox/left.jpg",
		"textures/skybox/bottom.jpg",
		"textures/skybox/top.jpg",
		"textures/skybox/front.jpg",
		"textures/skybox/back.jpg"
	};


	VAO debugvao;
	VBO debugvbo;

	debugvao.Bind();
	debugvbo.Bind();

	debugvbo.BufferData(sizeof(debugquadVertices), &debugquadVertices);
	debugvao.vertexattrib(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	debugvao.vertexattrib(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

	// generating the VAO,VBO for Skybox 

	unsigned int skyboxvao, skyboxvbo;
	glGenVertexArrays(1, &skyboxvao);
	glGenBuffers(1, &skyboxvbo);

	glBindVertexArray(skyboxvao);
	glBindBuffer(GL_ARRAY_BUFFER,skyboxvbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	unsigned int cubemaptex = LoadCubeMap(faces);

	std::vector<Vertex> GrassVerticies(grassvertex, grassvertex + sizeof(grassvertex) / sizeof(Vertex));
	std::vector<unsigned int> GrassIndicies(grassindices, grassindices + sizeof(grassindices) / sizeof(unsigned int));
	std::vector<Texture> GrassTextures(grasstex, grasstex + sizeof(grasstex) / sizeof(Texture));

	// Generating the grass mesh.
	Mesh grass(GrassVerticies, GrassIndicies, GrassTextures);

	std::string path = "textures/metal.png";
	std::string spath = "textures/metalspecular.png";
	Texture textures[]{
		//Texture(faces,"diffuse",GL_TEXTURE_CUBE_MAP,GL_UNSIGNED_BYTE),
		Texture(path.c_str(),"diffuse",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma),
		Texture(spath.c_str(),"specular",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma)

	};

	std::string cPath = "textures/WoodFloor.png";
	std::string csPath = "textures/WoodFloorSpecular.png";
	Texture cubetexture[] =
	{
		Texture(cPath.c_str(),"diffuse",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma),
		Texture(csPath.c_str(),"specular",GL_TEXTURE_2D,GL_UNSIGNED_BYTE,gamma)
	};

	std::vector<Vertex> Verticies(vertex, vertex + sizeof(vertex) / sizeof(Vertex));
	std::vector<unsigned int> Indicies(indicies, indicies + sizeof(indicies) / sizeof(unsigned int));
	std::vector<Texture> Textures(textures, textures + sizeof(textures) / sizeof(Texture));
	std::vector<Texture> cTextures(cubetexture, cubetexture + sizeof(cubetexture) / sizeof(Texture));

	Mesh Box(Verticies, Indicies, Textures);
	Mesh cube(Verticies, Indicies, cTextures);

	std::vector<Vertex>LightVerticies(lightsourcevertex, lightsourcevertex + sizeof(lightsourcevertex) / sizeof(Vertex));
	std::vector<unsigned int> LightIndicies(lighsourceindices, lighsourceindices + sizeof(lighsourceindices) / sizeof(unsigned int));

	Mesh light(LightVerticies, LightIndicies, Textures);

	// Loadig the rock model for intanciating.

	std::string modelpath = "Models/backpack/backpack.obj";
	std::string rockspath = "Models/rock/rock.obj";
	Model models(modelpath.c_str(),gamma);
	Model rocks(rockspath.c_str(),gamma);

	// generate large list of semi-random model transformation matrices.
	unsigned int amount = 1000;
	glm::mat4* modelMatrices;
	modelMatrices = new glm::mat4[amount];
	srand(static_cast<unsigned int>(glfwGetTime()));
	float radius = 25.0f;
	float offset = 5.0f;

	for (unsigned int i = 0;i < amount;i++)
	{
		glm::mat4 rockmodel = glm::mat4(1.0f);

		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;

		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f;

		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;

		rockmodel = glm::translate(rockmodel, glm::vec3(x, y, z));

		// 2. Scale the model between 0.05f to 0.25f
		float scale = static_cast<float>((rand() % 20) / 100.0f + 0.5f);
		rockmodel = glm::scale(rockmodel, glm::vec3(scale));


		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotationangel = static_cast<float>((rand() % 360));
		rockmodel = glm::rotate(rockmodel, rotationangel, glm::vec3(0.4f, 0.6f, 0.6f));

		modelMatrices[i] = rockmodel;
	}

	VBO ivbo;
	ivbo.Bind();
	ivbo.BufferData(amount * sizeof(glm::mat4), &modelMatrices[0]);

	// instanciating the rock arround the planet

	for (unsigned int i = 0;i < rocks.meshes.size();i++)
	{
		unsigned int ivao = rocks.meshes[i].vao.ID;
		glBindVertexArray(ivao);
		// set attribute pointers for matrix (4 times vec4)
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);

		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);

		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);

	}

	// Generating the quad for post-processing.

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);


	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	 unsigned int framebuffertexture;
	glGenTextures(1, &framebuffertexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebuffertexture);

	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, Width, Height,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebuffertexture, 0);
	
	unsigned int HDRcolortex[2];
	glGenTextures(2, HDRcolortex);

	for (unsigned int i = 0;i < 2;i++)
	{
		glBindTexture(GL_TEXTURE_2D, HDRcolortex[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, HDRcolortex[i], 0);
	}

	unsigned int renderbuffer;
	glGenRenderbuffers(1, &renderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Width, Height);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4,GL_DEPTH24_STENCIL8, Width, Height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderbuffer);


	unsigned int attachment[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachment);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR :: FRAMEBUFFER :: Framebuffer is not complete." << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	unsigned int gaussianBlurFBO[2];
	glGenFramebuffers(2, gaussianBlurFBO);

	unsigned int gaussianBlurTex[2];
	glGenTextures(2, gaussianBlurTex);

	for (unsigned int i = 0;i < 2;i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurFBO[i]);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurTex[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, Width, Height, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gaussianBlurTex[i], 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR :: GAUSSIAN BLUR :: FrameBuffer is not complete." << std::endl;
		}
	}

	 unsigned int intermediateFBO;
	glGenFramebuffers(1, &intermediateFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

	unsigned int intermediateTex;
	glGenTextures(1, &intermediateTex);
	glBindTexture(GL_TEXTURE_2D, intermediateTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermediateTex, 0);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR :: FRAMEBUFFER :: intermediateFBO is not complete." << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 


	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	
	const unsigned int Shadow_width = 1024, Shadow_height = 1024;

	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Shadow_width, Shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float boarderColor[] = { 1.0,1.0,1.0,1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, boarderColor);

	unsigned int depthCubeMap;
	glGenTextures(1, &depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
	for (unsigned int i = 0;i < 6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, Shadow_width, Shadow_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.0f));
	glm::mat4 model1 = glm::mat4(1.0f);
	model1 = glm::translate(model1, glm::vec3(0.0, 2.0, 1.0));
	model1 = glm::scale(model1, glm::vec3(0.2));

	glm::mat4 model2 = glm::mat4(1.0f);
	model2 = glm::translate(model2, glm::vec3(0.0, 2.0, 0.0));
	model2 = glm::scale(model2, glm::vec3(0.2));
	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection;
	

	
	glm::vec3 lightcolor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 objectColor = glm::vec3(1.0f, 0.5f, 0.31f);


	// Assigning the values of uniforms

	unsigned int modelloc = glGetUniformLocation(shader.ID, "model");
	unsigned int viewloc = glGetUniformLocation(shader.ID, "view");
	unsigned int projectionloc = glGetUniformLocation(shader.ID, "projection");
	unsigned int objectcolorloc = glGetUniformLocation(shader.ID, "objectColor");
	unsigned int lightcolorloc = glGetUniformLocation(shader.ID, "lightColor");
	unsigned int camposloc = glGetUniformLocation(shader.ID, "campos");
	unsigned int timeloc = glGetUniformLocation(shader.ID, "time");
	unsigned int lightSpaceMatrixloc = glGetUniformLocation(shader.ID, "lightSpaceMatrix");
	unsigned int heightScaleloc = glGetUniformLocation(shader.ID, "heightScale");

	unsigned int materialshadowdepthmaploc = glGetUniformLocation(shader.ID, "material.shadowdepthMap");
	unsigned int materialshadowmaploc = glGetUniformLocation(shader.ID, "material.shadowMap");
	unsigned int materialshininessloc = glGetUniformLocation(shader.ID, "material.shininess");
	unsigned int lightposloc = glGetUniformLocation(shader.ID, "light.Position");
	unsigned int lightdirectionloc = glGetUniformLocation(shader.ID, "light.direction");
	unsigned int lightambientloc = glGetUniformLocation(shader.ID, "light.ambient");
	unsigned int lightdiffuseloc = glGetUniformLocation(shader.ID, "light.diffuse");
	unsigned int lightspecularloc = glGetUniformLocation(shader.ID, "light.specular");
	unsigned int lightconstantloc = glGetUniformLocation(shader.ID, "light.constant");
	unsigned int lightlinearloc = glGetUniformLocation(shader.ID, "light.linearterm");
	unsigned int lightquadraticloc = glGetUniformLocation(shader.ID, "light.quadraticterm");
	unsigned int lightcutoffloc = glGetUniformLocation(shader.ID, "light.cutoff");
	unsigned int lightouttercutoffloc = glGetUniformLocation(shader.ID, "light.outtercutoff");
	unsigned int lightblinnloc = glGetUniformLocation(shader.ID, "light.blinn");
	unsigned int lightgammaloc = glGetUniformLocation(shader.ID, "light.gamma");

	unsigned int lightmodelloc = glGetUniformLocation(lightshader.ID, "model");
	unsigned int lightviewloc = glGetUniformLocation(lightshader.ID, "view");
	unsigned int lightprojectionloc = glGetUniformLocation(lightshader.ID, "projection");

	unsigned int stencilmodelloc = glGetUniformLocation(stencilshader.ID, "model");
	unsigned int stencilviewloc = glGetUniformLocation(stencilshader.ID, "view");
	unsigned int stencilprojectionloc = glGetUniformLocation(stencilshader.ID, "projection");

	unsigned int blendingmodelloc = glGetUniformLocation(blendingshader.ID, "model");
	unsigned int blendingviewloc = glGetUniformLocation(blendingshader.ID, "view");
	unsigned int blendingprojectionloc = glGetUniformLocation(blendingshader.ID, "projection");
	
	unsigned int skyboxviewloc = glGetUniformLocation(skyboxshader.ID, "view");
	unsigned int skyboxprojectionloc = glGetUniformLocation(skyboxshader.ID, "projection");

	unsigned int normalviewloc = glGetUniformLocation(normalshader.ID, "view");
	unsigned int normalProjectionloc = glGetUniformLocation(normalshader.ID, "projection");
	unsigned int normalmodelloc = glGetUniformLocation(normalshader.ID, "model");

	unsigned int instanceviewloc = glGetUniformLocation(instanceshader.ID, "view");
	unsigned int instanceProjectionloc = glGetUniformLocation(instanceshader.ID, "projection");
	unsigned int instanceTexloc = glGetUniformLocation(instanceshader.ID, "tex0");

	unsigned int shadowlightspacematrixloc = glGetUniformLocation(shadowshader.ID, "lightSpaceMatrix");
	unsigned int shadowmodelloc = glGetUniformLocation(shadowshader.ID, "model");

	unsigned int pointshadowmodelloc = glGetUniformLocation(pointshadowshader.ID, "model");
	unsigned int pointshadowlightposloc = glGetUniformLocation(pointshadowshader.ID, "lightpos");
	unsigned int pointshadowfarplaneloc = glGetUniformLocation(pointshadowshader.ID, "far_plane");

	unsigned int hdrloc = glGetUniformLocation(HDRshader.ID, "hdr");
	unsigned int hdrtexloc = glGetUniformLocation(HDRshader.ID, "hdrBuffer");
	unsigned int hdrbloomblurloc = glGetUniformLocation(HDRshader.ID, "bloomBlur");
	unsigned int exposureloc = glGetUniformLocation(HDRshader.ID, "exposure");

	unsigned int imageloc = glGetUniformLocation(GaussainBlurshader.ID, "image");
	unsigned int horizontalloc = glGetUniformLocation(GaussainBlurshader.ID, "horizontal");

	// enabling the depth function and stencil function for depth testing and for stentil shader.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0XFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// Making window stay unitil it closes manually 
	while (!glfwWindowShouldClose(window))
	{
		float currentframe = static_cast<float>(glfwGetTime());
		deltatime = currentframe - lasttime;
		lasttime = currentframe;

		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		// clearing the color and rendering new color
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		float near_plane = 0.1f, far_plane = 7.5f, point_far_plane = 25.0f;

		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)Shadow_width / (float)Shadow_height, near_plane, point_far_plane);
		std::vector<glm::mat4> shadowtransforms;
		shadowtransforms.push_back(shadowProj * glm::lookAt(lightpos, lightpos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowtransforms.push_back(shadowProj * glm::lookAt(lightpos, lightpos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowtransforms.push_back(shadowProj * glm::lookAt(lightpos, lightpos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowtransforms.push_back(shadowProj * glm::lookAt(lightpos, lightpos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowtransforms.push_back(shadowProj * glm::lookAt(lightpos, lightpos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowtransforms.push_back(shadowProj * glm::lookAt(lightpos, lightpos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		glm::mat4 lightprojection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		glm::mat4 lightview = glm::lookAt(lightpos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 lightSpaceMatrix = lightprojection * lightview;

		projection = glm::perspective(glm::radians(camera.Fov), (float)Width / (float)Height, 0.1f, 100.0f);
		view = camera.GetviewMatrix();

		glm::mat4 lightmodel = glm::mat4(1.0f);
		lightmodel = glm::translate(lightmodel, lightpos);
		lightmodel = glm::scale(lightmodel, glm::vec3(0.5f));


		shadowshader.Active();
		glUniformMatrix4fv(shadowlightspacematrixloc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
			

		pointshadowshader.Active();
		for (unsigned int i = 0; i < 6; i++)
		{
			std::string loc = "shadowMatrices[" + std::to_string(i) + "]";
			unsigned int shadowtransformloc = glGetUniformLocation(pointshadowshader.ID, loc.c_str());
			glUniformMatrix4fv(shadowtransformloc, 1, GL_FALSE, glm::value_ptr(shadowtransforms[i]));
		}
		glUniform1f(pointshadowfarplaneloc, point_far_plane);
		glUniform3f(pointshadowlightposloc, lightpos.x,lightpos.y,lightpos.z);


		glViewport(0, 0, Shadow_width, Shadow_height);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		shader.Active();
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionloc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3f(objectcolorloc, objectColor.x, objectColor.y, objectColor.z);
		glUniform3f(lightcolorloc, lightcolor.x, lightcolor.y, lightcolor.z);
		glUniform3f(camposloc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform1f(timeloc, glfwGetTime());

		glUniform1f(materialshininessloc, 32.0f);
		glUniform3f(lightdirectionloc, camera.Front.x,camera.Front.y,camera.Front.z);
		glUniform3f(lightambientloc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightdiffuseloc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightspecularloc, 1.0f, 1.0f, 1.0f);
		glUniform1f(lightconstantloc, 1.0f);
		glUniform1f(lightlinearloc, 0.09f);
		glUniform1f(lightquadraticloc, 0.032f);
		glUniform3f(lightposloc, lightpos.x, lightpos.y, lightpos.z);
		glUniform1f(lightcutoffloc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightouttercutoffloc, glm::cos(glm::radians(17.5f)));
		glUniform1i(lightblinnloc, 1);
		glUniform1i(lightgammaloc, gamma);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		


		glUniformMatrix4fv(pointshadowmodelloc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(shadowmodelloc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
		Box.Draw(pointshadowshader);
		models.Draw(pointshadowshader);

		glUniformMatrix4fv(pointshadowmodelloc, 1, GL_FALSE, glm::value_ptr(model1));
		glUniformMatrix4fv(shadowmodelloc, 1, GL_FALSE, glm::value_ptr(model1));
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model1));
		cube.Draw(pointshadowshader);

		glUniformMatrix4fv(pointshadowmodelloc, 1, GL_FALSE, glm::value_ptr(model2));
		cube.Draw(pointshadowshader);


		models.Draw(shader);

		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		glDisable(GL_DEPTH_TEST);

		stencilshader.Active();
		float scale = 1.01f;
		glm::mat4 stencilmodel = glm::mat4(1.0f);
		stencilmodel = glm::scale(stencilmodel, glm::vec3(scale));
		glUniformMatrix4fv(stencilmodelloc, 1, GL_FALSE, glm::value_ptr(stencilmodel));
		glUniformMatrix4fv(stencilviewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(stencilprojectionloc, 1, GL_FALSE, glm::value_ptr(projection));

		Box.Draw(stencilshader);
		models.Draw(stencilshader);

		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);

		normalshader.Active();
		glUniformMatrix4fv(normalmodelloc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(normalviewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(normalProjectionloc, 1, GL_FALSE, glm::value_ptr(projection));

		models.Draw(normalshader);

		instanceshader.Active();
		glUniform1i(instanceTexloc, 0);
		glUniformMatrix4fv(instanceviewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(instanceProjectionloc, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		for(unsigned int i =0;i<rocks.texture_loaded.size();i++)
			glBindTexture(GL_TEXTURE_2D, rocks.texture_loaded[0].ID);

		for (unsigned int i = 0;i < rocks.meshes.size();i++)
		{
			glBindVertexArray(rocks.meshes[i].vao.ID);
			glDrawElementsInstanced(GL_TRIANGLES, static_cast<unsigned int>(rocks.meshes[i].indicies.size()), GL_UNSIGNED_INT, 0, amount);
			glBindVertexArray(0);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, Width, Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.Active();
		glUniformMatrix4fv(viewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionloc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(lightSpaceMatrixloc, 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
		glUniform3f(objectcolorloc, objectColor.x, objectColor.y, objectColor.z);
		glUniform3f(lightcolorloc, lightcolor.x, lightcolor.y, lightcolor.z);
		glUniform3f(camposloc, camera.Position.x, camera.Position.y, camera.Position.z);
		glUniform1f(timeloc, glfwGetTime());

		glUniform1f(materialshininessloc, 32.0f);
		glUniform3f(lightdirectionloc, camera.Front.x, camera.Front.y, camera.Front.z);
		glUniform3f(lightambientloc, 0.2f, 0.2f, 0.2f);
		glUniform3f(lightdiffuseloc, 0.5f, 0.5f, 0.5f);
		glUniform3f(lightspecularloc, 1.0f, 1.0f, 1.0f);
		glUniform1f(lightconstantloc, 1.0f);
		glUniform1f(lightlinearloc, 0.09f);
		glUniform1f(lightquadraticloc, 0.032f);
		glUniform3f(lightposloc, lightpos.x, lightpos.y, lightpos.z);
		glUniform1f(lightcutoffloc, glm::cos(glm::radians(12.5f)));
		glUniform1f(lightouttercutoffloc, glm::cos(glm::radians(17.5f)));
		glUniform1i(lightblinnloc, 1);
		glUniform1i(lightgammaloc, gamma);
		glUniform1i(materialshadowmaploc, 2);
		glUniform1i(materialshadowdepthmaploc, 2);
		glUniform1f(heightScaleloc, 0.1f);

		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model));
		NormalQuad.Draw(shader);

		lightshader.Active();
		glUniformMatrix4fv(lightmodelloc, 1, GL_FALSE, glm::value_ptr(lightmodel));
		glUniformMatrix4fv(lightviewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(lightprojectionloc, 1, GL_FALSE, glm::value_ptr(projection));

		light.Draw(lightshader);

		bool horizontal = true;
		bool first_iteration = true;

		unsigned int amount = 10;

		GaussainBlurshader.Active();
		for (int i = 0;i < amount;i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, gaussianBlurFBO[horizontal]);
			glUniform1i(horizontalloc, horizontal);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? HDRcolortex[1] : gaussianBlurTex[!horizontal]);
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			horizontal = !horizontal;
			if (first_iteration)
				first_iteration = false;
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Box.Draw(shader);
		models.Draw(shader);
	
		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model1));
		cube.Draw(shader);

		glUniformMatrix4fv(modelloc, 1, GL_FALSE, glm::value_ptr(model2));
		cube.Draw(shader);

		

		blendingshader.Active();
		glUniformMatrix4fv(blendingviewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(blendingprojectionloc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 grassmodel = glm::mat4(1.0f);
		grassmodel = glm::translate(grassmodel, glm::vec3(0.0f, -0.8f, 0.5f));
		grassmodel = glm::scale(grassmodel, glm::vec3(2.0f));
		glUniformMatrix4fv(blendingmodelloc, 1, GL_FALSE, glm::value_ptr(grassmodel));

		grass.Draw(blendingshader);

		glDepthFunc(GL_LEQUAL);
		skyboxshader.Active();
		view = glm::mat4(glm::mat3(camera.GetviewMatrix()));
		glUniform1i(glGetUniformLocation(skyboxshader.ID, "skybox"), 0);
		glUniformMatrix4fv(skyboxviewloc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(skyboxprojectionloc, 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(skyboxvao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaptex);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);


		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		glBlitFramebuffer(0, 0, Width, Height, 0, 0, Width, Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);

		postprocessingshader.Active();


		HDRshader.Active();
		glUniform1i(hdrloc, hdr);
		glUniform1i(hdrtexloc, 0);
		glUniform1i(hdrbloomblurloc, 1);
		glUniform1f(exposureloc, exposure);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, HDRcolortex[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, gaussianBlurTex[!horizontal]);
		glDrawArrays(GL_TRIANGLES, 0, 6);




		processinput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (FirstMouseInput)
	{
		lastx = xpos;
		lastY = ypos;
		FirstMouseInput = false;
	}
	float xoffset = xpos - lastx;
	float yoffset = lastY - ypos;
	lastx = xpos;
	lastY = ypos;

	camera.ProcessMouseInput(xoffset, yoffset,true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void processinput(GLFWwindow* window)
{
	float camspeed = static_cast<float>(2.5 * deltatime);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{	
		camera.ProcessKeyboard(FORWARD, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(UP, deltatime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(DOWN, deltatime);
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		double xpos;
		double ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		mouse_callback(window, xpos, ypos);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		if (exposure < 1.0)
		{
			exposure += 0.1f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		if (exposure > 0.1)
		{
			exposure -= 0.1f;
		}
	}
	else {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
	
}

unsigned int LoadCubeMap(std::vector<std::string> faces)
{
	unsigned int Cubemap;
	glGenTextures(1, &Cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, Cubemap);

	int width, height, colorch;
	
	for (unsigned int i = 0;i < faces.size();i++)
	{
		
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &colorch,0);
		stbi_set_flip_vertically_on_load(true);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i , 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap tex fail to load." << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return Cubemap;
}


void CaluculateTangentAndBitangent(glm::vec3& tang, glm::vec3& bitan, glm::vec3 pos1, glm::vec3 pos2, glm::vec3 pos3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
{
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;


	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tang.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tang.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tang.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

	bitan.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitan.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitan.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
}
