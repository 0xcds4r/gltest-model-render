#version 330 core

in vec3 ourColor;          // Входной цвет (RGB)
in vec2 TexCoords[2];     // Массив входных текстурных координат

out vec4 FragColor;       // Выходной цвет (RGBA)

uniform sampler2D textures[1];  // Массив текстур (текущая версия использует только одну текстуру)

// Параметры для эффекта свечения
uniform float glowThreshold = 5.0;  // Порог для свечения
uniform float glowIntensity = 2.5;  // Интенсивность свечения

void main()
{
    vec4 color = vec4(0.0);

    // Суммируем цвета из текстур
    for (int i = 0; i < 1; ++i) {
        vec2 flippedTexCoord = vec2(TexCoords[i].x, 1.0 - TexCoords[i].y);
        color += texture(textures[i], flippedTexCoord);  
    }
    
    color /= 1.0;  // Средний цвет (для одной текстуры)

    // Применение свечения
    float brightness = max(max(color.r, color.g), color.b);  // Яркость цвета
    vec4 glowColor = vec4(0.0);
    
    if (brightness > glowThreshold) {
        float glowAmount = (brightness - glowThreshold) * glowIntensity;  // Интенсивность свечения
        glowColor = vec4(glowAmount, glowAmount, glowAmount, 1.0);  // Создаем цвет свечения
    }
    
    // Финальный цвет с учетом свечения и базового цвета
    FragColor = (color + glowColor) * vec4(ourColor, 1.0);  // Применяем основной цвет
}
