#version 330 core
out vec4 FragColor; // ����, ������� ����� ������� �� �����

in vec2 TexCoords; // ���������� ����������, ���������� �� ���������� �������

uniform sampler2D uTexture; // ��������, ������� ����� �������������� ��� �������
uniform float uTime; // ���������� ��� �������, ����� ������� ��������

void main()
{
        
    // ������ ������������� ���� �� ������ �������
    vec2 uv = TexCoords;
    float noise = sin(uv.x * 10.0 + uTime) * cos(uv.y * 10.0 + uTime);
    
    // ��������� ����� �� �������� � ����������� ��� �� ����
    vec4 texColor = texture(uTexture, uv);
    FragColor = texColor * vec4(vec3(noise), 1.0); // ������� �������
}