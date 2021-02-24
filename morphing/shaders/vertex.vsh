uniform highp mat4 matrix;
uniform highp float morphRate;

in highp vec4 posAttr;

void main()
{
    highp vec4 transformedPos = posAttr;

    transformedPos.x = posAttr.x * sqrt(1.0f - ((posAttr.y * posAttr.y + posAttr.z * posAttr.z) / 2.0f
                                                - posAttr.y * posAttr.y * posAttr.z * posAttr.z / 3.0f) * morphRate);

    transformedPos.y = posAttr.y * sqrt(1.0f - ((posAttr.x * posAttr.x + posAttr.z * posAttr.z) / 2.0f
                                                - posAttr.x * posAttr.x * posAttr.z * posAttr.z / 3.0f) * morphRate);

    transformedPos.z = posAttr.z * sqrt(1.0f - ((posAttr.x * posAttr.x + posAttr.y * posAttr.y) / 2.0f
                                                - posAttr.x * posAttr.x * posAttr.y * posAttr.y / 3.0f) * morphRate);

    gl_Position = matrix * transformedPos;
}
