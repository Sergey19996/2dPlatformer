#version 330 core
out vec4 FragColor; // Цвет, который будет выведен на экран

in vec2 TexCoords; // Текстурные координаты, переданные из вершинного шейдера

uniform sampler2D uTexture; // Текстура, которая будет использоваться для окраски
uniform float uTime; // Переменная для времени, чтобы создать анимацию

void main()
{
        
    // Пример использования шума на основе времени
    vec2 uv = TexCoords;
    float noise = sin(uv.x * 10.0 + uTime) * cos(uv.y * 10.0 + uTime);
    
    // Получение цвета из текстуры и модификация его по шуму
    vec4 texColor = texture(uTexture, uv);
    FragColor = texColor * vec4(vec3(noise), 1.0); // Окраска пикселя
}