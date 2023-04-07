#version 330 core

in vec2 coord;
in vec3 inColor;

out vec4 outColor;

void main()
{
    float real  = coord.x;
    float imag  = coord.y;
    float Creal = real;  
    float Cimag = imag;  

    float r2 = 0.0;
    int i = 0;

    while (i < 1000 && r2 < (1 << 16))
    {
        float treal = real;

        real = (treal * treal) - (imag * imag) + Creal;
        imag = 2.0 * treal * imag + Cimag;
        r2   = (real * real) + (imag * imag);

        i++;
    }

    float pi = 4.0 * atan(1.0);

    //float tmp = (cos(coord.x) * sin(coord.y)) / (sin(coord.x) * cos(coord.y));
    float tmp = cos(coord.x) + sin(coord.x);

    vec3 co = vec3(cos(coord.x), sin(coord.y), atan(tmp/2));
    //vec3 co = inColor;

    if(i < 1000)
    {
        co.x = min(255.0, log(i + 2.0 - log(log(abs(imag)))/log(2.0))*75.0) / 255.0;
        co.y = min(255.0, log(i + 2.0 - log(log(abs(real)))/log(2.0))*75.0) / 255.0;
        //co = vec3(coord, 0.0);
    }
    
    outColor = vec4(co, 1.0f);
}
