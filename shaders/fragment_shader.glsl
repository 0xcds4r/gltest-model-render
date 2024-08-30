#version 330 core

in vec3 ourColor;          // ������� ���� (RGB)
in vec2 TexCoords[2];     // ������ ������� ���������� ���������

out vec4 FragColor;       // �������� ���� (RGBA)

uniform sampler2D textures[1];  // ������ ������� (������� ������ ���������� ������ ���� ��������)

// ��������� ��� ������� ��������
uniform float glowThreshold = 5.0;  // ����� ��� ��������
uniform float glowIntensity = 2.5;  // ������������� ��������

void main()
{
    vec4 color = vec4(0.0);

    // ��������� ����� �� �������
    for (int i = 0; i < 1; ++i) {
        vec2 flippedTexCoord = vec2(TexCoords[i].x, 1.0 - TexCoords[i].y);
        color += texture(textures[i], flippedTexCoord);  
    }
    
    color /= 1.0;  // ������� ���� (��� ����� ��������)

    // ���������� ��������
    float brightness = max(max(color.r, color.g), color.b);  // ������� �����
    vec4 glowColor = vec4(0.0);
    
    if (brightness > glowThreshold) {
        float glowAmount = (brightness - glowThreshold) * glowIntensity;  // ������������� ��������
        glowColor = vec4(glowAmount, glowAmount, glowAmount, 1.0);  // ������� ���� ��������
    }
    
    // ��������� ���� � ������ �������� � �������� �����
    FragColor = (color + glowColor) * vec4(ourColor, 1.0);  // ��������� �������� ����
}
