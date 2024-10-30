#version 330 core
layout(location = 0) in vec3 aPos; // Входная позиция вершины (x, y, z)
layout(location = 1) in vec2 aTexCoord; // Входные текстурные координаты (u, v)

out vec2 TexCoords; // Выходные текстурные координаты

void main()
{
    gl_Position = vec4(aPos, 1.0); // Преобразуем вершину в пространство клипа
    TexCoords = aTexCoord; // Передаем текстурные координаты во фрагментный шейдер

    gl_Position
}