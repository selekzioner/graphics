uniform highp mat4 matrix;
uniform highp float morphRate;

in highp vec4 posAttr;

void main()
{
    highp vec4 new_pos = posAttr;

    new_pos.x = posAttr.x * sqrt(1.0f - ((posAttr.y * posAttr.y + posAttr.z * posAttr.z) / 2. - posAttr.y * posAttr.y * posAttr.z * posAttr.z / 3.) * morphRate);

    new_pos.y = posAttr.y * sqrt(1.0f - ((posAttr.x * posAttr.x + posAttr.z * posAttr.z) / 2. - posAttr.x * posAttr.x * posAttr.z * posAttr.z / 3.) * morphRate);

    new_pos.z = posAttr.z * sqrt(1.0f - ((posAttr.x * posAttr.x + posAttr.y * posAttr.y) / 2. - posAttr.x * posAttr.x * posAttr.y * posAttr.y / 3.) * morphRate);

    gl_Position = matrix * new_pos;
}
