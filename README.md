Лабораторная работа №2 по дисциплине "Инженерная и компьютерная графика"

Перемещение треугольника по координате X:
Создаём глобальную переменную gWorldLocation:

GLuint gWorldLocation;

Добавляем код шейдера:

#version 330                                                                                               
layout (location = 0) in vec3 Position;
uniform mat4 gWorld;
void main() {
   gl_Position = gWorld * vec4(Position, 1.0);
}

В функцию отрисовки добавляем:

static float Scale = -1.0f; 
Scale += 0.001f;

// Подготавливаем матрицу 4x4 для изменения координаты X на значение синуса Scale
glm::mat4 World;
World[0][0] = 1.0f; World[0][1] = 0.0f; World[0][2] = 0.0f; World[0][3] = sinf(Scale);
World[1][0] = 0.0f; World[1][1] = 1.0f; World[1][2] = 0.0f; World[1][3] = 0.0f;
World[2][0] = 0.0f; World[2][1] = 0.0f; World[2][2] = 1.0f; World[2][3] = 0.0f;
World[3][0] = 0.0f; World[3][1] = 0.0f; World[3][2] = 0.0f; World[3][3] = 1.0f;

// Загружаем данные в uniform-переменные шейдера (адрес переменной, количество матриц, передаётся ли матрица по строкам, указатель на первый элемент матрицы)
glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World[0][0]);

Добавляем две новые функции:

static void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType) // Функция, добавляющая шейдер к программе
{
GLuint ShaderObj = glCreateShader(ShaderType);// Создаём шейдер

if (ShaderObj == 0) {
      fprintf(stderr, "Error creating shader type %d\n", ShaderType);
      exit(0);
  }

// Сохраняем код шейдера в массиве
const GLchar *p[1];
p[0] = pShaderText.c_str();

// Массив длин кодов шейдеров
GLint lengths[1];
lengths[0] = pShaderText.length();

glShaderSource(ShaderObj, 1, p, Lengths);// Задаём исходники шейдера
glCompileShader(ShaderObj);// Компилируем шейдер

// Проверяем, что шейдер успешно скомпилировался
GLint success;
glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
if (!success) {
      GLchar InfoLog[1024];
      glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
      fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
      exit(1);
  }

// Добавляем шейдер в программу
glAttachShader(ShaderProgram, ShaderObj);
}

// Функция, компилирующая программу-шейдер
static void CompileShaders()
{
GLuint ShaderProgram = glCreateProgram();// Создаём программу-шейдер
if (ShaderProgram == 0) {
      fprintf(stderr, "Error creating shader program\n");
      exit(1);
  }

// Добавляем шейдер для вершин
AddShader(ShaderProgram, pVS, GL_VERTEX_SHADER);

GLint Success = 0;
GLchar ErrorLog[1024] = { 0 };
// Линкуем программу
glLinkProgram(ShaderProgram);
// Проверяем, что линковка прошла успешно
glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
if (Success == 0) {
glGetProgramInfoLog(ShaderProgram, sizeof(errorLog), nullptr, ErrorLog);
fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
exit(1);
}

// Валидируем программу
glValidateProgram(ShaderProgram);
// Проверяем, что валидация прошла успешно
glGetProgramiv(ShaderProgram, GL_VALIDATE_STATUS, &Success);
if (Success == 0) {
glGetProgramInfoLog(ShaderProgram, sizeof(ErrorLog), nullptr, ErrorLog);
fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
exit(1);
}

// Указываем OpenGL, что надо использовать эту программу
glUseProgram(ShaderProgram);

// Сохраняем местоположение переменной gWorld
gWorldLocation = glGetUniformLocation(ShaderProgram, "gWorld");
assert(gWorldLocation != 0xFFFFFFFF);
}

Вращение треугольника вокруг оси OZ

В функции отрисовки меняем матрицу World:

 World[0][0] = cosf(Scale); World[0][1] = -sinf(Scale); World[0][2] = 0.0f; World[0][3] = 0.0f;
 World[1][0] = sinf(Scale); World[1][1] = cosf(Scale);  World[1][2] = 0.0f; World[1][3] = 0.0f;
 World[2][0] = 0.0f;        World[2][1] = 0.0f;         World[2][2] = 1.0f; World[2][3] = 0.0f;
 World[3][0] = 0.0f;        World[3][1] = 0.0f;         World[3][2] = 0.0f; World[3][3] = 1.0f;
 
 Преобразования масштаба
 
 Как и в прошлом пункте меняем матрицу World:
 
 World[0][0] = sinf(Scale); World[0][1] = 0.0f;          World[0][2] = 0.0f;          World[0][3] = 0.0f;
World[1][0] = 0.0f;        World[1][1] = cosf(Scale);   World[1][2] = 0.0f;          World[1][3] = 0.0f;
World[2][0] = 0.0f;        World[2][1] = 0.0f;          World[2][2] = sinf(Scale);   World[2][3] = 0.0f;
World[3][0] = 0.0f;        World[3][1] = 0.0f;          World[3][2] = 0.0f;          World[3][3] = 1.0f;

Объединение преобразований

Создаем класс с пайплайном преобразований:

class Pipeline
{
public:
    Pipeline()
    {
        m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
        m_worldPos = glm::vec3(0.0f, 0.0f, 0.0f);
        m_rotateInfo = glm::vec3(0.0f, 0.0f, 0.0f);
    }

    void Scale(float ScaleX, float ScaleY, float ScaleZ)
    {
        m_scale.x = ScaleX;
        m_scale.y = ScaleY;
        m_scale.z = ScaleZ;
    }

    void WorldPos(float x, float y, float z)
    {
        m_worldPos.x = x;
        m_worldPos.y = y;
        m_worldPos.z = z;
    }

    void Rotate(float RotateX, float RotateY, float RotateZ)
    {
        m_rotateInfo.x = RotateX;
        m_rotateInfo.y = RotateY;
        m_rotateInfo.z = RotateZ;
    }

    void SetPerspectiveProj(float FOV, float Width, float Height, float zNear, float zFar)
    {
        m_persProj.FOV = FOV;
        m_persProj.Width = Width;
        m_persProj.Height = Height;
        m_persProj.zNear = zNear;
        m_persProj.zFar = zFar;
    }

    const glm::mat4* GetTrans();

private:
    void InitScaleTransform(glm::mat4& m) const;
    void InitRotateTransform(glm::mat4& m) const;
    void InitTranslationTransform(glm::mat4& m) const;
    void InitPerspectiveProj(glm::mat4& m) const;

    glm::vec3 m_scale;
    glm::vec3 m_worldPos;
    glm::vec3 m_rotateInfo;
    glm::mat4 m_transformation;

    struct {
        float FOV;
        float Width;
        float Height;
        float zNear;
        float zFar;
    } m_persProj;
};

В функции отрисовки создаем объект этого класса и работаем с ним

 Pipeline p;
    p.Scale(sinf(Scale * 0.1f), sinf(Scale * 0.1f), sinf(Scale * 0.1f));
    p.WorldPos(sinf(Scale), 0.0f, 0.0f);
    p.Rotate(sinf(Scale) * 90.0f, sinf(Scale) * 90.0f, sinf(Scale) * 90.0f);
    p.SetPerspectiveProj(30.0f, WINDOW_WIDTH, WINDOW_HEIGHT, 1.0f, 1000.0f);


    glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetTrans());
    
В методе GetTrans добавляем в умножение матрицу перспективы:

    m_transformation = PersProjTrans * TranslationTrans * RotateTrans * ScaleTrans;
